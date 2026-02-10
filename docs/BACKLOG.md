# CombatAI — Product Backlog

> **Project:** Third-Person Combat AI System
> **Timeline:** 16 Weeks (8 Sprints, 2 weeks each)
> **Sprint Cadence:** 2-week sprints
> **Story Points:** Fibonacci (1, 2, 3, 5, 8, 13)
> **Velocity Target:** ~20–26 SP per sprint
> **Last Updated:** February 10, 2026

---

## Table of Contents

1. [Epics Overview](#1-epics-overview)
2. [Epic Details & User Stories](#2-epic-details--user-stories)
3. [Sprint Plan](#3-sprint-plan)
4. [Test Plan](#4-test-plan)
5. [Definition of Done](#5-definition-of-done)
6. [Backlog Prioritization Key](#6-backlog-prioritization-key)

---

## 1. Epics Overview

| ID | Epic | Phase | Priority | Total SP |
|---|---|---|---|---|
| E-01 | Project Foundation & Player Movement | Phase 1 | P0 | 21 |
| E-02 | Player Combat System | Phase 1 | P0 | 29 |
| E-03 | Camera System | Phase 1 | P0 | 13 |
| E-04 | Core AI Framework | Phase 1 | P0 | 26 |
| E-05 | Rusher Archetype ("The Blade") | Phase 2 | P0 | 21 |
| E-06 | Watcher Archetype ("The Watcher") | Phase 2 | P0 | 21 |
| E-07 | Warden Archetype ("The Warden") | Phase 2 | P0 | 24 |
| E-08 | AI Data Assets & Designer Tooling | Phase 2 | P0 | 16 |
| E-09 | Squad Coordination System | Phase 3 | P0 | 26 |
| E-10 | Encounter & Wave System | Phase 3 | P0 | 18 |
| E-11 | Debug & Developer Tools | Phase 4 | P1 | 21 |
| E-12 | Combat Feel & Polish (Juice) | Phase 4 | P1 | 18 |
| E-13 | UI & HUD | Phase 4 | P1 | 21 |
| E-14 | Difficulty System | Phase 4 | P2 | 10 |
| E-15 | Audio & VFX | Phase 4 | P1 | 16 |
| E-16 | Documentation & Packaging | Phase 4 | P1 | 10 |

**Grand Total: ~291 Story Points across 16 weeks**

---

## 2. Epic Details & User Stories

---

### E-01: Project Foundation & Player Movement

> *As a player, I can control a responsive third-person character that moves fluidly through the arena so that navigation feels natural and responsive.*

**Dependencies:** None (this is the foundation)
**Phase:** 1 — Foundation (Weeks 1–2)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-001 | UE5 project setup with folder structure, build config, Git + LFS | P0 | 2 | S1 |
| S-002 | Player character base class with skeletal mesh and capsule collision | P0 | 3 | S1 |
| S-003 | Player locomotion: walk (400 cm/s), sprint (700 cm/s) with stamina cost | P0 | 5 | S1 |
| S-004 | Dodge roll with directional input (600 cm/s, 0.5s duration) | P0 | 5 | S1 |
| S-005 | Single jump (300 cm height) for platform traversal | P0 | 3 | S1 |
| S-006 | Input mapping for gamepad and keyboard/mouse (all movement actions) | P0 | 3 | S1 |

#### Story Details

**S-001: Project Setup**
> As a developer, I need a properly structured UE5 project with version control so that I have a clean foundation to build on.

Acceptance Criteria:
- [ ] UE5 project compiles and runs in editor
- [ ] Folder structure matches technical architecture spec (`/Source/CombatAI/Core/`, `/Player/`, `/AI/`, etc.)
- [ ] Git repository initialized with `.gitignore` for UE5 and Git LFS configured for binary assets (`.uasset`, `.umap`)
- [ ] `CombatAIGameMode`, `CombatAIGameState`, and `CombatAIPlayerController` base classes exist
- [ ] Project runs at 60 FPS in an empty level

**S-002: Player Character Base**
> As a developer, I need a player character actor with proper collision and a placeholder mesh so that I have a physical entity in the world.

Acceptance Criteria:
- [ ] `PlayerCharacter` class inherits from `ACharacter`
- [ ] Skeletal mesh component configured with placeholder humanoid mesh (Mixamo/Mannequin)
- [ ] Capsule collision sized appropriately for combat (half-height ~88, radius ~34)
- [ ] Character spawns at `PlayerStart` and is visible in-game
- [ ] `PlayerStatsComponent` attached with Health (100), Stamina (100), and Ammo (12)

**S-003: Locomotion**
> As a player, I can walk and sprint through the arena so that I can navigate toward and away from enemies.

Acceptance Criteria:
- [ ] Walk speed is 400 cm/s by default
- [ ] Sprint speed is 700 cm/s when sprint input is held
- [ ] Sprint consumes stamina (configurable rate)
- [ ] Stamina regenerates over time when not sprinting/dodging
- [ ] Movement direction is relative to camera facing
- [ ] Locomotion blend space plays walk/run animations based on speed
- [ ] Instant turn rate with animation blending (no sluggish rotation)

**S-004: Dodge Roll**
> As a player, I can dodge roll to avoid enemy attacks so that I have a defensive option against incoming damage.

Acceptance Criteria:
- [ ] Dodge roll activates on B/Circle/Space input
- [ ] Roll direction matches movement input (or backward if no input)
- [ ] Roll velocity is 600 cm/s for 0.5s duration
- [ ] Invincibility frames active during frames 0.1–0.35s of the roll
- [ ] Dodge consumes stamina (configurable amount)
- [ ] Dodge cancels most player actions (attack recovery, movement)
- [ ] Cannot dodge when stamina is depleted
- [ ] Dodge animation plays correctly with root motion

**S-005: Jump**
> As a player, I can jump to reach elevated platforms so that I can access vertical areas of the arena.

Acceptance Criteria:
- [ ] Single jump reaches 300 cm height
- [ ] Jump activates on standard jump input
- [ ] Airborne physics feel responsive (not floaty)
- [ ] Landing animation plays on ground contact
- [ ] Cannot jump while in dodge roll

**S-006: Input Mapping**
> As a developer, I need a complete input mapping for gamepad and keyboard/mouse so that all player actions are bindable.

Acceptance Criteria:
- [ ] Enhanced Input System configured with Input Mapping Context
- [ ] All movement actions mapped per the input table in the GDD (Move, Camera, Sprint, Dodge, Jump)
- [ ] Gamepad and keyboard/mouse mappings both functional
- [ ] Input is responsive with no perceptible lag (< 100ms input-to-action)
- [ ] Dead zones configured appropriately for gamepad sticks

---

### E-02: Player Combat System

> *As a player, I can execute melee combos, charged attacks, parries, and ranged shots so that I have a full toolkit to engage enemies in combat.*

**Dependencies:** E-01 (Player Movement)
**Phase:** 1 — Foundation (Weeks 2–3)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-007 | Light attack 3-hit combo chain (10 dmg per hit) | P0 | 5 | S1 |
| S-008 | Heavy attack with charge mechanic (25 dmg, high stagger) | P0 | 5 | S1 |
| S-009 | Parry system with timed block window and counter-attack (30 dmg) | P0 | 5 | S2 |
| S-010 | Ranged attack with projectile (15 dmg, 12 ammo, cooldown) | P0 | 5 | S2 |
| S-011 | Lock-on targeting system with target switching | P0 | 5 | S2 |
| S-012 | Stagger system — player stagger resistance and recovery | P1 | 2 | S2 |
| S-013 | Damage system — shared interface for applying/receiving damage | P0 | 2 | S2 |

#### Story Details

**S-007: Light Attack Combo**
> As a player, I can perform a 3-hit light attack combo to deal quick damage to nearby enemies.

Acceptance Criteria:
- [ ] X/Square/Left-Click triggers light attack
- [ ] 3-hit combo string plays sequentially with continued input
- [ ] Each hit deals 10 damage with Low stagger value
- [ ] Combo timing window is forgiving (~0.5s between presses)
- [ ] Combo resets to hit 1 after hit 3 or after timeout
- [ ] Attack animations play with root motion positioning
- [ ] Hit detection uses animation-driven collision volumes (not constant hitbox)
- [ ] Damage applies only during active hit frames (anim notify)
- [ ] Player can cancel combo recovery into dodge at any point

**S-008: Heavy Attack**
> As a player, I can charge and release a heavy attack to break enemy guards and deal high damage.

Acceptance Criteria:
- [ ] Y/Triangle/Right-Click (hold) begins charge animation
- [ ] Release triggers the heavy attack strike
- [ ] Deals 25 damage with High stagger value
- [ ] Longer wind-up than light attack (~0.8s)
- [ ] Visually distinct from light attacks (wider swing, different animation)
- [ ] Can break enemy guard/stance when stagger threshold is exceeded
- [ ] Can be canceled into dodge during charge (not during active frames)
- [ ] Stagger value is significantly higher than light attacks

**S-009: Parry System**
> As a player, I can time a parry to deflect enemy attacks and punish with a counter-attack.

Acceptance Criteria:
- [ ] LB/L1/Q activates parry stance
- [ ] Parry window is tight (~0.15–0.2s active frames)
- [ ] Successful parry against an incoming attack staggers the enemy
- [ ] Counter-attack window opens after successful parry (~0.8s)
- [ ] Counter-attack deals 30 damage with Max stagger
- [ ] Failed parry (mistimed) results in a brief recovery animation (punishable)
- [ ] Visual feedback on successful parry (flash, spark VFX placeholder)
- [ ] Audio cue on successful parry (placeholder SFX)

**S-010: Ranged Attack**
> As a player, I can fire a ranged projectile to engage distant enemies or finish off low-health targets.

Acceptance Criteria:
- [ ] RT/R2/Middle-Click fires a projectile
- [ ] Projectile deals 15 damage with Medium stagger
- [ ] 12 ammo capacity; displays on HUD
- [ ] Ammo replenished by defeating enemies or collecting pickups
- [ ] Projectile has visible trail (placeholder particle)
- [ ] Projectile speed is moderate (not hitscan — dodgeable by enemies)
- [ ] Cooldown between shots prevents spam (~0.3s)
- [ ] Soft aim assist when locked on to a target

**S-011: Lock-On System**
> As a player, I can lock on to an enemy so that my attacks track the target and the camera orbits them.

Acceptance Criteria:
- [ ] RS Click/R3/Tab toggles lock-on to nearest enemy
- [ ] Camera orbits the locked target; player strafes
- [ ] Player movement becomes relative to target (strafe left/right, advance/retreat)
- [ ] Right stick/mouse allows switching between targets (flick direction)
- [ ] Lock-on breaks when target dies or moves beyond range
- [ ] Lock-on indicator widget displayed on locked target
- [ ] Lock-on range is configurable (default ~2500 cm)
- [ ] Melee attacks orient toward locked target automatically

**S-012: Player Stagger**
> As a player, I can be staggered by enemy attacks, creating vulnerable windows that enemies can exploit.

Acceptance Criteria:
- [ ] Player has a stagger resistance value (Medium)
- [ ] Attacks that exceed stagger threshold cause a stagger animation
- [ ] Player cannot act during stagger recovery (~0.5s)
- [ ] Stagger recovery is faster than enemy stagger (player advantage)
- [ ] Stagger meter resets after recovery

**S-013: Damage System**
> As a developer, I need a shared damage interface so that any actor can deal and receive damage consistently.

Acceptance Criteria:
- [ ] `CombatComponent` provides `ApplyDamage()` and `ReceiveDamage()` interface
- [ ] Damage struct includes: Amount, StaggerValue, DamageSource, HitDirection, DamageType
- [ ] Both player and AI characters use the same damage pipeline
- [ ] Damage events fire delegates (`OnDamageReceived`, `OnDeath`)
- [ ] Health clamped at 0; death triggered when health reaches 0
- [ ] Damage numbers/direction available for UI display

---

### E-03: Camera System

> *As a player, I experience a responsive third-person camera that frames combat clearly and enhances the feel of every action.*

**Dependencies:** E-01 (Player Movement)
**Phase:** 1 — Foundation (Week 1)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-014 | Third-person follow camera with over-the-shoulder offset | P0 | 5 | S1 |
| S-015 | Camera collision avoidance (pull forward on geometry occlusion) | P0 | 3 | S1 |
| S-016 | Lock-on camera mode (orbit target, player strafes) | P0 | 3 | S2 |
| S-017 | Camera parameters exposed as designer-tunable properties | P1 | 2 | S2 |

#### Story Details

**S-014: Follow Camera**
> As a player, I see my character from a comfortable third-person over-the-shoulder perspective.

Acceptance Criteria:
- [ ] Camera positioned behind and above the player (Follow Distance: 350, Height: 120, Offset: 60 right)
- [ ] Smooth follow with configurable lag/interpolation
- [ ] Look-ahead adjusts camera in the direction of movement (strength: 0.3)
- [ ] Camera controlled by Right Stick / Mouse
- [ ] Default FOV of 75 degrees
- [ ] No camera jitter or snapping during normal movement

**S-015: Camera Collision**
> As a player, the camera never clips through walls or geometry — it intelligently adjusts position.

Acceptance Criteria:
- [ ] Sphere trace from target to ideal camera position detects collision
- [ ] Camera pulls forward smoothly when geometry is between camera and player
- [ ] Camera returns to ideal position smoothly when obstruction clears
- [ ] No visual popping or jarring transitions
- [ ] Minimum distance clamp prevents camera from entering the player mesh

**S-016: Lock-On Camera**
> As a player, when I lock on to an enemy, the camera reframes to keep both me and my target visible.

Acceptance Criteria:
- [ ] Camera position adjusts to frame both player and locked target
- [ ] Camera smoothly transitions between free and lock-on modes
- [ ] Player character strafes relative to locked target
- [ ] Target switching causes smooth camera pan (not snap)
- [ ] Camera maintains minimum/maximum distance from player during lock-on

**S-017: Camera Parameters**
> As a designer, I can tweak all camera values (distance, height, offset, FOV, lag) without code changes.

Acceptance Criteria:
- [ ] All camera parameters from the GDD exposed as `UPROPERTY(EditAnywhere)` with metadata
- [ ] Parameters editable in Data Asset or directly on the component in the editor
- [ ] Changes apply at runtime during PIE without recompile
- [ ] Clamping ranges prevent invalid values (e.g., FOV 60–100)

---

### E-04: Core AI Framework

> *As a developer, I have a modular AI architecture (Perception + Utility AI + Behavior Trees) that powers all enemy archetypes with clean separation of concerns.*

**Dependencies:** E-01, E-02 (Player exists to perceive and react to)
**Phase:** 1 — Foundation (Weeks 3–4)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-018 | AI Character base class with health, movement, and animation hooks | P0 | 3 | S2 |
| S-019 | AI Controller base class with Behavior Tree integration | P0 | 3 | S2 |
| S-020 | Perception System — sight (FOV cone, LOS raycasts, last known position) | P0 | 5 | S2 |
| S-021 | Perception System — hearing (sound events, radius propagation) | P1 | 3 | S2 |
| S-022 | Perception System — threat assessment (scoring 0–100, threat memory decay) | P0 | 3 | S2 |
| S-023 | Utility AI Component — action evaluation pipeline with response curves | P0 | 5 | S3 |
| S-024 | Response Curve system (Linear, Quadratic, Logistic, Step, Inverse) | P0 | 2 | S3 |
| S-025 | AI State Machine (Idle → Alert → Combat → Searching → Dead) | P0 | 2 | S3 |

#### Story Details

**S-018: AI Character Base**
> As a developer, I need a base enemy character class so all archetypes share common functionality.

Acceptance Criteria:
- [ ] `AICharacterBase` inherits from `ACharacter`
- [ ] Has health component (configurable max health)
- [ ] Has `CombatComponent` for shared damage pipeline
- [ ] Skeletal mesh and animation blueprint slots
- [ ] Death handling (ragdoll/death anim → cleanup/destroy after delay)
- [ ] Stagger system integrated (threshold from data asset, stagger animation, recovery)
- [ ] NavMesh agent configured for pathfinding
- [ ] Blueprint events fire for: `OnDamageReceived`, `OnStaggered`, `OnDeath`

**S-019: AI Controller Base**
> As a developer, I need a base AI controller that runs Behavior Trees and interfaces with the Utility AI.

Acceptance Criteria:
- [ ] `AIControllerBase` inherits from `AAIController`
- [ ] Behavior Tree assigned and started on possess
- [ ] Blackboard configured with standard keys (TargetActor, CurrentAction, AIState, etc.)
- [ ] Interface to receive Utility AI decisions and write to blackboard
- [ ] Clean start/stop lifecycle (pause AI when dead, resume when needed)

**S-020: Sight Perception**
> As an enemy, I can see the player within my field of view and maintain awareness of their last known position.

Acceptance Criteria:
- [ ] FOV cone configurable per archetype (default 120°)
- [ ] Detection range configurable per archetype
- [ ] Line-of-sight raycast checks at configurable frequency (default 0.2s)
- [ ] Peripheral detection at wider angle (270°) with reduced certainty
- [ ] `bCanSeePlayer` flag updated each perception tick
- [ ] `LastKnownPlayerPosition` stored when LOS breaks
- [ ] `TimeSinceLastSeen` tracked and accessible
- [ ] Perception does NOT see through walls/geometry

**S-021: Hearing Perception**
> As an enemy, I can hear player actions (attacks, sprint, projectile impacts) and investigate the sound source.

Acceptance Criteria:
- [ ] Player attacks, sprint footsteps, and projectile impacts generate sound events
- [ ] Sound events have configurable radius
- [ ] Sound attenuates with distance; blocked by geometry (simplified)
- [ ] `bCanHearPlayer` flag set when sound event in range
- [ ] Hearing stimulus can trigger Alert state if player is not visible
- [ ] Investigation behavior moves AI toward sound source location

**S-022: Threat Assessment**
> As an enemy, I evaluate a continuous threat score that influences my behavior decisions.

Acceptance Criteria:
- [ ] Threat Level scored 0–100 based on: proximity, player action, recent damage taken, ally casualties
- [ ] Each factor has a configurable weight
- [ ] Threat memory decays over time (configurable decay rate)
- [ ] Threat doesn't reset instantly when player disengages (memory persistence)
- [ ] `FPerceptionData` struct populated every perception tick with all fields from the GDD
- [ ] `OnPerceptionUpdated` delegate fires with updated data

**S-023: Utility AI Component**
> As a developer, I need a Utility AI evaluator that scores candidate actions using weighted considerations and response curves.

Acceptance Criteria:
- [ ] `UtilityAIComponent` evaluates a set of `UUtilityAction` objects each decision tick
- [ ] Each action has multiple `UUtilityConsideration` objects
- [ ] Considerations normalize world-state input to 0–1 range
- [ ] Response curve applied to normalized input to produce consideration score
- [ ] Scores combined multiplicatively per the evaluation pipeline
- [ ] Final score multiplied by global action weight
- [ ] Highest scoring action selected (with optional random noise for variety)
- [ ] Selected action written to Behavior Tree blackboard
- [ ] Decision tick rate configurable (default 5 Hz)
- [ ] `OnActionSelected` delegate fires with action and score

**S-024: Response Curves**
> As a designer, I can define response curves (Linear, Quadratic, Logistic, Step, Inverse) in data assets to shape AI decision-making.

Acceptance Criteria:
- [ ] `ResponseCurve` base class with `Evaluate(float Input) → float Output`
- [ ] Linear: `y = mx + b` (configurable slope and intercept)
- [ ] Quadratic: `y = ax² + bx + c` (configurable exponent)
- [ ] Logistic: S-curve with configurable steepness and midpoint
- [ ] Step: Binary threshold (configurable threshold value)
- [ ] Inverse: `y = 1 - input` (simple inversion)
- [ ] Curves stored as Data Assets and assignable to considerations
- [ ] Curve preview in editor (visual graph) if possible

**S-025: AI State Machine**
> As an enemy, I transition through high-level states (Idle → Alert → Combat → Searching → Dead) based on perception.

Acceptance Criteria:
- [ ] 5 states: Idle, Alert, Combat, Searching, Dead
- [ ] Idle → Alert: triggered by perception stimulus (sound or peripheral sight)
- [ ] Alert → Combat: triggered by confirmed visual sighting
- [ ] Combat → Searching: triggered by losing LOS beyond timeout
- [ ] Searching → Combat: triggered by re-acquiring LOS
- [ ] Searching → Idle: triggered by search timeout with no re-acquisition
- [ ] Any → Dead: triggered by health reaching 0
- [ ] State transitions fire `OnStateChanged` delegate
- [ ] AI tick rate varies by state (Idle = low, Combat = full rate)
- [ ] Behavior Tree selects sub-trees based on current state

---

### E-05: Rusher Archetype ("The Blade")

> *As a player, I face aggressive melee enemies that sprint at me, attack in combos, lunge to close gaps, circle to find openings, and retreat when wounded.*

**Dependencies:** E-04 (Core AI Framework)
**Phase:** 2 — Enemy Archetypes (Week 5)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-026 | Rusher character class with stats (40 HP, 650 cm/s, low stagger threshold) | P0 | 3 | S3 |
| S-027 | Rusher Utility Actions: Chase, Melee Attack, Lunge, Circle Strafe, Flank, Retreat, Investigate | P0 | 8 | S3 |
| S-028 | Rusher Behavior Tree subtrees for all actions | P0 | 5 | S3 |
| S-029 | Rusher attack patterns — Slash Combo (2-hit, 12 dmg), Lunge Strike (20 dmg), Circling Slash | P0 | 5 | S3 |

#### Story Details

**S-026: Rusher Base Setup**
> As a developer, I need a Rusher character with correct stats, visuals, and animation blueprint.

Acceptance Criteria:
- [ ] `RusherCharacter` inherits from `AICharacterBase`
- [ ] Stats set from GDD: 40 HP, 650 cm/s speed, 2500 cm detection, 120° FOV
- [ ] Placeholder lean/agile mesh with red energy accent material
- [ ] Animation Blueprint with locomotion blend space (idle, walk, run, sprint)
- [ ] Stagger threshold set to 20 damage
- [ ] Death animation and ragdoll configured

**S-027: Rusher Utility Actions**
> As a Rusher enemy, I evaluate 7 actions each decision tick based on the game state.

Acceptance Criteria:
- [ ] **Chase** — High utility when far from player and player is visible (inverse logistic distance)
- [ ] **Melee Attack** — High utility when within 200 cm and attack cooldown is ready
- [ ] **Lunge Attack** — High utility at 300–600 cm range when player is stationary (logistic distance)
- [ ] **Circle Strafe** — High utility when close but cooldown is active or ally is attacking
- [ ] **Flank** — Triggered by squad coordinator request when player is facing directly
- [ ] **Retreat** — Triggered when health below 25% or heavy damage received recently
- [ ] **Investigate** — Triggered when can't see player but have a last known position
- [ ] All considerations use proper response curves from data assets
- [ ] Action weights are configurable in data assets

**S-028: Rusher Behavior Trees**
> As a developer, I need BT subtrees that execute each Rusher action with proper animation and movement.

Acceptance Criteria:
- [ ] Chase subtree: MoveTo player with path updates every 0.3s; sprint animation
- [ ] Melee Attack subtree: Face player → play attack montage → damage on anim notify
- [ ] Lunge subtree: Face player → root motion lunge → damage at apex → recovery
- [ ] Circle Strafe subtree: Strafe around player at configured speed → timer → re-evaluate
- [ ] Retreat subtree: Move away from player → timer → re-evaluate
- [ ] Investigate subtree: Move to last known position → look around → return to idle/resume
- [ ] Flank subtree: Move to assigned flanking sector → then re-evaluate
- [ ] All subtrees exit cleanly on interruption (stagger, death)

**S-029: Rusher Attack Patterns**
> As a Rusher, I perform distinct telegraphed attacks that the player can learn to read and counter.

Acceptance Criteria:
- [ ] **Slash Combo**: 2-hit melee; 0.8s total; 12 damage per hit; short recovery
- [ ] **Lunge Strike**: Gap-closer from 300–600 cm; 0.6s charge; 20 damage; dodge window 0.3–0.5s
- [ ] **Circling Slash**: Strafe 90° then attack; used when player is blocking/facing
- [ ] All attacks have clear wind-up animation (telegraph)
- [ ] Hit detection via animation-driven collision volumes
- [ ] Attack cooldown enforced between attack strings (configurable)
- [ ] Attacks can be parried by the player (window matches wind-up)
- [ ] Attacks push through damage pipeline with correct stagger values

---

### E-06: Watcher Archetype ("The Watcher")

> *As a player, I face ranged enemies that maintain distance, take aimed shots, suppress me during ally engagements, seek cover and high ground, and flee when I close in.*

**Dependencies:** E-04 (Core AI Framework)
**Phase:** 2 — Enemy Archetypes (Week 6)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-030 | Watcher character class with stats (30 HP, 400 cm/s, 4000 cm detect, optimal range 1500–2500) | P0 | 3 | S4 |
| S-031 | Watcher Utility Actions: Aimed Shot, Overwatch Burst, Seek Cover, Seek High Ground, Retreat, Reposition | P0 | 8 | S4 |
| S-032 | Watcher Behavior Tree subtrees for all actions | P0 | 5 | S4 |
| S-033 | Watcher attack patterns — Aimed Shot (18 dmg, 1.2s wind-up), Overwatch Burst (8 dmg x3), Retreat Shot | P0 | 5 | S4 |

#### Story Details

**S-030: Watcher Base Setup**
> As a developer, I need a Watcher character with correct stats, visuals, and animation blueprint.

Acceptance Criteria:
- [ ] `WatcherCharacter` inherits from `AICharacterBase`
- [ ] Stats: 30 HP, 400 cm/s speed, 4000 cm detection, 120° FOV, optimal range 1500–2500 cm
- [ ] Placeholder tall/slender mesh with blue/cyan energy accent material
- [ ] Animation Blueprint with locomotion and aiming blend spaces
- [ ] Stagger threshold set to 10 damage (very low — fragile)
- [ ] Death animation configured

**S-031: Watcher Utility Actions**
> As a Watcher enemy, I evaluate 6 actions to maintain distance and provide ranged support.

Acceptance Criteria:
- [ ] **Aimed Shot** — High utility at 1500–2500 cm with clear LOS and cooldown ready
- [ ] **Overwatch Burst** — High utility when ally is in melee with player and LOS clear
- [ ] **Seek Cover** — High utility when exposed, player is facing, or threat level is high
- [ ] **Seek High Ground** — High utility when elevation available and not already elevated
- [ ] **Retreat** — High utility when player is within 500 cm or health is low
- [ ] **Reposition** — High utility when LOS blocked or current position score is poor
- [ ] All considerations use response curves; weights configurable in data assets

**S-032: Watcher Behavior Trees**
> As a developer, I need BT subtrees that execute each Watcher action.

Acceptance Criteria:
- [ ] Aimed Shot subtree: Stop movement → face player → wind-up (1.2s) → fire projectile
- [ ] Overwatch Burst subtree: Face player → burst fire (3 rounds) → short cooldown
- [ ] Seek Cover subtree: Evaluate nearby cover nodes → move to best cover → crouch/stand behind
- [ ] Seek High Ground subtree: Find elevated nav point → path to it → take position
- [ ] Retreat subtree: Move away from player while backpedaling → optionally fire retreat shot
- [ ] Reposition subtree: Find new position with LOS → move to it
- [ ] All subtrees handle interruption (stagger, death, new higher-priority action)

**S-033: Watcher Attack Patterns**
> As a Watcher, I perform ranged attacks with clear telegraphs that the player can react to.

Acceptance Criteria:
- [ ] **Aimed Shot**: 1.2s wind-up with visible indicator (laser sight / charging glow); 18 damage; high accuracy
- [ ] **Overwatch Burst**: 3-round burst; 8 damage per hit; reduced accuracy; suppressive intent
- [ ] **Retreat Shot**: Fired while backpedaling; reduced accuracy; used when player closes in
- [ ] Projectiles are physical actors with collision, not hitscan
- [ ] Projectile speed allows player reaction time (dodgeable at range)
- [ ] Wind-up VFX/SFX placeholder for telegraph readability
- [ ] Accuracy modified by: base accuracy, target movement, own movement state

---

### E-07: Warden Archetype ("The Warden")

> *As a player, I face a slow, heavily armored tank enemy that blocks frontal attacks with a shield, performs devastating AOE attacks, charges to disrupt my positioning, and protects nearby allies — with a glowing weak point on its back.*

**Dependencies:** E-04 (Core AI Framework)
**Phase:** 2 — Enemy Archetypes (Week 7)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-034 | Warden character class with stats (150 HP, 250 cm/s, frontal armor, weak point) | P0 | 3 | S4 |
| S-035 | Warden frontal shield — 50% damage reduction from front, 2x damage on back weak point | P0 | 5 | S4 |
| S-036 | Warden Utility Actions: Advance, Ground Slam, Shield Charge, Shield Bash, Stomp Wave, Protect Ally, Face Player | P0 | 8 | S4 |
| S-037 | Warden Behavior Tree subtrees for all actions | P0 | 5 | S5 |
| S-038 | Warden attack patterns — Ground Slam (30 dmg AOE), Shield Charge (25 dmg), Shield Bash (15 dmg), Stomp Wave (20 dmg) | P0 | 3 | S5 |

#### Story Details

**S-034: Warden Base Setup**
> As a developer, I need a Warden character with correct stats, visuals, and unique armor mechanics.

Acceptance Criteria:
- [ ] `WardenCharacter` inherits from `AICharacterBase`
- [ ] Stats: 150 HP, 250 cm/s speed, 800 cm/s charge speed, 2000 cm detection
- [ ] Scale is 1.5x player size
- [ ] Placeholder large/heavy mesh with amber/orange accent material
- [ ] Visible shield mesh on front
- [ ] Visible weak point (glowing core) on back
- [ ] Stagger threshold set to 60 damage (very high)
- [ ] Heavy footstep animations; ground impact feel

**S-035: Warden Armor System**
> As a Warden, frontal attacks deal reduced damage, but my back weak point takes amplified damage.

Acceptance Criteria:
- [ ] Damage from front (relative to Warden facing) reduced by 50%
- [ ] Damage to back weak point multiplied by 2x
- [ ] Directional damage check uses dot product between damage direction and Warden forward
- [ ] Shield impact VFX/SFX placeholder when frontal hit is reduced
- [ ] Weak point hit VFX/SFX placeholder when back is struck
- [ ] Armor values configurable in data asset (`FrontalArmorReduction`, `WeakPointMultiplier`)

**S-036: Warden Utility Actions**
> As a Warden enemy, I evaluate 7 actions to control space, protect allies, and overwhelm the player.

Acceptance Criteria:
- [ ] **Advance** — High utility when player is beyond 500 cm and visible
- [ ] **Ground Slam** — High utility when within 300 cm and cooldown ready
- [ ] **Shield Charge** — High utility at 500–1200 cm with clear path and cooldown ready
- [ ] **Shield Bash** — High utility when within 150 cm and player is attacking from front
- [ ] **Stomp Wave** — Only available below enrage threshold (30% HP); high utility when surrounded
- [ ] **Protect Ally** — High utility when nearby Watcher is under threat
- [ ] **Face Player** — High utility when player is behind (protect weak point)
- [ ] All considerations use response curves; weights configurable

**S-037: Warden Behavior Trees**
> As a developer, I need BT subtrees that execute each Warden action with weight and impact.

Acceptance Criteria:
- [ ] Advance subtree: Slow walk toward player; maintain facing; intimidating gait
- [ ] Ground Slam subtree: Wind-up 1.5s → slam AOE 300 cm radius → recovery
- [ ] Shield Charge subtree: Aim direction → charge 800 cm in a line → recovery on miss (1.0s)
- [ ] Shield Bash subtree: Quick swing → knockback player 200 cm
- [ ] Stomp Wave subtree: Below enrage HP → 360° shockwave → recovery
- [ ] Protect Ally subtree: Move to position between player and protected ally
- [ ] Face Player subtree: Rotate to face player (protect weak point)
- [ ] All attack subtrees have clear wind-up phases (player dodge windows)

**S-038: Warden Attack Patterns**
> As a Warden, I perform powerful attacks that deny space and punish overcommitment.

Acceptance Criteria:
- [ ] **Ground Slam**: 1.5s wind-up; 300 cm AOE; 30 damage; Max stagger; dodge window 0.8–1.2s
- [ ] **Shield Charge**: 800 cm linear charge; 25 damage + knockdown; 1.0s recovery on miss
- [ ] **Shield Bash**: Quick close-range; 15 damage; High stagger; pushes player 200 cm
- [ ] **Stomp Wave**: Only below 30% HP; 360° AOE; 20 damage; forces dodge/jump
- [ ] AOE attacks visualized with ground indicators (placeholder decal/ring)
- [ ] Charge attack has clear directional telegraph (player can sidestep)
- [ ] All attacks use damage pipeline with correct values

---

### E-08: AI Data Assets & Designer Tooling

> *As a designer, I can tune all enemy behaviors, stats, and AI parameters through editable Data Assets without touching code.*

**Dependencies:** E-04, E-05, E-06, E-07 (Archetypes must exist to data-drive)
**Phase:** 2 — Enemy Archetypes (Week 8)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-039 | Enemy Profile Data Asset class with all stats, perception, and behavior parameters | P0 | 5 | S5 |
| S-040 | Utility Action Data Asset class with considerations and response curve references | P0 | 3 | S5 |
| S-041 | Create default Data Asset instances for all 3 archetypes | P0 | 3 | S5 |
| S-042 | Blueprint event exposure for all AI events (state change, action selected, damage, stagger, death) | P0 | 3 | S5 |
| S-043 | Runtime parameter hot-reload during PIE | P1 | 2 | S5 |

#### Story Details

**S-039: Enemy Profile Data Asset**
> As a designer, I can open an Enemy Profile Data Asset and tweak all stats for an archetype.

Acceptance Criteria:
- [ ] `UEnemyProfileData` class with all properties from GDD (Identity, Stats, Perception, Behavior, Visual, Utility)
- [ ] All UPROPERTY metadata includes: `EditAnywhere`, `Category`, `ClampMin`/`ClampMax`, `ToolTip`
- [ ] Properties organized into clear categories in the editor details panel
- [ ] Enemy characters read all values from assigned Data Asset at spawn
- [ ] Changing a Data Asset value and re-spawning reflects the change immediately
- [ ] Supports creating variants (e.g., `DA_Rusher_Elite` with different stats)

**S-040: Utility Action Data Asset**
> As a designer, I can create and configure Utility Actions with considerations and curves in data assets.

Acceptance Criteria:
- [ ] `UUtilityActionData` class with: Action Name, Weight, array of Consideration references
- [ ] Each consideration references a Response Curve data asset and an input source
- [ ] Curve parameters (slope, midpoint, steepness, threshold) editable per consideration
- [ ] New actions can be created and assigned to enemy profiles without code
- [ ] Action weight editable for global priority adjustment

**S-041: Default Data Asset Instances**
> As a developer, I need pre-configured Data Asset instances for Rusher, Watcher, and Warden defaults.

Acceptance Criteria:
- [ ] `DA_Rusher_Default` with all GDD values for The Blade
- [ ] `DA_Watcher_Default` with all GDD values for The Watcher
- [ ] `DA_Warden_Default` with all GDD values for The Warden
- [ ] All utility actions configured with correct considerations and curves
- [ ] Each enemy spawns and behaves correctly using only its data asset
- [ ] Assets stored under `/Content/Data/AI/EnemyProfiles/`

**S-042: Blueprint Event Exposure**
> As a designer, I can hook into AI events via Blueprint for custom responses and encounter scripting.

Acceptance Criteria:
- [ ] `OnStateChanged(OldState, NewState)` — fires on AI state transitions
- [ ] `OnActionSelected(Action, Score)` — fires when Utility AI picks a new action
- [ ] `OnAttackTokenGranted()` / `OnAttackTokenReturned()` — fires on token events
- [ ] `OnPerceptionUpdated(PerceptionData)` — fires each perception tick
- [ ] `OnDamageReceived(Amount, Source)` — fires on hit
- [ ] `OnStaggered()` / `OnStaggerRecovered()` — fires on stagger events
- [ ] `OnDeath()` — fires on death
- [ ] All delegates are `UPROPERTY(BlueprintAssignable)`
- [ ] Events testable in a simple Blueprint test actor

**S-043: Hot-Reload**
> As a designer, I can change Data Asset values during Play In Editor and see them apply immediately.

Acceptance Criteria:
- [ ] Modifying a Data Asset property during PIE updates the affected enemy's behavior
- [ ] No editor restart or level reload required
- [ ] Hot-reload applies to: stats, perception range, behavior weights, utility action weights
- [ ] Changes are preserved in the Data Asset after stopping PIE

---

### E-09: Squad Coordination System

> *As a player, I face enemies that fight as a coordinated squad — they flank, manage their attack timing, use cover, and react as a group to my actions.*

**Dependencies:** E-04, E-05, E-06, E-07 (Enemies exist to coordinate)
**Phase:** 3 — Group Coordination (Weeks 9–10)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-044 | Squad Coordinator actor — manages group-level tactical decisions | P0 | 5 | S5 |
| S-045 | Attack Token System — limits concurrent attacks with priority queue | P0 | 5 | S5 |
| S-046 | Flanking System — 8-sector assignment around the player | P0 | 5 | S6 |
| S-047 | Aggro Management — track player focus and adjust ally behavior | P0 | 3 | S6 |
| S-048 | Cover System — cover nodes with evaluation scoring | P0 | 5 | S6 |
| S-049 | Tactical Situation Recognition — detect and respond to 6 tactical situations | P1 | 3 | S6 |

#### Story Details

**S-044: Squad Coordinator**
> As a developer, I need a central coordinator that provides squad-level information to individual AI agents.

Acceptance Criteria:
- [ ] `SquadCoordinator` actor exists as a singleton per encounter
- [ ] Maintains a list of all active squad members
- [ ] Provides shared data: player position, aggro target, active threats
- [ ] Subsystems: Threat Manager, Role Assignment, Attack Tokens, Position Manager
- [ ] Communicates to individuals via influence on Utility scores (not direct control)
- [ ] Squad members register on spawn and deregister on death
- [ ] Configurable via `USquadProfileData` data asset

**S-045: Attack Token System**
> As a squad, we limit how many of us attack the player simultaneously to create readable combat rhythm.

Acceptance Criteria:
- [ ] Configurable `MaxConcurrentAttacks` (default 2–3)
- [ ] Enemies request tokens before initiating attack actions
- [ ] Priority determined by: distance to player, time since last attack, archetype role
- [ ] Tokens returned after attack completes or after `TokenHoldDuration` timeout
- [ ] Enemies without tokens choose non-attack actions (circle, reposition, wait)
- [ ] Token count adjustable at runtime (for difficulty and wave escalation)
- [ ] `OnAttackTokenGranted` / `OnAttackTokenReturned` events fire on the enemy

**S-046: Flanking System**
> As a squad, we spread out around the player from different directions to create crossfire pressure.

Acceptance Criteria:
- [ ] Player space divided into 8 sectors (45° each), tracked relative to player facing
- [ ] Squad Coordinator assigns each enemy a preferred sector
- [ ] Unoccupied sectors are preferred; rear sectors have highest priority
- [ ] Assignments update dynamically as player rotates and moves
- [ ] `SectorReassignmentInterval` configurable (default every 1s)
- [ ] `MinSectorSpacing` prevents enemy clumping within a sector
- [ ] Flanking preference influences Utility AI "Flank" action score
- [ ] Visually verifiable: enemies approach from different directions

**S-047: Aggro Management**
> As a squad, we recognize which of us the player is focused on and adjust our behavior accordingly.

Acceptance Criteria:
- [ ] Track the player's current target (locked-on or closest facing enemy)
- [ ] Aggro holder gets a defensive behavior bonus (evasive, blocking)
- [ ] Non-aggro allies get an offensive behavior bonus (flank, close distance, attack)
- [ ] Aggro target updates in real-time as player switches targets or kills enemies
- [ ] If aggro target dies, next closest enemy becomes new perceived target
- [ ] Aggro state communicated to Utility AI as a consideration input

**S-048: Cover System**
> As an enemy, I can evaluate and move to cover positions in the environment.

Acceptance Criteria:
- [ ] Cover nodes placed in the level with: position, forward direction, height (low/high), width (narrow/wide)
- [ ] `CoverSystem` evaluates nodes with weighted scoring: protection angle, range, occupancy, LOS to fire, distance
- [ ] Nodes track occupancy (only 1 agent per narrow, 2 per wide)
- [ ] Watchers proactively seek cover; Rushers use cover briefly during retreat
- [ ] Cover evaluation runs on demand (not every tick) when cover-seeking action is selected
- [ ] Invalid cover (doesn't protect from player direction) scored low
- [ ] Cover nodes placed throughout the arena per the layout diagram

**S-049: Tactical Situations**
> As a squad coordinator, I recognize macro tactical situations and broadcast appropriate responses.

Acceptance Criteria:
- [ ] **Player Surrounded** (3+ enemies in different sectors): All increase aggression, reduced token requirement
- [ ] **Ally Down** (ally killed): Nearby allies briefly increase aggression; Watchers reposition
- [ ] **Player at Range** (far from all enemies): Rushers sprint; Watchers fire freely; Tank charges
- [ ] **Player in Cover** (near cover geometry): Rushers flank; Watchers reposition; Tank advances
- [ ] **Player Low Health** (below 25% HP): All increase aggression
- [ ] **Squad Depleted** (1 enemy remaining): Last enemy becomes desperate (increased aggro, new moves)
- [ ] Situation detection runs every 0.5s
- [ ] Responses modify Utility AI inputs (not override direct control)

---

### E-10: Encounter & Wave System

> *As a player, I progress through 5 escalating encounter waves that introduce and combine enemy archetypes, with clear transitions and pacing.*

**Dependencies:** E-05, E-06, E-07, E-09 (Enemies + coordination)
**Phase:** 3 — Group Coordination (Week 11)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-050 | Arena level with modular geometry — open zones, cover, platforms, chokepoints, flanking routes | P0 | 5 | S6 |
| S-051 | Spawn Point actors with archetype and wave assignment | P0 | 2 | S6 |
| S-052 | Encounter Manager — wave progression, spawning, wave-clear detection | P0 | 5 | S6 |
| S-053 | Encounter Data Asset — define wave composition, tokens, spawn timing, pickups | P0 | 3 | S6 |
| S-054 | All 5 waves configured per the GDD encounter design | P0 | 3 | S6 |

#### Story Details

**S-050: Arena Level**
> As a player, I fight in an arena with diverse geometry that supports varied tactical gameplay.

Acceptance Criteria:
- [ ] Arena matches the layout diagram from the GDD (open arena, cover clusters, elevated platforms, chokepoints, flanking corridors)
- [ ] Cover geometry has appropriate collision for AI cover system
- [ ] Elevated platforms accessible via ramps (for both player and AI)
- [ ] NavMesh generated and validated — all areas pathable by all enemy types
- [ ] Spawn zones at far end from player start
- [ ] Cover nodes placed at all cover positions
- [ ] Lighting supports readability (enemies visible at distance, glow colors distinguishable)
- [ ] Placeholder industrial-sci-fi visual theme

**S-051: Spawn Points**
> As a developer, I need spawn point actors that define where and what enemies appear.

Acceptance Criteria:
- [ ] `SpawnPoint` actor with: archetype type, wave index, spawn position/rotation
- [ ] Spawn points placed per wave composition from GDD
- [ ] Visual indicator in editor showing spawn location and archetype
- [ ] Support for staggered spawning (delay between spawns)

**S-052: Encounter Manager**
> As a developer, I need a system that manages wave progression from start to finish.

Acceptance Criteria:
- [ ] `EncounterManager` reads from Encounter Data Asset
- [ ] Spawns enemies for current wave with configured timing
- [ ] Detects wave clear (all enemies dead)
- [ ] Configurable delay between waves (default 3s)
- [ ] Assigns Squad Coordinator to each wave's enemy group
- [ ] Spawns health pickups if configured for the wave
- [ ] Triggers wave transition events (for UI)
- [ ] Handles game over (player death) and victory (all waves cleared)

**S-053: Encounter Data Asset**
> As a designer, I can configure wave composition through a data asset.

Acceptance Criteria:
- [ ] `UEncounterData` with array of `FEncounterWave` structs
- [ ] Each wave has: name, enemy list (type + count), squad profile, attack token count, spawn timing, pickup flag, delay
- [ ] Editable in editor without code changes
- [ ] Encounter Manager reads and executes the data asset

**S-054: Five Waves Configured**
> As a player, I progress through 5 waves that teach me the game and escalate in complexity.

Acceptance Criteria:
- [ ] **Wave 1**: 3 Rushers, 1 attack token (teach melee combat)
- [ ] **Wave 2**: 2 Watchers + 2 Rushers, 2 tokens (introduce ranged + synergy)
- [ ] **Wave 3**: 1 Warden + 2 Rushers, 2 tokens (introduce tank + flanking)
- [ ] **Wave 4**: 1 Warden + 2 Watchers + 3 Rushers, 3 tokens (full squad tactics)
- [ ] **Wave 5**: 2 Wardens + 3 Watchers + 4 Rushers, 3–4 tokens + health pickups (gauntlet)
- [ ] Each wave is completable by a skilled player
- [ ] Pacing feels escalating but fair

---

### E-11: Debug & Developer Tools

> *As a developer or designer, I can visualize all AI systems in real-time with debug overlays, inspect individual enemies, and pause/step the AI for analysis.*

**Dependencies:** E-04, E-09 (AI systems must exist to debug)
**Phase:** 4 — Polish (Week 12)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-055 | Debug HUD overlay — AI state, current action, threat level above each enemy | P0 | 5 | S7 |
| S-056 | Perception debug visualization — FOV cones, sight lines, hearing radius | P0 | 3 | S7 |
| S-057 | Utility Score bar chart for selected enemy | P1 | 3 | S7 |
| S-058 | Squad debug overlay — flanking sectors, attack tokens, aggro target | P0 | 3 | S7 |
| S-059 | Cover node visualization — color-coded availability | P1 | 2 | S7 |
| S-060 | Console commands (ai.debug.*, ai.spawn, ai.difficulty) | P0 | 3 | S7 |
| S-061 | AI pause and step-through for frame-by-frame inspection | P1 | 2 | S7 |

#### Story Details

**S-055: Debug HUD Overlay**
> As a developer, I can see each enemy's AI state and current action at a glance.

Acceptance Criteria:
- [ ] Toggle with backtick (`) key
- [ ] Displays above each enemy: current state (Idle/Alert/Combat/etc.), current action name, action score
- [ ] Threat level shown as color-coded bar (green → yellow → red)
- [ ] Information readable from gameplay distance
- [ ] Minimal performance impact when enabled

**S-056: Perception Visualization**
> As a developer, I can see what each enemy perceives — their vision cones and sight lines.

Acceptance Criteria:
- [ ] FOV cone drawn for selected or all enemies
- [ ] Sight line to player drawn (green = can see, red = blocked)
- [ ] Hearing radius displayed as wireframe sphere
- [ ] Last known player position shown as marker when LOS is lost
- [ ] Togglable via `ai.debug.perception` command

**S-057: Utility Score Display**
> As a developer, I can see the utility scores for all candidate actions on a selected enemy.

Acceptance Criteria:
- [ ] Select an enemy via `ai.debug.select [id]` or clicking
- [ ] Bar chart shows all action names and their current scores
- [ ] Highest scoring action highlighted
- [ ] Scores update in real-time
- [ ] Individual consideration breakdown available

**S-058: Squad Debug Overlay**
> As a developer, I can visualize the squad coordination layer — flanking sectors, token holders, and aggro.

Acceptance Criteria:
- [ ] 8 flanking sectors drawn around the player (colored by occupancy)
- [ ] Lines from enemies to their assigned sectors
- [ ] Attack token holders highlighted with an icon
- [ ] Current aggro target marked with a distinct indicator
- [ ] Togglable via `ai.debug.squad` command

**S-059: Cover Visualization**
> As a developer, I can see all cover nodes and their current state.

Acceptance Criteria:
- [ ] Cover nodes rendered as colored boxes: green = available, red = occupied, gray = invalid for current situation
- [ ] Forward direction shown as an arrow
- [ ] Height (low/high) visually distinguishable
- [ ] Togglable via `ai.debug.cover` command

**S-060: Console Commands**
> As a developer, I have console commands to control AI debugging and testing.

Acceptance Criteria:
- [ ] `ai.debug.all` — Toggle all overlays
- [ ] `ai.debug.perception` — Toggle perception vis
- [ ] `ai.debug.utility` — Toggle utility scores
- [ ] `ai.debug.squad` — Toggle squad overlay
- [ ] `ai.debug.cover` — Toggle cover vis
- [ ] `ai.debug.tokens` — Toggle token display
- [ ] `ai.debug.select [id]` — Select specific enemy
- [ ] `ai.debug.pause` — Freeze all AI
- [ ] `ai.debug.step` — Advance one AI tick (when paused)
- [ ] `ai.debug.log [id]` — Dump decision log to output
- [ ] `ai.spawn [archetype] [count]` — Spawn enemies
- [ ] `ai.difficulty [preset]` — Switch difficulty

**S-061: AI Pause and Step**
> As a developer, I can freeze all AI and advance frame-by-frame to inspect decisions.

Acceptance Criteria:
- [ ] `ai.debug.pause` freezes all AI decision-making and movement
- [ ] Enemies hold their current pose/position
- [ ] `ai.debug.step` advances all AI by exactly one decision tick
- [ ] Debug overlays update on step to show new state
- [ ] Player remains controllable during AI pause (for repositioning)

---

### E-12: Combat Feel & Polish (Juice)

> *As a player, every attack, hit, and dodge feels impactful and satisfying through hit stop, screen shake, VFX, and responsive animation timing.*

**Dependencies:** E-02, E-05, E-06, E-07 (Combat must exist to polish)
**Phase:** 4 — Polish (Week 13)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-062 | Hitlag / freeze frame system (2–4 frames on heavy hits) | P1 | 3 | S7 |
| S-063 | Screen shake on melee impacts (configurable intensity per attack type) | P1 | 3 | S7 |
| S-064 | Hit spark VFX on melee impact | P1 | 2 | S7 |
| S-065 | Slash trail VFX on weapon swings | P1 | 2 | S7 |
| S-066 | Attack telegraph VFX (red flash/glow before enemy attacks) | P0 | 3 | S7 |
| S-067 | Damage vignette and low health warning effects | P1 | 2 | S8 |
| S-068 | Kill slow-motion on final enemy of a wave (optional, togglable) | P2 | 3 | S8 |

#### Story Details

**S-062: Hitlag**
> As a player, heavy attacks cause a brief freeze frame that communicates impact weight.

Acceptance Criteria:
- [ ] Hitlag pauses game time for configurable duration (default 0.05s)
- [ ] Light attacks: minimal or no hitlag
- [ ] Heavy attacks: 2–4 frame pause
- [ ] Parry counter: strongest hitlag
- [ ] Duration configurable per attack type via data
- [ ] Does not affect input buffering (player inputs still register)

**S-063: Screen Shake**
> As a player, melee impacts produce screen shake that scales with attack strength.

Acceptance Criteria:
- [ ] Light attack: subtle shake (intensity 2.0)
- [ ] Heavy attack: moderate shake (intensity 5.0)
- [ ] Warden slam/charge: strong shake
- [ ] Intensity and duration configurable per attack type
- [ ] Shake uses camera component (not post-process)
- [ ] Shake stacks appropriately (doesn't cause disorienting oscillation)

**S-064: Hit Spark VFX**
> As a player, I see satisfying spark particles on every melee impact.

Acceptance Criteria:
- [ ] Niagara particle system spawns at hit contact point
- [ ] Direction matches hit angle
- [ ] Scale/intensity varies with attack strength
- [ ] Different materials for: normal hit, armor hit (shield), weak point hit
- [ ] Particles auto-destroy after completion

**S-065: Slash Trail VFX**
> As a player, weapon swings leave a visible arc trail.

Acceptance Criteria:
- [ ] Trail effect attached to weapon bone
- [ ] Activates during attack animation active frames
- [ ] Deactivates after swing completes
- [ ] Color/intensity varies by attack type (light = subtle, heavy = pronounced)

**S-066: Attack Telegraph VFX**
> As a player, I can see a clear visual warning before every enemy attack.

Acceptance Criteria:
- [ ] Red glow/flash on enemy during wind-up of every attack type
- [ ] Telegraph timing matches actual wind-up duration
- [ ] Watcher laser sight visible during aimed shot wind-up
- [ ] Warden ground slam shows AOE indicator on the ground
- [ ] Warden charge shows directional indicator
- [ ] Telegraph intensity proportional to attack danger

**S-067: Screen Effects**
> As a player, I receive visual feedback when taking damage and when my health is low.

Acceptance Criteria:
- [ ] Damage vignette: red screen edges flash on hit, fade over 0.3s
- [ ] Low health warning: pulsing red vignette when below 25% HP
- [ ] Stagger screen shake when player is staggered
- [ ] Effects use post-process material (not HUD overlay)

**S-068: Kill Slow-Mo**
> As a player, the final kill of each wave triggers a brief slow-motion effect for dramatic impact.

Acceptance Criteria:
- [ ] When the last enemy in a wave reaches 0 HP, time scale drops to 0.3x for 0.5s
- [ ] Camera focuses on the dying enemy briefly
- [ ] Togglable in settings (default ON)
- [ ] Smooth time scale ramp back to 1.0x
- [ ] Only triggers on wave-ending kills (not mid-wave kills)

---

### E-13: UI & HUD

> *As a player, I have a clean, readable HUD showing my health, stamina, ammo, wave progress, and enemy information.*

**Dependencies:** E-02, E-10 (Combat stats + wave system to display)
**Phase:** 4 — Polish (Week 14)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-069 | Player HUD — health bar, stamina bar, ammo counter | P0 | 3 | S7 |
| S-070 | Enemy overhead UI — health bar and stagger meter (visible when damaged) | P0 | 3 | S7 |
| S-071 | Wave indicator — current wave / total waves, enemies remaining | P0 | 2 | S7 |
| S-072 | Lock-on target indicator widget | P0 | 2 | S7 |
| S-073 | Main Menu — Start, Difficulty Select, Controls, Quit | P1 | 3 | S8 |
| S-074 | Pause Menu — Resume, Restart Wave, Restart All, Difficulty, Quit | P1 | 3 | S8 |
| S-075 | Wave Complete Screen with stats (time, damage, parries) | P1 | 3 | S8 |
| S-076 | Game Over and Victory screens | P1 | 2 | S8 |

#### Story Details

**S-069: Player HUD**
> As a player, I can see my current health, stamina, and ammo at all times.

Acceptance Criteria:
- [ ] Health bar in bottom-left; visually fills/drains with damage/healing
- [ ] Stamina bar adjacent to health; depletes with dodge/sprint, regenerates visually
- [ ] Ammo counter shown as diamond icons (filled = available, hollow = spent)
- [ ] HUD elements are UMG widgets
- [ ] Layout matches HUD mockup from GDD
- [ ] Elements are readable during fast combat (not too small, good contrast)

**S-070: Enemy Overhead UI**
> As a player, I can see enemy health and stagger progress above their heads.

Acceptance Criteria:
- [ ] Health bar appears above enemy when they take damage; fades after 3s of no damage
- [ ] Stagger meter below health bar fills as player lands hits
- [ ] Stagger meter flashes when threshold is reached (stagger occurs)
- [ ] Bars are world-space widgets that face the camera
- [ ] Bars scale down with distance for readability
- [ ] Warden shows armor indicator (shield icon) when frontal hit is reduced

**S-071: Wave Indicator**
> As a player, I can see which wave I'm on and how many enemies remain.

Acceptance Criteria:
- [ ] Top-left: "WAVE X/5" text
- [ ] Top-right: "ENEMIES: X" counter
- [ ] Counter decrements as enemies die
- [ ] Wave number updates on wave transition
- [ ] Brief "WAVE X" splash text on wave start (fades after 2s)

**S-072: Lock-On Indicator**
> As a player, I can see which enemy I'm locked on to with a clear UI element.

Acceptance Criteria:
- [ ] Reticle/bracket widget displayed on the locked-on enemy
- [ ] Shows enemy name and health bar in a centered lower-screen panel
- [ ] Indicator smoothly follows target position
- [ ] Indicator disappears when lock-on is released

**S-073: Main Menu**
> As a player, I see a main menu when the game starts with all necessary options.

Acceptance Criteria:
- [ ] Full-screen menu widget with: Start Game, Difficulty (Easy/Normal/Hard), Controls Reference, Quit
- [ ] Difficulty selection stores the chosen preset
- [ ] Controls screen shows gamepad + keyboard/mouse bindings
- [ ] Clean visual design; functional buttons with hover states
- [ ] Game starts at Main Menu, not directly in gameplay

**S-074: Pause Menu**
> As a player, I can pause the game and access options mid-encounter.

Acceptance Criteria:
- [ ] Start/Escape opens pause menu; game time pauses
- [ ] Options: Resume, Restart Current Wave, Restart All Waves, Change Difficulty, Quit to Menu
- [ ] Resume unpauses; all AI and gameplay resumes
- [ ] Restart Wave resets to current wave start
- [ ] Restart All resets to Wave 1
- [ ] Quit returns to Main Menu

**S-075: Wave Complete Screen**
> As a player, I see my performance stats after clearing each wave.

Acceptance Criteria:
- [ ] Displays after wave clear (during inter-wave delay)
- [ ] Stats shown: Clear Time, Damage Taken, Parries Landed, Enemies Killed
- [ ] "Continue" button proceeds to next wave
- [ ] Clean layout; stats animate in
- [ ] After final wave, transitions to Victory Screen instead

**S-076: Game Over / Victory**
> As a player, I see an end screen when I die or complete all waves.

Acceptance Criteria:
- [ ] **Game Over**: Displays on player death; shows wave reached, total stats; Retry / Quit buttons
- [ ] **Victory**: Displays after Wave 5 clear; shows full run stats; Replay / Quit buttons
- [ ] Both screens have clean visual treatment
- [ ] Retry restarts from Wave 1
- [ ] Quit returns to Main Menu

---

### E-14: Difficulty System

> *As a player, I can select a difficulty that adjusts enemy stats, AI behavior intensity, and player advantages to match my skill level.*

**Dependencies:** E-08, E-10 (Data assets + encounter system)
**Phase:** 4 — Polish (Week 15)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-077 | Difficulty Profile Data Asset with all tunable parameters per preset | P1 | 3 | S8 |
| S-078 | Three difficulty presets (Easy, Normal, Hard) with values from GDD | P1 | 3 | S8 |
| S-079 | Difficulty applies globally at encounter start; switchable from menus | P1 | 2 | S8 |
| S-080 | (Stretch) Dynamic difficulty adjustment system based on player performance | P2 | 2 | S8 |

#### Story Details

**S-077: Difficulty Data Asset**
> As a designer, I can define difficulty presets in a data asset.

Acceptance Criteria:
- [ ] `UDifficultyProfileData` with fields: Health/Damage multipliers, token count, reaction time modifier, flanking aggressiveness, ranged accuracy, stagger duration modifier, player health/stamina modifiers, pickup frequency
- [ ] All fields match the difficulty table in the GDD
- [ ] Fields have appropriate editor metadata (ranges, tooltips)

**S-078: Three Presets**
> As a developer, I need Easy, Normal, and Hard data asset instances configured per GDD.

Acceptance Criteria:
- [ ] `DA_Difficulty_Easy`: 0.7x enemy HP/DMG, 1 token, +0.3s reaction, 0.3 flanking, 0.5 accuracy, +30% stagger duration, 130 player HP, +25% stamina regen, high pickups
- [ ] `DA_Difficulty_Normal`: 1.0x everything, 2–3 tokens, base values
- [ ] `DA_Difficulty_Hard`: 1.3x enemy HP, 1.2x enemy DMG, 3–4 tokens, -0.1s reaction, 0.9 flanking, 0.85 accuracy, -20% stagger, 80 player HP, -15% stamina regen, low pickups
- [ ] Default selection is Normal

**S-079: Difficulty Application**
> As a player, my selected difficulty affects gameplay immediately.

Acceptance Criteria:
- [ ] Encounter Manager reads active Difficulty Profile at encounter start
- [ ] Enemy stats modified by multipliers
- [ ] Player stats modified per preset
- [ ] Attack token count set per difficulty
- [ ] AI behavior modifiers (reaction time, flanking, accuracy) applied
- [ ] Difficulty changeable from Main Menu and Pause Menu
- [ ] Changing difficulty mid-run takes effect on the next wave

**S-080: Dynamic Difficulty (Stretch)**
> As a system, I subtly adjust challenge parameters based on player performance.

Acceptance Criteria:
- [ ] Tracks: deaths per wave, clear time, damage taken, parry/dodge success rates
- [ ] Adjusts: token count, enemy reaction time, ranged accuracy, pickup frequency
- [ ] Adjustments bounded by configurable min/max ranges
- [ ] Invisible to the player
- [ ] Togglable (default OFF for portfolio demo)
- [ ] Does not override manual difficulty selection — layers on top

---

### E-15: Audio & VFX

> *As a player, combat sounds and visual effects reinforce every action, making the experience feel polished and responsive.*

**Dependencies:** E-02, E-05, E-06, E-07, E-12 (Combat + polish framework)
**Phase:** 4 — Polish (Weeks 13–14)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-081 | Player combat audio — sword swings, hit impacts, dodge whoosh | P1 | 3 | S7 |
| S-082 | Enemy audio — attack telegraph cues, hurt sounds, death sounds | P1 | 3 | S7 |
| S-083 | Projectile VFX — energy trail for player and Watcher ranged attacks | P1 | 3 | S7 |
| S-084 | Warden-specific VFX — ground slam ring, charge dust trail, weak point glow, shield impact | P1 | 3 | S8 |
| S-085 | UI audio — menu navigation, wave announcements, parry success cue | P2 | 2 | S8 |
| S-086 | Enemy death dissolve VFX | P2 | 2 | S8 |

#### Story Details

**S-081: Player Combat Audio**
> As a player, I hear satisfying audio on every melee swing, hit, and dodge.

Acceptance Criteria:
- [ ] Sword swing whoosh sound on each attack animation
- [ ] Impact sound on hit (varies: flesh hit, armor hit, parry clang)
- [ ] Dodge roll whoosh sound
- [ ] Ranged attack fire sound
- [ ] Sounds sourced from free SFX libraries (Freesound/SONNISS)
- [ ] Volume and pitch slight randomization for variety

**S-082: Enemy Audio**
> As a player, I hear audio cues that telegraph enemy actions and provide feedback on hits.

Acceptance Criteria:
- [ ] Rusher: charging grunt before lunge; slash sound; hurt cry; death sound
- [ ] Watcher: charging whine before aimed shot; projectile fire sound; hurt/death sounds
- [ ] Warden: ground shake on footsteps; slam impact; charge roar; bash clang; hurt grunt; death sound
- [ ] Telegraph audio begins during wind-up (in sync with VFX telegraph)
- [ ] Sounds are distinct per archetype (player can identify threats by sound)

**S-083: Projectile VFX**
> As a player, I can see ranged projectiles clearly for both player and enemy shots.

Acceptance Criteria:
- [ ] Player projectile: distinct energy color (e.g., green), trail particle, impact burst
- [ ] Watcher projectile: blue/cyan energy, trail particle, impact burst
- [ ] Trail visible for full flight path
- [ ] Impact VFX spawns at contact point
- [ ] Projectile size appropriate for readability (not too small at distance)

**S-084: Warden VFX**
> As a player, the Warden's attacks have dramatic visual effects that communicate their power.

Acceptance Criteria:
- [ ] Ground Slam: expanding ring/crack decal on ground at impact point
- [ ] Shield Charge: dust/spark trail behind the charging Warden
- [ ] Weak Point: constant pulsing glow on Warden's back (intensifies when exposed during attack)
- [ ] Shield Impact: sparks/deflection particles on frontal hits that are armor-reduced
- [ ] All VFX implemented as Niagara systems

**S-085: UI Audio**
> As a player, I hear feedback sounds for menu interactions and key game events.

Acceptance Criteria:
- [ ] Menu button hover and click sounds
- [ ] Wave start announcement sound
- [ ] Wave complete jingle
- [ ] Parry success distinct "clang" sound
- [ ] Game over sting; victory fanfare

**S-086: Death Dissolve**
> As a player, dead enemies dissolve away rather than remaining as corpses.

Acceptance Criteria:
- [ ] Material-based dissolve effect over ~1.5s after death
- [ ] Dissolve progresses from extremities toward center
- [ ] Particle emission during dissolve (embers / energy dissipation)
- [ ] Actor destroyed after dissolve completes
- [ ] Works on all three archetypes

---

### E-16: Documentation & Packaging

> *As a portfolio reviewer, I can understand the entire project's design philosophy, technical decisions, and AI architecture through comprehensive documentation.*

**Dependencies:** All previous epics (documenting the finished product)
**Phase:** 4 — Polish (Week 16)

| ID | Story | Priority | SP | Sprint |
|---|---|---|---|---|
| S-087 | AI Architecture technical document | P1 | 3 | S8 |
| S-088 | Designer Tuning Guide (how to modify behaviors via Data Assets) | P1 | 2 | S8 |
| S-089 | Debug Tools Reference document | P1 | 2 | S8 |
| S-090 | Project postmortem / technical breakdown | P1 | 3 | S8 |

#### Story Details

**S-087: AI Architecture Doc**
> As a reviewer, I understand the Utility AI + BT hybrid system, perception pipeline, and how decisions are made.

Acceptance Criteria:
- [ ] Explains hybrid architecture: Utility selects WHAT, BT handles HOW
- [ ] Diagrams for: perception pipeline, utility evaluation, BT structure, state machine
- [ ] Code examples for key systems
- [ ] Explains design tradeoffs (why hybrid, why custom perception, etc.)
- [ ] Saved as `/docs/AI_ARCHITECTURE.md`

**S-088: Designer Tuning Guide**
> As a reviewer, I understand how designers can modify all gameplay parameters without code.

Acceptance Criteria:
- [ ] Step-by-step guide to creating/editing Enemy Profile Data Assets
- [ ] Guide to creating new Utility Actions and Considerations
- [ ] Guide to configuring encounter waves
- [ ] Screenshot/instructions for debug tool usage
- [ ] Saved as `/docs/DESIGNER_TUNING_GUIDE.md`

**S-089: Debug Tools Reference**
> As a reviewer, I understand the full suite of debug and development tools available.

Acceptance Criteria:
- [ ] Complete console command reference with descriptions
- [ ] Debug overlay descriptions and toggle instructions
- [ ] How to inspect individual enemy AI decisions
- [ ] How to use pause/step for AI analysis
- [ ] Saved as `/docs/DEBUG_TOOLS_REFERENCE.md`

**S-090: Postmortem**
> As a reviewer, I understand the development journey, technical challenges, and design decisions.

Acceptance Criteria:
- [ ] Summary of development timeline and what was accomplished
- [ ] Technical challenges encountered and how they were solved
- [ ] What went well and what would be done differently
- [ ] Key design decisions and their rationale
- [ ] Demonstrates reflection and growth mindset
- [ ] Saved as `/docs/POSTMORTEM.md`

---

## 3. Sprint Plan

### Sprint 1 (Weeks 1–2): Foundation

**Goal:** Playable player character in a basic level with movement, combat basics, and camera.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-001 | Project setup, folder structure, Git + LFS | 2 | E-01 |
| S-002 | Player character base with mesh and collision | 3 | E-01 |
| S-003 | Locomotion — walk, sprint, stamina | 5 | E-01 |
| S-004 | Dodge roll with i-frames | 5 | E-01 |
| S-005 | Jump | 3 | E-01 |
| S-006 | Input mapping (gamepad + KB/M) | 3 | E-01 |
| S-007 | Light attack 3-hit combo | 5 | E-02 |
| S-008 | Heavy attack with charge | 5 | E-02 |
| S-014 | Third-person follow camera | 5 | E-03 |
| S-015 | Camera collision avoidance | 3 | E-03 |

**Sprint Total: 39 SP** *(Ambitious for Sprint 1 — some may carry over)*
**Milestone:** Player character moves, attacks (light + heavy), dodges, and jumps with a functional camera.

---

### Sprint 2 (Weeks 3–4): Player Combat + AI Foundation

**Goal:** Complete player combat kit. AI framework in place with basic perception and decision-making.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-009 | Parry system with counter-attack | 5 | E-02 |
| S-010 | Ranged attack with projectile | 5 | E-02 |
| S-011 | Lock-on targeting system | 5 | E-02 |
| S-012 | Player stagger system | 2 | E-02 |
| S-013 | Shared damage system interface | 2 | E-02 |
| S-016 | Lock-on camera mode | 3 | E-03 |
| S-017 | Camera parameter exposure | 2 | E-03 |
| S-018 | AI Character base class | 3 | E-04 |
| S-019 | AI Controller base with BT integration | 3 | E-04 |
| S-020 | Perception — sight system | 5 | E-04 |
| S-021 | Perception — hearing system | 3 | E-04 |
| S-022 | Perception — threat assessment | 3 | E-04 |

**Sprint Total: 41 SP** *(Heavy — prioritize P0 items, carry hearing to S3 if needed)*
**Milestone:** Full player combat. AI enemy that can see, hear, and assess threats.

---

### Sprint 3 (Weeks 5–6): Utility AI + Rusher

**Goal:** Utility AI framework complete. Rusher archetype fully functional.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-023 | Utility AI Component — action evaluation pipeline | 5 | E-04 |
| S-024 | Response Curve system | 2 | E-04 |
| S-025 | AI State Machine | 2 | E-04 |
| S-026 | Rusher base character setup | 3 | E-05 |
| S-027 | Rusher Utility Actions (all 7) | 8 | E-05 |
| S-028 | Rusher Behavior Tree subtrees | 5 | E-05 |
| S-029 | Rusher attack patterns (Slash, Lunge, Circling) | 5 | E-05 |

**Sprint Total: 30 SP**
**Milestone:** Complete Utility AI framework. Rusher enemy fights intelligently: chases, attacks in combos, lunges, circles, flanks, retreats when hurt.

---

### Sprint 4 (Weeks 7–8): Watcher + Warden

**Goal:** Both remaining archetypes fully functional with unique behaviors.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-030 | Watcher base character setup | 3 | E-06 |
| S-031 | Watcher Utility Actions (all 6) | 8 | E-06 |
| S-032 | Watcher Behavior Tree subtrees | 5 | E-06 |
| S-033 | Watcher attack patterns (Aimed, Burst, Retreat Shot) | 5 | E-06 |
| S-034 | Warden base character setup | 3 | E-07 |
| S-035 | Warden frontal armor + weak point system | 5 | E-07 |
| S-036 | Warden Utility Actions (all 7) | 8 | E-07 |

**Sprint Total: 37 SP** *(Heavy — Warden BT/attacks carry to S5)*
**Milestone:** All three enemy archetypes functional. Watcher maintains range and suppresses; Warden tanks and protects with directional armor.

---

### Sprint 5 (Weeks 9–10): Warden Completion + Data Assets + Coordination Start

**Goal:** Finish Warden. All enemies data-driven. Squad coordination core (tokens + flanking).

| ID | Story | SP | Epic |
|---|---|---|---|
| S-037 | Warden Behavior Tree subtrees | 5 | E-07 |
| S-038 | Warden attack patterns (Slam, Charge, Bash, Stomp) | 3 | E-07 |
| S-039 | Enemy Profile Data Asset class | 5 | E-08 |
| S-040 | Utility Action Data Asset class | 3 | E-08 |
| S-041 | Default data asset instances (all 3 archetypes) | 3 | E-08 |
| S-042 | Blueprint event exposure for all AI events | 3 | E-08 |
| S-043 | Runtime parameter hot-reload | 2 | E-08 |
| S-044 | Squad Coordinator actor | 5 | E-09 |
| S-045 | Attack Token System | 5 | E-09 |

**Sprint Total: 34 SP**
**Milestone:** Warden complete. All enemies configured via data assets. Attack tokens prevent dogpiling. Squad coordinator online.

---

### Sprint 6 (Weeks 11–12): Coordination Complete + Encounters

**Goal:** Full squad coordination. Arena built. All 5 waves playable.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-046 | Flanking System (8-sector) | 5 | E-09 |
| S-047 | Aggro Management | 3 | E-09 |
| S-048 | Cover System | 5 | E-09 |
| S-049 | Tactical Situation Recognition | 3 | E-09 |
| S-050 | Arena level (geometry, cover, platforms, routes) | 5 | E-10 |
| S-051 | Spawn Point actors | 2 | E-10 |
| S-052 | Encounter Manager (wave progression) | 5 | E-10 |
| S-053 | Encounter Data Asset | 3 | E-10 |
| S-054 | All 5 waves configured | 3 | E-10 |

**Sprint Total: 34 SP**
**Milestone:** Enemies flank, use cover, manage aggro, and recognize tactical situations. 5 waves playable end-to-end in the arena.

---

### Sprint 7 (Weeks 13–14): Debug Tools + Polish + Core UI

**Goal:** Debug tools functional. Combat feel polished. Core HUD and UI operational.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-055 | Debug HUD overlay | 5 | E-11 |
| S-056 | Perception debug visualization | 3 | E-11 |
| S-057 | Utility score bar chart | 3 | E-11 |
| S-058 | Squad debug overlay | 3 | E-11 |
| S-059 | Cover node visualization | 2 | E-11 |
| S-060 | Console commands | 3 | E-11 |
| S-061 | AI pause and step | 2 | E-11 |
| S-062 | Hitlag system | 3 | E-12 |
| S-063 | Screen shake | 3 | E-12 |
| S-064 | Hit spark VFX | 2 | E-12 |
| S-065 | Slash trail VFX | 2 | E-12 |
| S-066 | Attack telegraph VFX | 3 | E-12 |
| S-069 | Player HUD (health, stamina, ammo) | 3 | E-13 |
| S-070 | Enemy overhead UI (health, stagger) | 3 | E-13 |
| S-071 | Wave indicator | 2 | E-13 |
| S-072 | Lock-on indicator | 2 | E-13 |
| S-081 | Player combat audio | 3 | E-15 |
| S-082 | Enemy audio | 3 | E-15 |
| S-083 | Projectile VFX | 3 | E-15 |

**Sprint Total: 53 SP** *(Overloaded — this is the polish sprint; items are smaller scope. Overflow to S8.)*
**Milestone:** Full debug tooling. Combat feels impactful with hitlag, shake, VFX, and audio. Core HUD displays all player and enemy info.

---

### Sprint 8 (Weeks 15–16): Final Polish + Difficulty + Menus + Documentation

**Goal:** Difficulty system, all menus, remaining VFX/audio, documentation, final tuning pass.

| ID | Story | SP | Epic |
|---|---|---|---|
| S-067 | Damage vignette and low health effects | 2 | E-12 |
| S-068 | Kill slow-mo (stretch) | 3 | E-12 |
| S-073 | Main Menu | 3 | E-13 |
| S-074 | Pause Menu | 3 | E-13 |
| S-075 | Wave Complete Screen | 3 | E-13 |
| S-076 | Game Over / Victory screens | 2 | E-13 |
| S-077 | Difficulty Profile Data Asset | 3 | E-14 |
| S-078 | Three difficulty presets | 3 | E-14 |
| S-079 | Difficulty application at runtime | 2 | E-14 |
| S-080 | Dynamic difficulty (stretch) | 2 | E-14 |
| S-084 | Warden VFX (slam, charge, glow, shield) | 3 | E-15 |
| S-085 | UI audio | 2 | E-15 |
| S-086 | Death dissolve VFX | 2 | E-15 |
| S-087 | AI Architecture document | 3 | E-16 |
| S-088 | Designer Tuning Guide | 2 | E-16 |
| S-089 | Debug Tools Reference | 2 | E-16 |
| S-090 | Postmortem / technical breakdown | 3 | E-16 |

**Sprint Total: 43 SP**
**Milestone:** Portfolio-ready build. All menus functional. Difficulty selectable. All VFX/audio in place. Full documentation suite complete.

---

### Sprint Velocity Summary

| Sprint | Weeks | SP Planned | Phase |
|---|---|---|---|
| S1 | 1–2 | 39 | Foundation |
| S2 | 3–4 | 41 | Foundation + AI |
| S3 | 5–6 | 30 | Utility AI + Rusher |
| S4 | 7–8 | 37 | Watcher + Warden |
| S5 | 9–10 | 34 | Data Assets + Coordination |
| S6 | 11–12 | 34 | Coordination + Encounters |
| S7 | 13–14 | 53 | Debug + Polish + UI |
| S8 | 15–16 | 43 | Final Polish + Docs |
| **Total** | **16 weeks** | **311** | |

> **Note:** Sprints 1, 2, and 7 are intentionally overloaded. Sprint planning should re-evaluate at each sprint boundary and defer lower-priority items (P1/P2) if velocity is lower than planned. The buffer days per phase in the GDD provide additional slack.

---

## 4. Test Plan

### 4.1 Testing Strategy

| Test Type | Description | When |
|---|---|---|
| **Unit Tests** | Individual system validation (damage calc, curve eval, token logic) | During development |
| **Integration Tests** | System interactions (AI perception → utility → BT → animation) | End of each sprint |
| **Playtest Sessions** | End-to-end gameplay; combat feel; difficulty; AI readability | Sprints 3–8 |
| **Performance Tests** | FPS profiling with max enemies; AI tick budget; memory | Sprints 6–8 |
| **Regression Tests** | Re-run previous test suites after changes | Every sprint |

---

### 4.2 Test Cases by Epic

---

#### E-01: Player Movement Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-001 | Walk speed validation | Measure player speed during walk | 400 cm/s ±5% | P0 |
| TC-002 | Sprint speed validation | Hold sprint; measure speed | 700 cm/s ±5% | P0 |
| TC-003 | Sprint stamina drain | Sprint until stamina depleted | Stamina drains; sprint disabled at 0; player slows to walk | P0 |
| TC-004 | Stamina regeneration | Deplete stamina; stop sprinting; time regen | Stamina regenerates to full; rate matches config | P0 |
| TC-005 | Dodge roll distance and duration | Dodge roll; measure distance and time | ~300 cm traveled in 0.5s | P0 |
| TC-006 | Dodge i-frames | Enemy attacks during i-frame window (0.1–0.35s) | No damage received | P0 |
| TC-007 | Dodge i-frame expiration | Enemy attacks after i-frame window (>0.35s) | Damage received | P0 |
| TC-008 | Dodge stamina gate | Deplete stamina; attempt dodge | Dodge fails; player cannot roll | P0 |
| TC-009 | Dodge cancels attack | Start light attack; press dodge during recovery | Attack interrupted; dodge executes | P0 |
| TC-010 | Jump height | Jump; measure peak height | 300 cm ±10% | P0 |
| TC-011 | Input responsiveness | Measure input-to-action latency | < 100ms from input to first animation frame | P0 |
| TC-012 | Gamepad movement | Test all movement with gamepad | All inputs functional; analog sensitivity correct | P0 |
| TC-013 | KB/M movement | Test all movement with keyboard/mouse | All inputs functional; mouse look smooth | P0 |

---

#### E-02: Player Combat Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-014 | Light combo damage | Hit enemy with 3-hit combo | 10 + 10 + 10 = 30 total damage | P0 |
| TC-015 | Light combo timing | Press attack 3x with varied timing | Combo chains within window; resets on timeout | P0 |
| TC-016 | Heavy attack damage | Charge and release heavy attack on enemy | 25 damage applied | P0 |
| TC-017 | Heavy attack stagger | Heavy attack on Rusher (threshold 20) | Enemy enters stagger state | P0 |
| TC-018 | Parry success | Time parry against Rusher slash | Rusher staggered; counter window opens | P0 |
| TC-019 | Parry failure | Mistime parry (too early/late) | Player enters recovery; takes damage | P0 |
| TC-020 | Parry counter damage | Successfully parry → counter-attack | 30 damage to enemy | P0 |
| TC-021 | Ranged attack damage | Fire projectile at enemy | 15 damage on hit | P0 |
| TC-022 | Ranged ammo depletion | Fire all 12 shots | Ammo reaches 0; cannot fire more | P0 |
| TC-023 | Ranged ammo replenish | Kill enemy with melee after depleting ammo | Ammo partially replenished | P1 |
| TC-024 | Lock-on toggle | Press lock-on near enemy | Camera orbits target; player strafes; indicator shown | P0 |
| TC-025 | Lock-on target switch | Flick right stick/mouse while locked on | Lock switches to adjacent enemy | P0 |
| TC-026 | Lock-on break on kill | Kill locked target | Lock releases; returns to free camera | P0 |
| TC-027 | Lock-on range limit | Move far from locked target | Lock breaks beyond configured range | P0 |
| TC-028 | Player death | Take damage until HP = 0 | Death animation plays; Game Over screen appears | P0 |
| TC-029 | Stagger on player | Enemy attack exceeds player stagger resist | Player enters stagger; unable to act during recovery | P0 |
| TC-030 | Damage from all sources | Take hits from Rusher, Watcher, and Warden | Correct damage amounts per GDD damage table | P0 |

---

#### E-03: Camera Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-031 | Camera follow position | Walk around; observe camera | Over-the-shoulder; 350cm back, 120cm up, 60cm right | P0 |
| TC-032 | Camera collision | Walk into a wall with camera behind | Camera pulls forward; never clips through geometry | P0 |
| TC-033 | Camera collision recovery | Walk away from wall | Camera smoothly returns to default distance | P0 |
| TC-034 | Lock-on camera | Lock onto enemy; strafe | Camera frames both player and target; smooth tracking | P0 |
| TC-035 | Camera FOV default | Check camera FOV in normal gameplay | 75 degrees | P0 |

---

#### E-04: Core AI Framework Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-036 | AI sight detection | Stand in front of enemy within range and FOV | `bCanSeePlayer` = true; AI enters Alert/Combat | P0 |
| TC-037 | AI sight blocked by wall | Stand behind wall within range | `bCanSeePlayer` = false; AI does not react | P0 |
| TC-038 | AI sight FOV boundary | Stand at edge of FOV cone | Detected within FOV; not detected outside | P0 |
| TC-039 | AI last known position | Break LOS after being seen | AI moves to last known position; enters Searching state | P0 |
| TC-040 | AI hearing detection | Sprint near enemy (out of sight) | `bCanHearPlayer` = true; AI investigates sound source | P1 |
| TC-041 | AI threat scoring | Approach enemy while attacking | Threat level increases as proximity and aggression increase | P0 |
| TC-042 | AI threat decay | Disengage from enemy; wait | Threat level decays over time toward 0 | P0 |
| TC-043 | Utility AI action selection | Observe AI at various ranges/states | Appropriate action selected (chase when far, attack when close, etc.) | P0 |
| TC-044 | Response curve — Linear | Unit test: input 0.5 through linear curve | Output matches expected linear value | P0 |
| TC-045 | Response curve — Logistic | Unit test: input values through logistic curve | Output matches S-curve with correct steepness/midpoint | P0 |
| TC-046 | Response curve — Step | Unit test: input values around threshold | Binary 0/1 output at threshold boundary | P0 |
| TC-047 | AI state: Idle → Alert | Peripheral stimulus detected | AI transitions to Alert; turns toward stimulus | P0 |
| TC-048 | AI state: Alert → Combat | Visual confirmation of player | AI transitions to Combat; full decision loop active | P0 |
| TC-049 | AI state: Combat → Searching | Break LOS; wait for timeout | AI transitions to Searching; moves to last known position | P0 |
| TC-050 | AI state: Searching → Idle | Search timeout expires with no re-acquisition | AI returns to Idle | P0 |
| TC-051 | AI state: → Dead | Enemy health reaches 0 | AI transitions to Dead; death animation/ragdoll; cleanup | P0 |

---

#### E-05: Rusher Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-052 | Rusher chase behavior | Stand far from Rusher in combat state | Rusher sprints toward player at 650 cm/s | P0 |
| TC-053 | Rusher melee combo | Let Rusher reach melee range | Executes 2-hit Slash Combo; 12 damage per hit | P0 |
| TC-054 | Rusher lunge | Stand at 400 cm from Rusher | Executes Lunge Strike; 20 damage; covers distance quickly | P0 |
| TC-055 | Rusher circle strafe | Face Rusher directly while another ally attacks | Rusher strafes to side rather than direct charge | P0 |
| TC-056 | Rusher retreat | Reduce Rusher health below 25% | Rusher retreats; moves away from player temporarily | P0 |
| TC-057 | Rusher attack telegraph | Observe Rusher wind-up | Clear wind-up animation visible before each attack type | P0 |
| TC-058 | Rusher parry-able | Parry during Rusher attack | Parry succeeds; Rusher staggered | P0 |
| TC-059 | Rusher death | Kill Rusher | Death animation/ragdoll; cleanup after delay | P0 |
| TC-060 | Rusher stagger | Hit Rusher for 20+ damage | Rusher enters stagger state; recovers after animation | P0 |

---

#### E-06: Watcher Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-061 | Watcher range maintenance | Approach Watcher; observe repositioning | Watcher maintains 1500–2500 cm optimal range | P0 |
| TC-062 | Watcher aimed shot | Stand at optimal range in LOS | Watcher fires after 1.2s wind-up; 18 damage; visible telegraph | P0 |
| TC-063 | Watcher overwatch burst | Engage a Rusher in melee near Watcher | Watcher fires 3-round burst (suppressive); 8 damage per hit | P0 |
| TC-064 | Watcher retreat | Rush at Watcher within 500 cm | Watcher retreats; fires retreat shot while backpedaling | P0 |
| TC-065 | Watcher cover seeking | Exposed Watcher with cover nearby | Watcher moves to cover position | P0 |
| TC-066 | Watcher high ground | Elevated platform available | Watcher seeks and occupies elevated position | P1 |
| TC-067 | Watcher LOS requirement | Break LOS from Watcher position | Watcher repositions to re-establish LOS | P0 |
| TC-068 | Watcher fragility | Hit Watcher with light attack (10 dmg) | Watcher staggers easily (threshold 10); 30 HP depletes quickly | P0 |
| TC-069 | Watcher telegraph | Observe aimed shot preparation | Visible laser sight / charging glow during 1.2s wind-up | P0 |

---

#### E-07: Warden Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-070 | Warden frontal armor | Attack Warden from front | Damage reduced by 50% | P0 |
| TC-071 | Warden weak point | Attack Warden from behind | Damage multiplied by 2x | P0 |
| TC-072 | Warden advance | Stand at range; observe Warden | Warden slowly advances toward player at 250 cm/s | P0 |
| TC-073 | Warden ground slam | Stand within 300 cm of Warden | Ground slam: 1.5s wind-up; AOE; 30 damage; Max stagger | P0 |
| TC-074 | Warden shield charge | Stand at 800 cm in clear line | Charge at 800 cm/s; 25 damage + knockdown on hit | P0 |
| TC-075 | Warden charge miss recovery | Dodge Warden charge | Warden has 1.0s recovery (exploitable window) | P0 |
| TC-076 | Warden shield bash | Attack Warden from front at close range | Quick shield bash; 15 damage; knockback 200 cm | P0 |
| TC-077 | Warden stomp wave | Reduce Warden below 30% HP; surround | 360° AOE shockwave; 20 damage; forces dodge | P0 |
| TC-078 | Warden bodyguard | Warden near Watcher; approach Watcher | Warden positions between player and Watcher | P0 |
| TC-079 | Warden face player | Get behind Warden | Warden rotates to face player (protect weak point) | P0 |
| TC-080 | Warden stagger threshold | Hit Warden for < 60 accumulated damage | No stagger occurs (very high threshold) | P0 |
| TC-081 | Warden stagger | Hit Warden for 60+ accumulated damage | Warden enters stagger; weak point fully exposed | P0 |
| TC-082 | Warden telegraphs | Observe all attack wind-ups | Ground slam indicator, charge direction, bash swing all visible | P0 |

---

#### E-08: Data Asset Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-083 | Profile data applies | Assign DA_Rusher_Default to Rusher; spawn | Rusher has 40 HP, 650 speed, 2500 detect, etc. | P0 |
| TC-084 | Profile variant | Create DA_Rusher_Elite with 80 HP; assign and spawn | Rusher has 80 HP; all other stats from elite profile | P0 |
| TC-085 | Hot-reload | Change DA value during PIE | Spawned enemies reflect new value immediately (or on re-query) | P1 |
| TC-086 | Blueprint events | Subscribe to OnDamageReceived in Blueprint; hit enemy | Blueprint event fires with correct damage and source | P0 |
| TC-087 | Utility action data | Create new action in data asset; assign to enemy | Enemy considers and potentially selects the new action | P0 |

---

#### E-09: Squad Coordination Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-088 | Attack token limit | Spawn 5 Rushers; set 2 tokens | Maximum 2 enemies attack simultaneously; others wait | P0 |
| TC-089 | Token return | Enemy finishes attack | Token returned; next queued enemy can now attack | P0 |
| TC-090 | Token timeout | Enemy holds token but doesn't attack (bugged) | Token force-returned after timeout duration | P0 |
| TC-091 | Flanking spread | Spawn 4 Rushers; observe approach | Enemies approach from different sectors; not all from same direction | P0 |
| TC-092 | Flanking rear preference | Face enemies; observe positioning | Enemies preferentially move to rear/flank sectors | P0 |
| TC-093 | Flanking updates on rotation | Rotate player 180°; observe AI adjustment | Sector assignments update; enemies reposition to maintain flanking | P0 |
| TC-094 | Aggro holder defense | Lock on to specific enemy | Aggro target becomes more defensive; others become more aggressive | P0 |
| TC-095 | Aggro transfer on kill | Kill aggro target | Aggro transfers to next closest enemy | P0 |
| TC-096 | Cover occupation | Watcher seeks cover; second Watcher seeks same area | Each takes different cover node; no double-occupation of narrow cover | P0 |
| TC-097 | Cover angle validation | Watcher seeks cover while player is at different angles | Cover that doesn't protect from player direction is scored low/ignored | P0 |
| TC-098 | Tactical: Player surrounded | Get surrounded by 3+ enemies | All enemies increase aggression; attack rhythm intensifies | P1 |
| TC-099 | Tactical: Ally down | Kill an enemy near its allies | Nearby allies briefly increase aggression; Watchers reposition | P1 |
| TC-100 | Tactical: Squad depleted | Kill all but 1 enemy | Last enemy becomes desperate; more aggressive, new attack patterns | P1 |

---

#### E-10: Encounter & Wave Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-101 | Wave 1 composition | Start encounter | 3 Rushers spawn; 1 attack token | P0 |
| TC-102 | Wave clear detection | Kill all Wave 1 enemies | Wave complete detected; transition begins | P0 |
| TC-103 | Wave transition timing | Clear wave; wait | 3s delay before next wave spawns | P0 |
| TC-104 | Wave 2 composition | Clear Wave 1 | 2 Watchers + 2 Rushers spawn; 2 tokens | P0 |
| TC-105 | Wave 3 composition | Clear Wave 2 | 1 Warden + 2 Rushers spawn; 2 tokens | P0 |
| TC-106 | Wave 4 composition | Clear Wave 3 | 1 Warden + 2 Watchers + 3 Rushers; 3 tokens | P0 |
| TC-107 | Wave 5 composition | Clear Wave 4 | 2 Wardens + 3 Watchers + 4 Rushers; 3–4 tokens; health pickups | P0 |
| TC-108 | Victory condition | Clear all 5 waves | Victory screen displayed | P0 |
| TC-109 | Game over condition | Player dies during any wave | Game Over screen displayed with wave reached | P0 |
| TC-110 | Health pickup spawn | Wave 5 with pickup config | Health pickups appear in arena | P1 |
| TC-111 | Encounter data asset swap | Assign different encounter data asset | Waves reflect new composition | P0 |

---

#### E-11: Debug Tool Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-112 | Debug HUD toggle | Press backtick (`) during gameplay | Debug overlay appears/disappears; AI state shown above enemies | P0 |
| TC-113 | Perception vis | Run `ai.debug.perception` | FOV cones and sight lines rendered for all enemies | P0 |
| TC-114 | Utility display | Run `ai.debug.utility`; select enemy | Bar chart of action scores visible and updating | P1 |
| TC-115 | Squad overlay | Run `ai.debug.squad` | Flanking sectors, token holders, aggro target visible | P0 |
| TC-116 | AI pause | Run `ai.debug.pause` | All AI freezes; player can still move | P1 |
| TC-117 | AI step | Run `ai.debug.step` while paused | AI advances one tick; debug info updates | P1 |
| TC-118 | AI spawn command | Run `ai.spawn Rusher 3` | 3 Rushers spawn at configured positions | P0 |
| TC-119 | Difficulty command | Run `ai.difficulty Hard` | Difficulty switches; enemy stats update | P1 |

---

#### E-12: Combat Feel Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-120 | Hitlag on heavy attack | Land heavy attack on enemy | Brief frame pause (0.05s); visible and felt | P1 |
| TC-121 | Screen shake on hit | Land melee attacks | Subtle shake on light; stronger on heavy; proportional | P1 |
| TC-122 | Hit sparks | Land melee hit | Spark particles spawn at contact point | P1 |
| TC-123 | Slash trails | Swing weapon | Arc trail visible during swing frames | P1 |
| TC-124 | Attack telegraph visibility | Observe enemy wind-ups | Red glow/flash clearly visible before every attack type | P0 |
| TC-125 | Damage vignette | Take damage | Red vignette flashes at screen edges; fades | P1 |
| TC-126 | Low health warning | Reach below 25% HP | Pulsing red vignette; persistent until healed or dead | P1 |
| TC-127 | Kill slow-mo | Kill final enemy in a wave | Brief slow-motion (0.3x for 0.5s); smooth ramp back | P2 |

---

#### E-13: UI Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-128 | Health bar display | Take damage; observe HUD | Health bar decreases proportionally; correct visual | P0 |
| TC-129 | Stamina bar display | Sprint/dodge; observe HUD | Stamina bar depletes and regenerates correctly | P0 |
| TC-130 | Ammo display | Fire ranged; observe HUD | Ammo icons change from filled to hollow | P0 |
| TC-131 | Enemy health bar | Damage an enemy | Health bar appears above enemy; shows correct HP | P0 |
| TC-132 | Enemy stagger meter | Hit enemy repeatedly | Stagger meter fills; flashes at threshold | P0 |
| TC-133 | Wave indicator | Progress through waves | Wave counter updates; enemy count decrements | P0 |
| TC-134 | Lock-on indicator | Lock on to enemy | Reticle/bracket appears on target; info panel shows | P0 |
| TC-135 | Main Menu flow | Launch game | Main menu displayed; all buttons functional | P1 |
| TC-136 | Pause menu | Press Start/Escape during gameplay | Game pauses; menu displays; all options work | P1 |
| TC-137 | Wave complete stats | Clear a wave | Stats screen shows time, damage taken, parries, kills | P1 |
| TC-138 | Restart wave | Select "Restart Wave" from pause menu | Current wave resets; enemies respawn; player health resets | P1 |

---

#### E-14: Difficulty Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-139 | Easy mode enemy HP | Select Easy; check Rusher HP | Rusher has 28 HP (40 × 0.7) | P1 |
| TC-140 | Hard mode enemy HP | Select Hard; check Rusher HP | Rusher has 52 HP (40 × 1.3) | P1 |
| TC-141 | Easy mode tokens | Select Easy; spawn 5 enemies | Max 1 concurrent attack | P1 |
| TC-142 | Hard mode tokens | Select Hard; spawn 5 enemies | Max 3–4 concurrent attacks | P1 |
| TC-143 | Easy mode player HP | Select Easy; check player HP | Player has 130 HP | P1 |
| TC-144 | Difficulty switch mid-game | Change difficulty from Pause menu | New values apply on next wave | P1 |
| TC-145 | Dynamic difficulty (stretch) | Die repeatedly on Wave 2 | System subtly reduces difficulty (more pickups, fewer tokens) | P2 |

---

#### E-15: Audio & VFX Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-146 | Player swing audio | Perform all attack types | Unique swing sound per attack type | P1 |
| TC-147 | Hit impact audio | Land melee hits | Impact sound on contact; varies by target type | P1 |
| TC-148 | Enemy telegraph audio | Wait for enemy attack | Audio cue plays during wind-up; distinct per archetype | P1 |
| TC-149 | Projectile VFX | Fire player and Watcher ranged | Visible trails and impact bursts; distinct colors | P1 |
| TC-150 | Warden slam VFX | Warden performs ground slam | Expanding ring/crack effect at impact point | P1 |
| TC-151 | Warden weak point glow | Observe Warden from behind | Constant glowing core visible on back | P1 |
| TC-152 | Death dissolve | Kill an enemy | Dissolve material effect over ~1.5s; actor removed | P2 |
| TC-153 | Menu audio | Navigate menus | Hover and click sounds play | P2 |

---

#### Performance Tests

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-154 | FPS with 9 enemies | Wave 5 active (9 enemies, full AI) | Stable 60 FPS on mid-range PC | P0 |
| TC-155 | FPS with 12 enemies | Spawn 12 enemies via console | Stable 60 FPS (max capacity test) | P0 |
| TC-156 | AI tick budget | Profile AI cost per frame with 9 enemies | AI total < 3ms per frame | P0 |
| TC-157 | Memory per enemy | Profile memory usage | < 2 KB per AI instance (data-driven, shared assets) | P1 |
| TC-158 | NavMesh performance | Profile pathfinding cost | < 1ms per frame for all active paths | P1 |

---

#### End-to-End Playtest Scenarios

| TC ID | Test Case | Steps | Expected Result | Priority |
|---|---|---|---|---|
| TC-159 | Full run (Normal) | Complete all 5 waves on Normal | All waves beatable; fair challenge; AI visibly coordinated | P0 |
| TC-160 | Full run (Easy) | Complete all 5 waves on Easy | Easier than Normal; reduced enemy aggression; more pickups | P1 |
| TC-161 | Full run (Hard) | Complete all 5 waves on Hard | Harder than Normal; aggressive flanking; fewer mistakes allowed | P1 |
| TC-162 | AI never gets stuck | Play for 15 minutes; observe all enemies | No enemy stuck on geometry or in degenerate states | P0 |
| TC-163 | No soft-locks | Die, restart, change difficulty, pause/unpause | Game state always recoverable; no frozen states | P0 |
| TC-164 | Enemy variety readable | Observe all 3 types in Wave 4+ | Distinct silhouettes, colors, and behaviors identifiable at a glance | P0 |
| TC-165 | Attack tokens visible in gameplay | Observe Wave 4+ combat rhythm | Enemies take turns attacking; not all attacking at once | P0 |
| TC-166 | Flanking visible in gameplay | Observe enemy positioning in Wave 4+ | Enemies approach from multiple directions; rear/flank preferred | P0 |
| TC-167 | Watcher + Rusher synergy | Engage Rushers near Watchers | Watchers fire suppressive bursts while Rushers close in | P0 |
| TC-168 | Warden + squad synergy | Engage squad with Warden | Warden blocks access to Watchers; Rushers flank around Warden | P0 |

---

## 5. Definition of Done

A story is considered **Done** when ALL of the following are met:

### Code Quality
- [ ] Code compiles with zero errors and zero warnings
- [ ] Code follows UE5 naming conventions (`PascalCase`, `bPrefix`, `E`/`F` prefixes)
- [ ] All public functions have doc comments
- [ ] Class header comments explain the class's role
- [ ] No magic numbers — all tunable values in data assets or configurable properties

### Functionality
- [ ] All acceptance criteria for the story are met
- [ ] Feature works with both gamepad and keyboard/mouse (where applicable)
- [ ] No regressions to previously completed features

### Designer Accessibility
- [ ] All gameplay-relevant parameters exposed as `UPROPERTY(EditAnywhere)` with metadata
- [ ] Properties have tooltips and value ranges
- [ ] Blueprint events exposed where applicable

### Testing
- [ ] Associated test cases pass
- [ ] No AI stuck on geometry or in degenerate states
- [ ] Feature runs at 60 FPS on target hardware
- [ ] Tested in the context of a live encounter (not just in isolation)

### Debug Support
- [ ] Debug visualization added for any AI system introduced (if applicable)
- [ ] System observable through existing debug tools

---

## 6. Backlog Prioritization Key

| Priority | Label | Description |
|---|---|---|
| **P0** | Must Have | Ship blocker — project cannot be presented without this |
| **P1** | Should Have | Polish item — significantly improves quality but project is functional without it |
| **P2** | Nice to Have | Stretch goal — only if time permits |

### Cut Order (if behind schedule)
1. **Cut first:** P2 items (S-068 Kill Slow-Mo, S-080 Dynamic Difficulty, S-086 Death Dissolve)
2. **Cut second:** P1 items in E-15 Audio/VFX (S-085 UI Audio, S-084 Warden VFX details)
3. **Cut third:** P1 items in E-13 UI (S-075 Wave Complete Stats, S-076 Game Over/Victory can be simplified)
4. **Never cut:** P0 items — all P0 stories are required for a portfolio-quality deliverable

---

*Document Version: 1.0*
*Last Updated: February 10, 2026*
*Author: Portfolio Project — CombatAI*
