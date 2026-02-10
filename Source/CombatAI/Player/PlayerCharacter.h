// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "PlayerCharacter.generated.h"

class UPlayerStatsComponent;
class UCombatComponent;
class UCombatCameraComponent;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

/**
 * APlayerCharacter
 *
 * The player-controlled third-person character. Handles:
 *   - Movement (walk, sprint, dodge roll, jump)
 *   - Combat (light combo, heavy attack, parry, ranged, lock-on)
 *   - Integration with PlayerStatsComponent for health/stamina/ammo
 *   - Integration with CombatComponent for the shared damage pipeline
 *   - Camera spring arm ownership
 *
 * All input is received via Request*() methods called by the PlayerController.
 * This decouples input binding from character logic.
 */
UCLASS()
class COMBATAI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	// ───── Components ─────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerStatsComponent> StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCombatComponent> CombatComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FollowCamera;

	// ───── Movement Parameters ─────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "100", ClampMax = "600", ToolTip = "Base walk speed in cm/s"))
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "400", ClampMax = "1200", ToolTip = "Sprint speed in cm/s"))
	float SprintSpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "200", ClampMax = "1000", ToolTip = "Dodge roll velocity in cm/s"))
	float DodgeSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "0.1", ClampMax = "1.5", ToolTip = "Dodge roll duration in seconds"))
	float DodgeDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "0.0", ClampMax = "0.5", ToolTip = "I-frame start time into dodge"))
	float DodgeIFrameStart = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
		meta = (ClampMin = "0.1", ClampMax = "1.0", ToolTip = "I-frame end time into dodge"))
	float DodgeIFrameEnd = 0.35f;

	// ───── Combat State ─────

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	EPlayerCombatState CombatState = EPlayerCombatState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 ComboCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInvulnerable = false;

	// ───── Lock-On ─────

	UPROPERTY(BlueprintReadOnly, Category = "Combat|LockOn")
	TObjectPtr<AActor> LockedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|LockOn",
		meta = (ClampMin = "500", ClampMax = "5000", ToolTip = "Maximum lock-on range"))
	float LockOnRange = 2500.0f;

	UFUNCTION(BlueprintPure, Category = "Combat|LockOn")
	bool IsLockedOn() const { return LockedTarget != nullptr; }

	// ───── Combat Montages ─────

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> LightAttack1Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> LightAttack2Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> LightAttack3Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> HeavyAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> ParryMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Animation")
	TObjectPtr<UAnimMontage> StaggerMontage;

	// ───── Input Request Interface ─────
	// Called by PlayerController. Decouples input from logic.

	void RequestMove(const FVector2D& Input);
	void RequestLook(const FVector2D& Input);
	void RequestSprintStart();
	void RequestSprintStop();
	void RequestJump();
	void RequestDodge();
	void RequestLightAttack();
	void RequestHeavyAttackStart();
	void RequestHeavyAttackRelease();
	void RequestParry();
	void RequestRangedAttack();
	void RequestLockOnToggle();

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChanged, EPlayerCombatState, NewState);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnCombatStateChanged OnCombatStateChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockOnChanged, AActor*, NewTarget);
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnLockOnChanged OnLockOnChanged;

protected:
	virtual void BeginPlay() override;

	// ───── Combat Execution ─────

	/** Execute the next attack in the light combo chain. */
	void ExecuteLightAttack();

	/** Execute the heavy attack (on release). */
	void ExecuteHeavyAttack();

	/** Execute the dodge roll in the current movement direction. */
	void ExecuteDodge();

	/** Execute the parry stance. */
	void ExecuteParry();

	/** Fire a ranged projectile. */
	void ExecuteRangedAttack();

	// ───── Lock-On ─────

	AActor* FindBestLockOnTarget() const;
	void SetLockedTarget(AActor* NewTarget);
	void ClearLockedTarget();
	void UpdateLockOn();

	// ───── State Management ─────

	void SetCombatState(EPlayerCombatState NewState);
	bool CanPerformAction() const;
	bool CanCancelInto(EPlayerCombatState DesiredState) const;

	// ───── Callbacks ─────

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HandleDeath();

	UFUNCTION()
	void HandleStaggered();

	UFUNCTION()
	void HandleStaggerRecovered();

private:
	/** Current movement input vector (camera-relative). */
	FVector2D CurrentMoveInput = FVector2D::ZeroVector;

	/** Timer tracking dodge roll progress. */
	float DodgeTimer = 0.0f;

	/** Direction of current dodge roll. */
	FVector DodgeDirection = FVector::ZeroVector;

	/** Combo input buffer — stores whether player pressed attack during a combo window. */
	bool bComboInputBuffered = false;

	/** Time window in which combo inputs are accepted. */
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float ComboWindowDuration = 0.5f;

	/** Timer for combo window expiration. */
	float ComboWindowTimer = 0.0f;

	/** Ranged attack cooldown tracking. */
	float RangedCooldownTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Ranged", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float RangedCooldown = 0.3f;

	/** Apply movement from input each tick. */
	void ProcessMovement(float DeltaTime);

	/** Tick dodge roll logic. */
	void ProcessDodge(float DeltaTime);
};
