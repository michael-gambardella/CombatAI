// Copyright CombatAI Project. All Rights Reserved.

#include "EncounterManager.h"
#include "CombatAI/Data/EncounterData.h"
#include "CombatAI/Data/EnemyProfileData.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/AI/Coordination/SquadCoordinator.h"
#include "CombatAI/AI/Coordination/AttackTokenSystem.h"
#include "CombatAI/AI/Archetypes/RusherCharacter.h"
#include "CombatAI/AI/Archetypes/WatcherCharacter.h"
#include "CombatAI/AI/Archetypes/WardenCharacter.h"
#include "CombatAI/CombatAI.h"

AEncounterManager::AEncounterManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEncounterManager::BeginPlay()
{
	Super::BeginPlay();
}

int32 AEncounterManager::GetTotalWaves() const
{
	return EncounterDataAsset ? EncounterDataAsset->GetTotalWaveCount() : 0;
}

// ───── Encounter Flow ─────

void AEncounterManager::StartEncounter()
{
	if (!EncounterDataAsset)
	{
		UE_LOG(LogCombatAI, Error, TEXT("EncounterManager: No EncounterData asset assigned!"));
		return;
	}

	bEncounterActive = true;
	CurrentWaveIndex = -1;

	UE_LOG(LogCombatAI, Log, TEXT("Encounter started: %s (%d waves)"),
		*EncounterDataAsset->EncounterName.ToString(),
		EncounterDataAsset->GetTotalWaveCount());

	AdvanceWave();
}

void AEncounterManager::AdvanceWave()
{
	CurrentWaveIndex++;

	if (!EncounterDataAsset || CurrentWaveIndex >= EncounterDataAsset->Waves.Num())
	{
		// All waves complete.
		bEncounterActive = false;
		UE_LOG(LogCombatAI, Log, TEXT("All waves cleared — Encounter Complete!"));
		OnEncounterComplete.Broadcast();
		return;
	}

	const FEncounterWave& Wave = EncounterDataAsset->Waves[CurrentWaveIndex];

	// Delay before spawning (inter-wave pacing).
	float Delay = (CurrentWaveIndex > 0) ? Wave.DelayAfterPreviousWave : 0.0f;

	FTimerHandle SpawnTimer;
	GetWorldTimerManager().SetTimer(SpawnTimer, [this]()
	{
		SpawnWave(CurrentWaveIndex);
	}, FMath::Max(Delay, 0.01f), false);
}

void AEncounterManager::SpawnWave(int32 WaveIndex)
{
	if (!EncounterDataAsset || WaveIndex >= EncounterDataAsset->Waves.Num()) return;

	const FEncounterWave& Wave = EncounterDataAsset->Waves[WaveIndex];

	UE_LOG(LogCombatAI, Log, TEXT("=== WAVE %d: %s (%d enemies) ==="),
		WaveIndex + 1, *Wave.WaveName.ToString(), Wave.Enemies.Num());

	SpawnedEnemies.Empty();

	for (const FEnemySpawnEntry& Entry : Wave.Enemies)
	{
		AAICharacterBase* Enemy = SpawnEnemy(Entry);
		if (Enemy)
		{
			SpawnedEnemies.Add(Enemy);
		}
	}

	EnemiesRemainingInWave = SpawnedEnemies.Num();

	// Create squad coordinator.
	if (ActiveCoordinator)
	{
		ActiveCoordinator->Destroy();
	}

	FActorSpawnParameters CoordParams;
	CoordParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActiveCoordinator = GetWorld()->SpawnActor<ASquadCoordinator>(
		ASquadCoordinator::StaticClass(), FTransform::Identity, CoordParams);

	if (ActiveCoordinator)
	{
		ActiveCoordinator->InitializeSquad(SpawnedEnemies, Wave.SquadProfile);
		ActiveCoordinator->TokenSystem->SetMaxTokens(Wave.AttackTokenCount);

		// Bind death callbacks.
		ActiveCoordinator->OnMemberDied.AddDynamic(this, &AEncounterManager::HandleEnemyDeath);
	}

	OnWaveStarted.Broadcast(WaveIndex + 1);
}

AAICharacterBase* AEncounterManager::SpawnEnemy(const FEnemySpawnEntry& Entry)
{
	TSubclassOf<AAICharacterBase> EnemyClass;

	switch (Entry.Archetype)
	{
	case EEnemyArchetype::Rusher:
		EnemyClass = ARusherCharacter::StaticClass();
		break;
	case EEnemyArchetype::Watcher:
		EnemyClass = AWatcherCharacter::StaticClass();
		break;
	case EEnemyArchetype::Warden:
		EnemyClass = AWardenCharacter::StaticClass();
		break;
	default:
		UE_LOG(LogCombatAI, Error, TEXT("Unknown archetype for spawn entry"));
		return nullptr;
	}

	FTransform SpawnXform = GetSpawnTransform(Entry.SpawnPointIndex);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAICharacterBase* Enemy = GetWorld()->SpawnActor<AAICharacterBase>(
		EnemyClass, SpawnXform, Params);

	if (Enemy && Entry.Profile)
	{
		Entry.Profile->ApplyToCharacter(Enemy);
	}

	return Enemy;
}

FTransform AEncounterManager::GetSpawnTransform(int32 SpawnPointIndex) const
{
	if (SpawnPoints.IsValidIndex(SpawnPointIndex) && SpawnPoints[SpawnPointIndex])
	{
		return SpawnPoints[SpawnPointIndex]->GetActorTransform();
	}

	// Fallback: random position in a ring around world origin.
	float Angle = FMath::RandRange(0.0f, 2.0f * PI);
	float Radius = FMath::RandRange(800.0f, 1200.0f);
	FVector Pos(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);

	return FTransform(FRotator::ZeroRotator, Pos);
}

// ───── Death Tracking ─────

void AEncounterManager::HandleEnemyDeath(AAICharacterBase* Enemy)
{
	EnemiesRemainingInWave = FMath::Max(EnemiesRemainingInWave - 1, 0);
	OnEnemyKilled.Broadcast(Enemy);

	UE_LOG(LogCombatAI, Log, TEXT("Enemy killed: %s. Remaining: %d"),
		*Enemy->GetName(), EnemiesRemainingInWave);

	CheckWaveComplete();
}

void AEncounterManager::CheckWaveComplete()
{
	if (EnemiesRemainingInWave <= 0)
	{
		UE_LOG(LogCombatAI, Log, TEXT("Wave %d complete!"), CurrentWaveIndex + 1);
		OnWaveCompleted.Broadcast(CurrentWaveIndex + 1);

		// Auto-advance.
		AdvanceWave();
	}
}
