// Copyright CombatAI Project. All Rights Reserved.

#include "AttackTokenSystem.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/CombatAI.h"
#include "Kismet/GameplayStatics.h"

void UAttackTokenSystem::Initialize(int32 InMaxTokens, float InPriorityWeightDistance,
	float InPriorityWeightFairness, float InHoldDuration)
{
	MaxTokens = FMath::Max(1, InMaxTokens);
	PriorityWeightDistance = InPriorityWeightDistance;
	PriorityWeightFairness = InPriorityWeightFairness;
	HoldDuration = InHoldDuration;
}

bool UAttackTokenSystem::RequestToken(AAICharacterBase* Requester)
{
	if (!Requester) return false;

	// Already holding a token?
	if (ActiveHolders.Contains(Requester)) return true;

	// Token available?
	if (ActiveHolders.Num() < MaxTokens)
	{
		ActiveHolders.Add(Requester);
		HoldTimers.Add(Requester, 0.0f);
		Requester->GrantAttackToken();
		UE_LOG(LogCombatAI, Log, TEXT("Token GRANTED to %s (%d/%d active)"),
			*Requester->GetName(), ActiveHolders.Num(), MaxTokens);
		return true;
	}

	// Queue the request.
	FTokenRequest Request;
	Request.Requester = Requester;
	Request.Priority = CalculatePriority(Requester);
	Request.RequestTime = Requester->GetWorld()->GetTimeSeconds();

	// Don't double-queue.
	for (const FTokenRequest& Existing : PendingQueue)
	{
		if (Existing.Requester == Requester) return false;
	}

	PendingQueue.Add(Request);
	return false;
}

void UAttackTokenSystem::ReturnToken(AAICharacterBase* Holder)
{
	if (!Holder) return;

	if (ActiveHolders.Remove(Holder) > 0)
	{
		HoldTimers.Remove(Holder);
		Holder->ReturnAttackToken();
		UE_LOG(LogCombatAI, Log, TEXT("Token RETURNED by %s (%d/%d active)"),
			*Holder->GetName(), ActiveHolders.Num(), MaxTokens);

		// Process queue to grant next request.
		ProcessQueue();
	}
}

void UAttackTokenSystem::Tick(float DeltaTime)
{
	EnforceTimeouts(DeltaTime);
	ProcessQueue();
}

bool UAttackTokenSystem::HasToken(AAICharacterBase* Character) const
{
	return ActiveHolders.Contains(Character);
}

TArray<AAICharacterBase*> UAttackTokenSystem::GetTokenHolders() const
{
	TArray<AAICharacterBase*> Result;
	for (const auto& Holder : ActiveHolders)
	{
		if (Holder) Result.Add(Holder);
	}
	return Result;
}

void UAttackTokenSystem::RemoveEnemy(AAICharacterBase* Enemy)
{
	ReturnToken(Enemy);
	PendingQueue.RemoveAll([Enemy](const FTokenRequest& R) { return R.Requester == Enemy; });
}

// ───── Internal ─────

void UAttackTokenSystem::ProcessQueue()
{
	while (ActiveHolders.Num() < MaxTokens && PendingQueue.Num() > 0)
	{
		// Sort by priority (highest first).
		PendingQueue.Sort([](const FTokenRequest& A, const FTokenRequest& B)
		{
			return A.Priority > B.Priority;
		});

		FTokenRequest Best = PendingQueue[0];
		PendingQueue.RemoveAt(0);

		if (Best.Requester && Best.Requester->GetAIState() != EAIState::Dead)
		{
			ActiveHolders.Add(Best.Requester);
			HoldTimers.Add(Best.Requester, 0.0f);
			Best.Requester->GrantAttackToken();

			UE_LOG(LogCombatAI, Log, TEXT("Token GRANTED from queue to %s"),
				*Best.Requester->GetName());
		}
	}
}

float UAttackTokenSystem::CalculatePriority(AAICharacterBase* Requester) const
{
	if (!Requester) return 0.0f;

	float Priority = 0.0f;

	// Distance factor (closer = higher priority).
	AActor* Player = UGameplayStatics::GetPlayerCharacter(Requester->GetWorld(), 0);
	if (Player)
	{
		float Dist = FVector::Dist(Requester->GetActorLocation(), Player->GetActorLocation());
		float NormDist = FMath::Clamp(1.0f - (Dist / 2500.0f), 0.0f, 1.0f);
		Priority += NormDist * PriorityWeightDistance;
	}

	// Fairness factor (longer wait = higher priority).
	float TimeSinceRequest = Requester->GetWorld()->GetTimeSeconds();
	Priority += FMath::Clamp(TimeSinceRequest * 0.1f, 0.0f, 1.0f) * PriorityWeightFairness;

	return Priority;
}

void UAttackTokenSystem::EnforceTimeouts(float DeltaTime)
{
	TArray<AAICharacterBase*> TimedOut;

	for (auto& Pair : HoldTimers)
	{
		Pair.Value += DeltaTime;
		if (Pair.Value >= HoldDuration)
		{
			TimedOut.Add(Pair.Key);
		}
	}

	for (AAICharacterBase* Enemy : TimedOut)
	{
		UE_LOG(LogCombatAI, Warning, TEXT("Token TIMED OUT for %s (held %.1fs)"),
			*Enemy->GetName(), HoldDuration);
		ReturnToken(Enemy);
	}
}
