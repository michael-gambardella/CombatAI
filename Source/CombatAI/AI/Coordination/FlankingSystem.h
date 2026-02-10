// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlankingSystem.generated.h"

class AAICharacterBase;

/**
 * UFlankingSystem
 *
 * Manages the 8-sector flanking ring around the player.
 * Assigns sectors to enemies based on approach angle, availability,
 * and archetype preference (Rushers → front/sides, Watchers → rear).
 */
UCLASS(BlueprintType)
class COMBATAI_API UFlankingSystem : public UObject
{
	GENERATED_BODY()

public:
	static constexpr int32 NUM_SECTORS = 8;
	static constexpr float SECTOR_ANGLE = 45.0f; // 360 / 8

	void Initialize(float InMinSpacing = 150.0f);

	/** Assign sectors to all active squad members. */
	void AssignSectors(const TArray<AAICharacterBase*>& Members, AActor* Player);

	/** Get the assigned sector index for a specific member. */
	int32 GetSectorForMember(AAICharacterBase* Member) const;

	/** Get the world position for a sector at a given radius. */
	FVector GetSectorPosition(int32 Sector, AActor* Player, float Radius) const;

	/** Check if a sector is occupied. */
	bool IsSectorOccupied(int32 Sector) const;

	/** Get sector occupancy for debug display. */
	TArray<bool> GetOccupancy() const { return OccupiedSectors; }

private:
	TMap<AAICharacterBase*, int32> Assignments;
	TArray<bool> OccupiedSectors;
	float MinSpacing = 150.0f;

	int32 FindBestSector(AAICharacterBase* Member, AActor* Player) const;
	float ScoreSector(int32 Sector, AAICharacterBase* Member, AActor* Player) const;
};
