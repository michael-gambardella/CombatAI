// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "AIControllerBase.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class AAICharacterBase;

/**
 * AAIControllerBase
 *
 * Base AI controller for all enemy archetypes. Manages:
 *   - Behavior Tree lifecycle (start/stop/pause)
 *   - Blackboard key management for AI state
 *   - Interface between Utility AI decisions and BT execution
 *   - Perception tick scheduling
 */
UCLASS()
class COMBATAI_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	// ───── Blackboard Key Names ─────

	static const FName BB_TargetActor;
	static const FName BB_CurrentAction;
	static const FName BB_AIState;
	static const FName BB_LastKnownPosition;
	static const FName BB_DistanceToPlayer;
	static const FName BB_ThreatLevel;
	static const FName BB_HasAttackToken;

	// ───── Behavior Tree ─────

	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree")
	TObjectPtr<UBlackboardData> BlackboardAsset;

	/** Start or restart the behavior tree. */
	UFUNCTION(BlueprintCallable, Category = "AI|BehaviorTree")
	void StartAIBehavior();

	/** Stop the behavior tree. */
	UFUNCTION(BlueprintCallable, Category = "AI|BehaviorTree")
	void StopAIBehavior();

	/** Pause/unpause AI for debug stepping. */
	UFUNCTION(BlueprintCallable, Category = "AI|BehaviorTree")
	void SetAIPaused(bool bPaused);

	// ───── Blackboard Helpers ─────

	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetBlackboardTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetBlackboardAction(const FName& ActionName);

	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void SetBlackboardAIState(EAIState State);

	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard")
	void UpdateBlackboardPerception(const FPerceptionData& Data);

	// ───── Access ─────

	UFUNCTION(BlueprintPure, Category = "AI")
	AAICharacterBase* GetAICharacter() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BTComponent;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BBComponent;
};
