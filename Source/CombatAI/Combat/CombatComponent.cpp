// Copyright CombatAI Project. All Rights Reserved.

#include "CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetCombatState();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stagger accumulation decay.
	if (CurrentStaggerAccum > 0.0f && !bIsStaggered)
	{
		StaggerDecayTimer -= DeltaTime;
		if (StaggerDecayTimer <= 0.0f)
		{
			CurrentStaggerAccum = FMath::Max(CurrentStaggerAccum - StaggerDecayRate * DeltaTime, 0.0f);
		}
	}
}

// ───── Damage Pipeline ─────

float UCombatComponent::ReceiveDamage(const FDamageInfo& InDamage)
{
	if (!IsAlive()) return 0.0f;

	// Step 1: Apply directional armor modifiers.
	bool bWeakPoint = false;
	float FinalDamage = ApplyArmorModifiers(InDamage.Amount, InDamage.HitDirection, bWeakPoint);

	// Step 2: Apply damage to health.
	CurrentHealth = FMath::Max(CurrentHealth - FinalDamage, 0.0f);

	// Step 3: Build the output damage info with final values.
	FDamageInfo ProcessedDamage = InDamage;
	ProcessedDamage.Amount = FinalDamage;
	ProcessedDamage.bIsWeakPointHit = bWeakPoint;

	// Step 4: Broadcast damage event.
	OnDamageReceived.Broadcast(ProcessedDamage);

	UE_LOG(LogCombatAI, Verbose, TEXT("%s received %.1f damage (raw: %.1f). HP: %.1f / %.1f %s"),
		*GetOwner()->GetName(), FinalDamage, InDamage.Amount,
		CurrentHealth, MaxHealth, bWeakPoint ? TEXT("[WEAK POINT]") : TEXT(""));

	// Step 5: Process stagger.
	ProcessStagger(InDamage.StaggerValue);

	// Step 6: Check death.
	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogCombatAI, Log, TEXT("%s has died."), *GetOwner()->GetName());
		OnDeath.Broadcast();
	}

	return FinalDamage;
}

bool UCombatComponent::DealDamage(AActor* Target, const FDamageInfo& DamageInfo)
{
	if (!Target) return false;

	UCombatComponent* TargetCombat = Target->FindComponentByClass<UCombatComponent>();
	if (!TargetCombat) return false;

	TargetCombat->ReceiveDamage(DamageInfo);
	return true;
}

// ───── Health ─────

float UCombatComponent::GetHealthPercent() const
{
	return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f;
}

bool UCombatComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

// ───── Stagger ─────

float UCombatComponent::GetStaggerPercent() const
{
	return StaggerThreshold > 0.0f ? FMath::Clamp(CurrentStaggerAccum / StaggerThreshold, 0.0f, 1.0f) : 0.0f;
}

void UCombatComponent::ProcessStagger(float StaggerValue)
{
	if (bIsStaggered || StaggerValue <= 0.0f) return;

	CurrentStaggerAccum += StaggerValue;
	StaggerDecayTimer = StaggerDecayDelay;

	if (CurrentStaggerAccum >= StaggerThreshold)
	{
		bIsStaggered = true;
		CurrentStaggerAccum = 0.0f;

		UE_LOG(LogCombatAI, Log, TEXT("%s staggered!"), *GetOwner()->GetName());
		OnStaggered.Broadcast();

		// Schedule stagger recovery.
		GetWorld()->GetTimerManager().SetTimer(
			StaggerTimerHandle, this, &UCombatComponent::EndStagger, StaggerDuration, false);
	}
}

void UCombatComponent::EndStagger()
{
	bIsStaggered = false;
	UE_LOG(LogCombatAI, Log, TEXT("%s recovered from stagger."), *GetOwner()->GetName());
	OnStaggerRecovered.Broadcast();
}

// ───── Armor ─────

float UCombatComponent::ApplyArmorModifiers(float BaseDamage, const FVector& HitDirection, bool& bOutWeakPoint) const
{
	bOutWeakPoint = false;

	if (FrontalArmorReduction <= 0.0f && WeakPointMultiplier <= 1.0f)
	{
		return BaseDamage;
	}

	const FVector OwnerForward = GetOwner()->GetActorForwardVector();
	const FVector NormalizedHit = HitDirection.GetSafeNormal();

	// Dot product: +1 = hit from front, -1 = hit from behind.
	const float Dot = FVector::DotProduct(OwnerForward, NormalizedHit);

	if (Dot > 0.3f && FrontalArmorReduction > 0.0f)
	{
		// Frontal hit — reduce damage.
		return BaseDamage * (1.0f - FrontalArmorReduction);
	}
	else if (Dot < -0.3f && WeakPointMultiplier > 1.0f)
	{
		// Rear hit — weak point bonus.
		bOutWeakPoint = true;
		return BaseDamage * WeakPointMultiplier;
	}

	return BaseDamage;
}

// ───── Reset ─────

void UCombatComponent::ResetCombatState()
{
	CurrentHealth = MaxHealth;
	CurrentStaggerAccum = 0.0f;
	bIsStaggered = false;
	StaggerDecayTimer = 0.0f;

	GetWorld()->GetTimerManager().ClearTimer(StaggerTimerHandle);
}
