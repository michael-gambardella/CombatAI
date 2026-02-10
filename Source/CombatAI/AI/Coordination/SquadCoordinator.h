// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SquadCoordinator.generated.h"

class AAICharacterBase;
class UAttackTokenSystem;
class USquadProfileData;

/**
 * Enumerates tactical situations the squad can recognize and respond to.
 */
UENUM(BlueprintType)
enum class ETacticalSituation : uint8
{
	None              UMETA(DisplayName = "None"),
	PlayerSurrounded  UMETA(DisplayName = "Player Surrounded"),
	AllyDown          UMETA(DisplayName = "Ally Down"),
	PlayerAtRange     UMETA(DisplayName = "Player At Range"),
	PlayerInCover     UMETA(DisplayName = "Player In Cover"),
	PlayerLowHealth   UMETA(DisplayName = "Player Low Health"),
	SquadDepleted     UMETA(DisplayName = "Squad Depleted")
};

/**
 * ASquadCoordinator
 *
 * Central coordinator for group-level tactics. Manages:
 *   - Attack token allocation
 *   - Flanking sector assignment (8 sectors around the player)
 *   - Aggro target tracking
 *   - Tactical situation recognition and response
 *
 * One instance per active encounter wave.
 * Does NOT control individual AI — provides influence via shared state.
 */
UCLASS()
class COMBATAI_API ASquadCoordinator : public AActor
{
	GENERATED_BODY()

public:
	ASquadCoordinator();

	virtual void Tick(float DeltaTime) override;

	// ───── Configuration ─────

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void InitializeSquad(const TArray<AAICharacterBase*>& Members, USquadProfileData* Profile);

	// ───── Member Management ─────

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void RegisterMember(AAICharacterBase* Member);

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void UnregisterMember(AAICharacterBase* Member);

	UFUNCTION(BlueprintPure, Category = "Squad")
	int32 GetAliveCount() const;

	// ───── Attack Tokens ─────

	UPROPERTY(BlueprintReadOnly, Category = "Squad|Tokens")
	TObjectPtr<UAttackTokenSystem> TokenSystem;

	// ───── Flanking ─────

	/** Get the assigned flanking sector index (0–7) for a member. */
	UFUNCTION(BlueprintPure, Category = "Squad|Flanking")
	int32 GetAssignedSector(AAICharacterBase* Member) const;

	/** Get the world position for a member's assigned flanking sector. */
	UFUNCTION(BlueprintPure, Category = "Squad|Flanking")
	FVector GetSectorWorldPosition(int32 SectorIndex, float Radius = 400.0f) const;

	// ───── Aggro ─────

	UPROPERTY(BlueprintReadOnly, Category = "Squad|Aggro")
	TObjectPtr<AAICharacterBase> CurrentAggroTarget;

	UFUNCTION(BlueprintCallable, Category = "Squad|Aggro")
	void UpdateAggroTarget();

	UFUNCTION(BlueprintPure, Category = "Squad|Aggro")
	bool IsAggroTarget(AAICharacterBase* Member) const;

	// ───── Tactical Situations ─────

	UPROPERTY(BlueprintReadOnly, Category = "Squad|Tactics")
	ETacticalSituation CurrentSituation = ETacticalSituation::None;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemberDied, AAICharacterBase*, Member);
	UPROPERTY(BlueprintAssignable, Category = "Squad")
	FOnMemberDied OnMemberDied;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalSituationChanged, ETacticalSituation, Situation);
	UPROPERTY(BlueprintAssignable, Category = "Squad")
	FOnTacticalSituationChanged OnTacticalSituationChanged;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<AAICharacterBase>> SquadMembers;

	TMap<AAICharacterBase*, int32> SectorAssignments;

	float SectorReassignTimer = 0.0f;
	float SectorReassignInterval = 1.0f;
	float TacticalCheckTimer = 0.0f;

	UFUNCTION()
	void HandleMemberDeath();

	void UpdateFlankingSectors();
	void EvaluateTacticalSituation();
	void SetTacticalSituation(ETacticalSituation NewSituation);
	AActor* GetPlayer() const;
};
