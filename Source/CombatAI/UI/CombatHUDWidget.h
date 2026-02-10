// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class APlayerCharacter;

/**
 * UCombatHUDWidget
 *
 * Main in-game HUD widget displaying player status:
 *   - Health bar
 *   - Stamina bar
 *   - Ammo counter
 *   - Wave indicator
 *   - Enemy count
 *   - Lock-on reticle
 */
UCLASS()
class COMBATAI_API UCombatHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ───── Widget Bindings ─────

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemyCountText;

	// ───── Update Methods ─────

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateAmmo(int32 Current, int32 Max);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateWaveInfo(int32 CurrentWave, int32 TotalWaves);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateEnemyCount(int32 Remaining);

	/** Bind to the player character's delegates. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void BindToPlayer(APlayerCharacter* Player);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION()
	void HandleStaminaChanged(float NewStamina, float MaxStamina);

	UFUNCTION()
	void HandleAmmoChanged(int32 NewAmmo, int32 MaxAmmo);
};
