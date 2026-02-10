// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "PlayerAnimInstance.generated.h"

/**
 * UPlayerAnimInstance
 *
 * Animation blueprint backing class for the player character.
 * Provides variables to the animation graph for:
 *   - Locomotion blend space (speed, direction)
 *   - Combat state for state-machine transitions
 *   - Flags: airborne, dodging, sprinting, locked-on
 */
UCLASS()
class COMBATAI_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ───── Locomotion ─────

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Locomotion")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Locomotion")
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Locomotion")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Locomotion")
	bool bIsSprinting = false;

	// ───── Combat ─────

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Combat")
	EPlayerCombatState CombatState = EPlayerCombatState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Combat")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Combat")
	bool bIsLockedOn = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Combat")
	bool bIsStaggered = false;

	UPROPERTY(BlueprintReadOnly, Category = "Animation|Combat")
	bool bIsDead = false;

private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;
};
