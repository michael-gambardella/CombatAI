// Copyright CombatAI Project. All Rights Reserved.

#include "RusherCharacter.h"
#include "CombatAI/AI/UtilityAI/UtilityAIComponent.h"
#include "CombatAI/AI/Core/AIPerceptionComponent.h"
#include "CombatAI/Combat/CombatComponent.h"
#include "CombatAI/Combat/HitDetection.h"
#include "CombatAI/CombatAI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARusherCharacter::ARusherCharacter()
{
	// Identity.
	Archetype = EEnemyArchetype::Rusher;
	EnemyDisplayName = TEXT("The Blade");

	// Stats from GDD.
	CombatComp->MaxHealth = 40.0f;
	CombatComp->StaggerThreshold = 20.0f;
	CombatComp->StaggerDuration = 0.8f;

	GetCharacterMovement()->MaxWalkSpeed = 650.0f;

	// Components.
	UtilityAI = CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAI"));
	UtilityAI->DecisionRate = 5.0f;
	UtilityAI->ScoreNoise = 0.05f;

	PerceptionComp = CreateDefaultSubobject<UAICombatPerceptionComponent>(TEXT("Perception"));
	PerceptionComp->DetectionRange = 2500.0f;
	PerceptionComp->FieldOfView = 120.0f;
	PerceptionComp->HearingRange = 1500.0f;
}

void ARusherCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureUtilityActions();
}

void ARusherCharacter::OnEnterCombat_Implementation()
{
	UE_LOG(LogCombatAI, Log, TEXT("Rusher %s entering combat!"), *GetName());
}

// ═══════════════════════════════════════════════════════════════
// UTILITY ACTION CONFIGURATION
// ═══════════════════════════════════════════════════════════════

