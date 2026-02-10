// Copyright CombatAI Project. All Rights Reserved.

#include "AICharacterBase.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/Combat/HitDetection.h"
#include "CombatAI/CombatAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AAICharacterBase::AAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HitDetection = CreateDefaultSubobject<UHitDetectionComponent>(TEXT("HitDetection"));

	// AI characters rotate independently of controller.
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (CombatComp)
	{
		CombatComp->OnDeath.AddDynamic(this, &AAICharacterBase::HandleDeath);
		CombatComp->OnStaggered.AddDynamic(this, &AAICharacterBase::HandleStagger);
		CombatComp->OnStaggerRecovered.AddDynamic(this, &AAICharacterBase::HandleStaggerRecovery);
	}
}

void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ───── AI State ─────

void AAICharacterBase::SetAIState(EAIState NewState)
{
	if (CurrentAIState == NewState) return;

	const EAIState OldState = CurrentAIState;
	CurrentAIState = NewState;

	UE_LOG(LogCombatAI, Log, TEXT("%s state: %s -> %s"),
		*GetName(),
		*UEnum::GetValueAsString(OldState),
		*UEnum::GetValueAsString(NewState));

	OnAIStateChanged.Broadcast(OldState, NewState);

	if (NewState == EAIState::Combat)
	{
		OnEnterCombat();
	}
}

// ───── Attack Token ─────

void AAICharacterBase::GrantAttackToken()
{
	bHasAttackToken = true;
	UE_LOG(LogCombatAI, Verbose, TEXT("%s granted attack token."), *GetName());
}

void AAICharacterBase::ReturnAttackToken()
{
	bHasAttackToken = false;
	UE_LOG(LogCombatAI, Verbose, TEXT("%s returned attack token."), *GetName());
}

// ───── Event Handlers ─────

void AAICharacterBase::HandleDeath()
{
	SetAIState(EAIState::Dead);

	// Disable movement and collision.
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Return attack token if held.
	if (bHasAttackToken)
	{
		ReturnAttackToken();
	}

	// Play death montage or enable ragdoll.
	if (DeathMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(DeathMontage, 1.0f);
		}
	}
	else
	{
		// Ragdoll fallback.
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	OnAIDeath.Broadcast();

	// Schedule cleanup.
	FTimerHandle CleanupTimer;
	GetWorldTimerManager().SetTimer(CleanupTimer, this,
		&AAICharacterBase::DestroyAfterDelay, DeathCleanupDelay, false);
}

void AAICharacterBase::HandleStagger()
{
	OnStaggered();

	if (StaggerMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(StaggerMontage, 1.0f);
		}
	}
}

void AAICharacterBase::HandleStaggerRecovery()
{
	OnStaggerRecovered();
}

void AAICharacterBase::DestroyAfterDelay()
{
	Destroy();
}

// ───── Virtual Hooks ─────

void AAICharacterBase::OnEnterCombat_Implementation()
{
	// Base: no-op. Override in subclasses.
}

void AAICharacterBase::OnStaggered_Implementation()
{
	UE_LOG(LogCombatAI, Log, TEXT("%s staggered."), *GetName());
}

void AAICharacterBase::OnStaggerRecovered_Implementation()
{
	UE_LOG(LogCombatAI, Log, TEXT("%s stagger recovered."), *GetName());
}
