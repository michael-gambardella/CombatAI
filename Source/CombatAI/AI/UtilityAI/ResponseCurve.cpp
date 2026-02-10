// Copyright CombatAI Project. All Rights Reserved.

#include "ResponseCurve.h"

float FResponseCurve::Evaluate(float Input) const
{
	Input = FMath::Clamp(Input, 0.0f, 1.0f);

	float Result = 0.0f;

	switch (CurveType)
	{
	case EResponseCurveType::Linear:
		Result = Slope * Input + Offset;
		break;

	case EResponseCurveType::Quadratic:
		Result = FMath::Pow(Input, Exponent);
		break;

	case EResponseCurveType::Logistic:
		Result = 1.0f / (1.0f + FMath::Exp(-Steepness * (Input - Midpoint)));
		break;

	case EResponseCurveType::Step:
		Result = (Input >= Threshold) ? 1.0f : 0.0f;
		break;

	case EResponseCurveType::Inverse:
		Result = 1.0f - Input;
		break;
	}

	return FMath::Clamp(Result, 0.0f, 1.0f);
}

FResponseCurve FResponseCurve::MakeLinear(float InSlope, float InOffset)
{
	FResponseCurve C;
	C.CurveType = EResponseCurveType::Linear;
	C.Slope = InSlope;
	C.Offset = InOffset;
	return C;
}

FResponseCurve FResponseCurve::MakeInverse()
{
	FResponseCurve C;
	C.CurveType = EResponseCurveType::Inverse;
	return C;
}

FResponseCurve FResponseCurve::MakeStep(float InThreshold)
{
	FResponseCurve C;
	C.CurveType = EResponseCurveType::Step;
	C.Threshold = InThreshold;
	return C;
}

FResponseCurve FResponseCurve::MakeLogistic(float InSteepness, float InMidpoint)
{
	FResponseCurve C;
	C.CurveType = EResponseCurveType::Logistic;
	C.Steepness = InSteepness;
	C.Midpoint = InMidpoint;
	return C;
}

FResponseCurve FResponseCurve::MakeQuadratic(float InExponent)
{
	FResponseCurve C;
	C.CurveType = EResponseCurveType::Quadratic;
	C.Exponent = InExponent;
	return C;
}
