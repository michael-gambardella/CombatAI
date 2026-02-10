// Copyright CombatAI Project. All Rights Reserved.

#include "EnemyHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyHealthBarWidget::UpdateHealth(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));

		// Color interpolation: green → yellow → red.
		FLinearColor Color;
		if (Percent > 0.5f)
		{
			Color = FMath::Lerp(FLinearColor(1, 1, 0), FLinearColor(0, 1, 0), (Percent - 0.5f) * 2.0f);
		}
		else
		{
			Color = FMath::Lerp(FLinearColor(1, 0, 0), FLinearColor(1, 1, 0), Percent * 2.0f);
		}
		HealthBar->SetFillColorAndOpacity(Color);
	}
}

void UEnemyHealthBarWidget::UpdateStagger(float Percent)
{
	if (StaggerBar)
	{
		StaggerBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

void UEnemyHealthBarWidget::UpdateName(const FText& Name)
{
	if (NameText)
	{
		NameText->SetText(Name);
	}
}

void UEnemyHealthBarWidget::SetTokenIndicator(bool bHasToken)
{
	// Token indicator shown via color change on name.
	if (NameText)
	{
		NameText->SetColorAndOpacity(bHasToken ?
			FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White));
	}
}
