// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "CombatComponent.generated.h"

/**
 * UCombatComponent
 *
 * Shared combat interface attached to both the player and AI characters.
 * Provides a unified damage pipeline: any actor with this component can deal
 * and receive damage through the same API. Handles:
 *   - Damage application with directional modifiers (frontal armor, weak point)
 *   - Stagger accumulation and threshold checking
 *   - Death detection
 *   - Delegate broadcasting for VFX, audio, and UI
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	// ───── Damage Pipeline ─────

	/**
	 * Apply damage to this component's owner.
	 * Runs through the full pipeline: armor → multiplier → stagger check → health reduction.
	 * @param InDamage  Damage info struct with amount, direction, type, etc.
	 * @return          Actual damage dealt after modifiers.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float ReceiveDamage(const FDamageInfo& InDamage);

	/** Build a damage info struct and apply it to a target's CombatComponent. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static bool DealDamage(AActor* Target, const FDamageInfo& DamageInfo);

	// ───── Health (delegated from owner or managed here for AI) ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Health",
		meta = (ClampMin = "1", ClampMax = "500"))
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Health")
	float CurrentHealth = 100.0f;

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Health")
	bool IsAlive() const;

	// ───── Stagger ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stagger",
		meta = (ClampMin = "0", ClampMax = "200", ToolTip = "Accumulated stagger damage needed to trigger stagger"))
	float StaggerThreshold = 20.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Stagger")
	float CurrentStaggerAccum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stagger",
		meta = (ClampMin = "0.1", ClampMax = "5.0", ToolTip = "Time in seconds before stagger accumulation decays"))
	float StaggerDecayDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stagger",
		meta = (ClampMin = "1", ClampMax = "100", ToolTip = "Stagger decay rate per second"))
	float StaggerDecayRate = 15.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat|Stagger")
	bool bIsStaggered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stagger",
		meta = (ClampMin = "0.1", ClampMax = "3.0", ToolTip = "Duration of the stagger state"))
	float StaggerDuration = 0.8f;

	UFUNCTION(BlueprintPure, Category = "Combat|Stagger")
	float GetStaggerPercent() const;

	// ───── Armor (optional, used by Warden) ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Armor",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Frontal damage reduction (0 = none, 1 = immune)"))
	float FrontalArmorReduction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Armor",
		meta = (ClampMin = "1", ClampMax = "5", ToolTip = "Damage multiplier for weak-point hits"))
	float WeakPointMultiplier = 1.0f;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceived, const FDamageInfo&, DamageInfo);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnDamageReceived OnDamageReceived;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggered);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnStaggered OnStaggered;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaggerRecovered);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnStaggerRecovered OnStaggerRecovered;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnDeath OnDeath;

	/** Reset health and stagger for respawn / restart. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCombatState();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Apply directional armor modifiers to incoming damage. */
	float ApplyArmorModifiers(float BaseDamage, const FVector& HitDirection, bool& bOutWeakPoint) const;

	/** Process stagger accumulation and trigger stagger state if threshold met. */
	void ProcessStagger(float StaggerValue);

	/** End stagger state after duration. */
	void EndStagger();

	float StaggerDecayTimer = 0.0f;
	FTimerHandle StaggerTimerHandle;
};
