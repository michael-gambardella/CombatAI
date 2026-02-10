// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * UCombatCameraComponent
 *
 * Manages combat-specific camera behaviors layered on top of UE5's
 * built-in spring arm collision. Provides:
 *   - Designer-tunable follow parameters
 *   - Smooth collision avoidance recovery
 *   - Lock-on camera framing
 *   - Combat effects (screen shake, hitlag, FOV adjustments)
 *
 * Attach to the player character alongside a SpringArmComponent and CameraComponent.
 */
UCLASS(ClassGroup = (CombatAI), meta = (BlueprintSpawnableComponent))
class COMBATAI_API UCombatCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatCameraComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// ───── Camera References (set by owning character) ─────

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	// ───── Follow Parameters (Designer-Tunable) ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "200", ClampMax = "600", ToolTip = "Spring arm length in cm"))
	float FollowDistance = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "50", ClampMax = "200", ToolTip = "Camera height offset in cm"))
	float FollowHeight = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "0", ClampMax = "120", ToolTip = "Horizontal shoulder offset in cm"))
	float HorizontalOffset = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "60", ClampMax = "100", ToolTip = "Camera field of view in degrees"))
	float BaseFOV = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "0", ClampMax = "1", ToolTip = "Strength of look-ahead in movement direction"))
	float LookAheadStrength = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Follow",
		meta = (ClampMin = "1", ClampMax = "20", ToolTip = "Camera position interpolation speed"))
	float FollowInterpSpeed = 10.0f;

	// ───── Combat Effects ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Effects",
		meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Screen shake intensity for light attacks"))
	float ShakeIntensityLight = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Effects",
		meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Screen shake intensity for heavy attacks"))
	float ShakeIntensityHeavy = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Effects",
		meta = (ClampMin = "0", ClampMax = "0.15", ToolTip = "Hitlag freeze duration in seconds"))
	float HitlagDuration = 0.05f;

	// ───── Effect Triggers ─────

	/** Trigger screen shake with specified intensity and duration. */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void TriggerScreenShake(float Intensity, float Duration = 0.15f);

	/** Trigger hitlag (brief time dilation). */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void TriggerHitlag(float Duration = -1.0f);

	/** Trigger slow-motion for kill cam. */
	UFUNCTION(BlueprintCallable, Category = "Camera|Effects")
	void TriggerSlowMotion(float TimeScale = 0.3f, float Duration = 0.5f);

	/** Apply parameters to the spring arm / camera (call after changing values). */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ApplyParameters();

protected:
	virtual void BeginPlay() override;

private:
	/** Active shake remaining. */
	float CurrentShakeIntensity = 0.0f;
	float ShakeTimer = 0.0f;

	/** Hitlag state. */
	bool bHitlagActive = false;
	float HitlagTimer = 0.0f;

	/** Slow-motion state. */
	bool bSlowMotionActive = false;
	float SlowMotionTimer = 0.0f;
	float OriginalTimeDilation = 1.0f;

	void ProcessShake(float DeltaTime);
	void ProcessHitlag(float DeltaTime);
	void ProcessSlowMotion(float DeltaTime);
};
