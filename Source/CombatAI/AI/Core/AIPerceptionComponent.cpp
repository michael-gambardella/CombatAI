// Copyright CombatAI Project. All Rights Reserved.

#include "AIPerceptionComponent.h"
#include "AICharacterBase.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UAICombatPerceptionComponent::UAICombatPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAICombatPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	SightCheckTimer = FMath::RandRange(0.0f, SightCheckInterval); // Stagger initial checks
}

void UAICombatPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Player = FindPlayer();
	if (!Player) return;

	// Staggered sight checks for performance.
	SightCheckTimer -= DeltaTime;
	if (SightCheckTimer <= 0.0f)
	{
		SightCheckTimer = SightCheckInterval;
		UpdateSight(Player);
	}

	UpdateHearing(Player);
	UpdateThreat(Player, DeltaTime);

	// Update distance.
	CachedPerception.DistanceToPlayer = FVector::Dist(GetOwner()->GetActorLocation(), Player->GetActorLocation());

	// Update ally count.
	CachedPerception.NearbyAllyCount = CountNearbyAllies();

	// Track time since last seen.
	if (!CachedPerception.bCanSeePlayer)
	{
		CachedPerception.TimeSinceLastSeen += DeltaTime;
	}
	else
	{
		CachedPerception.TimeSinceLastSeen = 0.0f;
	}

	// Check player attacking state.
	if (const APlayerCharacter* PC = Cast<APlayerCharacter>(Player))
	{
		CachedPerception.bPlayerIsAttacking = 
			(PC->CombatState == EPlayerCombatState::LightAttack ||
			 PC->CombatState == EPlayerCombatState::HeavyRelease ||
			 PC->CombatState == EPlayerCombatState::RangedAttack);
		CachedPerception.bPlayerIsStaggered = (PC->CombatState == EPlayerCombatState::Staggered);
	}

	// Decay damage received.
	CachedPerception.DamageReceivedRecently = FMath::Max(
		CachedPerception.DamageReceivedRecently - 10.0f * DeltaTime, 0.0f);

	// Write perception to owning character.
	if (AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner()))
	{
		AIChar->PerceptionData = CachedPerception;
	}

	OnPerceptionUpdated.Broadcast(CachedPerception);
}

// ───── Sight ─────

void UAICombatPerceptionComponent::UpdateSight(AActor* Player)
{
	const bool bPrevSeen = CachedPerception.bCanSeePlayer;
	CachedPerception.bCanSeePlayer = false;

	if (!Player) return;

	const float Dist = FVector::Dist(GetOwner()->GetActorLocation(), Player->GetActorLocation());
	if (Dist > DetectionRange) return;

	// Primary FOV check.
	if (IsInFOV(Player, FieldOfView))
	{
		if (CheckLineOfSight(Player))
		{
			CachedPerception.bCanSeePlayer = true;
			CachedPerception.LastKnownPlayerPosition = Player->GetActorLocation();
		}
	}
	// Peripheral check (wider angle, still need LOS).
	else if (IsInFOV(Player, PeripheralFOV))
	{
		if (CheckLineOfSight(Player))
		{
			// Peripheral detection: update last known but don't set full visibility
			// unless already in combat state.
			AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner());
			if (AIChar && AIChar->GetAIState() == EAIState::Combat)
			{
				CachedPerception.bCanSeePlayer = true;
			}
			CachedPerception.LastKnownPlayerPosition = Player->GetActorLocation();
		}
	}

	// Fire events on visibility change.
	if (CachedPerception.bCanSeePlayer && !bPrevSeen)
	{
		OnPlayerSpotted.Broadcast();
		UE_LOG(LogCombatAI, Verbose, TEXT("%s spotted the player."), *GetOwner()->GetName());
	}
	else if (!CachedPerception.bCanSeePlayer && bPrevSeen)
	{
		OnPlayerLost.Broadcast();
		UE_LOG(LogCombatAI, Verbose, TEXT("%s lost sight of the player."), *GetOwner()->GetName());
	}
}

