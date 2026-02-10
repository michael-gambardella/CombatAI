// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResponseCurve.generated.h"

/**
 * Enumerates the available response curve types for Utility AI considerations.
 */
UENUM(BlueprintType)
enum class EResponseCurveType : uint8
{
	Linear    UMETA(DisplayName = "Linear"),
	Quadratic UMETA(DisplayName = "Quadratic"),
	Logistic  UMETA(DisplayName = "Logistic"),
	Step      UMETA(DisplayName = "Step"),
	Inverse   UMETA(DisplayName = "Inverse")
};

/**
 * FResponseCurve
 *
 * A configurable mathematical curve that maps a normalized input (0–1)
 * to a score (0–1). Used by Utility AI considerations to transform
 * world-state values into action scores.
 *
 * Supported types:
 *   Linear:    y = Slope * x + Offset
 *   Quadratic: y = (x ^ Exponent)
 *   Logistic:  y = 1 / (1 + e^(-Steepness * (x - Midpoint)))
 *   Step:      y = (x >= Threshold) ? 1 : 0
 *   Inverse:   y = 1 - x
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FResponseCurve
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	EResponseCurveType CurveType = EResponseCurveType::Linear;

	// ── Linear parameters ──
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Linear",
		meta = (EditCondition = "CurveType == EResponseCurveType::Linear"))
	float Slope = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Linear",
		meta = (EditCondition = "CurveType == EResponseCurveType::Linear"))
	float Offset = 0.0f;

	// ── Quadratic parameters ──
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Quadratic",
		meta = (EditCondition = "CurveType == EResponseCurveType::Quadratic", ClampMin = "0.1", ClampMax = "10"))
	float Exponent = 2.0f;

	// ── Logistic parameters ──
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Logistic",
		meta = (EditCondition = "CurveType == EResponseCurveType::Logistic", ClampMin = "-20", ClampMax = "20"))
	float Steepness = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Logistic",
		meta = (EditCondition = "CurveType == EResponseCurveType::Logistic", ClampMin = "0", ClampMax = "1"))
	float Midpoint = 0.5f;

	// ── Step parameters ──
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve|Step",
		meta = (EditCondition = "CurveType == EResponseCurveType::Step", ClampMin = "0", ClampMax = "1"))
	float Threshold = 0.5f;

	/**
	 * Evaluate the curve for a given input.
	 * @param Input  Normalized value in [0, 1].
	 * @return       Score in [0, 1].
	 */
	float Evaluate(float Input) const;

	/** Create common curve presets. */
	static FResponseCurve MakeLinear(float InSlope = 1.0f, float InOffset = 0.0f);
	static FResponseCurve MakeInverse();
	static FResponseCurve MakeStep(float InThreshold);
	static FResponseCurve MakeLogistic(float InSteepness, float InMidpoint);
	static FResponseCurve MakeQuadratic(float InExponent);
};
