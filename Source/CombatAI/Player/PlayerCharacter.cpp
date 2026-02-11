// Copyright CombatAI Project. All Rights Reserved.

#include "PlayerCharacter.h"
#include "PlayerStatsComponent.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/Camera/CombatCameraComponent.h"
#include "CombatAI/CombatAI.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule defaults.
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 88.0f);

	// Character movement defaults.
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->MaxWalkSpeed = WalkSpeed;
	MoveComp->JumpZVelocity = 600.0f; // Tuned for ~300 cm height
	MoveComp->AirControl = 0.35f;
	MoveComp->bOrientRotationToMovement = true;
	MoveComp->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Don't rotate character with controller (camera handles rotation).
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Camera boom (spring arm).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 60.0f, 120.0f)); // Over-the-shoulder offset
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->ProbeSize = 12.0f;
	CameraBoom->ProbeChannel = ECC_Camera;

	// Follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->FieldOfView = 75.0f;

	// Player stats.
	StatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("PlayerStats"));

	// Combat component.
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComp->MaxHealth = 100.0f;
	CombatComp->StaggerThreshold = 40.0f; // Medium resistance
	CombatComp->StaggerDuration = 0.5f;   // Quick recovery
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Apply movement speed.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Bind combat delegates.
	if (CombatComp)
	{
		CombatComp->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);
		CombatComp->OnStaggered.AddDynamic(this, &APlayerCharacter::HandleStaggered);
		CombatComp->OnStaggerRecovered.AddDynamic(this, &APlayerCharacter::HandleStaggerRecovered);
	}

	// Bind montage end callback.
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		Anim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMontageEnded);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessMovement(DeltaTime);
	ProcessDodge(DeltaTime);
	UpdateLockOn();

	// Combo window timeout.
	if (ComboWindowTimer > 0.0f)
	{
		ComboWindowTimer -= DeltaTime;
		if (ComboWindowTimer <= 0.0f)
		{
			ComboCount = 0;
			bComboInputBuffered = false;
		}
	}

	// Ranged cooldown.
	if (RangedCooldownTimer > 0.0f)
	{
		RangedCooldownTimer -= DeltaTime;
	}

	// Sprint stamina drain.
	if (bIsSprinting && StatsComponent)
	{
		if (!StatsComponent->ConsumeStamina(StatsComponent->SprintStaminaCost * DeltaTime))
		{
			RequestSprintStop();
		}
	}
}

// ═══════════════════════════════════════════════════════════════
// INPUT REQUEST INTERFACE
// ═══════════════════════════════════════════════════════════════

void APlayerCharacter::RequestMove(const FVector2D& Input)
{
	CurrentMoveInput = Input;
}

void APlayerCharacter::RequestLook(const FVector2D& Input)
{
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void APlayerCharacter::RequestSprintStart()
{
	if (CombatState != EPlayerCombatState::Idle) return;
	if (!StatsComponent || !StatsComponent->HasStamina(1.0f)) return;

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::RequestSprintStop()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::RequestJump()
{
	if (bIsDodging || CombatState == EPlayerCombatState::Dead) return;
	Jump();
}

void APlayerCharacter::RequestDodge()
{
	if (bIsDodging || CombatState == EPlayerCombatState::Dead) return;
	if (!StatsComponent || !StatsComponent->ConsumeStamina(StatsComponent->DodgeStaminaCost)) return;

	// Cancel current action into dodge.
	if (CombatState != EPlayerCombatState::Idle)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Stop(0.1f);
		}
	}

	ExecuteDodge();
}

void APlayerCharacter::RequestLightAttack()
{
	if (CombatState == EPlayerCombatState::Dead || CombatState == EPlayerCombatState::Staggered) return;

	// Buffer combo input if in the middle of an attack.
	if (CombatState == EPlayerCombatState::LightAttack)
	{
		bComboInputBuffered = true;
		return;
	}

	if (CanCancelInto(EPlayerCombatState::LightAttack))
	{
		RequestSprintStop();
		ComboCount = 0;
		ExecuteLightAttack();
	}
}

void APlayerCharacter::RequestHeavyAttackStart()
{
	if (!CanPerformAction()) return;
	RequestSprintStop();
	SetCombatState(EPlayerCombatState::HeavyCharge);
	// Heavy charge animation would start here.
}

void APlayerCharacter::RequestHeavyAttackRelease()
{
	if (CombatState == EPlayerCombatState::HeavyCharge)
	{
		ExecuteHeavyAttack();
	}
}

void APlayerCharacter::RequestParry()
{
	if (!CanPerformAction()) return;
	RequestSprintStop();
	ExecuteParry();
}

void APlayerCharacter::RequestRangedAttack()
{
	if (!CanPerformAction()) return;
	if (RangedCooldownTimer > 0.0f) return;
	if (!StatsComponent || !StatsComponent->ConsumeAmmo(1)) return;

	RequestSprintStop();
	ExecuteRangedAttack();
}

