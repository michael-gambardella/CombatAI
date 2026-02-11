// Copyright CombatAI Project. All Rights Reserved.

#include "WatcherCharacter.h"
#include "CombatAI/AI/UtilityAI/UtilityAIComponent.h"
#include "CombatAI/AI/Core/CombatAIPerceptionComponent.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/CombatAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AWatcherCharacter::AWatcherCharacter()
{
	Archetype = EEnemyArchetype::Watcher;
	EnemyDisplayName = TEXT("The Watcher");

	CombatComp->MaxHealth = 30.0f;
	CombatComp->StaggerThreshold = 10.0f;
	CombatComp->StaggerDuration = 1.0f;

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	UtilityAI = CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAI"));
	UtilityAI->DecisionRate = 5.0f;

	PerceptionComp = CreateDefaultSubobject<UAICombatPerceptionComponent>(TEXT("Perception"));
	PerceptionComp->DetectionRange = 4000.0f;
	PerceptionComp->FieldOfView = 120.0f;
	PerceptionComp->HearingRange = 2000.0f;
}

void AWatcherCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureUtilityActions();
}

void AWatcherCharacter::OnEnterCombat_Implementation()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s entering combat!"), *GetName());
}

void AWatcherCharacter::ConfigureUtilityActions()
{
	UtilityAI->Actions.Empty();

	// 1. Aimed Shot
	{
		FUtilityAction A;
		A.ActionName = TEXT("AimedShot");
		A.Weight = 1.2f;
		A.Cooldown = 2.0f;
		A.bRequiresAttackToken = true;

		FUtilityConsideration Dist;
		Dist.ConsiderationName = TEXT("OptimalRange");
		Dist.InputType = EConsiderationInput::DistanceToPlayer;
		Dist.InputMin = OptimalRangeMin;
		Dist.InputMax = OptimalRangeMax;
		Dist.Curve = FResponseCurve::MakeLogistic(8.0f, 0.5f);

		FUtilityConsideration LOS;
		LOS.ConsiderationName = TEXT("HasLOS");
		LOS.InputType = EConsiderationInput::PlayerVisible;
		LOS.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(Dist);
		A.Considerations.Add(LOS);
		UtilityAI->Actions.Add(A);
	}

	// 2. Overwatch Burst
	{
		FUtilityAction A;
		A.ActionName = TEXT("OverwatchBurst");
		A.Weight = 1.0f;
		A.Cooldown = 3.0f;
		A.bRequiresAttackToken = true;

		FUtilityConsideration AllyMelee;
		AllyMelee.ConsiderationName = TEXT("AllyInMelee");
		AllyMelee.InputType = EConsiderationInput::AllyInMelee;
		AllyMelee.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration LOS;
		LOS.ConsiderationName = TEXT("HasLOS");
		LOS.InputType = EConsiderationInput::PlayerVisible;
		LOS.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(AllyMelee);
		A.Considerations.Add(LOS);
		UtilityAI->Actions.Add(A);
	}

	// 3. Seek Cover
	{
		FUtilityAction A;
		A.ActionName = TEXT("SeekCover");
		A.Weight = 1.1f;

		FUtilityConsideration Exposed;
		Exposed.ConsiderationName = TEXT("Exposed");
		Exposed.InputType = EConsiderationInput::Exposed;
		Exposed.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration Threat;
		Threat.ConsiderationName = TEXT("ThreatLevel");
		Threat.InputType = EConsiderationInput::ThreatLevel;
		Threat.InputMin = 0.0f;
		Threat.InputMax = 100.0f;
		Threat.Curve = FResponseCurve::MakeLinear(1.0f, 0.0f);

		A.Considerations.Add(Exposed);
		A.Considerations.Add(Threat);
		UtilityAI->Actions.Add(A);
	}

	// 4. Seek High Ground
	{
		FUtilityAction A;
		A.ActionName = TEXT("SeekHighGround");
		A.Weight = 0.8f;

		FUtilityConsideration Elev;
		Elev.ConsiderationName = TEXT("ElevationAvailable");
		Elev.InputType = EConsiderationInput::Elevation;
		Elev.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration NotAt;
		NotAt.ConsiderationName = TEXT("NotAtElevation");
		NotAt.InputType = EConsiderationInput::NotAtElevation;
		NotAt.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(Elev);
		A.Considerations.Add(NotAt);
		UtilityAI->Actions.Add(A);
	}

	// 5. Retreat
	{
		FUtilityAction A;
		A.ActionName = TEXT("Retreat");
		A.Weight = 1.4f;

		FUtilityConsideration Close;
		Close.ConsiderationName = TEXT("TooClose");
		Close.InputType = EConsiderationInput::DistanceToPlayer;
		Close.InputMin = 0.0f;
		Close.InputMax = RetreatDistance;
		Close.Curve = FResponseCurve::MakeInverse();

		FUtilityConsideration Health;
		Health.ConsiderationName = TEXT("LowHealth");
		Health.InputType = EConsiderationInput::OwnHealth;
		Health.Curve = FResponseCurve::MakeInverse();

		A.Considerations.Add(Close);
		A.Considerations.Add(Health);
		UtilityAI->Actions.Add(A);
	}

	// 6. Reposition
	{
		FUtilityAction A;
		A.ActionName = TEXT("Reposition");
		A.Weight = 0.7f;

		FUtilityConsideration LOSBlocked;
		LOSBlocked.ConsiderationName = TEXT("LOSBlocked");
		LOSBlocked.InputType = EConsiderationInput::LOSBlocked;
		LOSBlocked.Curve = FResponseCurve::MakeStep(0.5f);

		A.Considerations.Add(LOSBlocked);
		UtilityAI->Actions.Add(A);
	}
}

// ───── Action Execution ─────

void AWatcherCharacter::ExecuteAimedShot()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Aimed Shot"), *GetName());
	if (AimedShotMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(AimedShotMontage, 1.0f);
		}
	}
	// Projectile fired after wind-up via anim notify calling FireProjectile.
}

void AWatcherCharacter::ExecuteOverwatchBurst()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Overwatch Burst"), *GetName());
	if (BurstFireMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(BurstFireMontage, 1.0f);
		}
	}
}

void AWatcherCharacter::ExecuteSeekCover()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Seeking cover"), *GetName());
}

void AWatcherCharacter::ExecuteSeekHighGround()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Seeking high ground"), *GetName());
}

void AWatcherCharacter::ExecuteRetreat()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Retreating"), *GetName());
}

void AWatcherCharacter::ExecuteReposition()
{
	UE_LOG(LogCombatAI, Log, TEXT("Watcher %s: Repositioning"), *GetName());
}

void AWatcherCharacter::FireProjectile(float Damage, float Accuracy)
{
	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	// Accuracy check: randomize hit based on accuracy value.
	const bool bHit = FMath::FRand() <= Accuracy;

	if (bHit)
	{
		FDamageInfo DmgInfo;
		DmgInfo.Amount = Damage;
		DmgInfo.StaggerLevel = (Damage >= 15.0f) ? EStaggerLevel::High : EStaggerLevel::Low;
		DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(DmgInfo.StaggerLevel);
		DmgInfo.DamageSource = this;
		DmgInfo.HitDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		DmgInfo.HitLocation = Player->GetActorLocation();
		DmgInfo.DamageType = EDamageType::Ranged;

		UCombatComponent::DealDamage(Player, DmgInfo);
	}

	UE_LOG(LogCombatAI, Verbose, TEXT("Watcher %s fired projectile: %s (Accuracy: %.2f)"),
		*GetName(), bHit ? TEXT("HIT") : TEXT("MISS"), Accuracy);
}
