// Copyright CombatAI Project. All Rights Reserved.

#include "SquadCoordinator.h"
#include "AttackTokenSystem.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/Data/SquadProfileData.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "Kismet/GameplayStatics.h"

ASquadCoordinator::ASquadCoordinator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASquadCoordinator::BeginPlay()
{
	Super::BeginPlay();

	TokenSystem = NewObject<UAttackTokenSystem>(this);
	TokenSystem->Initialize(2);
}

void ASquadCoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TokenSystem)
	{
		TokenSystem->Tick(DeltaTime);
	}

	// Flanking updates.
	SectorReassignTimer -= DeltaTime;
	if (SectorReassignTimer <= 0.0f)
	{
		SectorReassignTimer = SectorReassignInterval;
		UpdateFlankingSectors();
	}

	// Tactical situation check.
	TacticalCheckTimer -= DeltaTime;
	if (TacticalCheckTimer <= 0.0f)
	{
		TacticalCheckTimer = 0.5f;
		EvaluateTacticalSituation();
	}

	UpdateAggroTarget();
}

// ───── Initialization ─────

void ASquadCoordinator::InitializeSquad(const TArray<AAICharacterBase*>& Members,
	USquadProfileData* Profile)
{
	SquadMembers = Members;

	if (Profile && TokenSystem)
	{
		TokenSystem->Initialize(
			Profile->MaxConcurrentAttacks,
			Profile->TokenPriority_Distance,
			Profile->TokenPriority_TimeSinceAttack,
			Profile->TokenHoldDuration);

		SectorReassignInterval = Profile->SectorReassignmentInterval;
	}

	// Register death callbacks.
	for (AAICharacterBase* Member : SquadMembers)
	{
		if (Member)
		{
			Member->OnAIDeath.AddDynamic(this, &ASquadCoordinator::HandleMemberDeath);
		}
	}

	UpdateFlankingSectors();

	UE_LOG(LogCombatAI, Log, TEXT("Squad initialized with %d members"), SquadMembers.Num());
}

// ───── Member Management ─────

void ASquadCoordinator::RegisterMember(AAICharacterBase* Member)
{
	if (Member && !SquadMembers.Contains(Member))
	{
		SquadMembers.Add(Member);
		Member->OnAIDeath.AddDynamic(this, &ASquadCoordinator::HandleMemberDeath);
	}
}

void ASquadCoordinator::UnregisterMember(AAICharacterBase* Member)
{
	SquadMembers.Remove(Member);
	SectorAssignments.Remove(Member);
	if (TokenSystem)
	{
		TokenSystem->RemoveEnemy(Member);
	}
}

int32 ASquadCoordinator::GetAliveCount() const
{
	int32 Count = 0;
	for (const auto& Member : SquadMembers)
	{
		if (Member && Member->GetAIState() != EAIState::Dead)
		{
			Count++;
		}
	}
	return Count;
}

void ASquadCoordinator::HandleMemberDeath()
{
	// Find which member died.
	for (auto It = SquadMembers.CreateIterator(); It; ++It)
	{
		AAICharacterBase* Member = *It;
		if (Member && Member->GetAIState() == EAIState::Dead)
		{
			OnMemberDied.Broadcast(Member);
			UnregisterMember(Member);
			break;
		}
	}
}

// ───── Flanking ─────

void ASquadCoordinator::UpdateFlankingSectors()
{
	AActor* Player = GetPlayer();
	if (!Player) return;

	SectorAssignments.Empty();
	TArray<bool> OccupiedSectors;
	OccupiedSectors.Init(false, 8);

	const FVector PlayerPos = Player->GetActorLocation();
	const FVector PlayerForward = Player->GetActorForwardVector();

	for (AAICharacterBase* Member : SquadMembers)
	{
		if (!Member || Member->GetAIState() == EAIState::Dead) continue;

		// Calculate which sector this enemy is currently closest to.
		FVector ToEnemy = (Member->GetActorLocation() - PlayerPos).GetSafeNormal2D();
		float Angle = FMath::Atan2(ToEnemy.Y, ToEnemy.X) - FMath::Atan2(PlayerForward.Y, PlayerForward.X);
		Angle = FMath::Fmod(Angle + 2 * PI, 2 * PI);
		int32 BestSector = FMath::FloorToInt(Angle / (PI / 4.0f)) % 8;

		// Prefer unoccupied sectors; prefer rear sectors (4,5,6 = behind player).
		int32 AssignedSector = BestSector;
		if (OccupiedSectors[BestSector])
		{
			// Find nearest unoccupied sector, preferring rear.
			for (int32 Offset = 1; Offset <= 4; ++Offset)
			{
				int32 CW = (BestSector + Offset) % 8;
				int32 CCW = (BestSector - Offset + 8) % 8;

				if (!OccupiedSectors[CW]) { AssignedSector = CW; break; }
				if (!OccupiedSectors[CCW]) { AssignedSector = CCW; break; }
			}
		}

		SectorAssignments.Add(Member, AssignedSector);
		OccupiedSectors[AssignedSector] = true;
	}
}

