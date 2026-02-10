// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EncounterManager.generated.h"

class UEncounterData;
class ASquadCoordinator;
class AAICharacterBase;

/**
 * AEncounterManager
 *
 * Drives wave progression for the arena encounter:
 *   1. Reads an EncounterData asset for wave definitions
 *   2. Spawns enemies at designated spawn points per wave
 *   3. Creates a SquadCoordinator per wave
 *   4. Tracks enemy deaths and advances waves
 *   5. Signals game-over or victory to the GameMode
 *
 * One instance per encounter (usually one per level).
 */
UCLASS()
class COMBATAI_API AEncounterManager : public AActor
{
	GENERATED_BODY()

public:
	AEncounterManager();

	// ───── Configuration ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter",
		meta = (ToolTip = "The encounter definition to use"))
	TObjectPtr<UEncounterData> EncounterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter",
		meta = (ToolTip = "Spawn point actors placed in the level"))
	TArray<TObjectPtr<AActor>> SpawnPoints;

	// ───── State ─────

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	int32 CurrentWaveIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	bool bEncounterActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	int32 EnemiesRemainingInWave = 0;

	// ───── API ─────

	/** Start the encounter from Wave 0. */
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void StartEncounter();

	/** Manually advance to the next wave (for testing). */
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void AdvanceWave();

	/** Get the total number of waves. */
	UFUNCTION(BlueprintPure, Category = "Encounter")
	int32 GetTotalWaves() const;

	/** Get the current wave number (1-based). */
	UFUNCTION(BlueprintPure, Category = "Encounter")
	int32 GetCurrentWaveNumber() const { return CurrentWaveIndex + 1; }

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int32, WaveNumber);
	UPROPERTY(BlueprintAssignable, Category = "Encounter")
	FOnWaveStarted OnWaveStarted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCompleted, int32, WaveNumber);
	UPROPERTY(BlueprintAssignable, Category = "Encounter")
	FOnWaveCompleted OnWaveCompleted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterComplete);
	UPROPERTY(BlueprintAssignable, Category = "Encounter")
	FOnEncounterComplete OnEncounterComplete;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, AAICharacterBase*, Enemy);
	UPROPERTY(BlueprintAssignable, Category = "Encounter")
	FOnEnemyKilled OnEnemyKilled;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ASquadCoordinator> ActiveCoordinator;

	UPROPERTY()
	TArray<TObjectPtr<AAICharacterBase>> SpawnedEnemies;

	void SpawnWave(int32 WaveIndex);
	AAICharacterBase* SpawnEnemy(const struct FEnemySpawnEntry& Entry);
	FTransform GetSpawnTransform(int32 SpawnPointIndex) const;

	UFUNCTION()
	void HandleEnemyDeath(AAICharacterBase* Enemy);

	void CheckWaveComplete();
};
