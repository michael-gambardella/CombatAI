// Copyright CombatAI Project. All Rights Reserved.

#include "AIDebugHUD.h"
#include "CombatAI/AI/Core/AICharacterBase.h"
#include "CombatAI/AI/UtilityAI/UtilityAIComponent.h"
#include "CombatAI/AI/Coordination/SquadCoordinator.h"
#include "CombatAI/AI/Coordination/AttackTokenSystem.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/Combat/CombatTypes.h"
#include "CombatAI/CombatAI.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

AAIDebugHUD::AAIDebugHUD()
{
}

void AAIDebugHUD::ToggleAIDebug()
{
	bDebugEnabled = !bDebugEnabled;
	UE_LOG(LogCombatAI, Log, TEXT("AI Debug HUD: %s"), bDebugEnabled ? TEXT("ON") : TEXT("OFF"));
}

void AAIDebugHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!bDebugEnabled) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Draw per-enemy info.
	for (TActorIterator<AAICharacterBase> It(World); It; ++It)
	{
		DrawEnemyInfo(*It, PC);
	}

	// Find the active squad coordinator.
	ASquadCoordinator* Coord = nullptr;
	for (TActorIterator<ASquadCoordinator> It(World); It; ++It)
	{
		Coord = *It;
		break;
	}

	if (Coord)
	{
		if (bShowTokens)       DrawTokenInfo(Coord, PC);
		if (bShowFlankingSectors) DrawFlankingRing(Coord, PC);
		if (bShowTacticalSituation) DrawTacticalInfo(Coord);
	}
}

void AAIDebugHUD::DrawEnemyInfo(AAICharacterBase* Enemy, APlayerController* PC)
{
	if (!Enemy || Enemy->GetAIState() == EAIState::Dead) return;

	// Project world position to screen.
	FVector WorldPos = Enemy->GetActorLocation() + FVector(0, 0, 120.0f);
	FVector2D ScreenPos;
	if (!PC->ProjectWorldLocationToScreen(WorldPos, ScreenPos)) return;

	float Y = ScreenPos.Y;

	// State label.
	if (bShowAIStates)
	{
		FString StateStr = FString::Printf(TEXT("[%s] %s"),
			*GetStateName(Enemy->GetAIState()), *Enemy->EnemyDisplayName.ToString());

		DrawText(StateStr, GetStateColor(Enemy->GetAIState()),
			ScreenPos.X - 50.0f, Y, nullptr, 1.0f, false);
		Y += 14.0f;
	}

	// Health bar.
	if (Enemy->CombatComp)
	{
		float HealthPct = Enemy->CombatComp->GetHealthPercent();
		float BarWidth = 80.0f;
		float BarHeight = 6.0f;
		float BarX = ScreenPos.X - BarWidth * 0.5f;

		DrawRect(FLinearColor(0.3f, 0.0f, 0.0f, 0.8f), BarX, Y, BarWidth, BarHeight);
		DrawRect(FLinearColor(0.0f, 0.8f, 0.0f, 1.0f), BarX, Y, BarWidth * HealthPct, BarHeight);
		Y += 10.0f;

		// Stagger bar.
		float StaggerPct = Enemy->CombatComp->GetStaggerPercent();
		DrawRect(FLinearColor(0.1f, 0.1f, 0.3f, 0.8f), BarX, Y, BarWidth, 4.0f);
		DrawRect(FLinearColor(0.6f, 0.6f, 1.0f, 1.0f), BarX, Y, BarWidth * StaggerPct, 4.0f);
		Y += 8.0f;
	}

	// Utility AI scores.
	if (bShowUtilityScores)
	{
		UUtilityAIComponent* UAI = Enemy->FindComponentByClass<UUtilityAIComponent>();
		if (UAI)
		{
			FString ActionStr = FString::Printf(TEXT("Action: %s (%.2f)"),
				*UAI->CurrentActionName.ToString(), UAI->CurrentActionScore);
			DrawText(ActionStr, FColor::Cyan, ScreenPos.X - 50.0f, Y, nullptr, 0.8f, false);
			Y += 12.0f;
		}
	}

	// Token indicator.
	if (bShowTokens && Enemy->bHasAttackToken)
	{
		DrawText(TEXT("[TOKEN]"), FColor::Red, ScreenPos.X - 20.0f, Y, nullptr, 0.8f, false);
		Y += 12.0f;
	}

	// Perception cone (3D debug draw).
	if (bShowPerception)
	{
		FVector EyePos = Enemy->GetActorLocation() + FVector(0, 0, 60.0f);
		FVector Forward = Enemy->GetActorForwardVector();

		// Draw forward direction.
		DrawDebugLine(GetWorld(), EyePos, EyePos + Forward * 500.0f,
			Enemy->PerceptionData.bCanSeePlayer ? FColor::Green : FColor::Red,
			false, -1.0f, 0, 1.5f);
	}
}

