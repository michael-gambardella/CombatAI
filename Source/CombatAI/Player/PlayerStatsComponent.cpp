// Copyright CombatAI Project. All Rights Reserved.

#include "PlayerStatsComponent.h"
#include "CombatAI/CombatAI.h"

UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetAllStats();
}

void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stamina regeneration with delay.
	if (!bIsRegenerating)
	{
		StaminaRegenTimer -= DeltaTime;
		if (StaminaRegenTimer <= 0.0f)
		{
			bIsRegenerating = true;
		}
	}

	if (bIsRegenerating && CurrentStamina < MaxStamina)
	{
		const float OldStamina = CurrentStamina;
		CurrentStamina = FMath::Min(CurrentStamina + StaminaRegenRate * DeltaTime, MaxStamina);
		if (!FMath::IsNearlyEqual(OldStamina, CurrentStamina))
		{
			OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
		}
	}
}

// ───── Health ─────

void UPlayerStatsComponent::ApplyDamage(float Amount)
{
	if (!IsAlive() || Amount <= 0.0f) return;

	CurrentHealth = FMath::Max(CurrentHealth - Amount, 0.0f);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	UE_LOG(LogCombatAI, Verbose, TEXT("Player took %.1f damage. Health: %.1f / %.1f"), Amount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogCombatAI, Log, TEXT("Player has died."));
		OnDeath.Broadcast();
	}
}

void UPlayerStatsComponent::Heal(float Amount)
{
	if (!IsAlive() || Amount <= 0.0f) return;

	CurrentHealth = FMath::Min(CurrentHealth + Amount, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UPlayerStatsComponent::GetHealthPercent() const
{
	return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f;
}

bool UPlayerStatsComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

// ───── Stamina ─────

bool UPlayerStatsComponent::ConsumeStamina(float Amount)
{
	if (CurrentStamina < Amount) return false;

	CurrentStamina = FMath::Max(CurrentStamina - Amount, 0.0f);
	bIsRegenerating = false;
	StaminaRegenTimer = StaminaRegenDelay;

	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	return true;
}

float UPlayerStatsComponent::GetStaminaPercent() const
{
	return MaxStamina > 0.0f ? CurrentStamina / MaxStamina : 0.0f;
}

bool UPlayerStatsComponent::HasStamina(float Amount) const
{
	return CurrentStamina >= Amount;
}

// ───── Ammo ─────

bool UPlayerStatsComponent::ConsumeAmmo(int32 Amount)
{
	if (CurrentAmmo < Amount) return false;

	CurrentAmmo -= Amount;
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
	return true;
}

void UPlayerStatsComponent::ReplenishAmmo(int32 Amount)
{
	CurrentAmmo = FMath::Min(CurrentAmmo + Amount, MaxAmmo);
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}

// ───── Reset ─────

void UPlayerStatsComponent::ResetAllStats()
{
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CurrentAmmo = MaxAmmo;
	bIsRegenerating = true;
	StaminaRegenTimer = 0.0f;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
}
