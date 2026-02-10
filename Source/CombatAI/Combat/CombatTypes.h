// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatTypes.generated.h"

/**
 * Enumerates all enemy archetype categories.
 * Used for identification, data asset assignment, and debug display.
 */
UENUM(BlueprintType)
enum class EEnemyArchetype : uint8
{
	None     UMETA(DisplayName = "None"),
	Rusher   UMETA(DisplayName = "Rusher"),
	Watcher  UMETA(DisplayName = "Watcher"),
	Warden   UMETA(DisplayName = "Warden")
};

/**
 * Enumerates the high-level AI states that govern enemy behavior phases.
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle       UMETA(DisplayName = "Idle"),
	Alert      UMETA(DisplayName = "Alert"),
	Combat     UMETA(DisplayName = "Combat"),
	Searching  UMETA(DisplayName = "Searching"),
	Dead       UMETA(DisplayName = "Dead")
};

/**
 * Enumerates stagger severity levels applied by attacks.
 */
UENUM(BlueprintType)
enum class EStaggerLevel : uint8
{
	None   UMETA(DisplayName = "None"),
	Low    UMETA(DisplayName = "Low"),
	Medium UMETA(DisplayName = "Medium"),
	High   UMETA(DisplayName = "High"),
	Max    UMETA(DisplayName = "Max")
};

/**
 * Enumerates damage types for hit-reaction and audio/VFX selection.
 */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Melee      UMETA(DisplayName = "Melee"),
	Ranged     UMETA(DisplayName = "Ranged"),
	AOE        UMETA(DisplayName = "AOE"),
	Counter    UMETA(DisplayName = "Counter"),
	Charge     UMETA(DisplayName = "Charge")
};

/**
 * Enumerates player combat action states for input management and cancellation rules.
 */
UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	Idle           UMETA(DisplayName = "Idle"),
	LightAttack    UMETA(DisplayName = "Light Attack"),
	HeavyCharge    UMETA(DisplayName = "Heavy Charge"),
	HeavyRelease   UMETA(DisplayName = "Heavy Release"),
	Dodging        UMETA(DisplayName = "Dodging"),
	Parrying       UMETA(DisplayName = "Parrying"),
	Staggered      UMETA(DisplayName = "Staggered"),
	RangedAttack   UMETA(DisplayName = "Ranged Attack"),
	Dead           UMETA(DisplayName = "Dead")
};

/**
 * Contains all information about a single damage event.
 * Passed through the damage pipeline from source to target.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FDamageInfo
{
	GENERATED_BODY()

	/** Raw damage amount before any modifiers (armor, weak point). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Amount = 0.0f;

	/** Stagger severity this attack inflicts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EStaggerLevel StaggerLevel = EStaggerLevel::None;

	/** Numerical stagger value for threshold comparison. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float StaggerValue = 0.0f;

	/** The actor that dealt this damage. */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	TObjectPtr<AActor> DamageSource = nullptr;

	/** World-space direction the damage came from (Source → Target). */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	FVector HitDirection = FVector::ZeroVector;

	/** World-space location of the hit impact. */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	FVector HitLocation = FVector::ZeroVector;

	/** Type classification for VFX / audio selection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDamageType DamageType = EDamageType::Melee;

	/** Whether this hit was a critical/weak-point hit. */
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	bool bIsWeakPointHit = false;
};

/**
 * AI Perception data snapshot — updated every perception tick.
 */
USTRUCT(BlueprintType)
struct COMBATAI_API FPerceptionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	bool bCanSeePlayer = false;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	bool bCanHearPlayer = false;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	FVector LastKnownPlayerPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	float TimeSinceLastSeen = 999.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	float ThreatLevel = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	float DistanceToPlayer = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	bool bPlayerIsAttacking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	bool bPlayerIsStaggered = false;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	int32 NearbyAllyCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Perception")
	float DamageReceivedRecently = 0.0f;
};

/**
 * Utility function library for combat calculations.
 */
namespace CombatUtils
{
	/** Convert a stagger enum level to its numerical value. */
	inline float StaggerLevelToValue(EStaggerLevel Level)
	{
		switch (Level)
		{
		case EStaggerLevel::None:   return 0.0f;
		case EStaggerLevel::Low:    return 5.0f;
		case EStaggerLevel::Medium: return 15.0f;
		case EStaggerLevel::High:   return 30.0f;
		case EStaggerLevel::Max:    return 60.0f;
		default:                    return 0.0f;
		}
	}
}
