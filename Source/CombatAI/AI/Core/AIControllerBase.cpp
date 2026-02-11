// Copyright CombatAI Project. All Rights Reserved.

#include "AIControllerBase.h"
#include "AICharacterBase.h"
#include "CombatAI/CombatAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// ───── Blackboard Key Names ─────
const FName AAIControllerBase::BB_TargetActor      = TEXT("TargetActor");
const FName AAIControllerBase::BB_CurrentAction    = TEXT("CurrentAction");
const FName AAIControllerBase::BB_AIState          = TEXT("AIState");
const FName AAIControllerBase::BB_LastKnownPosition = TEXT("LastKnownPosition");
const FName AAIControllerBase::BB_DistanceToPlayer = TEXT("DistanceToPlayer");
const FName AAIControllerBase::BB_ThreatLevel      = TEXT("ThreatLevel");
const FName AAIControllerBase::BB_HasAttackToken   = TEXT("HasAttackToken");

AAIControllerBase::AAIControllerBase()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BlackboardAsset)
	{
		UBlackboardComponent* BBComp = BBComponent.Get();
		UseBlackboard(BlackboardAsset, BBComp);
	}

	StartAIBehavior();

	UE_LOG(LogCombatAI, Log, TEXT("AIController possessed %s"), *InPawn->GetName());
}

void AAIControllerBase::OnUnPossess()
{
	StopAIBehavior();
	Super::OnUnPossess();
}

// ───── Behavior Tree ─────

void AAIControllerBase::StartAIBehavior()
{
	if (BehaviorTreeAsset && BTComponent)
	{
		BTComponent->StartTree(*BehaviorTreeAsset);
		UE_LOG(LogCombatAI, Log, TEXT("BehaviorTree started for %s"), *GetName());
	}
}

void AAIControllerBase::StopAIBehavior()
{
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AAIControllerBase::SetAIPaused(bool bPaused)
{
	if (BTComponent)
	{
		if (bPaused)
		{
			BTComponent->PauseLogic(TEXT("Debug Pause"));
		}
		else
		{
			BTComponent->ResumeLogic(TEXT("Debug Resume"));
		}
	}
}

// ───── Blackboard Helpers ─────

void AAIControllerBase::SetBlackboardTarget(AActor* Target)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsObject(BB_TargetActor, Target);
	}
}

void AAIControllerBase::SetBlackboardAction(const FName& ActionName)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsName(BB_CurrentAction, ActionName);
	}
}

void AAIControllerBase::SetBlackboardAIState(EAIState State)
{
	if (BBComponent)
	{
		BBComponent->SetValueAsEnum(BB_AIState, static_cast<uint8>(State));
	}
}

void AAIControllerBase::UpdateBlackboardPerception(const FPerceptionData& Data)
{
	if (!BBComponent) return;

	BBComponent->SetValueAsFloat(BB_DistanceToPlayer, Data.DistanceToPlayer);
	BBComponent->SetValueAsFloat(BB_ThreatLevel, Data.ThreatLevel);
	BBComponent->SetValueAsVector(BB_LastKnownPosition, Data.LastKnownPlayerPosition);
}

// ───── Access ─────

AAICharacterBase* AAIControllerBase::GetAICharacter() const
{
	return Cast<AAICharacterBase>(GetPawn());
}
