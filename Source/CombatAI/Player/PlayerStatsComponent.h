// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"

/**
 * UPlayerStatsComponent
 *
 * Manages the player's core resources: Health, Stamina, and Ammo.
 * Fires delegates on value changes so HUD and game systems can respond.
 * All base values are designer-tunable.
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStatsComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// ───── Health ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health",
		meta = (ClampMin = "1", ClampMax = "500", ToolTip = "Maximum health points"))
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats|Health")
	float CurrentHealth = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	void ApplyDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	void Heal(float Amount);

	UFUNCTION(BlueprintPure, Category = "Stats|Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Stats|Health")
	bool IsAlive() const;

	// ───── Stamina ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina",
		meta = (ClampMin = "1", ClampMax = "200", ToolTip = "Maximum stamina points"))
	float MaxStamina = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stats|Stamina")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina",
		meta = (ClampMin = "1", ClampMax = "100", ToolTip = "Stamina regeneration per second"))
	float StaminaRegenRate = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina",
		meta = (ClampMin = "0", ClampMax = "3", ToolTip = "Delay before stamina starts regenerating after use"))
	float StaminaRegenDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina",
		meta = (ClampMin = "1", ClampMax = "50", ToolTip = "Stamina cost per dodge roll"))
	float DodgeStaminaCost = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Stamina",
		meta = (ClampMin = "1", ClampMax = "50", ToolTip = "Stamina cost per second of sprinting"))
	float SprintStaminaCost = 15.0f;

	UFUNCTION(BlueprintCallable, Category = "Stats|Stamina")
	bool ConsumeStamina(float Amount);

	UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintPure, Category = "Stats|Stamina")
	bool HasStamina(float Amount) const;

	// ───── Ammo ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Ammo",
		meta = (ClampMin = "1", ClampMax = "50", ToolTip = "Maximum ranged ammo capacity"))
	int32 MaxAmmo = 12;

	UPROPERTY(BlueprintReadOnly, Category = "Stats|Ammo")
	int32 CurrentAmmo = 12;

	UFUNCTION(BlueprintCallable, Category = "Stats|Ammo")
	bool ConsumeAmmo(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Stats|Ammo")
	void ReplenishAmmo(int32 Amount);

	// ───── Reset ─────

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetAllStats();

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnHealthChanged OnHealthChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStaminaChanged OnStaminaChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, int32, NewAmmo, int32, MaxAmmo);
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnAmmoChanged OnAmmoChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;

private:
	/** Timer tracking delay before stamina regeneration resumes. */
	float StaminaRegenTimer = 0.0f;

	/** Whether stamina is currently regenerating. */
	bool bIsRegenerating = true;
};