int32 ASquadCoordinator::GetAssignedSector(AAICharacterBase* Member) const
{
	const int32* Sector = SectorAssignments.Find(Member);
	return Sector ? *Sector : 0;
}

FVector ASquadCoordinator::GetSectorWorldPosition(int32 SectorIndex, float Radius) const
{
	AActor* Player = GetPlayer();
	if (!Player) return FVector::ZeroVector;

	const FVector PlayerPos = Player->GetActorLocation();
	const FVector PlayerForward = Player->GetActorForwardVector();

	float BaseAngle = FMath::Atan2(PlayerForward.Y, PlayerForward.X);
	float SectorAngle = BaseAngle + (SectorIndex * PI / 4.0f);

	return PlayerPos + FVector(FMath::Cos(SectorAngle), FMath::Sin(SectorAngle), 0.0f) * Radius;
}

// ───── Aggro ─────

void ASquadCoordinator::UpdateAggroTarget()
{
	AActor* Player = GetPlayer();
	if (!Player) return;

	// Determine which enemy the player is focused on (closest to player facing direction).
	float BestDot = -2.0f;
	AAICharacterBase* BestTarget = nullptr;

	for (AAICharacterBase* Member : SquadMembers)
	{
		if (!Member || Member->GetAIState() == EAIState::Dead) continue;

		FVector ToEnemy = (Member->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();
		float Dot = FVector::DotProduct(Player->GetActorForwardVector(), ToEnemy);

		if (Dot > BestDot)
		{
			BestDot = Dot;
			BestTarget = Member;
		}
	}

	CurrentAggroTarget = BestTarget;
}

bool ASquadCoordinator::IsAggroTarget(AAICharacterBase* Member) const
{
	return CurrentAggroTarget == Member;
}

// ───── Tactical Situations ─────

void ASquadCoordinator::EvaluateTacticalSituation()
{
	AActor* Player = GetPlayer();
	if (!Player) return;

	int32 Alive = GetAliveCount();

	// Squad Depleted.
	if (Alive <= 1)
	{
		SetTacticalSituation(ETacticalSituation::SquadDepleted);
		return;
	}

	// Player Low Health.
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(Player))
	{
		if (PC->StatsComponent && PC->StatsComponent->GetHealthPercent() < 0.25f)
		{
			SetTacticalSituation(ETacticalSituation::PlayerLowHealth);
			return;
		}
	}

	// Player Surrounded (3+ in different sectors).
	{
		TSet<int32> OccupiedSectors;
		for (const auto& Pair : SectorAssignments)
		{
			OccupiedSectors.Add(Pair.Value);
		}
		if (OccupiedSectors.Num() >= 3)
		{
			SetTacticalSituation(ETacticalSituation::PlayerSurrounded);
			return;
		}
	}

	// Player at Range (all enemies far).
	{
		bool bAllFar = true;
		for (AAICharacterBase* Member : SquadMembers)
		{
			if (!Member || Member->GetAIState() == EAIState::Dead) continue;
			float Dist = FVector::Dist(Member->GetActorLocation(), Player->GetActorLocation());
			if (Dist < 1000.0f) { bAllFar = false; break; }
		}
		if (bAllFar && Alive > 0)
		{
			SetTacticalSituation(ETacticalSituation::PlayerAtRange);
			return;
		}
	}

	SetTacticalSituation(ETacticalSituation::None);
}

void ASquadCoordinator::SetTacticalSituation(ETacticalSituation NewSituation)
{
	if (CurrentSituation == NewSituation) return;

	CurrentSituation = NewSituation;
	OnTacticalSituationChanged.Broadcast(NewSituation);

	UE_LOG(LogCombatAI, Log, TEXT("Tactical situation: %s"),
		*UEnum::GetValueAsString(NewSituation));
}

AActor* ASquadCoordinator::GetPlayer() const
{
	return UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}
