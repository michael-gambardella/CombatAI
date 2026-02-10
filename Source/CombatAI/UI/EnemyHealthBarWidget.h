// Copyright CombatAI Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * UEnemyHealthBarWidget
 *
 * Widget component attached to enemy characters via WidgetComponent.
 * Shows:
 *   - Health bar (red)
 *   - Stagger meter (blue)
 *   - Enemy name
 *   - Attack token indicator
 */
UCLASS()
class COMBATAI_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaggerBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealth(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateStagger(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateName(const FText& Name);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTokenIndicator(bool bHasToken);
};
