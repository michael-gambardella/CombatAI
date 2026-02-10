// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DifficultyManager.generated.h"

class UDifficultyProfileData;

/**
 * UDifficultyManager
 *
 * Game Instance subsystem that manages difficulty presets.
 * Persists across level transitions. Applies global modifiers
 * to enemy stats, player stats, and encounter parameters.
 */
UCLASS()
class COMBATAI_API UDifficultyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// ───── Preset Selection ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	TObjectPtr<UDifficultyProfileData> EasyProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	TObjectPtr<UDifficultyProfileData> NormalProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	TObjectPtr<UDifficultyProfileData> HardProfile;

	UPROPERTY(BlueprintReadOnly, Category = "Difficulty")
	TObjectPtr<UDifficultyProfileData> ActiveProfile;

	UPROPERTY(BlueprintReadOnly, Category = "Difficulty")
	int32 CurrentPresetIndex = 1; // 0=Easy, 1=Normal, 2=Hard

	UFUNCTION(BlueprintCallable, Category = "Difficulty")
	void SetDifficulty(int32 PresetIndex);

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	UDifficultyProfileData* GetActiveProfile() const { return ActiveProfile; }

	// ───── Getters (for applying to systems) ─────

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	float GetEnemyHealthMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	float GetEnemyDamageMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	int32 GetMaxAttackTokens() const;

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	float GetRangedAccuracy() const;

	UFUNCTION(BlueprintPure, Category = "Difficulty")
	float GetPlayerHealth() const;

	// ───── Dynamic Difficulty (Optional) ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty|Dynamic")
	bool bDynamicDifficultyEnabled = false;

	/** Update dynamic difficulty based on player performance metrics. */
	UFUNCTION(BlueprintCallable, Category = "Difficulty|Dynamic")
	void UpdateDynamicDifficulty(float DamageTaken, int32 DeathCount,
		float AverageWaveTime, int32 ParriesLanded);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, int32, NewPreset);
	UPROPERTY(BlueprintAssignable, Category = "Difficulty")
	FOnDifficultyChanged OnDifficultyChanged;
};
