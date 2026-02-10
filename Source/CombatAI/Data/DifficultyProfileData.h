// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DifficultyProfileData.generated.h"

/**
 * UDifficultyProfileData
 *
 * Designer-editable difficulty preset. Modifies enemy and player stats
 * globally when applied. Three presets (Easy, Normal, Hard) per the GDD.
 */
UCLASS(BlueprintType)
class COMBATAI_API UDifficultyProfileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName DifficultyName = TEXT("Normal");

	// ───── Enemy Modifiers ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float EnemyHealthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float EnemyDamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "1", ClampMax = "8"))
	int32 MaxAttackTokens = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "-1", ClampMax = "2", ToolTip = "Added to AI reaction time (negative = faster)"))
	float AIReactionTimeModifier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "0", ClampMax = "1"))
	float FlankingAggressiveness = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "0", ClampMax = "1"))
	float RangedAccuracy = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy",
		meta = (ClampMin = "0.5", ClampMax = "2.0", ToolTip = "Multiplier on enemy stagger duration"))
	float EnemyStaggerDurationMult = 1.0f;

	// ───── Player Modifiers ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player",
		meta = (ClampMin = "50", ClampMax = "200"))
	float PlayerHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player",
		meta = (ClampMin = "0.5", ClampMax = "2.0"))
	float PlayerStaminaRegenMult = 1.0f;

	// ───── Pickups ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickups",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "0=none, 1=frequent"))
	float HealthPickupFrequency = 0.5f;
};
