// Copyright CombatAI Project. All Rights Reserved.

#include "UtilityConsideration.h"

float FUtilityConsideration::Evaluate(const FUtilityContext& Context) const
{
	float RawInput = 0.0f;

	switch (InputType)
	{
	case EConsiderationInput::DistanceToPlayer:
		RawInput = Context.DistanceToPlayer;
		break;
	case EConsiderationInput::PlayerVisible:
		RawInput = Context.Perception.bCanSeePlayer ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::PlayerInRange:
		RawInput = (Context.DistanceToPlayer <= InputMax) ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::PlayerFacing:
		RawInput = FMath::Clamp((Context.PlayerFacingDot + 1.0f) * 0.5f, 0.0f, 1.0f);
		break;
	case EConsiderationInput::PlayerAttacking:
		RawInput = Context.Perception.bPlayerIsAttacking ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::PlayerStaggered:
		RawInput = Context.Perception.bPlayerIsStaggered ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::OwnHealth:
		RawInput = Context.OwnHealthPercent;
		break;
	case EConsiderationInput::DamageReceivedRecently:
		RawInput = Context.Perception.DamageReceivedRecently;
		break;
	case EConsiderationInput::CooldownReady:
		RawInput = Context.bCooldownReady ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::HasAttackToken:
		RawInput = Context.bHasAttackToken ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::AllyAttacking:
		RawInput = Context.bAllyAttacking ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::AllyInMelee:
		RawInput = Context.bAllyInMelee ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::AllyCount:
		RawInput = static_cast<float>(Context.NearbyAllyCount);
		break;
	case EConsiderationInput::AllyNearby:
		RawInput = Context.bAllyNearby ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::AllyUnderThreat:
		RawInput = Context.bAllyUnderThreat ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::Elevation:
		RawInput = Context.bElevationAvailable ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::NotAtElevation:
		RawInput = Context.bAtElevation ? 0.0f : 1.0f;
		break;
	case EConsiderationInput::Exposed:
		RawInput = Context.bExposed ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::LOSBlocked:
		RawInput = Context.bLOSBlocked ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::ClearPath:
		RawInput = Context.bClearPath ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::HealthBelowEnrage:
		RawInput = Context.bHealthBelowEnrage ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::GroupRequest:
		RawInput = Context.bGroupFlankRequest ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::LastKnownPositionValid:
		RawInput = (Context.Perception.TimeSinceLastSeen < 30.0f &&
			!Context.Perception.LastKnownPlayerPosition.IsZero()) ? 1.0f : 0.0f;
		break;
	case EConsiderationInput::CantSeePlayer:
		RawInput = Context.Perception.bCanSeePlayer ? 0.0f : 1.0f;
		break;
	case EConsiderationInput::ThreatLevel:
		RawInput = Context.Perception.ThreatLevel;
		break;
	case EConsiderationInput::Surrounded:
		RawInput = Context.bSurrounded ? 1.0f : 0.0f;
		break;
	}

	// Normalize to [0, 1].
	float Normalized;
	if (FMath::IsNearlyEqual(InputMax, InputMin))
	{
		Normalized = RawInput >= InputMin ? 1.0f : 0.0f;
	}
	else
	{
		Normalized = FMath::Clamp((RawInput - InputMin) / (InputMax - InputMin), 0.0f, 1.0f);
	}

	// Apply response curve.
	return Curve.Evaluate(Normalized);
}
