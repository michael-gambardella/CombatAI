// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "WardenCharacter.generated.h"

class UUtilityAIComponent;
class UAICombatPerceptionComponent;

/**
 * AWardenCharacter — "The Warden"
 *
 * Heavy tank that controls space with AOE attacks, charges to disrupt positioning,
 * blocks frontal attacks with a shield, protects weaker allies, and has a
 * critical weak point on its back.
 *
 * Stats: HP 150 | Speed 250 cm/s | Charge Speed 800 cm/s | Detection 2000 cm
 * Frontal Armor: 50% reduction | Weak Point: 2x damage | Stagger Threshold: 60
 */
UCLASS()
class COMBATAI_API AWardenCharacter : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AWardenCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UUtilityAIComponent> UtilityAI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAICombatPerceptionComponent> PerceptionComp;

	// ───── Behavior Parameters ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1"))
	float FrontalArmorValue = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "1", ClampMax = "5"))
	float WeakPointMultValue = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "200", ClampMax = "1500"))
	float ChargeDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "2", ClampMax = "15"))
	float ChargeCooldown = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "100", ClampMax = "500"))
	float SlamRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "200", ClampMax = "1500"))
	float BodyguardRange = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "30", ClampMax = "720"))
	float TurnSpeed = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior",
		meta = (ClampMin = "0", ClampMax = "1"))
	float EnrageHealthThreshold = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Behavior")
	float ChargeSpeed = 800.0f;

	// ───── Damage Values ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float SlamDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float ChargeDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float BashDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage")
	float StompDamage = 20.0f;

	// ───── Montages ─────

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> SlamMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> ChargeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> BashMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> StompMontage;

	// ───── State ─────

	UPROPERTY(BlueprintReadOnly, Category = "AI|State")
	bool bIsEnraged = false;

	UPROPERTY(BlueprintReadOnly, Category = "AI|State")
	bool bIsCharging = false;

	// ───── Actions ─────

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteAdvance();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteGroundSlam();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteShieldCharge();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteShieldBash();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteStompWave();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteProtectAlly();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	void ExecuteFacePlayer();

	/** Apply AOE damage around the Warden. */
	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void ApplyAOEDamage(float Radius, float Damage, EStaggerLevel Stagger);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnEnterCombat_Implementation() override;

private:
	void ConfigureUtilityActions();
	void CheckEnrageStatus();
};
