// Copyright CombatAI Project. All Rights Reserved.

#include "UtilityAction.h"
#include "CombatAI/CombatAI.h"

float FUtilityAction::Evaluate(const FUtilityContext& Context)
{
	if (!IsCooldownReady())
	{
		LastScore = 0.0f;
		return 0.0f;
	}

	if (bRequiresAttackToken && !Context.bHasAttackToken)
	{
		LastScore = 0.0f;
		return 0.0f;
	}

	// Multiplicative scoring across all considerations.
	float Score = 1.0f;

	for (const FUtilityConsideration& Consideration : Considerations)
	{
		const float ConsiderationScore = Consideration.Evaluate(Context);
		Score *= ConsiderationScore;

		// Early out if score drops to zero.
		if (Score <= KINDA_SMALL_NUMBER)
		{
			LastScore = 0.0f;
			return 0.0f;
		}
	}

	// Apply global weight.
	Score *= Weight;

	LastScore = Score;
	return Score;
}

void FUtilityAction::TickCooldown(float DeltaTime)
{
	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(CooldownRemaining - DeltaTime, 0.0f);
	}
}

void FUtilityAction::StartCooldown()
{
	CooldownRemaining = Cooldown;
}