void AAIDebugHUD::DrawTokenInfo(ASquadCoordinator* Coord, APlayerController* PC)
{
	if (!Coord || !Coord->TokenSystem) return;

	float X = 20.0f;
	float Y = 200.0f;

	DrawText(TEXT("=== ATTACK TOKENS ==="), FColor::Yellow, X, Y);
	Y += 16.0f;

	FString Info = FString::Printf(TEXT("Active: %d / %d"),
		Coord->TokenSystem->GetActiveTokenCount(), Coord->TokenSystem->GetMaxTokenCount());
	DrawText(Info, FColor::White, X, Y);
	Y += 14.0f;

	for (AAICharacterBase* Holder : Coord->TokenSystem->GetTokenHolders())
	{
		if (Holder)
		{
			DrawText(FString::Printf(TEXT("  > %s"), *Holder->GetName()),
				FColor::Red, X, Y);
			Y += 12.0f;
		}
	}
}

void AAIDebugHUD::DrawFlankingRing(ASquadCoordinator* Coord, APlayerController* PC)
{
	if (!Coord) return;

	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	// Draw 8 sector markers.
	for (int32 i = 0; i < 8; ++i)
	{
		FVector SectorPos = Coord->GetSectorWorldPosition(i, 400.0f);
		FColor Color = FColor::Green;

		DrawDebugSphere(GetWorld(), SectorPos, 30.0f, 8, Color, false, -1.0f, 0, 1.0f);
		DrawDebugString(GetWorld(), SectorPos + FVector(0, 0, 40),
			FString::Printf(TEXT("S%d"), i), nullptr, Color, 0.0f, true);
	}
}

void AAIDebugHUD::DrawTacticalInfo(ASquadCoordinator* Coord)
{
	if (!Coord) return;

	float X = 20.0f;
	float Y = 320.0f;

	DrawText(TEXT("=== TACTICAL SITUATION ==="), FColor::Yellow, X, Y);
	Y += 16.0f;

	FString SitStr = UEnum::GetValueAsString(Coord->CurrentSituation);
	DrawText(SitStr, FColor::Orange, X, Y);
	Y += 14.0f;

	FString AliveStr = FString::Printf(TEXT("Alive: %d"), Coord->GetAliveCount());
	DrawText(AliveStr, FColor::White, X, Y);
}

// ───── Helpers ─────

FColor AAIDebugHUD::GetStateColor(EAIState State) const
{
	switch (State)
	{
	case EAIState::Idle:      return FColor::White;
	case EAIState::Alert:     return FColor::Yellow;
	case EAIState::Combat:    return FColor::Red;
	case EAIState::Searching: return FColor::Orange;
	case EAIState::Dead:      return FColor(80, 80, 80);
	default:                  return FColor::White;
	}
}

FString AAIDebugHUD::GetStateName(EAIState State) const
{
	switch (State)
	{
	case EAIState::Idle:      return TEXT("IDLE");
	case EAIState::Alert:     return TEXT("ALERT");
	case EAIState::Combat:    return TEXT("COMBAT");
	case EAIState::Searching: return TEXT("SEARCH");
	case EAIState::Dead:      return TEXT("DEAD");
	default:                  return TEXT("???");
	}
}
