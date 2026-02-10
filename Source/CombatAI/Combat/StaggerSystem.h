// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "StaggerSystem.generated.h"

/**
 * UStaggerSystem
 *
 * Static utility library for stagger-related calculations.
 * Stagger logic is primarily handled inside CombatComponent,
 * but this class provides shared query functions and helpers
 * used by both player and AI.
 */
UCLASS()
class COMBATAI_API UStaggerSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Check if a given stagger value would trigger stagger on a target. */
	UFUNCTION(BlueprintPure, Category = "Combat|Stagger")
	static bool WouldStagger(float CurrentAccumulation, float StaggerValue, float Threshold);

	/** Get stagger severity as a 0-1 percentage toward threshold. */
	UFUNCTION(BlueprintPure, Category = "Combat|Stagger")
	static float GetStaggerProgress(float CurrentAccumulation, float Threshold);

	/** Convert EStaggerLevel to its numeric stagger value. */
	UFUNCTION(BlueprintPure, Category = "Combat|Stagger")
	static float StaggerLevelToFloat(EStaggerLevel Level);
};
