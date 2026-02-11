// Copyright CombatAI Project. All Rights Reserved.

#include "EnemyProfileData.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/AI/Core/CombatAIPerceptionComponent.h"
#include "CombatAI/AI/UtilityAI/UtilityAIComponent.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyProfileData::ApplyToCharacter(AAICharacterBase* Character) const
{
	if (!Character) return;

	// Identity.
	Character->Archetype = Archetype;
	Character->EnemyDisplayName = EnemyName;
	Character->SetActorScale3D(FVector(Scale));

	// Stats via CombatComponent.
	if (UCombatComponent* CC = Character->CombatComp)
	{
		CC->MaxHealth = MaxHealth;
		CC->CurrentHealth = MaxHealth;
		CC->StaggerThreshold = StaggerThreshold;
		CC->StaggerDuration = StaggerDuration;
		CC->FrontalArmorReduction = FrontalArmorReduction;
		CC->WeakPointMultiplier = WeakPointMultiplier;
	}

	// Movement.
	if (UCharacterMovementComponent* MC = Character->GetCharacterMovement())
	{
		MC->MaxWalkSpeed = MoveSpeed;
	}

	// Perception.
	if (UAICombatPerceptionComponent* PC = Character->FindComponentByClass<UAICombatPerceptionComponent>())
	{
		PC->DetectionRange = DetectionRange;
		PC->FieldOfView = FieldOfView;
		PC->HearingRange = HearingRange;
	}

	// Utility Actions.
	if (UUtilityAIComponent* UAI = Character->FindComponentByClass<UUtilityAIComponent>())
	{
		if (AvailableActions.Num() > 0)
		{
			UAI->Actions = AvailableActions;
		}
	}

	UE_LOG(LogCombatAI, Log, TEXT("Applied profile '%s' to %s"), *EnemyName.ToString(), *Character->GetName());
}