void ARusherCharacter::ConfigureUtilityActions()
{
	UtilityAI->Actions.Empty();

	// ── 1. Chase Player ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("Chase");
		Action.Weight = 1.0f;

		FUtilityConsideration DistCon;
		DistCon.ConsiderationName = TEXT("Distance");
		DistCon.InputType = EConsiderationInput::DistanceToPlayer;
		DistCon.InputMin = 0.0f;
		DistCon.InputMax = 2500.0f;
		DistCon.Curve = FResponseCurve::MakeLogistic(8.0f, 0.3f); // Higher score when far

		FUtilityConsideration VisCon;
		VisCon.ConsiderationName = TEXT("Visible");
		VisCon.InputType = EConsiderationInput::PlayerVisible;
		VisCon.Curve = FResponseCurve::MakeStep(0.5f);

		Action.Considerations.Add(DistCon);
		Action.Considerations.Add(VisCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 2. Melee Attack ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("MeleeAttack");
		Action.Weight = 1.2f;
		Action.Cooldown = AttackCooldown;
		Action.bRequiresAttackToken = true;

		FUtilityConsideration DistCon;
		DistCon.ConsiderationName = TEXT("InMeleeRange");
		DistCon.InputType = EConsiderationInput::DistanceToPlayer;
		DistCon.InputMin = 0.0f;
		DistCon.InputMax = 200.0f;
		DistCon.Curve = FResponseCurve::MakeInverse(); // Closer = higher

		FUtilityConsideration CoolCon;
		CoolCon.ConsiderationName = TEXT("CooldownReady");
		CoolCon.InputType = EConsiderationInput::CooldownReady;
		CoolCon.Curve = FResponseCurve::MakeStep(0.5f);

		Action.Considerations.Add(DistCon);
		Action.Considerations.Add(CoolCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 3. Lunge Attack ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("Lunge");
		Action.Weight = 1.1f;
		Action.Cooldown = 3.0f;
		Action.bRequiresAttackToken = true;

		FUtilityConsideration DistCon;
		DistCon.ConsiderationName = TEXT("LungeRange");
		DistCon.InputType = EConsiderationInput::DistanceToPlayer;
		DistCon.InputMin = 300.0f;
		DistCon.InputMax = 600.0f;
		DistCon.Curve = FResponseCurve::MakeLogistic(10.0f, 0.5f);

		FUtilityConsideration StationaryCon;
		StationaryCon.ConsiderationName = TEXT("PlayerStationary");
		StationaryCon.InputType = EConsiderationInput::PlayerAttacking;
		StationaryCon.Curve = FResponseCurve::MakeLinear(0.5f, 0.5f); // Slightly prefer stationary

		Action.Considerations.Add(DistCon);
		Action.Considerations.Add(StationaryCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 4. Circle Strafe ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("CircleStrafe");
		Action.Weight = 0.8f;

		FUtilityConsideration DistCon;
		DistCon.ConsiderationName = TEXT("CloseRange");
		DistCon.InputType = EConsiderationInput::DistanceToPlayer;
		DistCon.InputMin = 0.0f;
		DistCon.InputMax = 300.0f;
		DistCon.Curve = FResponseCurve::MakeInverse();

		FUtilityConsideration AllyCon;
		AllyCon.ConsiderationName = TEXT("AllyAttacking");
		AllyCon.InputType = EConsiderationInput::AllyAttacking;
		AllyCon.Curve = FResponseCurve::MakeStep(0.5f);

		Action.Considerations.Add(DistCon);
		Action.Considerations.Add(AllyCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 5. Flank ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("Flank");
		Action.Weight = 0.9f;

		FUtilityConsideration GroupCon;
		GroupCon.ConsiderationName = TEXT("FlankRequest");
		GroupCon.InputType = EConsiderationInput::GroupRequest;
		GroupCon.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration FacingCon;
		FacingCon.ConsiderationName = TEXT("PlayerFacing");
		FacingCon.InputType = EConsiderationInput::PlayerFacing;
		FacingCon.Curve = FResponseCurve::MakeInverse(); // Higher when player is NOT facing

		Action.Considerations.Add(GroupCon);
		Action.Considerations.Add(FacingCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 6. Retreat ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("Retreat");
		Action.Weight = 1.3f;

		FUtilityConsideration HealthCon;
		HealthCon.ConsiderationName = TEXT("LowHealth");
		HealthCon.InputType = EConsiderationInput::OwnHealth;
		HealthCon.InputMin = 0.0f;
		HealthCon.InputMax = 1.0f;
		HealthCon.Curve = FResponseCurve::MakeQuadratic(2.0f); // Inverted in eval via low threshold
		// We want retreat to score HIGH when health is LOW, so use Inverse curve on health
		HealthCon.Curve = FResponseCurve::MakeInverse();

		FUtilityConsideration DmgCon;
		DmgCon.ConsiderationName = TEXT("DamageReceived");
		DmgCon.InputType = EConsiderationInput::DamageReceivedRecently;
		DmgCon.InputMin = 0.0f;
		DmgCon.InputMax = 30.0f;
		DmgCon.Curve = FResponseCurve::MakeLinear(1.0f, 0.0f);

		Action.Considerations.Add(HealthCon);
		Action.Considerations.Add(DmgCon);
		UtilityAI->Actions.Add(Action);
	}

	// ── 7. Investigate ──
	{
		FUtilityAction Action;
		Action.ActionName = TEXT("Investigate");
		Action.Weight = 0.6f;

		FUtilityConsideration LKPCon;
		LKPCon.ConsiderationName = TEXT("LastKnownValid");
		LKPCon.InputType = EConsiderationInput::LastKnownPositionValid;
		LKPCon.Curve = FResponseCurve::MakeStep(0.5f);

		FUtilityConsideration CantSeeCon;
		CantSeeCon.ConsiderationName = TEXT("CantSeePlayer");
		CantSeeCon.InputType = EConsiderationInput::CantSeePlayer;
		CantSeeCon.Curve = FResponseCurve::MakeStep(0.5f);

		Action.Considerations.Add(LKPCon);
		Action.Considerations.Add(CantSeeCon);
		UtilityAI->Actions.Add(Action);
	}
}

// ═══════════════════════════════════════════════════════════════
// ACTION EXECUTION (called by Behavior Tree Tasks)
// ═══════════════════════════════════════════════════════════════

void ARusherCharacter::ExecuteChase()
{
	// BT Task will handle MoveTo; this just logs intent.
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Chasing player"), *GetName());
}

void ARusherCharacter::ExecuteSlashCombo()
{
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Executing Slash Combo"), *GetName());

	if (SlashComboMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(SlashComboMontage, 1.0f);
		}
	}

	// Enable hit detection with slash damage.
	FDamageInfo DmgInfo;
	DmgInfo.Amount = SlashDamage;
	DmgInfo.StaggerLevel = EStaggerLevel::Low;
	DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(EStaggerLevel::Low);
	DmgInfo.DamageType = EDamageType::Melee;
	HitDetection->EnableHitDetection(DmgInfo);
}

void ARusherCharacter::ExecuteLunge()
{
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Executing Lunge"), *GetName());

	// Face the player.
	if (AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector Dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		SetActorRotation(Dir.Rotation());
	}

	if (LungeMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(LungeMontage, 1.0f);
		}
	}

	// Lunge damage.
	FDamageInfo DmgInfo;
	DmgInfo.Amount = LungeDamage;
	DmgInfo.StaggerLevel = EStaggerLevel::Medium;
	DmgInfo.StaggerValue = CombatUtils::StaggerLevelToValue(EStaggerLevel::Medium);
	DmgInfo.DamageType = EDamageType::Melee;
	HitDetection->EnableHitDetection(DmgInfo);
}

void ARusherCharacter::ExecuteCircleStrafe()
{
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Circle strafing"), *GetName());
	// BT Task handles orbit movement around the player at CircleSpeed.
}

void ARusherCharacter::ExecuteRetreat()
{
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Retreating"), *GetName());
	// BT Task handles MoveTo away from player.
}

void ARusherCharacter::ExecuteInvestigate()
{
	UE_LOG(LogCombatAI, Verbose, TEXT("Rusher %s: Investigating last known position"), *GetName());
	// BT Task handles MoveTo LastKnownPlayerPosition.
}
