// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "RusherCharacter.generated.h"

class UUtilityAIComponent;
class UAICombatPerceptionComponent;
class UAnimMontage;

/**
 * ARusherCharacter — "The Blade"
 *
 * Aggressive melee enemy that sprints toward the player, attacks in combos,
 * lunges to close gaps, circles to find openings, and retreats when wounded.
 * Individually manageable but dangerous in packs.
 *
 * Stats (from GDD):
 *   HP: 40 | Speed: 650 cm/s | Detection: 2500 cm | FOV: 120°
 *   Stagger Threshold: 20 | Aggression: Very High
 */
UCLASS()
class COMBATAI_API ARusherCharacter : public AAICharacterBase
{
	GENERATED_BODY()

public:
	ARusherCharacter();

	// ───── Components ─────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUtilityAIComponent> UtilityAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAICombatPerceptionComponent> PerceptionComp;

	// ───── Behavior Parameters (Designer-Tunable) ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "How eagerly the Rusher closes distance"))
	float AggressionLevel = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Tendency to circle to player's flank"))
	float FlankPreference = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "HP percentage triggering retreat"))
	float RetreatHealthThreshold = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0.1", ClampMax = "5.0", ToolTip = "Minimum seconds between attack strings"))
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "200", ClampMax = "800", ToolTip = "Distance at which lunge activates"))
	float LungeRange = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "100", ClampMax = "500", ToolTip = "Speed while strafing around the player"))
	float CircleSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "1", ClampMax = "5", ToolTip = "Number of hits in a combo string"))
	int32 ComboLength = 2;

	// ───── Attack Montages ─────

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> SlashComboMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> LungeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> CirclingSlashMontage;

	// ───── Attack Damage Values ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float SlashDamage = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float LungeDamage = 20.0f;

	// ───── Execution Interface (called by BT Tasks) ─────

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteChase();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteSlashCombo();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteLunge();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteCircleStrafe();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteRetreat();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteInvestigate();

protected:
	virtual void BeginPlay() override;
	virtual void OnEnterCombat_Implementation() override;

private:
	/** Configure all 7 Utility Actions with their considerations. */
	void ConfigureUtilityActions();
};