void APlayerCharacter::RequestLockOnToggle()
{
	if (IsLockedOn())
	{
		ClearLockedTarget();
	}
	else
	{
		AActor* Target = FindBestLockOnTarget();
		SetLockedTarget(Target);
	}
}

// ═══════════════════════════════════════════════════════════════
// COMBAT EXECUTION
// ═══════════════════════════════════════════════════════════════

void APlayerCharacter::ExecuteLightAttack()
{
	SetCombatState(EPlayerCombatState::LightAttack);

	UAnimMontage* MontageToPlay = nullptr;
	switch (ComboCount)
	{
	case 0: MontageToPlay = LightAttack1Montage; break;
	case 1: MontageToPlay = LightAttack2Montage; break;
	case 2: MontageToPlay = LightAttack3Montage; break;
	default: MontageToPlay = LightAttack1Montage; ComboCount = 0; break;
	}

	if (MontageToPlay)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(MontageToPlay, 1.0f);
		}
	}

	// Advance combo counter.
	ComboCount = (ComboCount + 1) % 3;
	ComboWindowTimer = ComboWindowDuration;

	// Orient toward locked target.
	if (IsLockedOn())
	{
		FVector Dir = (LockedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		SetActorRotation(Dir.Rotation());
	}
}

void APlayerCharacter::ExecuteHeavyAttack()
{
	SetCombatState(EPlayerCombatState::HeavyRelease);

	if (HeavyAttackMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(HeavyAttackMontage, 1.0f);
		}
	}

	if (IsLockedOn())
	{
		FVector Dir = (LockedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		SetActorRotation(Dir.Rotation());
	}
}

void APlayerCharacter::ExecuteDodge()
{
	bIsDodging = true;
	bIsInvulnerable = false;
	DodgeTimer = 0.0f;

	// Determine dodge direction from input, or backward if no input.
	if (CurrentMoveInput.SizeSquared() > 0.01f)
	{
		const FRotator ControlRot = GetControlRotation();
		const FRotator YawRot(0.0f, ControlRot.Yaw, 0.0f);
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		DodgeDirection = (Forward * CurrentMoveInput.Y + Right * CurrentMoveInput.X).GetSafeNormal();
	}
	else
	{
		DodgeDirection = -GetActorForwardVector();
	}

	SetCombatState(EPlayerCombatState::Dodging);

	if (DodgeMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(DodgeMontage, 1.0f);
		}
	}
}

void APlayerCharacter::ExecuteParry()
{
	SetCombatState(EPlayerCombatState::Parrying);

	if (ParryMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(ParryMontage, 1.0f);
		}
	}
}

void APlayerCharacter::ExecuteRangedAttack()
{
	SetCombatState(EPlayerCombatState::RangedAttack);
	RangedCooldownTimer = RangedCooldown;

	// Projectile spawning will be implemented as part of hit detection.
	// For now, do a line trace as a placeholder.
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 5000.0f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
	{
		FDamageInfo DmgInfo;
		DmgInfo.Amount = 15.0f;
		DmgInfo.StaggerLevel = EStaggerLevel::Medium;
		DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(EStaggerLevel::Medium);
		DmgInfo.DamageSource = this;
		DmgInfo.HitDirection = (Hit.ImpactPoint - GetActorLocation()).GetSafeNormal();
		DmgInfo.HitLocation = Hit.ImpactPoint;
		DmgInfo.DamageType = EDamageType::Ranged;

		UCombatComponent::DealDamage(Hit.GetActor(), DmgInfo);
	}

	// Return to idle after a brief delay (animation-driven in full implementation).
	FTimerHandle TempHandle;
	GetWorldTimerManager().SetTimer(TempHandle, [this]()
	{
		if (CombatState == EPlayerCombatState::RangedAttack)
		{
			SetCombatState(EPlayerCombatState::Idle);
		}
	}, 0.4f, false);
}

// ═══════════════════════════════════════════════════════════════
// MOVEMENT PROCESSING
// ═══════════════════════════════════════════════════════════════

void APlayerCharacter::ProcessMovement(float DeltaTime)
{
	if (bIsDodging || CombatState == EPlayerCombatState::Dead) return;

	if (CurrentMoveInput.SizeSquared() > 0.01f)
	{
		const FRotator ControlRot = GetControlRotation();
		const FRotator YawRot(0.0f, ControlRot.Yaw, 0.0f);
		const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		const FVector MoveDir = Forward * CurrentMoveInput.Y + Right * CurrentMoveInput.X;

		AddMovementInput(MoveDir, 1.0f);
	}

	// Clear input each frame (re-set by controller if still held).
	CurrentMoveInput = FVector2D::ZeroVector;
}

