// Copyright CombatAI Project. All Rights Reserved.

#include "UtilityAIComponent.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/AI/Core/AIControllerBase.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "Kismet/GameplayStatics.h"

UUtilityAIComponent::UUtilityAIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();
	// Stagger initial decision timers across enemies.
	DecisionTimer = FMath::RandRange(0.0f, 1.0f / FMath::Max(DecisionRate, 1.0f));
}

void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Tick all action cooldowns.
	for (FUtilityAction& Action : Actions)
	{
		Action.TickCooldown(DeltaTime);
	}

	// Only evaluate in combat state.
	AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner());
	if (!AIChar || AIChar->GetAIState() != EAIState::Combat) return;

	// Decision rate limiter.
	DecisionTimer -= DeltaTime;
	if (DecisionTimer <= 0.0f)
	{
		DecisionTimer = 1.0f / FMath::Max(DecisionRate, 1.0f);
		EvaluateActions();
	}
}

// ───── Evaluation ─────

void UUtilityAIComponent::EvaluateActions()
{
	const FUtilityContext Context = BuildContext();

	for (FUtilityAction& Action : Actions)
	{
		Action.Evaluate(Context);
	}

	FUtilityAction* Best = SelectBestAction();
	if (Best && Best->ActionName != CurrentActionName)
	{
		CurrentActionName = Best->ActionName;
		CurrentActionScore = Best->LastScore;

		UE_LOG(LogCombatAI, Verbose, TEXT("%s selected action: %s (score: %.3f)"),
			*GetOwner()->GetName(), *CurrentActionName.ToString(), CurrentActionScore);

		// Write to blackboard.
		if (AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner()))
		{
			if (AAIControllerBase* AIC = Cast<AAIControllerBase>(AIChar->GetController()))
			{
				AIC->SetBlackboardAction(CurrentActionName);
			}
		}

		OnActionSelected.Broadcast(CurrentActionName, CurrentActionScore);
	}
}

FUtilityAction* UUtilityAIComponent::SelectBestAction()
{
	FUtilityAction* Best = nullptr;
	float BestScore = -1.0f;

	for (FUtilityAction& Action : Actions)
	{
		float Score = Action.LastScore;

		// Add noise for variety.
		if (Score > 0.0f && ScoreNoise > 0.0f)
		{
			Score += FMath::RandRange(-ScoreNoise, ScoreNoise);
			Score = FMath::Max(Score, 0.0f);
		}

		if (Score > BestScore)
		{
			BestScore = Score;
			Best = &Action;
		}
	}

	return (Best && BestScore > KINDA_SMALL_NUMBER) ? Best : nullptr;
}

// ───── Context Building ─────

FUtilityContext UUtilityAIComponent::BuildContext_Implementation() const
{
	FUtilityContext Context;

	AAICharacterBase* AIChar = Cast<AAICharacterBase>(GetOwner());
	if (!AIChar) return Context;

	Context.Perception = AIChar->PerceptionData;
	Context.DistanceToPlayer = AIChar->PerceptionData.DistanceToPlayer;
	Context.bHasAttackToken = AIChar->bHasAttackToken;
	Context.NearbyAllyCount = AIChar->PerceptionData.NearbyAllyCount;

	if (AIChar->CombatComp)
	{
		Context.OwnHealthPercent = AIChar->CombatComp->GetHealthPercent();
	}

	// Calculate player facing dot.
	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		FVector ToAI = (AIChar->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();
		Context.PlayerFacingDot = FVector::DotProduct(Player->GetActorForwardVector(), ToAI);
	}

	return Context;
}

// ───── Queries ─────

TArray<FName> UUtilityAIComponent::GetActionNames() const
{
	TArray<FName> Names;
	for (const FUtilityAction& Action : Actions)
	{
		Names.Add(Action.ActionName);
	}
	return Names;
}

float UUtilityAIComponent::GetActionScore(FName ActionName) const
{
	for (const FUtilityAction& Action : Actions)
	{
		if (Action.ActionName == ActionName)
		{
			return Action.LastScore;
		}
	}
	return 0.0f;
}

void UUtilityAIComponent::NotifyActionCompleted(FName ActionName)
{
	for (FUtilityAction& Action : Actions)
	{
		if (Action.ActionName == ActionName)
		{
			Action.StartCooldown();
			break;
		}
	}
}
