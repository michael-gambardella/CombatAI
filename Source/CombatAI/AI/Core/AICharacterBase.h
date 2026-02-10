// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "AICharacterBase.generated.h"

class UCombatComponent;
class UHitDetectionComponent;
class UAnimMontage;

/**
 * AAICharacterBase
 *
 * Base class for all enemy archetypes (Rusher, Watcher, Warden).
 * Provides:
 *   - CombatComponent for shared damage pipeline
 *   - HitDetection for animation-driven attacks
 *   - AI state (EAIState) with transition delegates
 *   - Death handling (ragdoll + cleanup)
 *   - Stagger response
 *   - Virtual hooks for archetype-specific behavior
 *
 * Subclasses override archetype-specific methods without duplicating base logic.
 */
UCLASS(Abstract)
class COMBATAI_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacterBase();

	virtual void Tick(float DeltaTime) override;

	// ───── Components ─────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCombatComponent> CombatComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UHitDetectionComponent> HitDetection;

	// ───── Identity ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Identity")
	EEnemyArchetype Archetype = EEnemyArchetype::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Identity")
	FName EnemyDisplayName = TEXT("Enemy");

	// ───── AI State ─────

	UPROPERTY(BlueprintReadOnly, Category = "AI|State")
	EAIState CurrentAIState = EAIState::Idle;

	UFUNCTION(BlueprintCallable, Category = "AI|State")
	void SetAIState(EAIState NewState);

	UFUNCTION(BlueprintPure, Category = "AI|State")
	EAIState GetAIState() const { return CurrentAIState; }

	// ───── Perception Data (written by perception system) ─────

	UPROPERTY(BlueprintReadOnly, Category = "AI|Perception")
	FPerceptionData PerceptionData;

	// ───── Attack Token ─────

	UPROPERTY(BlueprintReadOnly, Category = "AI|Combat")
	bool bHasAttackToken = false;

	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void GrantAttackToken();

	UFUNCTION(BlueprintCallable, Category = "AI|Combat")
	void ReturnAttackToken();

	// ───── Montages ─────

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> StaggerMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIStateChanged, EAIState, OldState, EAIState, NewState);
	UPROPERTY(BlueprintAssignable, Category = "AI")
	FOnAIStateChanged OnAIStateChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIDeath);
	UPROPERTY(BlueprintAssignable, Category = "AI")
	FOnAIDeath OnAIDeath;

	// ───── Virtual Hooks ─────

	/** Called when entering combat state. Override for archetype-specific setup. */
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void OnEnterCombat();
	virtual void OnEnterCombat_Implementation();

	/** Called when this enemy is staggered. */
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void OnStaggered();
	virtual void OnStaggered_Implementation();

	/** Called when stagger recovery completes. */
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void OnStaggerRecovered();
	virtual void OnStaggerRecovered_Implementation();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleStagger();

	UFUNCTION()
	void HandleStaggerRecovery();

	/** Delayed destruction after death. */
	void DestroyAfterDelay();

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "1", ClampMax = "10"))
	float DeathCleanupDelay = 3.0f;
};
