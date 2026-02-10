// Copyright CombatAI Project. All Rights Reserved.

#include "StaggerSystem.h"

bool UStaggerSystem::WouldStagger(float CurrentAccumulation, float StaggerValue, float Threshold)
{
	return (CurrentAccumulation + StaggerValue) >= Threshold;
}

float UStaggerSystem::GetStaggerProgress(float CurrentAccumulation, float Threshold)
{
	if (Threshold <= 0.0f) return 1.0f;
	return FMath::Clamp(CurrentAccumulation / Threshold, 0.0f, 1.0f);
}

float UStaggerSystem::StaggerLevelToFloat(EStaggerLevel Level)
{
	return CombatUtils::StaggerLevelToValue(Level);
}
