// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverSystem.generated.h"

/**
 * ACoverNode
 * A static point in the level that provides cover from a specific direction.
 * Placed by level designers. Evaluated at runtime by AI seeking cover.
 */
UCLASS()
class COMBATAI_API ACoverNode : public AActor
{
	GENERATED_BODY()

public:
	ACoverNode();

	/** Direction this cover protects FROM (world-space). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cover")
	FVector CoverDirection = FVector(1, 0, 0);

	/** How much protection this cover offers (0=none, 1=full). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cover",
		meta = (ClampMin = "0", ClampMax = "1"))
	float CoverQuality = 0.7f;

	/** Whether this cover provides elevation advantage. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cover")
	bool bIsElevated = false;

	/** Whether this node is currently occupied by an AI. */
	UPROPERTY(BlueprintReadOnly, Category = "Cover")
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly, Category = "Cover")
	TObjectPtr<AActor> Occupant = nullptr;

	void Occupy(AActor* InOccupant);
	void Vacate();
};

/**
 * UCoverSystem
 *
 * Evaluates and assigns cover nodes to AI based on:
 *   - Proximity to the requesting AI
 *   - Cover direction vs. player position
 *   - Cover quality
 *   - Availability (not already occupied)
 *   - Elevation bonus for ranged units
 */
UCLASS(BlueprintType)
class COMBATAI_API UCoverSystem : public UObject
{
	GENERATED_BODY()

public:
	/** Register all cover nodes in the level. */
	void DiscoverCoverNodes(UWorld* World);

	/** Find the best cover node for an AI given its position and the player's. */
	UFUNCTION(BlueprintCallable, Category = "Cover")
	ACoverNode* FindBestCover(AActor* Requester, AActor* ThreatSource,
		float SearchRadius = 800.0f, bool bPreferElevated = false);

	/** Release a cover node when AI leaves. */
	UFUNCTION(BlueprintCallable, Category = "Cover")
	void ReleaseCover(ACoverNode* Node);

	/** Get all cover nodes for debug display. */
	TArray<ACoverNode*> GetAllNodes() const { return CoverNodes; }

private:
	UPROPERTY()
	TArray<TObjectPtr<ACoverNode>> CoverNodes;

	float ScoreCoverNode(ACoverNode* Node, AActor* Requester, AActor* Threat,
		bool bPreferElevated) const;
};
