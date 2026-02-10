// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "AIPerceptionComponent.generated.h"

class AAICharacterBase;

/**
 * UAIPerceptionComponent (Custom)
 *
 * Custom perception system for the CombatAI project. Replaces UE5's built-in
 * AIPerception for greater control over:
 *   - Sight: FOV cone + LOS raycasts with configurable frequency
 *   - Hearing: Sound event radius with geometry blocking
 *   - Threat Assessment: Continuous 0-100 scoring with memory decay
 *
 * Updates a FPerceptionData struct on the owning AICharacterBase every tick.
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UAICombatPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAICombatPerceptionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// ───── Sight Configuration ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight",
		meta = (ClampMin = "500", ClampMax = "5000", ToolTip = "Maximum detection range in cm"))
	float DetectionRange = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight",
		meta = (ClampMin = "30", ClampMax = "360", ToolTip = "Primary field of view in degrees"))
	float FieldOfView = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight",
		meta = (ClampMin = "90", ClampMax = "360", ToolTip = "Peripheral detection angle (requires more stimuli)"))
	float PeripheralFOV = 270.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Sight",
		meta = (ClampMin = "0.05", ClampMax = "1.0", ToolTip = "LOS raycast frequency in seconds"))
	float SightCheckInterval = 0.2f;

	// ───── Hearing Configuration ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Hearing",
		meta = (ClampMin = "0", ClampMax = "5000", ToolTip = "Hearing radius in cm"))
	float HearingRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Hearing")
	bool bCanHear = true;

	// ───── Threat Configuration ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Threat",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Weight for proximity in threat calculation"))
	float ThreatWeight_Proximity = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Threat",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Weight for player aggression in threat calculation"))
	float ThreatWeight_PlayerAction = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Threat",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Weight for received damage in threat calculation"))
	float ThreatWeight_DamageReceived = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Threat",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Weight for ally casualties in threat calculation"))
	float ThreatWeight_AllyCasualties = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception|Threat",
		meta = (ClampMin = "1", ClampMax = "50", ToolTip = "Threat decay rate per second"))
	float ThreatDecayRate = 10.0f;

	// ───── Output ─────

	UFUNCTION(BlueprintPure, Category = "Perception")
	const FPerceptionData& GetPerceptionData() const { return CachedPerception; }

	// ───── Sound Event Interface ─────

	/** Register an external sound event at a world location. */
	UFUNCTION(BlueprintCallable, Category = "Perception|Hearing")
	void RegisterSoundEvent(const FVector& Location, float Loudness);

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerceptionUpdated, const FPerceptionData&, Data);
	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnPerceptionUpdated OnPerceptionUpdated;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSpotted);
	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnPlayerSpotted OnPlayerSpotted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLost);
	UPROPERTY(BlueprintAssignable, Category = "Perception")
	FOnPlayerLost OnPlayerLost;

protected:
	virtual void BeginPlay() override;

private:
	FPerceptionData CachedPerception;

	float SightCheckTimer = 0.0f;
	float DamageReceivedDecayTimer = 0.0f;
	bool bWasPlayerVisible = false;

	// ───── Internal ─────

	AActor* FindPlayer() const;
	bool CheckLineOfSight(AActor* Target) const;
	bool IsInFOV(AActor* Target, float FOVAngle) const;
	float CalculateThreatLevel(AActor* Player, float DeltaTime);
	void UpdateSight(AActor* Player);
	void UpdateHearing(AActor* Player);
	void UpdateThreat(AActor* Player, float DeltaTime);
	int32 CountNearbyAllies() const;
};
