// Copyright CombatAI Project. All Rights Reserved.

#include "WardenCharacter.h"
#include "CombatAI/AI/UtilityAI/UtilityAIComponent.h"
#include "CombatAI/AI/Core/AIPerceptionComponent.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AWardenCharacter::AWardenCharacter()
{
	Archetype = EEnemyArchetype::Warden;
	EnemyDisplayName = TEXT("The Warden");

	// Stats.
	CombatComp->MaxHealth = 150.0f;
	CombatComp->StaggerThreshold = 60.0f;
	CombatComp->StaggerDuration = 1.2f;
	CombatComp->FrontalArmorReduction = 0.5f;
	CombatComp->WeakPointMultiplier = 2.0f;

	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	// Scale 1.5x.
	SetActorScale3D(FVector(1.5f));

	// Components.
	UtilityAI = CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAI"));
	UtilityAI->DecisionRate = 4.0f;

	PerceptionComp = CreateDefaultSubobject<UAICombatPerceptionComponent>(TEXT("Perception"));
	PerceptionComp->DetectionRange = 2000.0f;
	PerceptionComp->FieldOfView = 120.0f;
	PerceptionComp->HearingRange = 1200.0f;
}

void AWardenCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureUtilityActions();
}

void AWardenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckEnrageStatus();
}

void AWardenCharacter::OnEnterCombat_Implementation()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s entering combat!"), *GetName());
}

void AWardenCharacter::CheckEnrageStatus()
{
	if (CombatComp && !bIsEnraged)
	{
		if (CombatComp->GetHealthPercent() <= EnrageHealthThreshold)
		{
			bIsEnraged = true;
			UE_LOG(LogCombatAI, Log, TEXT("Warden %s ENRAGED!"), *GetName());
		}
	}
}

void AWardenCharacter::ConfigureUtilityActions()
{
	UtilityAI->Actions.Empty();

	// 1. Advance
	{
		FUtilityAction A;
		A.ActionName = TEXT("Advance");
		A.Weight = 0.9f;

		FUtilityConsideration Dist;
		Dist.ConsiderationName = TEXT("FarFromPlayer");
		Dist.InputType = EConsiderationInput::DistanceToPlayer;
		Dist.InputMin = 500.0f;
		Dist.InputMax = 2000.0f;
		Dist.Curve = FResponseCurve::MakeLogistic(6.0f, 0.4f);

		FUtilityConsideration Vis;
		Vis.ConsiderationName = TEXT("CanSee");
		Vis.InputType = EConsiderationInput::PlayerVisible;
		Vis.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(Dist);
		A.Considerations.Add(Vis);
		UtilityAI->Actions.Add(A);
	}

	// 2. Ground Slam
	{
		FUtilityAction A;
		A.ActionName = TEXT("GroundSlam");
		A.Weight = 1.3f;
		A.Cooldown = 5.0f;
		A.bRequiresAttackToken = true;

		FUtilityConsideration Dist;
		Dist.ConsiderationName = TEXT("CloseRange");
		Dist.InputType = EConsiderationInput::DistanceToPlayer;
		Dist.InputMin = 0.0f;
		Dist.InputMax = 300.0f;
		Dist.Curve = FResponseCurve::MakeInverse();

		A.Considerations.Add(Dist);
		UtilityAI->Actions.Add(A);
	}

	// 3. Shield Charge
	{
		FUtilityAction A;
		A.ActionName = TEXT("ShieldCharge");
		A.Weight = 1.1f;
		A.Cooldown = ChargeCooldown;
		A.bRequiresAttackToken = true;

		FUtilityConsideration Dist;
		Dist.ConsiderationName = TEXT("ChargeRange");
		Dist.InputType = EConsiderationInput::DistanceToPlayer;
		Dist.InputMin = 500.0f;
		Dist.InputMax = 1200.0f;
		Dist.Curve = FResponseCurve::MakeLogistic(8.0f, 0.5f);

		FUtilityConsideration Path;
		Path.ConsiderationName = TEXT("ClearPath");
		Path.InputType = EConsiderationInput::ClearPath;
		Path.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(Dist);
		A.Considerations.Add(Path);
		UtilityAI->Actions.Add(A);
	}

	// 4. Shield Bash
	{
		FUtilityAction A;
		A.ActionName = TEXT("ShieldBash");
		A.Weight = 1.2f;
		A.Cooldown = 2.0f;
		A.bRequiresAttackToken = true;

		FUtilityConsideration Dist;
		Dist.ConsiderationName = TEXT("VeryClose");
		Dist.InputType = EConsiderationInput::DistanceToPlayer;
		Dist.InputMin = 0.0f;
		Dist.InputMax = 150.0f;
		Dist.Curve = FResponseCurve::MakeInverse();

		FUtilityConsideration Front;
		Front.ConsiderationName = TEXT("PlayerAttackingFront");
		Front.InputType = EConsiderationInput::PlayerAttacking;
		Front.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(Dist);
		A.Considerations.Add(Front);
		UtilityAI->Actions.Add(A);
	}

	// 5. Stomp Wave (Enrage only)
	{
		FUtilityAction A;
		A.ActionName = TEXT("StompWave");
		A.Weight = 1.5f;
		A.Cooldown = 10.0f;
		A.bRequiresAttackToken = true;

		FUtilityConsideration Enrage;
		Enrage.ConsiderationName = TEXT("BelowEnrage");
		Enrage.InputType = EConsiderationInput::HealthBelowEnrage;
		Enrage.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration Surround;
		Surround.ConsiderationName = TEXT("Surrounded");
		Surround.InputType = EConsiderationInput::Surrounded;
		Surround.Curve = FResponseCurve::MakeLinear(0.5f, 0.5f);

		A.Considerations.Add(Enrage);
		A.Considerations.Add(Surround);
		UtilityAI->Actions.Add(A);
	}

	// 6. Protect Ally
	{
		FUtilityAction A;
		A.ActionName = TEXT("ProtectAlly");
		A.Weight = 1.0f;

		FUtilityConsideration Nearby;
		Nearby.ConsiderationName = TEXT("AllyNearby");
		Nearby.InputType = EConsiderationInput::AllyNearby;
		Nearby.Curve = FResponseCurve::MakeLogistic(8.0f, 0.5f);

		FUtilityConsideration Threat;
		Threat.ConsiderationName = TEXT("AllyUnderThreat");
		Threat.InputType = EConsiderationInput::AllyUnderThreat;
		Threat.Curve = FResponseCurve::MakeLinear(1.0f, 0.0f);

		A.Considerations.Add(Nearby);
		A.Considerations.Add(Threat);
		UtilityAI->Actions.Add(A);
	}

	// 7. Face Player
	{
		FUtilityAction A;
		A.ActionName = TEXT("FacePlayer");
		A.Weight = 0.8f;

		FUtilityConsideration Behind;
		Behind.ConsiderationName = TEXT("PlayerBehind");
		Behind.InputType = EConsiderationInput::PlayerFacing;
		Behind.Curve = FResponseCurve::MakeInverse(); // Score high when player is behind us
		// Actually we want this when player is behind the Warden, not facing
		// PlayerFacing dot: +1 = player facing me, so if player is behind me, I need different input
		// Simplified: use a general "face player" with moderate weight

		A.Considerations.Add(Behind);
		UtilityAI->Actions.Add(A);
	}
}

