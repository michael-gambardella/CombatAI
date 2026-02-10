// Copyright CombatAI Project. All Rights Reserved.

#include "CombatHUDWidget.h"
#include "CombatAI/Player/PlayerCharacter.h"
#include "CombatAI/Player/PlayerStatsComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCombatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatHUDWidget::BindToPlayer(APlayerCharacter* Player)
{
	if (!Player || !Player->StatsComponent) return;

	Player->StatsComponent->OnHealthChanged.AddDynamic(this, &UCombatHUDWidget::HandleHealthChanged);
	Player->StatsComponent->OnStaminaChanged.AddDynamic(this, &UCombatHUDWidget::HandleStaminaChanged);
	Player->StatsComponent->OnAmmoChanged.AddDynamic(this, &UCombatHUDWidget::HandleAmmoChanged);

	// Initial update.
	UpdateHealth(Player->StatsComponent->CurrentHealth, Player->StatsComponent->MaxHealth);
	UpdateStamina(Player->StatsComponent->CurrentStamina, Player->StatsComponent->MaxStamina);
	UpdateAmmo(Player->StatsComponent->CurrentAmmo, Player->StatsComponent->MaxAmmo);
}

void UCombatHUDWidget::UpdateHealth(float Current, float Max)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Max > 0.0f ? Current / Max : 0.0f);
	}
}

void UCombatHUDWidget::UpdateStamina(float Current, float Max)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(Max > 0.0f ? Current / Max : 0.0f);
	}
}

void UCombatHUDWidget::UpdateAmmo(int32 Current, int32 Max)
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Current, Max)));
	}
}

void UCombatHUDWidget::UpdateWaveInfo(int32 CurrentWave, int32 TotalWaves)
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("WAVE %d / %d"), CurrentWave, TotalWaves)));
	}
}

void UCombatHUDWidget::UpdateEnemyCount(int32 Remaining)
{
	if (EnemyCountText)
	{
		EnemyCountText->SetText(FText::FromString(FString::Printf(TEXT("Enemies: %d"), Remaining)));
	}
}

void UCombatHUDWidget::HandleHealthChanged(float NewHealth, float MaxHealth)
{
	UpdateHealth(NewHealth, MaxHealth);
}

void UCombatHUDWidget::HandleStaminaChanged(float NewStamina, float MaxStamina)
{
	UpdateStamina(NewStamina, MaxStamina);
}

void UCombatHUDWidget::HandleAmmoChanged(int32 NewAmmo, int32 MaxAmmo)
{
	UpdateAmmo(NewAmmo, MaxAmmo);
}
