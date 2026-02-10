// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CombatAIGameState.generated.h"

/**
 * ACombatAIGameState
 *
 * Holds globally-accessible game state: current wave, score, difficulty preset.
 * Replicated in multiplayer-ready fashion even though this project is single-player,
 * to demonstrate best practices.
 */
UCLASS()
class COMBATAI_API ACombatAIGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACombatAIGameState();

	// ───── Wave Tracking ─────

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	int32 CurrentWave = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	int32 TotalWaves = 5;

	UPROPERTY(BlueprintReadOnly, Category = "Encounter")
	int32 EnemiesRemaining = 0;

	// ───── Performance Stats ─────

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float TotalDamageTaken = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TotalParriesLanded = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	int32 TotalEnemiesKilled = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float CurrentWaveTime = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float TotalPlayTime = 0.0f;

	// ───── Difficulty ─────

	UPROPERTY(BlueprintReadWrite, Category = "Difficulty")
	int32 DifficultyPreset = 1; // 0=Easy, 1=Normal, 2=Hard

	// ───── Methods ─────

	/** Reset all stats for a new encounter. */
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetStats();

	virtual void Tick(float DeltaSeconds) override;
};
