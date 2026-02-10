// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SquadProfileData.generated.h"

/**
 * USquadProfileData
 *
 * Designer-editable configuration for squad coordination behavior.
 * Controls attack tokens, flanking aggressiveness, and tactical responses.
 */
UCLASS(BlueprintType)
class COMBATAI_API USquadProfileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tokens",
		meta = (ClampMin = "1", ClampMax = "8", ToolTip = "Max simultaneous attacks"))
	int32 MaxConcurrentAttacks = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tokens",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Distance priority weight for token allocation"))
	float TokenPriority_Distance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tokens",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Fairness weight (longer wait = higher priority)"))
	float TokenPriority_TimeSinceAttack = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tokens",
		meta = (ClampMin = "1", ClampMax = "10", ToolTip = "Max seconds an enemy can hold a token"))
	float TokenHoldDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flanking",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "How strongly enemies prefer non-frontal sectors"))
	float FlankPreference = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flanking",
		meta = (ClampMin = "0.5", ClampMax = "5", ToolTip = "How often flanking sectors are re-evaluated"))
	float SectorReassignmentInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flanking",
		meta = (ClampMin = "50", ClampMax = "500", ToolTip = "Minimum spacing between enemies in same sector"))
	float MinSectorSpacing = 150.0f;
};
