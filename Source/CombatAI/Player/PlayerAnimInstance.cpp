// Copyright CombatAI Project. All Rights Reserved.

#include "PlayerAnimInstance.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerRef = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerRef) return;

	// Locomotion.
	const FVector Velocity = PlayerRef->GetVelocity();
	Speed = Velocity.Size2D();
	Direction = UKismetMathLibrary::NormalizedDeltaRotator(
		Velocity.Rotation(), PlayerRef->GetActorRotation()).Yaw;

	bIsInAir = PlayerRef->GetCharacterMovement()->IsFalling();
	bIsSprinting = PlayerRef->bIsSprinting;

	// Combat.
	CombatState = PlayerRef->CombatState;
	bIsDodging = PlayerRef->bIsDodging;
	bIsLockedOn = PlayerRef->IsLockedOn();
	bIsStaggered = (CombatState == EPlayerCombatState::Staggered);
	bIsDead = (CombatState == EPlayerCombatState::Dead);
}
