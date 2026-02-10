// Copyright CombatAI Project. All Rights Reserved.

#include "CombatAIGameState.h"

ACombatAIGameState::ACombatAIGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatAIGameState::ResetStats()
{
	CurrentWave = 0;
	EnemiesRemaining = 0;
	TotalDamageTaken = 0.0f;
	TotalParriesLanded = 0;
	TotalEnemiesKilled = 0;
	CurrentWaveTime = 0.0f;
	TotalPlayTime = 0.0f;
}

void ACombatAIGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TotalPlayTime += DeltaSeconds;
	CurrentWaveTime += DeltaSeconds;
}