void APlayerCharacter::ProcessDodge(float DeltaTime)
{
	if (!bIsDodging) return;

	DodgeTimer += DeltaTime;

	// I-frame window.
	bIsInvulnerable = (DodgeTimer >= DodgeIFrameStart && DodgeTimer <= DodgeIFrameEnd);

	// Apply dodge movement.
	const FVector DodgeVelocity = DodgeDirection * DodgeSpeed;
	GetCharacterMovement()->Velocity = FVector(DodgeVelocity.X, DodgeVelocity.Y,
		GetCharacterMovement()->Velocity.Z);

	// End dodge.
	if (DodgeTimer >= DodgeDuration)
	{
		bIsDodging = false;
		bIsInvulnerable = false;
		DodgeTimer = 0.0f;
		SetCombatState(EPlayerCombatState::Idle);
	}
}

// ═══════════════════════════════════════════════════════════════
// LOCK-ON SYSTEM
// ═══════════════════════════════════════════════════════════════

AActor* APlayerCharacter::FindBestLockOnTarget() const
{
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(LockOnRange);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->OverlapMultiByChannel(
		Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);

	AActor* Best = nullptr;
	float BestScore = FLT_MAX;

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Candidate = Overlap.GetActor();
		if (!Candidate || Candidate == this) continue;

		// Only lock onto actors with a CombatComponent that are alive.
		UCombatComponent* CC = Candidate->FindComponentByClass<UCombatComponent>();
		if (!CC || !CC->IsAlive()) continue;

		// Score by distance (closest preferred).
		float Dist = FVector::Dist(GetActorLocation(), Candidate->GetActorLocation());
		if (Dist < BestScore)
		{
			BestScore = Dist;
			Best = Candidate;
		}
	}

	return Best;
}

void APlayerCharacter::SetLockedTarget(AActor* NewTarget)
{
	LockedTarget = NewTarget;
	OnLockOnChanged.Broadcast(LockedTarget);

	if (LockedTarget)
	{
		// Switch to strafing movement.
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
}

void APlayerCharacter::ClearLockedTarget()
{
	LockedTarget = nullptr;
	OnLockOnChanged.Broadcast(nullptr);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::UpdateLockOn()
{
	if (!IsLockedOn()) return;

	// Check if target is still valid.
	UCombatComponent* TargetCC = LockedTarget->FindComponentByClass<UCombatComponent>();
	float Dist = FVector::Dist(GetActorLocation(), LockedTarget->GetActorLocation());

	if (!TargetCC || !TargetCC->IsAlive() || Dist > LockOnRange * 1.2f)
	{
		ClearLockedTarget();
		return;
	}

	// Face the locked target.
	FVector Dir = (LockedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	FRotator LookRot = Dir.Rotation();
	GetController()->SetControlRotation(FMath::RInterpTo(
		GetControlRotation(), LookRot, GetWorld()->GetDeltaSeconds(), 10.0f));
}

// ═══════════════════════════════════════════════════════════════
// STATE MANAGEMENT
// ═══════════════════════════════════════════════════════════════

void APlayerCharacter::SetCombatState(EPlayerCombatState NewState)
{
	if (CombatState == NewState) return;
	CombatState = NewState;
	OnCombatStateChanged.Broadcast(NewState);
}

bool APlayerCharacter::CanPerformAction() const
{
	return CombatState == EPlayerCombatState::Idle && !bIsDodging;
}

bool APlayerCharacter::CanCancelInto(EPlayerCombatState DesiredState) const
{
	if (CombatState == EPlayerCombatState::Dead || CombatState == EPlayerCombatState::Staggered)
		return false;

	if (CombatState == EPlayerCombatState::Idle)
		return true;

	// Allow canceling into dodge from most states (handled separately).
	if (DesiredState == EPlayerCombatState::Dodging)
		return true;

	// Allow light attack chaining.
	if (DesiredState == EPlayerCombatState::LightAttack && CombatState == EPlayerCombatState::LightAttack)
		return true;

	return false;
}

// ═══════════════════════════════════════════════════════════════
// CALLBACKS
// ═══════════════════════════════════════════════════════════════

void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) return;

	// Check for buffered combo input.
	if (CombatState == EPlayerCombatState::LightAttack && bComboInputBuffered)
	{
		bComboInputBuffered = false;
		ExecuteLightAttack();
		return;
	}

	// Return to idle after any attack montage completes.
	if (CombatState == EPlayerCombatState::LightAttack ||
		CombatState == EPlayerCombatState::HeavyRelease ||
		CombatState == EPlayerCombatState::Parrying ||
		CombatState == EPlayerCombatState::RangedAttack)
	{
		ComboCount = 0;
		SetCombatState(EPlayerCombatState::Idle);
	}
}

void APlayerCharacter::HandleDeath()
{
	SetCombatState(EPlayerCombatState::Dead);
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Play death montage or ragdoll here.
	UE_LOG(LogCombatAI, Log, TEXT("PlayerCharacter: Death state entered."));
}

void APlayerCharacter::HandleStaggered()
{
	SetCombatState(EPlayerCombatState::Staggered);

	if (StaggerMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(StaggerMontage, 1.0f);
		}
	}
}

void APlayerCharacter::HandleStaggerRecovered()
{
	SetCombatState(EPlayerCombatState::Idle);
}
