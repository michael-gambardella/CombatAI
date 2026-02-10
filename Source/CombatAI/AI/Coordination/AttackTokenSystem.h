// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttackTokenSystem.generated.h"

class AAICharacterBase;

/**
 * FTokenRequest
 * An enemy's pending request for an attack token.
 */
USTRUCT()
struct FTokenRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AAICharacterBase> Requester = nullptr;

	float Priority = 0.0f;
	float TimeSinceLastAttack = 0.0f;
	float RequestTime = 0.0f;
};

/**
 * UAttackTokenSystem
 *
 * Manages the pool of attack tokens that limit how many enemies can attack
 * the player simultaneously. Enemies must request and receive a token
 * before initiating an attack action.
 *
 * Priority is determined by: proximity, time since last attack, archetype role.
 */
UCLASS(BlueprintType)
class COMBATAI_API UAttackTokenSystem : public UObject
{
	GENERATED_BODY()

public:
	/** Initialize with a max token count. */
	void Initialize(int32 InMaxTokens, float InPriorityWeightDistance = 0.5f,
		float InPriorityWeightFairness = 0.5f, float InHoldDuration = 3.0f);

	/** Request a token. Returns true if granted immediately. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	bool RequestToken(AAICharacterBase* Requester);

	/** Return a token after attack completes. */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void ReturnToken(AAICharacterBase* Holder);

	/** Tick the system (process queue, enforce timeouts). */
	void Tick(float DeltaTime);

	/** Get current number of active tokens. */
	UFUNCTION(BlueprintPure, Category = "Tokens")
	int32 GetActiveTokenCount() const { return ActiveHolders.Num(); }

	UFUNCTION(BlueprintPure, Category = "Tokens")
	int32 GetMaxTokenCount() const { return MaxTokens; }

	/** Change max tokens at runtime (difficulty adjustment). */
	UFUNCTION(BlueprintCallable, Category = "Tokens")
	void SetMaxTokens(int32 NewMax) { MaxTokens = FMath::Max(1, NewMax); }

	/** Check if a specific enemy holds a token. */
	UFUNCTION(BlueprintPure, Category = "Tokens")
	bool HasToken(AAICharacterBase* Character) const;

	/** Get list of current token holders for debug. */
	TArray<AAICharacterBase*> GetTokenHolders() const;

	/** Remove a dead enemy from all token lists. */
	void RemoveEnemy(AAICharacterBase* Enemy);

private:
	int32 MaxTokens = 2;
	float PriorityWeightDistance = 0.5f;
	float PriorityWeightFairness = 0.5f;
	float HoldDuration = 3.0f;

	UPROPERTY()
	TArray<TObjectPtr<AAICharacterBase>> ActiveHolders;

	TArray<FTokenRequest> PendingQueue;
	TMap<AAICharacterBase*, float> HoldTimers;

	void ProcessQueue();
	float CalculatePriority(AAICharacterBase* Requester) const;
	void EnforceTimeouts(float DeltaTime);
};
