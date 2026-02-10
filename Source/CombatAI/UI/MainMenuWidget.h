// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;

/**
 * UMainMenuWidget
 *
 * Top-level menu widget providing:
 *   - Main Menu (Play, Difficulty, Quit)
 *   - Pause Menu (Resume, Restart, Quit)
 *   - Wave Complete screen
 *   - Game Over screen
 *   - Victory screen
 *
 * Uses a WidgetSwitcher to flip between menu states.
 */
UCLASS()
class COMBATAI_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ───── State Management ─────

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowWaveComplete(int32 WaveNumber);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOver();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowVictory();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void HideAll();

	// ───── Widget Bindings ─────

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	// Main Menu Page
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DifficultyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	// Pause Page
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PauseQuitButton;

	// Wave Complete Page
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveCompleteText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	// Game Over Page
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RetryButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameOverQuitButton;

	// Victory Page
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> VictoryText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> VictoryMenuButton;

	// ───── Delegates ─────

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayPressed);
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnPlayPressed OnPlayPressed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumePressed);
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnResumePressed OnResumePressed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartPressed);
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnRestartPressed OnRestartPressed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitPressed);
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnQuitPressed OnQuitPressed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinuePressed);
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnContinuePressed OnContinuePressed;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION() void HandlePlay();
	UFUNCTION() void HandleResume();
	UFUNCTION() void HandleRestart();
	UFUNCTION() void HandleQuit();
	UFUNCTION() void HandleContinue();
	UFUNCTION() void HandleRetry();
	UFUNCTION() void HandleVictoryMenu();
};