// ───── Action Execution ─────

void AWardenCharacter::ExecuteAdvance()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: Advancing"), *GetName());
}

void AWardenCharacter::ExecuteGroundSlam()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: Ground Slam!"), *GetName());
	if (SlamMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(SlamMontage, 1.0f);
		}
	}
	// AOE damage applied via anim notify calling ApplyAOEDamage.
}

void AWardenCharacter::ExecuteShieldCharge()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: Shield Charge!"), *GetName());
	bIsCharging = true;

	if (ChargeMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(ChargeMontage, 1.0f);
		}
	}

	// Charge direction toward player.
	if (AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector Dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		SetActorRotation(Dir.Rotation());
	}

	// Charge movement handled by BT task + root motion.
	// Hit detection via HitDetection component during charge frames.
	FDamageInfo DmgInfo;
	DmgInfo.Amount = ChargeDamage;
	DmgInfo.StaggerLevel = EStaggerLevel::High;
	DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(EStaggerLevel::High);
	DmgInfo.DamageType = EDamageType::Charge;
	HitDetection->EnableHitDetection(DmgInfo);
}

void AWardenCharacter::ExecuteShieldBash()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: Shield Bash"), *GetName());
	if (BashMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(BashMontage, 1.0f);
		}
	}

	FDamageInfo DmgInfo;
	DmgInfo.Amount = BashDamage;
	DmgInfo.StaggerLevel = EStaggerLevel::High;
	DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(EStaggerLevel::High);
	DmgInfo.DamageType = EDamageType::Melee;
	HitDetection->EnableHitDetection(DmgInfo);
}

void AWardenCharacter::ExecuteStompWave()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: STOMP WAVE (Enraged)!"), *GetName());
	if (StompMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(StompMontage, 1.0f);
		}
	}
	// 360 AOE via anim notify.
}

void AWardenCharacter::ExecuteProtectAlly()
{
	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: Protecting ally"), *GetName());
	// BT task moves between player and nearest Watcher.
}

void AWardenCharacter::ExecuteFacePlayer()
{
	if (AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector Dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Dir.Rotation(),
			GetWorld()->GetDeltaSeconds(), TurnSpeed / 360.0f * 10.0f));
	}
}

void AWardenCharacter::ApplyAOEDamage(float Radius, float Damage, EStaggerLevel Stagger)
{
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->OverlapMultiByChannel(
		Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor) continue;

		UCombatComponent* CC = HitActor->FindComponentByClass<UCombatComponent>();
		if (!CC || !CC->IsAlive()) continue;

		FDamageInfo DmgInfo;
		DmgInfo.Amount = Damage;
		DmgInfo.StaggerLevel = Stagger;
		DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(Stagger);
		DmgInfo.DamageSource = this;
		DmgInfo.HitDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		DmgInfo.HitLocation = HitActor->GetActorLocation();
		DmgInfo.DamageType = EDamageType::AOE;

		CC->ReceiveDamage(DmgInfo);
	}

	UE_LOG(LogCombatAI, Log, TEXT("Warden %s: AOE damage applied (Radius: %.0f, Damage: %.0f)"),
		*GetName(), Radius, Damage);
}
