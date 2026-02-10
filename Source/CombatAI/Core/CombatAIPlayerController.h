// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CombatAIPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;

/**
 * ACombatAIPlayerController
 *
 * Handles input binding via the UE5 Enhanced Input System.
 * Routes all input to the possessed PlayerCharacter.
 * Manages lock-on target selection and HUD ownership.
 */
UCLASS()
class COMBATAI_API ACombatAIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACombatAIPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// ───── Input Mapping Context ─────

	/** Primary gameplay mapping context (movement + combat). */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// ───── Input Actions ─────

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> LightAttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> ParryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> RangedAttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> LockOnAction;

	// ───── Input Handlers ─────

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleSprintStart();
	void HandleSprintStop();
	void HandleJump();
	void HandleDodge();
	void HandleLightAttack();
	void HandleHeavyAttackStart();
	void HandleHeavyAttackRelease();
	void HandleParry();
	void HandleRangedAttack();
	void HandleLockOnToggle();

private:
	/** Cached reference to the possessed player character. */
	UPROPERTY()
	TObjectPtr<APlayerCharacter> CachedPlayerCharacter;
};
