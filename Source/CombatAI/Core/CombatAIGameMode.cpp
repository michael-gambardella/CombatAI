// Copyright CombatAI Project. All Rights Reserved.

#include "CombatAIGameMode.h"
#include "CombatAI/Core/CombatAIPlayerController.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "CombatAI/CombatAI.h"

ACombatAIGameMode::ACombatAIGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ACombatAIPlayerController::StaticClass();
}

void ACombatAIGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogCombatAI, Log, TEXT("CombatAI GameMode initialized."));
}

void ACombatAIGameMode::StartEncounter()
{
	UE_LOG(LogCombatAI, Log, TEXT("Encounter started."));
	// Encounter Manager will be created in Sprint 6 (E-10) and hooked here.
}

void ACombatAIGameMode::RestartEncounter()
{
	UE_LOG(LogCombatAI, Log, TEXT("Encounter restarting."));
	// Reset player, despawn all enemies, restart from Wave 1.
}

void ACombatAIGameMode::OnPlayerDeath()
{
	UE_LOG(LogCombatAI, Log, TEXT("Player died — Game Over."));
	OnGameOver.Broadcast();
}

void ACombatAIGameMode::OnAllWavesCleared()
{
	UE_LOG(LogCombatAI, Log, TEXT("All waves cleared — Victory!"));
	OnVictory.Broadcast();
}
