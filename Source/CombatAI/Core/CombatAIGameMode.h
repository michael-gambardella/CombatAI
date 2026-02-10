// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatAIGameMode.generated.h"

/**
 * ACombatAIGameMode
 *
 * Root game mode for the CombatAI project.
 * Responsible for:
 *   - Selecting the default pawn, player controller, and HUD classes
 *   - Hosting the encounter lifecycle (delegated to EncounterManager)
 *   - Providing global game-state queries (difficulty, pause)
 */
UCLASS()
class COMBATAI_API ACombatAIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatAIGameMode();

	virtual void BeginPlay() override;

	// ───── Encounter Flow ─────

	/** Start the encounter from Wave 1. Called from UI or BeginPlay. */
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void StartEncounter();

	/** Restart from Wave 1, resetting player and encounter state. */
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void RestartEncounter();

	/** Called when the player dies — triggers Game Over. */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnPlayerDeath();

	/** Called when all waves are cleared — triggers Victory. */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnAllWavesCleared();

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameOver OnGameOver;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVictory);
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnVictory OnVictory;
};