bool UAICombatPerceptionComponent::IsInFOV(AActor* Target, float FOVAngle) const
{
	const FVector ToTarget = (Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const float Dot = FVector::DotProduct(Forward, ToTarget);
	const float HalfAngleRad = FMath::DegreesToRadians(FOVAngle * 0.5f);
	return Dot >= FMath::Cos(HalfAngleRad);
}

bool UAICombatPerceptionComponent::CheckLineOfSight(AActor* Target) const
{
	FVector Start = GetOwner()->GetActorLocation() + FVector(0, 0, 60.0f); // Eye height
	FVector End = Target->GetActorLocation() + FVector(0, 0, 60.0f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Target);

	bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	return !bBlocked;
}

// ───── Hearing ─────

void UAICombatPerceptionComponent::UpdateHearing(AActor* Player)
{
	CachedPerception.bCanHearPlayer = false;
	if (!bCanHear || !Player) return;

	const float Dist = FVector::Dist(GetOwner()->GetActorLocation(), Player->GetActorLocation());

	// Simplified: player sprinting or attacking within hearing range.
	if (Dist <= HearingRange)
	{
		if (const APlayerCharacter* PC = Cast<APlayerCharacter>(Player))
		{
			if (PC->bIsSprinting || PC->CombatState == EPlayerCombatState::LightAttack ||
				PC->CombatState == EPlayerCombatState::HeavyRelease)
			{
				CachedPerception.bCanHearPlayer = true;
			}
		}
	}
}

void UAICombatPerceptionComponent::RegisterSoundEvent(const FVector& Location, float Loudness)
{
	const float Dist = FVector::Dist(GetOwner()->GetActorLocation(), Location);
	if (Dist <= HearingRange * Loudness)
	{
		CachedPerception.bCanHearPlayer = true;
		CachedPerception.LastKnownPlayerPosition = Location;
	}
}

// ───── Threat ─────

void UAICombatPerceptionComponent::UpdateThreat(AActor* Player, float DeltaTime)
{
	if (!Player)
	{
		CachedPerception.ThreatLevel = FMath::Max(CachedPerception.ThreatLevel - ThreatDecayRate * DeltaTime, 0.0f);
		return;
	}

	float NewThreat = 0.0f;

	// Proximity factor (closer = higher threat).
	const float Dist = CachedPerception.DistanceToPlayer;
	const float ProximityScore = FMath::Clamp(1.0f - (Dist / DetectionRange), 0.0f, 1.0f) * 100.0f;
	NewThreat += ProximityScore * ThreatWeight_Proximity;

	// Player action factor.
	float ActionScore = 0.0f;
	if (CachedPerception.bPlayerIsAttacking) ActionScore = 80.0f;
	else if (CachedPerception.bPlayerIsStaggered) ActionScore = 10.0f;
	else ActionScore = 30.0f; // Idle/moving
	NewThreat += ActionScore * ThreatWeight_PlayerAction;

	// Damage received factor.
	const float DamageScore = FMath::Clamp(CachedPerception.DamageReceivedRecently * 2.0f, 0.0f, 100.0f);
	NewThreat += DamageScore * ThreatWeight_DamageReceived;

	// Ally casualties (simplified — lower ally count = higher threat).
	const float MaxAllies = 8.0f;
	const float AllyCasualtiesScore = FMath::Clamp(
		(1.0f - CachedPerception.NearbyAllyCount / MaxAllies) * 100.0f, 0.0f, 100.0f);
	NewThreat += AllyCasualtiesScore * ThreatWeight_AllyCasualties;

	// Blend with current threat (memory — doesn't snap).
	if (CachedPerception.bCanSeePlayer || CachedPerception.bCanHearPlayer)
	{
		CachedPerception.ThreatLevel = FMath::FInterpTo(
			CachedPerception.ThreatLevel, NewThreat, DeltaTime, 3.0f);
	}
	else
	{
		// Decay when no perception.
		CachedPerception.ThreatLevel = FMath::Max(
			CachedPerception.ThreatLevel - ThreatDecayRate * DeltaTime, 0.0f);
	}
}

// ───── Helpers ─────

AActor* UAICombatPerceptionComponent::FindPlayer() const
{
	return UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

int32 UAICombatPerceptionComponent::CountNearbyAllies() const
{
	int32 Count = 0;
	const float AllyRange = 2000.0f;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(AllyRange);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	GetWorld()->OverlapMultiByChannel(
		Overlaps, GetOwner()->GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere, Params);

	for (const FOverlapResult& Overlap : Overlaps)
	{
		if (AAICharacterBase* Ally = Cast<AAICharacterBase>(Overlap.GetActor()))
		{
			if (Ally->GetAIState() != EAIState::Dead)
			{
				Count++;
			}
		}
	}

	return Count;
}
