// Copyright CombatAI Project. All Rights Reserved.

#include "FlankingSystem.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/Combat/CombatTypes.h"

void UFlankingSystem::Initialize(float InMinSpacing)
{
	MinSpacing = InMinSpacing;
	OccupiedSectors.Init(false, NUM_SECTORS);
}

void UFlankingSystem::AssignSectors(const TArray<AAICharacterBase*>& Members, AActor* Player)
{
	if (!Player) return;

	Assignments.Empty();
	OccupiedSectors.Init(false, NUM_SECTORS);

	// Sort members by distance (closest gets priority assignment).
	TArray<AAICharacterBase*> Sorted = Members;
	Sorted.Sort([Player](const AAICharacterBase& A, const AAICharacterBase& B)
	{
		float DA = FVector::Dist(A.GetActorLocation(), Player->GetActorLocation());
		float DB = FVector::Dist(B.GetActorLocation(), Player->GetActorLocation());
		return DA < DB;
	});

	for (AAICharacterBase* Member : Sorted)
	{
		if (!Member || Member->GetAIState() == EAIState::Dead) continue;

		int32 BestSector = FindBestSector(Member, Player);
		Assignments.Add(Member, BestSector);
		OccupiedSectors[BestSector] = true;
	}
}

int32 UFlankingSystem::GetSectorForMember(AAICharacterBase* Member) const
{
	const int32* Sector = Assignments.Find(Member);
	return Sector ? *Sector : 0;
}

FVector UFlankingSystem::GetSectorPosition(int32 Sector, AActor* Player, float Radius) const
{
	if (!Player) return FVector::ZeroVector;

	const FVector Forward = Player->GetActorForwardVector();
	const float BaseAngle = FMath::Atan2(Forward.Y, Forward.X);
	const float SectorAngleRad = FMath::DegreesToRadians(Sector * SECTOR_ANGLE);
	const float Angle = BaseAngle + SectorAngleRad;

	return Player->GetActorLocation() +
		FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f) * Radius;
}

bool UFlankingSystem::IsSectorOccupied(int32 Sector) const
{
	if (Sector < 0 || Sector >= NUM_SECTORS) return false;
	return OccupiedSectors[Sector];
}

int32 UFlankingSystem::FindBestSector(AAICharacterBase* Member, AActor* Player) const
{
	int32 BestSector = 0;
	float BestScore = -FLT_MAX;

	for (int32 i = 0; i < NUM_SECTORS; ++i)
	{
		if (OccupiedSectors[i]) continue;

		float Score = ScoreSector(i, Member, Player);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestSector = i;
		}
	}

	// If all occupied, find least populated.
	if (BestScore <= -FLT_MAX)
	{
		BestSector = FMath::RandRange(0, NUM_SECTORS - 1);
	}

	return BestSector;
}

float UFlankingSystem::ScoreSector(int32 Sector, AAICharacterBase* Member, AActor* Player) const
{
	float Score = 0.0f;

	// 1. Proximity to current angle (prefer sectors enemy is already near).
	FVector SectorPos = GetSectorPosition(Sector, Player, 400.0f);
	float DistToSector = FVector::Dist(Member->GetActorLocation(), SectorPos);
	Score -= DistToSector * 0.001f; // Slight penalty for distance

	// 2. Archetype preference.
	switch (Member->Archetype)
	{
	case EEnemyArchetype::Rusher:
		// Rushers prefer front and side sectors (0, 1, 7).
		if (Sector <= 1 || Sector >= 7) Score += 20.0f;
		break;

	case EEnemyArchetype::Watcher:
		// Watchers prefer rear sectors (3, 4, 5).
		if (Sector >= 3 && Sector <= 5) Score += 30.0f;
		break;

	case EEnemyArchetype::Warden:
		// Wardens prefer direct front (0, 1, 7) to shield allies.
		if (Sector == 0 || Sector == 1 || Sector == 7) Score += 25.0f;
		break;

	default:
		break;
	}

	// 3. Bonus for rear sectors (universal flanking advantage).
	if (Sector >= 3 && Sector <= 5) Score += 10.0f;

	return Score;
}
