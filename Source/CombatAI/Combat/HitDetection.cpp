// Copyright CombatAI Project. All Rights Reserved.

#include "HitDetection.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "Components/SkeletalMeshComponent.h"

UHitDetectionComponent::UHitDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHitDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsActive)
	{
		PerformTrace();
	}
}

void UHitDetectionComponent::EnableHitDetection(const FDamageInfo& DamageTemplate)
{
	bIsActive = true;
	ActiveDamageTemplate = DamageTemplate;
	ActiveDamageTemplate.DamageSource = GetOwner();
	ClearHitList();
}

void UHitDetectionComponent::DisableHitDetection()
{
	bIsActive = false;
}

void UHitDetectionComponent::ClearHitList()
{
	HitActors.Empty();
}

void UHitDetectionComponent::PerformTrace()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	const FVector Start = Mesh->GetSocketLocation(TraceStartSocket);
	const FVector End = Mesh->GetSocketLocation(TraceEndSocket);

	TArray<FHitResult> Hits;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere, Params);

	if (!bHit) return;

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActors.Contains(HitActor)) continue;

		// Check if target has a CombatComponent.
		UCombatComponent* TargetCombat = HitActor->FindComponentByClass<UCombatComponent>();
		if (!TargetCombat || !TargetCombat->IsAlive()) continue;

		// Add to hit list to prevent double-hit.
		HitActors.Add(HitActor);

		// Build final damage info.
		FDamageInfo DmgInfo = ActiveDamageTemplate;
		DmgInfo.HitLocation = Hit.ImpactPoint;
		DmgInfo.HitDirection = (Hit.ImpactPoint - Owner->GetActorLocation()).GetSafeNormal();

		// Apply damage.
		TargetCombat->ReceiveDamage(DmgInfo);

		// Broadcast for VFX / audio.
		OnHitDetected.Broadcast(HitActor, DmgInfo);

		UE_LOG(LogCombatAI, Verbose, TEXT("Hit detected: %s -> %s (%.1f dmg)"),
			*Owner->GetName(), *HitActor->GetName(), DmgInfo.Amount);
	}
}
