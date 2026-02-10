// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAI/AI/UtilityAI/UtilityAction.h"
#include "UtilityAIComponent.generated.h"

class AAICharacterBase;
class AAIControllerBase;

/**
 * UUtilityAIComponent
 *
 * The brain of the Utility AI system. Evaluates all candidate actions
 * each decision tick, selects the highest scoring action, and communicates
 * the decision to the Behavior Tree via the blackboard.
 *
 * Attached to each AI character. Each archetype configures its own set
 * of FUtilityAction entries with archetype-specific considerations.
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUtilityAIComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// ───── Actions ─────

	/** Set of candidate actions this AI can choose from. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	TArray<FUtilityAction> Actions;

	/** Decision tick rate in Hz (evaluations per second). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI",
		meta = (ClampMin = "1", ClampMax = "30", ToolTip = "How many times per second the AI re-evaluates"))
	float DecisionRate = 5.0f;

	/** Small random noise added to scores for behavioral variety. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI",
		meta = (ClampMin = "0", ClampMax = "0.2"))
	float ScoreNoise = 0.05f;

	// ───── Output ─────

	/** The name of the currently selected action. */
	UPROPERTY(BlueprintReadOnly, Category = "UtilityAI")
	FName CurrentActionName = NAME_None;

	/** The score of the currently selected action. */
	UPROPERTY(BlueprintReadOnly, Category = "UtilityAI")
	float CurrentActionScore = 0.0f;

	/** Get all action names and their last scores for debug display. */
	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	TArray<FName> GetActionNames() const;

	UFUNCTION(BlueprintPure, Category = "UtilityAI")
	float GetActionScore(FName ActionName) const;

	// ───── Context Building ─────

	/** Build the utility context from current world state. Override for custom inputs. */
	UFUNCTION(BlueprintNativeEvent, Category = "UtilityAI")
	FUtilityContext BuildContext() const;
	virtual FUtilityContext BuildContext_Implementation() const;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionSelected, FName, ActionName, float, Score);
	UPROPERTY(BlueprintAssignable, Category = "UtilityAI")
	FOnActionSelected OnActionSelected;

	/** Notify the component that an action has completed execution. */
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void NotifyActionCompleted(FName ActionName);

protected:
	virtual void BeginPlay() override;

private:
	float DecisionTimer = 0.0f;

	void EvaluateActions();
	FUtilityAction* SelectBestAction();
};
