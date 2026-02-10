// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/AI/UtilityAI/UtilityConsideration.h"
#include "UtilityAction.generated.h"

/**
 * FUtilityAction
 *
 * Represents a single candidate action the AI can take.
 * Contains a list of considerations and a global weight.
 * The Utility AI Component evaluates all actions each tick and selects the highest.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FUtilityAction
{
	GENERATED_BODY()

	/** Unique action name (matches Behavior Tree action routing). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FName ActionName = TEXT("Unnamed");

	/** Global weight multiplier for this action's final score. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action",
		meta = (ClampMin = "0", ClampMax = "5"))
	float Weight = 1.0f;

	/** Considerations that contribute to this action's score. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	TArray<FUtilityConsideration> Considerations;

	/** Cooldown between executions of this action (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action",
		meta = (ClampMin = "0", ClampMax = "10"))
	float Cooldown = 0.0f;

	/** Whether this action requires an attack token. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool bRequiresAttackToken = false;

	// ── Runtime state (not serialized) ──

	/** Current cooldown timer remaining. */
	float CooldownRemaining = 0.0f;

	/** Last computed score. */
	float LastScore = 0.0f;

	/**
	 * Evaluate this action against the current world context.
	 * @return Final action score (0 = won't select, higher = preferred).
	 */
	float Evaluate(const FUtilityContext& Context);

	/** Tick cooldown timer. */
	void TickCooldown(float DeltaTime);

	/** Reset cooldown (called after action execution). */
	void StartCooldown();

	/** Is the action off cooldown? */
	bool IsCooldownReady() const { return CooldownRemaining <= 0.0f; }
};
