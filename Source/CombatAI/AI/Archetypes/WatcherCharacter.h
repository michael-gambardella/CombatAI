// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "WatcherCharacter.generated.h"

class UUtilityAIComponent;
class UAICombatPerceptionComponent;

/**
 * AWatcherCharacter — "The Watcher"
 *
 * Ranged supporter that maintains distance, suppresses the player with projectiles,
 * seeks cover and high ground, and retreats when threatened.
 *
 * Stats: HP 30 | Speed 400 cm/s | Detection 4000 cm | Optimal Range 1500–2500 cm
 * Stagger Threshold: 10 (very fragile)
 */
UCLASS()
class COMBATAI_API AWatcherCharacter : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AWatcherCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUtilityAIComponent> UtilityAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAICombatPerceptionComponent> PerceptionComp;

	// ───── Behavior Parameters ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "500", ClampMax = "4000"))
	float OptimalRangeMin = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "1000", ClampMax = "5000"))
	float OptimalRangeMax = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "100", ClampMax = "1000"))
	float RetreatDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0.5", ClampMax = "3.0"))
	float ShotWindUpTime = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior")
	bool bSuppressionModeEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "200", ClampMax = "2000"))
	float CoverSearchRadius = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1"))
	float ElevationPreference = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1"))
	float AccuracyBase = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1"))
	float AccuracyMoving = 0.4f;

	// ───── Damage Values ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float AimedShotDamage = 18.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float OverwatchBurstDamage = 8.0f;

	// ───── Montages ─────

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> AimedShotMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> BurstFireMontage;

	// ───── Actions ─────

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteAimedShot();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteOverwatchBurst();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteSeekCover();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteSeekHighGround();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteRetreat();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteReposition();

	/** Spawn and fire a projectile toward the player. */
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void FireProjectile(float Damage, float Accuracy);

protected:
	virtual void BeginPlay() override;
	virtual void OnEnterCombat_Implementation() override;

private:
	void ConfigureUtilityActions();
};
