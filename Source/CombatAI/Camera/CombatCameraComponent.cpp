// Copyright CombatAI Project. All Rights Reserved.

#include "CombatCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "CombatAI/CombatAI.h"

UCombatCameraComponent::UCombatCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyParameters();
}

void UCombatCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessShake(DeltaTime);
	ProcessHitlag(DeltaTime);
	ProcessSlowMotion(DeltaTime);
}

void UCombatCameraComponent::ApplyParameters()
{
	if (SpringArm)
	{
		SpringArm->TargetArmLength = FollowDistance;
		SpringArm->SetRelativeLocation(FVector(0.0f, HorizontalOffset, FollowHeight));
		SpringArm->CameraLagSpeed = FollowInterpSpeed;
		SpringArm->bEnableCameraLag = true;
	}

	if (Camera)
	{
		Camera->FieldOfView = BaseFOV;
	}
}

// ───── Screen Shake ─────

void UCombatCameraComponent::TriggerScreenShake(float Intensity, float Duration)
{
	CurrentShakeIntensity = Intensity;
	ShakeTimer = Duration;

	UE_LOG(LogCombatAI, Verbose, TEXT("Camera shake triggered: Intensity=%.1f Duration=%.2fs"), Intensity, Duration);
}

void UCombatCameraComponent::ProcessShake(float DeltaTime)
{
	if (ShakeTimer <= 0.0f) return;

	ShakeTimer -= DeltaTime;
	const float Alpha = FMath::Clamp(ShakeTimer / 0.15f, 0.0f, 1.0f);

	if (Camera)
	{
		const FVector ShakeOffset(
			FMath::RandRange(-1.0f, 1.0f) * CurrentShakeIntensity * Alpha,
			FMath::RandRange(-1.0f, 1.0f) * CurrentShakeIntensity * Alpha,
			0.0f
		);
		Camera->SetRelativeLocation(ShakeOffset);
	}

	if (ShakeTimer <= 0.0f && Camera)
	{
		Camera->SetRelativeLocation(FVector::ZeroVector);
	}
}

// ───── Hitlag ─────

void UCombatCameraComponent::TriggerHitlag(float Duration)
{
	const float FinalDuration = (Duration < 0.0f) ? HitlagDuration : Duration;
	if (FinalDuration <= 0.0f) return;

	bHitlagActive = true;
	HitlagTimer = FinalDuration;

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 0.01f);
	}

	UE_LOG(LogCombatAI, Verbose, TEXT("Hitlag triggered: Duration=%.3fs"), FinalDuration);
}

void UCombatCameraComponent::ProcessHitlag(float DeltaTime)
{
	if (!bHitlagActive) return;

	// Use real time, not dilated time.
	HitlagTimer -= FApp::GetDeltaTime();

	if (HitlagTimer <= 0.0f)
	{
		bHitlagActive = false;
		if (UWorld* World = GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);
		}
	}
}

// ───── Slow Motion ─────

void UCombatCameraComponent::TriggerSlowMotion(float TimeScale, float Duration)
{
	bSlowMotionActive = true;
	SlowMotionTimer = Duration;

	if (UWorld* World = GetWorld())
	{
		OriginalTimeDilation = World->GetWorldSettings()->TimeDilation;
		UGameplayStatics::SetGlobalTimeDilation(World, TimeScale);
	}

	UE_LOG(LogCombatAI, Verbose, TEXT("Slow motion triggered: Scale=%.2f Duration=%.2fs"), TimeScale, Duration);
}

void UCombatCameraComponent::ProcessSlowMotion(float DeltaTime)
{
	if (!bSlowMotionActive) return;

	SlowMotionTimer -= FApp::GetDeltaTime();

	if (SlowMotionTimer <= 0.0f)
	{
		bSlowMotionActive = false;
		if (UWorld* World = GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(World, OriginalTimeDilation);
		}
	}
}
