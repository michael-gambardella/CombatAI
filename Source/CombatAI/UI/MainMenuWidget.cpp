// Copyright CombatAI Project. All Rights Reserved.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button clicks.
	if (PlayButton) PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandlePlay);
	if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuit);
	if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleResume);
	if (RestartButton) RestartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleRestart);
	if (PauseQuitButton) PauseQuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuit);
	if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleContinue);
	if (RetryButton) RetryButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleRetry);
	if (GameOverQuitButton) GameOverQuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuit);
	if (VictoryMenuButton) VictoryMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleVictoryMenu);
}

// ───── State Management ─────

void UMainMenuWidget::ShowMainMenu()
{
	SetVisibility(ESlateVisibility::Visible);
	if (MenuSwitcher) MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::ShowPauseMenu()
{
	SetVisibility(ESlateVisibility::Visible);
	if (MenuSwitcher) MenuSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::ShowWaveComplete(int32 WaveNumber)
{
	SetVisibility(ESlateVisibility::Visible);
	if (MenuSwitcher) MenuSwitcher->SetActiveWidgetIndex(2);
	if (WaveCompleteText)
	{
		WaveCompleteText->SetText(FText::FromString(
			FString::Printf(TEXT("WAVE %d COMPLETE!"), WaveNumber)));
	}
}

void UMainMenuWidget::ShowGameOver()
{
	SetVisibility(ESlateVisibility::Visible);
	if (MenuSwitcher) MenuSwitcher->SetActiveWidgetIndex(3);
}

void UMainMenuWidget::ShowVictory()
{
	SetVisibility(ESlateVisibility::Visible);
	if (MenuSwitcher) MenuSwitcher->SetActiveWidgetIndex(4);
	if (VictoryText)
	{
		VictoryText->SetText(FText::FromString(TEXT("ALL WAVES CLEARED!\nYOU ARE VICTORIOUS!")));
	}
}

void UMainMenuWidget::HideAll()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

// ───── Button Handlers ─────

void UMainMenuWidget::HandlePlay()
{
	HideAll();
	OnPlayPressed.Broadcast();
}

void UMainMenuWidget::HandleResume()
{
	HideAll();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	OnResumePressed.Broadcast();
}

void UMainMenuWidget::HandleRestart()
{
	HideAll();
	OnRestartPressed.Broadcast();
}

void UMainMenuWidget::HandleQuit()
{
	OnQuitPressed.Broadcast();
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMainMenuWidget::HandleContinue()
{
	HideAll();
	OnContinuePressed.Broadcast();
}

void UMainMenuWidget::HandleRetry()
{
	HideAll();
	OnRestartPressed.Broadcast();
}

void UMainMenuWidget::HandleVictoryMenu()
{
	ShowMainMenu();
}
