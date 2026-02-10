// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AIDebugHUD.generated.h"

class AAICharacterBase;
class ASquadCoordinator;

/**
 * AAIDebugHUD
 *
 * In-game debug overlay for AI visualization. Renders:
 *   - Per-enemy: current state, selected action + score, perception cone, health bar
 *   - Token system: who holds tokens, queue status
 *   - Flanking sectors: ring visualization around player
 *   - Tactical situation: current squad-level assessment
 *   - Console commands for toggling layers
 *
 * Toggle with: `ShowDebug AI` or `CombatAI.DebugAI 1`
 */
UCLASS()
class COMBATAI_API AAIDebugHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAIDebugHUD();

	virtual void DrawHUD() override;

	// ───── Toggle Flags ─────

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowAIStates = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowUtilityScores = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowPerception = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowTokens = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowFlankingSectors = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bShowTacticalSituation = true;

	UPROPERTY(BlueprintReadWrite, Category = "Debug")
	bool bDebugEnabled = false;

	/** Toggle all debug display. */
	UFUNCTION(Exec, BlueprintCallable, Category = "Debug")
	void ToggleAIDebug();

private:
	void DrawEnemyInfo(AAICharacterBase* Enemy, APlayerController* PC);
	void DrawTokenInfo(ASquadCoordinator* Coord, APlayerController* PC);
	void DrawFlankingRing(ASquadCoordinator* Coord, APlayerController* PC);
	void DrawTacticalInfo(ASquadCoordinator* Coord);

	FColor GetStateColor(EAIState State) const;
	FString GetStateName(EAIState State) const;
};
