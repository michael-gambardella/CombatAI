// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "CombatAI/AI/UtilityAI/UtilityAction.h"
#include "EnemyProfileData.generated.h"

/**
 * UEnemyProfileData
 *
 * Designer-editable Data Asset containing all parameters for an enemy archetype.
 * Assigned to enemy characters at spawn — no code changes needed to tweak behaviors.
 * Supports creating variants (e.g., DA_Rusher_Elite with different stats).
 */
UCLASS(BlueprintType)
class COMBATAI_API UEnemyProfileData : public UDataAsset
{
	GENERATED_BODY()

public:
	// ───── Identity ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity",
		meta = (ToolTip = "Display name for this enemy"))
	FName EnemyName = TEXT("Enemy");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity",
		meta = (ToolTip = "Archetype classification"))
	EEnemyArchetype Archetype = EEnemyArchetype::None;

	// ───── Stats ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (ClampMin = "1", ClampMax = "500", ToolTip = "Maximum hit points"))
	float MaxHealth = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (ClampMin = "100", ClampMax = "1000", ToolTip = "Movement speed in cm/s"))
	float MoveSpeed = 650.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (ClampMin = "0", ClampMax = "200", ToolTip = "Stagger threshold (accumulated damage to trigger)"))
	float StaggerThreshold = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats",
		meta = (ClampMin = "0.1", ClampMax = "3.0", ToolTip = "How long stagger lasts"))
	float StaggerDuration = 0.8f;

	// ───── Perception ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception",
		meta = (ClampMin = "500", ClampMax = "5000", ToolTip = "Max sight range in cm"))
	float DetectionRange = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception",
		meta = (ClampMin = "30", ClampMax = "360", ToolTip = "Primary FOV in degrees"))
	float FieldOfView = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception",
		meta = (ClampMin = "0", ClampMax = "5000", ToolTip = "Hearing range in cm"))
	float HearingRange = 1500.0f;

	// ───── Behavior ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "How eagerly this enemy engages"))
	float AggressionLevel = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Tendency to flank vs. direct approach"))
	float FlankPreference = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "HP % threshold triggering retreat"))
	float RetreatHealthThreshold = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior",
		meta = (ClampMin = "0.1", ClampMax = "5.0", ToolTip = "Min seconds between attack strings"))
	float AttackCooldown = 1.0f;

	// ───── Armor (Warden) ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Frontal damage reduction (0=none, 1=immune)"))
	float FrontalArmorReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor",
		meta = (ClampMin = "1", ClampMax = "5", ToolTip = "Back weak-point damage multiplier"))
	float WeakPointMultiplier = 1.0f;

	// ───── Ranged (Watcher) ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Base shot accuracy"))
	float AccuracyBase = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged",
		meta = (ClampMin = "500", ClampMax = "5000", ToolTip = "Minimum optimal engagement range"))
	float OptimalRangeMin = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged",
		meta = (ClampMin = "1000", ClampMax = "6000", ToolTip = "Maximum optimal engagement range"))
	float OptimalRangeMax = 2500.0f;

	// ───── Utility Actions ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UtilityAI",
		meta = (ToolTip = "The set of actions this enemy evaluates each decision tick"))
	TArray<FUtilityAction> AvailableActions;

	// ───── Visual ─────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	FLinearColor AccentColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	float Scale = 1.0f;

	/** Apply this profile's values to an AI character and its components. */
	UFUNCTION(BlueprintCallable, Category = "Data")
	void ApplyToCharacter(class AAICharacterBase* Character) const;
};
