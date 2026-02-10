// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "HitDetection.generated.h"

/**
 * UHitDetectionComponent
 *
 * Animation-driven hit detection using swept collision volumes.
 * Activated and deactivated via Anim Notifies during attack montages.
 * Prevents multi-hits on the same target within a single swing.
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UHitDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitDetectionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Enable hit detection for a swing. Called from Anim Notify. */
	UFUNCTION(BlueprintCallable, Category = "Combat|HitDetection")
	void EnableHitDetection(const FDamageInfo& DamageTemplate);

	/** Disable hit detection after swing. Called from Anim Notify. */
	UFUNCTION(BlueprintCallable, Category = "Combat|HitDetection")
	void DisableHitDetection();

	/** Clear the hit list (for new attack). */
	UFUNCTION(BlueprintCallable, Category = "Combat|HitDetection")
	void ClearHitList();

	// ───── Configuration ─────

	/** Socket name on the skeletal mesh for trace start point. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HitDetection")
	FName TraceStartSocket = TEXT("weapon_start");

	/** Socket name on the skeletal mesh for trace end point. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HitDetection")
	FName TraceEndSocket = TEXT("weapon_end");

	/** Radius of the sphere sweep for hit detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|HitDetection",
		meta = (ClampMin = "5", ClampMax = "50"))
	float TraceRadius = 15.0f;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitDetected, AActor*, HitActor, const FDamageInfo&, DamageInfo);
	UPROPERTY(BlueprintAssignable, Category = "Combat|HitDetection")
	FOnHitDetected OnHitDetected;

private:
	bool bIsActive = false;
	FDamageInfo ActiveDamageTemplate;

	/** Actors already hit during this swing (prevent double-hit). */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;

	void PerformTrace();
};
