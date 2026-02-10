// Copyright CombatAI Project. All Rights Reserved.

#include "CoverSystem.h"
#include "CombatAI/CombatAI.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// ───── ACoverNode ─────

ACoverNode::ACoverNode()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	// Editor visualization.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
#endif
}

void ACoverNode::Occupy(AActor* InOccupant)
{
	bIsOccupied = true;
	Occupant = InOccupant;
}

void ACoverNode::Vacate()
{
	bIsOccupied = false;
	Occupant = nullptr;
}

// ───── UCoverSystem ─────

void UCoverSystem::DiscoverCoverNodes(UWorld* World)
{
	CoverNodes.Empty();
	if (!World) return;

	for (TActorIterator<ACoverNode> It(World); It; ++It)
	{
		CoverNodes.Add(*It);
	}

	UE_LOG(LogCombatAI, Log, TEXT("CoverSystem: Discovered %d cover nodes"), CoverNodes.Num());
}

ACoverNode* UCoverSystem::FindBestCover(AActor* Requester, AActor* ThreatSource,
	float SearchRadius, bool bPreferElevated)
{
	if (!Requester || !ThreatSource) return nullptr;

	ACoverNode* BestNode = nullptr;
	float BestScore = -FLT_MAX;

	for (ACoverNode* Node : CoverNodes)
	{
		if (!Node || Node->bIsOccupied) continue;

		float Dist = FVector::Dist(Requester->GetActorLocation(), Node->GetActorLocation());
		if (Dist > SearchRadius) continue;

		float Score = ScoreCoverNode(Node, Requester, ThreatSource, bPreferElevated);
		if (Score > BestScore)
		{
			BestScore = Score;
			BestNode = Node;
		}
	}

	return BestNode;
}

void UCoverSystem::ReleaseCover(ACoverNode* Node)
{
	if (Node)
	{
		Node->Vacate();
	}
}

float UCoverSystem::ScoreCoverNode(ACoverNode* Node, AActor* Requester, AActor* Threat,
	bool bPreferElevated) const
{
	float Score = 0.0f;

	// 1. Proximity (closer nodes preferred).
	float Dist = FVector::Dist(Requester->GetActorLocation(), Node->GetActorLocation());
	Score += (1.0f - FMath::Clamp(Dist / 1000.0f, 0.0f, 1.0f)) * 30.0f;

	// 2. Cover direction alignment (how well does this cover protect from threat?).
	FVector ThreatDir = (Threat->GetActorLocation() - Node->GetActorLocation()).GetSafeNormal();
	float Alignment = FVector::DotProduct(Node->CoverDirection.GetSafeNormal(), ThreatDir);
	Score += FMath::Clamp(Alignment, 0.0f, 1.0f) * 40.0f;

	// 3. Cover quality.
	Score += Node->CoverQuality * 20.0f;

	// 4. Elevation bonus for ranged.
	if (bPreferElevated && Node->bIsElevated)
	{
		Score += 25.0f;
	}

	return Score;
}
