// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "EncounterData.generated.h"

class UEnemyProfileData;
class USquadProfileData;

/**
 * FEnemySpawnEntry
 * Defines a single enemy to spawn: which profile and where.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	EEnemyArchetype Archetype = EEnemyArchetype::Rusher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TObjectPtr<UEnemyProfileData> Profile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 SpawnPointIndex = 0;
};

/**
 * FEncounterWave
 * Defines a single wave within an encounter: enemies, tokens, timing.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FEncounterWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	FText WaveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FEnemySpawnEntry> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TObjectPtr<USquadProfileData> SquadProfile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave",
		meta = (ClampMin = "1", ClampMax = "8"))
	int32 AttackTokenCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave",
		meta = (ClampMin = "0", ClampMax = "3"))
	float TimeBetweenSpawns = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	bool bSpawnHealthPickup = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave",
		meta = (ClampMin = "0", ClampMax = "10"))
	float DelayAfterPreviousWave = 3.0f;
};

/**
 * UEncounterData
 *
 * Master Data Asset that defines an entire encounter (all waves).
 * The Encounter Manager reads this asset to drive wave progression.
 */
UCLASS(BlueprintType)
class COMBATAI_API UEncounterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
	FText EncounterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
	TArray<FEncounterWave> Waves;

	UFUNCTION(BlueprintPure, Category = "Encounter")
	int32 GetTotalWaveCount() const { return Waves.Num(); }

	UFUNCTION(BlueprintPure, Category = "Encounter")
	int32 GetTotalEnemyCount() const;
};
