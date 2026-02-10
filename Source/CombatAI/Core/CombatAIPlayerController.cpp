// Copyright CombatAI Project. All Rights Reserved.

#include "CombatAIPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "CombatAI/CombatAI.h"

ACombatAIPlayerController::ACombatAIPlayerController()
{
}

void ACombatAIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add the default input mapping context.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	CachedPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
}

void ACombatAIPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Movement
	if (MoveAction)
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatAIPlayerController::HandleMove);
	}
	if (LookAction)
	{
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatAIPlayerController::HandleLook);
	}
	if (SprintAction)
	{
		EIC->BindAction(SprintAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleSprintStart);
		EIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &ACombatAIPlayerController::HandleSprintStop);
	}
	if (JumpAction)
	{
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleJump);
	}

	// Combat
	if (DodgeAction)
	{
		EIC->BindAction(DodgeAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleDodge);
	}
	if (LightAttackAction)
	{
		EIC->BindAction(LightAttackAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleLightAttack);
	}
	if (HeavyAttackAction)
	{
		EIC->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleHeavyAttackStart);
		EIC->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &ACombatAIPlayerController::HandleHeavyAttackRelease);
	}
	if (ParryAction)
	{
		EIC->BindAction(ParryAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleParry);
	}
	if (RangedAttackAction)
	{
		EIC->BindAction(RangedAttackAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleRangedAttack);
	}
	if (LockOnAction)
	{
		EIC->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACombatAIPlayerController::HandleLockOnToggle);
	}
}

// ───── Input Handlers ─────

void ACombatAIPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (CachedPlayerCharacter)
	{
		const FVector2D MoveInput = Value.Get<FVector2D>();
		CachedPlayerCharacter->RequestMove(MoveInput);
	}
}

void ACombatAIPlayerController::HandleLook(const FInputActionValue& Value)
{
	if (CachedPlayerCharacter)
	{
		const FVector2D LookInput = Value.Get<FVector2D>();
		CachedPlayerCharacter->RequestLook(LookInput);
	}
}

void ACombatAIPlayerController::HandleSprintStart()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestSprintStart();
	}
}

void ACombatAIPlayerController::HandleSprintStop()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestSprintStop();
	}
}

void ACombatAIPlayerController::HandleJump()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestJump();
	}
}

void ACombatAIPlayerController::HandleDodge()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestDodge();
	}
}

void ACombatAIPlayerController::HandleLightAttack()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestLightAttack();
	}
}

void ACombatAIPlayerController::HandleHeavyAttackStart()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestHeavyAttackStart();
	}
}

void ACombatAIPlayerController::HandleHeavyAttackRelease()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestHeavyAttackRelease();
	}
}

void ACombatAIPlayerController::HandleParry()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestParry();
	}
}

void ACombatAIPlayerController::HandleRangedAttack()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestRangedAttack();
	}
}

void ACombatAIPlayerController::HandleLockOnToggle()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->RequestLockOnToggle();
	}
}
