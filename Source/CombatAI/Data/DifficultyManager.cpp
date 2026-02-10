// Copyright CombatAI Project. All Rights Reserved.

#include "DifficultyManager.h"
#include "CombatAI/Data/DifficultyProfileData.h"
#include "CombatAI/CombatAI.h"

void UDifficultyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetDifficulty(1); // Default to Normal.
}

void UDifficultyManager::SetDifficulty(int32 PresetIndex)
{
	CurrentPresetIndex = FMath::Clamp(PresetIndex, 0, 2);

	switch (CurrentPresetIndex)
	{
	case 0: ActiveProfile = EasyProfile; break;
	case 1: ActiveProfile = NormalProfile; break;
	case 2: ActiveProfile = HardProfile; break;
	}

	UE_LOG(LogCombatAI, Log, TEXT("Difficulty set to preset %d (%s)"),
		CurrentPresetIndex,
		ActiveProfile ? *ActiveProfile->DifficultyName.ToString() : TEXT("None"));

	OnDifficultyChanged.Broadcast(CurrentPresetIndex);
}

float UDifficultyManager::GetEnemyHealthMultiplier() const
{
	return ActiveProfile ? ActiveProfile->EnemyHealthMultiplier : 1.0f;
}

float UDifficultyManager::GetEnemyDamageMultiplier() const
{
	return ActiveProfile ? ActiveProfile->EnemyDamageMultiplier : 1.0f;
}

int32 UDifficultyManager::GetMaxAttackTokens() const
{
	return ActiveProfile ? ActiveProfile->MaxAttackTokens : 2;
}

float UDifficultyManager::GetRangedAccuracy() const
{
	return ActiveProfile ? ActiveProfile->RangedAccuracy : 0.7f;
}

float UDifficultyManager::GetPlayerHealth() const
{
	return ActiveProfile ? ActiveProfile->PlayerHealth : 100.0f;
}

void UDifficultyManager::UpdateDynamicDifficulty(float DamageTaken, int32 DeathCount,
	float AverageWaveTime, int32 ParriesLanded)
{
	if (!bDynamicDifficultyEnabled) return;

	// Simple heuristic: if player is struggling, ease up; if dominating, increase.
	float PerformanceScore = 0.0f;

	// Death penalty.
	PerformanceScore -= DeathCount * 20.0f;

	// Damage taken penalty.
	PerformanceScore -= DamageTaken * 0.2f;

	// Parries reward.
	PerformanceScore += ParriesLanded * 5.0f;

	// Fast wave clear reward.
	if (AverageWaveTime < 60.0f) PerformanceScore += 10.0f;

	// Adjust difficulty.
	if (PerformanceScore < -30.0f && CurrentPresetIndex > 0)
	{
		SetDifficulty(CurrentPresetIndex - 1);
		UE_LOG(LogCombatAI, Log, TEXT("Dynamic difficulty: Lowering (score: %.1f)"), PerformanceScore);
	}
	else if (PerformanceScore > 30.0f && CurrentPresetIndex < 2)
	{
		SetDifficulty(CurrentPresetIndex + 1);
		UE_LOG(LogCombatAI, Log, TEXT("Dynamic difficulty: Raising (score: %.1f)"), PerformanceScore);
	}
}
