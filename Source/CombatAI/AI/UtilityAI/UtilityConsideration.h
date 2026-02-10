// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAI/AI/UtilityAI/ResponseCurve.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "UtilityConsideration.generated.h"

/**
 * Enumerates world-state inputs that considerations can read.
 */
UENUM(BlueprintType)
enum class EConsiderationInput : uint8
{
	DistanceToPlayer         UMETA(DisplayName = "Distance To Player"),
	PlayerVisible            UMETA(DisplayName = "Player Visible"),
	PlayerInRange            UMETA(DisplayName = "Player In Range"),
	PlayerFacing             UMETA(DisplayName = "Player Facing Me"),
	PlayerAttacking          UMETA(DisplayName = "Player Is Attacking"),
	PlayerStaggered          UMETA(DisplayName = "Player Is Staggered"),
	OwnHealth                UMETA(DisplayName = "Own Health Percent"),
	DamageReceivedRecently   UMETA(DisplayName = "Damage Received Recently"),
	CooldownReady            UMETA(DisplayName = "Cooldown Ready"),
	HasAttackToken           UMETA(DisplayName = "Has Attack Token"),
	AllyAttacking            UMETA(DisplayName = "Ally Is Attacking Player"),
	AllyInMelee              UMETA(DisplayName = "Ally In Melee Range"),
	AllyCount                UMETA(DisplayName = "Nearby Ally Count"),
	AllyNearby               UMETA(DisplayName = "Ally Nearby"),
	AllyUnderThreat          UMETA(DisplayName = "Ally Under Threat"),
	Elevation                UMETA(DisplayName = "Elevation Available"),
	NotAtElevation           UMETA(DisplayName = "Not At Elevation"),
	Exposed                  UMETA(DisplayName = "Currently Exposed"),
	LOSBlocked               UMETA(DisplayName = "LOS Blocked"),
	ClearPath                UMETA(DisplayName = "Clear Path To Player"),
	HealthBelowEnrage        UMETA(DisplayName = "Health Below Enrage"),
	GroupRequest             UMETA(DisplayName = "Group Flank Request"),
	LastKnownPositionValid   UMETA(DisplayName = "Last Known Position Valid"),
	CantSeePlayer            UMETA(DisplayName = "Cannot See Player"),
	ThreatLevel              UMETA(DisplayName = "Threat Level"),
	Surrounded               UMETA(DisplayName = "Player Surrounded")
};

/**
 * FUtilityConsideration
 *
 * A single factor in a Utility AI action evaluation.
 * Reads a world-state input, normalizes it to [0,1], and applies a response curve.
 * Multiple considerations are combined multiplicatively for an action's score.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FUtilityConsideration
{
	GENERATED_BODY()

	/** Human-readable name for debug display. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consideration")
	FName ConsiderationName = TEXT("Unnamed");

	/** Which world-state value this consideration reads. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consideration")
	EConsiderationInput InputType = EConsiderationInput::DistanceToPlayer;

	/** Range for normalizing the raw input to [0,1]. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consideration")
	float InputMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consideration")
	float InputMax = 1.0f;

	/** The response curve to apply to the normalized input. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consideration")
	FResponseCurve Curve;

	/**
	 * Evaluate this consideration given the current perception and context.
	 * @return Score in [0,1].
	 */
	float Evaluate(const struct FUtilityContext& Context) const;
};

/**
 * FUtilityContext
 *
 * Snapshot of world state passed to all considerations during evaluation.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FUtilityContext
{
	GENERATED_BODY()

	UPROPERTY() FPerceptionData Perception;
	UPROPERTY() float OwnHealthPercent = 1.0f;
	UPROPERTY() float DistanceToPlayer = 0.0f;
	UPROPERTY() bool bCooldownReady = true;
	UPROPERTY() bool bHasAttackToken = false;
	UPROPERTY() bool bAllyAttacking = false;
	UPROPERTY() bool bAllyInMelee = false;
	UPROPERTY() int32 NearbyAllyCount = 0;
	UPROPERTY() bool bAllyNearby = false;
	UPROPERTY() bool bAllyUnderThreat = false;
	UPROPERTY() bool bElevationAvailable = false;
	UPROPERTY() bool bAtElevation = false;
	UPROPERTY() bool bExposed = false;
	UPROPERTY() bool bLOSBlocked = false;
	UPROPERTY() bool bClearPath = true;
	UPROPERTY() bool bHealthBelowEnrage = false;
	UPROPERTY() bool bGroupFlankRequest = false;
	UPROPERTY() bool bSurrounded = false;
	UPROPERTY() float PlayerFacingDot = 0.0f; // +1 = facing me, -1 = facing away
};
