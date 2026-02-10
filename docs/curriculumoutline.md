# Third-Person Combat AI System — Course Curriculum

## Course: Advanced Game AI & Combat Systems in Unreal Engine 5

> **Duration:** 12 Weeks (3 Months)
> **Schedule:** 2 Lectures + 1 Lab per week (6 contact hours/week)
> **Prerequisites:** Intermediate C++, basic UE5 familiarity, linear algebra fundamentals
> **Engine:** Unreal Engine 5.4 (C++ & Blueprints)
> **Final Deliverable:** A playable third-person combat arena with 3 AI archetypes, squad coordination, and debug tools

---

## Course Overview

This course teaches students to build a complete third-person action game featuring sophisticated enemy AI from the ground up. Students will implement a hybrid Utility AI + Behavior Tree architecture, multiple enemy archetypes with distinct behaviors, group coordination tactics, a data-driven design pipeline, and professional debug visualization tools — all within Unreal Engine 5 using C++ with Blueprint integration.

The course is structured around four phases that mirror professional game development: **Foundation**, **AI Systems**, **Group Tactics & Content**, and **Polish & Delivery**. Each week culminates in a working, testable build that incrementally expands toward the final product.

---

## Learning Objectives

By the end of this course, students will be able to:

1. Architect a complete third-person combat system with animation-driven hit detection
2. Design and implement a hybrid Utility AI + Behavior Tree decision-making framework
3. Build a custom perception system with sight, hearing, and threat assessment
4. Create multiple distinct AI archetypes sharing a common base architecture
5. Implement group coordination systems (attack tokens, flanking, cover evaluation)
6. Author data-driven game parameters using UE5 Data Assets
7. Build real-time debug visualization tools for AI inspection
8. Apply game feel techniques (hitlag, screen shake, telegraphs) to combat systems
9. Write professional technical documentation and demonstrate collaboration-ready workflows
10. Profile and optimize AI systems for performance targets (60 FPS, 12+ active enemies)

---

## Grading Breakdown

| Component | Weight | Description |
|---|---|---|
| Weekly Lab Submissions | 30% | Working builds demonstrating each week's features |
| Midterm: AI Archetype Demo | 20% | One fully functional enemy archetype with Utility AI |
| Final Project | 35% | Complete game with all 3 archetypes, coordination, and polish |
| Documentation Portfolio | 15% | Technical docs, debug tool guide, and postmortem |

---

## Required Materials

- Unreal Engine 5.4 (free via Epic Games Launcher)
- Visual Studio 2022 or Rider for Unreal
- Git + Git LFS (source control)
- Mixamo account (free character/animation assets)
- Course GitHub repository (starter project + lecture slides)

---

# Phase 1: Foundation (Weeks 1–3)

*Goal: Establish the project, player character, combat system, and camera.*

---

## Week 1: Project Architecture & Player Movement

### Learning Objectives
- Set up a UE5 C++ project with proper module structure and source control
- Implement a responsive third-person player character with walk, sprint, and jump
- Integrate the UE5 Enhanced Input System for gamepad and keyboard/mouse support
- Understand UE5's actor component model for modular character design

### Lecture 1: Course Introduction & UE5 C++ Project Architecture
- Course roadmap and final deliverable walkthrough
- UE5 module system: `.Build.cs`, `.Target.cs`, `.uproject` configuration
- Project folder structure for scalable game development
- UPROPERTY, UFUNCTION, and UCLASS macros — the UE5 reflection system
- Git + Git LFS setup for Unreal projects (`.gitignore`, `.gitattributes`)

### Lecture 2: Character Movement & Enhanced Input
- `ACharacter` class hierarchy: capsule, mesh, movement component
- `UCharacterMovementComponent` deep dive: walk speed, jump velocity, air control, rotation
- Enhanced Input System: `UInputMappingContext`, `UInputAction`, `ETriggerEvent`
- Decoupled input architecture: Controller handles binding, Character handles logic
- The "Request" pattern: `RequestMove()`, `RequestJump()`, `RequestSprint()`
- Stamina system: resource management with regeneration delay

### Lab 1: Player Character Foundation
**Deliverable:** A player character that walks (400 cm/s), sprints (700 cm/s) with stamina drain, and jumps (300 cm) in a test level. Enhanced Input configured for both gamepad and KB/M.

**Exercises:**
1. Create the UE5 project with the full folder structure from the GDD
2. Implement `APlayerCharacter` with `UPlayerStatsComponent` (health, stamina, ammo)
3. Implement `ACombatAIPlayerController` with Enhanced Input binding
4. Configure walk/sprint speed switching with stamina consumption and regen delay
5. Initialize Git repository with proper `.gitignore` and `.gitattributes`

**Key Code Patterns:**
- Component composition (`CreateDefaultSubobject`)
- Delegate-driven stat updates (`OnHealthChanged`, `OnStaminaChanged`)
- Per-frame stamina regeneration with delay timer in `TickComponent`

---

## Week 2: Third-Person Camera & Dodge System

### Learning Objectives
- Implement a third-person camera with spring arm collision avoidance
- Build a dodge roll with invincibility frames (i-frames) and directional input
- Understand frame-accurate gameplay timing and its impact on combat feel
- Create camera effects (screen shake, hitlag) as reusable components

### Lecture 3: Third-Person Camera Systems
- `USpringArmComponent`: arm length, socket offset, probe collision, camera lag
- Over-the-shoulder offset for combat readability
- `UCameraComponent`: field of view, rotation inheritance
- Collision avoidance: sphere sweep, probe size, recovery speed
- Camera effects architecture: shake, hitlag (time dilation), slow-motion
- Designer-tunable parameters: why every value should be a `UPROPERTY`

### Lecture 4: Dodge Mechanics & I-Frames
- The dodge roll: input direction → velocity override → duration timer
- Invincibility frames: `bIsInvulnerable` flag gated by time window
- Frame windows explained: `DodgeIFrameStart` (0.1s) to `DodgeIFrameEnd` (0.35s)
- Dodge as a cancel: interrupting attack montages for responsiveness
- Stamina gating: dodge costs stamina, preventing infinite dodge spam
- State machines for player combat: `EPlayerCombatState` enum

### Lab 2: Camera & Dodge
**Deliverable:** A third-person camera with collision avoidance and designer-tunable parameters. A dodge roll with configurable i-frames and directional input.

**Exercises:**
1. Implement `UCombatCameraComponent` with follow distance, height offset, FOV control
2. Configure spring arm collision (probe size, recovery lag)
3. Implement `RequestDodge()` with directional velocity, i-frame window, and stamina gate
4. Add screen shake and hitlag trigger functions to the camera component
5. Create `EPlayerCombatState` enum and state transition logic

---

## Week 3: Melee Combat & Shared Damage Pipeline

### Learning Objectives
- Implement a 3-hit light combo chain with input buffering and combo windows
- Build a heavy charge attack with wind-up and release mechanics
- Design a shared damage pipeline that both player and AI will use
- Implement animation-driven hit detection using skeletal mesh socket sweeps
- Build the stagger system for both dealing and receiving stagger

### Lecture 5: Melee Combat Implementation
- Animation montages for attack sequences: play, blend in/out, notify events
- Combo system architecture: `ComboCount`, `bComboInputBuffered`, `ComboWindowTimer`
- Heavy attack charge: `HeavyCharge` state → hold duration → `HeavyRelease` state
- Input buffering: accepting next-attack input during current attack animation
- `OnMontageEnded` callback for state transitions and combo advancement
- Montage cancellation rules: which states can cancel into which

### Lecture 6: The Damage Pipeline & Hit Detection
- `FDamageInfo` struct: amount, stagger level, direction, location, type, source
- `UCombatComponent`: the shared interface for dealing and receiving damage
- `DealDamage()` / `ReceiveDamage()` — a universal damage pipeline
- Directional armor: dot product of hit direction vs. actor forward
- Weak point system: rear hits via negative dot product
- `UHitDetectionComponent`: socket-based sphere sweep between weapon start/end
- Per-swing hit list to prevent double-hits on the same target
- Stagger accumulation, threshold comparison, and recovery timer

### Lab 3: Combat System
**Deliverable:** Light 3-hit combo, heavy charge attack, animation-driven hit detection, and the shared damage pipeline with stagger system.

**Exercises:**
1. Implement `ExecuteLightAttack()` with combo chain (3 montage slots)
2. Implement `ExecuteHeavyAttack()` with charge → release pattern
3. Build `UCombatComponent` with health, stagger accumulation, and death
4. Build `UHitDetectionComponent` with socket-based sphere sweep
5. Implement `FDamageInfo` struct and directional armor modifiers
6. Wire `OnMontageEnded` for combo advancement and state reset
7. Test damage pipeline: player attacks deal damage, trigger stagger at threshold

---

# Phase 2: AI Systems (Weeks 4–7)

*Goal: Build the complete AI framework and all three enemy archetypes.*

---

## Week 4: AI Foundation — Base Classes & Perception

### Learning Objectives
- Architect a base AI character class that all archetypes inherit from
- Implement a custom perception system with sight (FOV + LOS), hearing, and threat assessment
- Set up the AI controller with Behavior Tree and Blackboard integration
- Understand the AI state machine (Idle → Alert → Combat → Searching → Dead)

### Lecture 7: Enemy AI Architecture
- `AAICharacterBase`: shared base for Rusher, Watcher, Warden
- Component composition for AI: `CombatComponent`, `HitDetection`, `PerceptionComponent`
- `AAIControllerBase`: `UBehaviorTreeComponent`, `UBlackboardComponent`
- Blackboard key management: static `FName` constants for type safety
- The AI state machine: `EAIState` enum with transition delegates
- Virtual hooks: `OnEnterCombat()`, `OnStaggered()`, `OnStaggerRecovered()`
- Death handling: disable movement, ragdoll, cleanup timer

### Lecture 8: Custom Perception System
- Why custom perception over UE5's `UAIPerceptionComponent` (control over threat scoring)
- Sight system: FOV cone check (`dot product >= cos(halfAngle)`), LOS raycast
- Peripheral detection: wider angle, requires combat state for full detection
- Hearing system: proximity-based with loudness, sprint/attack triggers
- Threat assessment: 4-factor weighted scoring (proximity, player action, damage, ally casualties)
- Threat memory and decay: `FInterpTo` for blending, decay rate when no perception
- `FPerceptionData` struct: the complete perception snapshot
- Staggered perception ticks: randomized initial offset for performance

### Lab 4: AI Base Classes & Perception
**Deliverable:** `AAICharacterBase` with perception, AI states, and controller. A test enemy that detects the player via sight/hearing and transitions through AI states.

**Exercises:**
1. Implement `AAICharacterBase` with CombatComponent, state machine, death handling
2. Implement `AAIControllerBase` with BT/BB lifecycle and blackboard helpers
3. Build `UAICombatPerceptionComponent` with sight (FOV + LOS), hearing, and threat
4. Wire perception data into the owning character and blackboard
5. Test: enemy transitions Idle → Alert → Combat when player enters FOV

---

## Week 5: Utility AI Framework

### Learning Objectives
- Understand the theory of Utility AI and why it complements Behavior Trees
- Implement the complete Utility AI evaluation pipeline
- Build 5 response curve types with configurable parameters
- Design considerations that normalize world-state to [0,1] and apply curves
- Implement multiplicative action scoring with early-out optimization

### Lecture 9: Utility AI Theory
- The problem with pure Behavior Trees: combinatorial explosion of conditionals
- Utility AI philosophy: "score everything, pick the best"
- The Infinite Axis Utility System (Dave Mark, GDC)
- Multiplicative vs. additive scoring: why multiplicative provides better discrimination
- The Compensation Formula: `finalScore = score ^ (1 / numConsiderations)` (optional)
- Score noise: adding small randomization to prevent repetitive behavior
- Decision rate: how often to re-evaluate (Hz) and why not every frame

### Lecture 10: Response Curves & Considerations
- The 5 curve types: Linear, Quadratic, Logistic, Step, Inverse
- `FResponseCurve` struct: type, slope, offset, exponent, steepness, midpoint, threshold
- `Evaluate(float Input)`: clamp → switch on type → clamp output
- `EConsiderationInput` enum: 26 world-state inputs AI can read
- `FUtilityConsideration`: input type, normalization range, response curve
- `FUtilityAction`: name, weight, considerations array, cooldown, token requirement
- `UUtilityAIComponent`: action list, decision timer, context building, best action selection
- Writing to the blackboard: `SetBlackboardAction()` bridges Utility to BT

### Lab 5: Utility AI Implementation
**Deliverable:** Complete Utility AI framework with response curves, considerations, and action evaluation. A test enemy that selects different actions based on changing context.

**Exercises:**
1. Implement `FResponseCurve` with all 5 curve types and `Evaluate()`
2. Implement `FUtilityConsideration` with `Evaluate(FUtilityContext)`
3. Implement `FUtilityAction` with multiplicative scoring and cooldown
4. Build `UUtilityAIComponent` with decision rate limiter and blackboard integration
5. Build `FUtilityContext` builder from perception data and character state
6. Test: modify context inputs in real-time and observe action selection changes
7. **Bonus:** Visualize all action scores in a debug bar chart

---

## Week 6: Rusher Archetype — Complete Implementation

### Learning Objectives
- Apply the Utility AI framework to create a complete enemy archetype
- Configure 7 distinct actions with archetype-specific considerations
- Implement action execution methods that bridge AI decisions to gameplay
- Understand how animation montages and hit detection integrate with AI actions

### Lecture 11: Designing AI Actions with Utility Considerations
- The Rusher action table: Chase, Melee Attack, Lunge, Circle Strafe, Flank, Retreat, Investigate
- Choosing the right curve for each consideration (worked examples)
- Weight balancing: how `Action.Weight` shifts overall priorities
- The attack token gate: `bRequiresAttackToken` blocks scoring without token
- Cooldown management: preventing action spam without making AI feel slow
- From decision to execution: `UUtilityAIComponent` → Blackboard → BT Task → Animation

### Lecture 12: Rusher Implementation Deep Dive
- `ARusherCharacter`: stats from GDD (HP 40, Speed 650, Stagger 20)
- `ConfigureUtilityActions()`: building all 7 actions in code
- Execution methods: `ExecuteSlashCombo()`, `ExecuteLunge()`, `ExecuteCircleStrafe()`
- Lunge attack: face player, montage play, HitDetection enable with damage template
- Circle strafe: orbit movement around the player (BT task)
- Retreat: inverse health curve drives urgency, move away from player
- Investigate: navigate to `LastKnownPlayerPosition` when player escapes sight

### Lab 6: Rusher Archetype
**Deliverable:** A fully functional Rusher enemy with 7 Utility Actions, attack montages, and correct behavior transitions.

**Exercises:**
1. Create `ARusherCharacter` inheriting from `AAICharacterBase`
2. Attach `UUtilityAIComponent` and `UAICombatPerceptionComponent`
3. Configure all 7 actions with considerations and response curves
4. Implement all 6 execution methods (Chase is BT-only)
5. Set up damage templates for slash (12 dmg, Low stagger) and lunge (20 dmg, Medium)
6. Test full combat loop: Rusher spots player → chases → attacks → retreats when hurt
7. **Midterm Preparation:** Record a 2-minute video showcasing Rusher behavior variety

**MIDTERM DUE: Rusher Archetype Demo (end of Week 6)**

---

## Week 7: Watcher & Warden Archetypes

### Learning Objectives
- Build a ranged AI archetype that maintains distance and seeks cover
- Build a tank AI archetype with directional armor and AOE attacks
- Implement projectile-based attacks with accuracy gating
- Implement AOE damage via overlap queries
- Understand how different archetypes reuse the same framework with different configurations

### Lecture 13: The Watcher — Ranged AI Design
- Ranged AI challenges: LOS requirements, optimal range bands, repositioning
- Watcher action table: Aimed Shot, Overwatch Burst, Seek Cover, High Ground, Retreat, Reposition
- Accuracy system: `FMath::FRand() <= Accuracy` gating
- `FireProjectile()`: line trace with damage application (or spawned projectile actor)
- Suppressive fire: triggered when ally is in melee, reducing player movement freedom
- Cover seeking: scoring cover nodes by direction alignment and proximity

### Lecture 14: The Warden — Tank AI Design
- The tank fantasy: slow, armored, space-controlling, and protectable
- Directional armor in `CombatComponent`: `FrontalArmorReduction` via dot product
- Weak point system: rear hits get `WeakPointMultiplier`
- AOE attacks: `OverlapMultiByChannel` with sphere collision → iterate hits
- Shield Charge: face player, enable hit detection, velocity override
- Enrage mechanic: health threshold unlocks Stomp Wave
- Bodyguard behavior: interpose between player and nearby Watchers

### Lab 7: Watcher & Warden
**Deliverable:** Both remaining archetypes fully functional with unique Utility Actions and attack patterns.

**Exercises:**
1. Implement `AWatcherCharacter` with 6 actions, aimed shot, and accuracy system
2. Implement `FireProjectile()` with accuracy-gated damage
3. Implement `AWardenCharacter` with 7 actions, frontal armor, and AOE attacks
4. Implement `ApplyAOEDamage()` using overlap query
5. Set up enrage threshold and Stomp Wave activation
6. Test: all 3 archetypes in the same level with player — verify distinct behaviors
7. Compare code across archetypes: identify shared patterns and unique extensions

---

# Phase 3: Group Tactics & Content (Weeks 8–10)

*Goal: Build squad coordination, encounter system, and data-driven design pipeline.*

---

## Week 8: Data Assets & Attack Token System

### Learning Objectives
- Design a data-driven architecture using UE5 Data Assets
- Build enemy profile, squad profile, encounter, and difficulty data assets
- Implement the attack token system that limits simultaneous attacks
- Understand priority queuing and fairness in token allocation

### Lecture 15: Data-Driven Design in UE5
- The case for Data Assets: designer autonomy, hot-reload, variant creation
- `UDataAsset` subclassing: `UEnemyProfileData`, `USquadProfileData`, `UEncounterData`
- `UPROPERTY` metadata: `ClampMin`, `ClampMax`, `ToolTip`, `EditCondition`
- `ApplyToCharacter()`: propagating profile values to components at spawn
- Creating variants: `DA_Rusher_Default` vs. `DA_Rusher_Elite` — same class, different values
- Difficulty profiles: `UDifficultyProfileData` with multipliers for health, damage, tokens, accuracy
- Content folder structure: organized by system, not by asset type

### Lecture 16: Attack Token System
- The dogpiling problem: why simultaneous attacks feel unfair
- Token pool architecture: max tokens, request queue, timeout enforcement
- Priority calculation: distance weight + fairness weight (time since last attack)
- Token lifecycle: Request → Grant → Hold (with timer) → Return → Process Queue
- `UAttackTokenSystem`: `RequestToken()`, `ReturnToken()`, `Tick()`, `EnforceTimeouts()`
- Integration with Utility AI: `bRequiresAttackToken` gates action scoring
- Difficulty integration: max tokens scale with difficulty preset

### Lab 8: Data Assets & Tokens
**Deliverable:** Complete data asset architecture with enemy/squad/encounter/difficulty profiles. A working attack token system limiting concurrent attacks.

**Exercises:**
1. Implement `UEnemyProfileData` with all GDD fields and `ApplyToCharacter()`
2. Implement `USquadProfileData`, `UEncounterData`, `UDifficultyProfileData`
3. Build `UAttackTokenSystem` with priority queue and timeout enforcement
4. Wire token system into `UUtilityAIComponent` context building
5. Test: spawn 4 Rushers but only 2 can attack at once; others circle/wait
6. Create 3 enemy profile variants (default, elite) demonstrating data-driven design

---

## Week 9: Squad Coordination — Flanking & Cover

### Learning Objectives
- Implement the Squad Coordinator that manages group-level tactics
- Build an 8-sector flanking system with archetype-weighted sector assignment
- Implement a cover system with node evaluation scoring
- Understand aggro management and tactical situation recognition

### Lecture 17: Squad Coordination Architecture
- `ASquadCoordinator`: one per wave, manages members, tokens, flanking, tactics
- Design philosophy: coordinator *influences*, doesn't *control* individual AI
- Member lifecycle: `RegisterMember()`, `UnregisterMember()`, death callbacks
- Aggro tracking: player facing direction → dot product → identify focus target
- Non-aggro allies get offensive bonuses; aggro holder gets defensive bonuses
- Tactical situation recognition: 6 situations (Surrounded, AllyDown, AtRange, InCover, LowHealth, Depleted)

### Lecture 18: Flanking & Cover Systems
- 8-sector flanking ring: divide 360° around player into 45° sectors
- Sector assignment algorithm: proximity priority, archetype preference, rear bonus
- Archetype sector preferences: Rushers front/sides, Watchers rear, Wardens front
- `UFlankingSystem`: `AssignSectors()`, `GetSectorPosition()`, `ScoreSector()`
- Cover nodes: `ACoverNode` as level-placed actors with direction, quality, elevation
- Cover evaluation: 4-factor scoring (proximity, direction alignment, quality, elevation bonus)
- `UCoverSystem`: `DiscoverCoverNodes()`, `FindBestCover()`, `ReleaseCover()`

### Lab 9: Squad Coordination
**Deliverable:** A working Squad Coordinator with flanking, cover, aggro, and tactical situations. Enemies visibly spread around the player and use cover.

**Exercises:**
1. Implement `ASquadCoordinator` with member tracking and token integration
2. Build `UFlankingSystem` with 8-sector assignment and archetype weighting
3. Build `UCoverSystem` with `ACoverNode` actors and evaluation scoring
4. Implement aggro tracking using player facing dot product
5. Implement `EvaluateTacticalSituation()` with all 6 situations
6. Place cover nodes in the test level; verify Watchers seek them
7. Test: 5 enemies with coordinator — verify spread, token enforcement, flanking

---

## Week 10: Encounter System & Wave Design

### Learning Objectives
- Build the Encounter Manager that drives wave progression
- Implement enemy spawning from encounter data assets
- Design the 5-wave escalating encounter from the GDD
- Handle wave completion, inter-wave pacing, and victory/game-over conditions

### Lecture 19: Encounter Management
- `AEncounterManager`: reads `UEncounterData`, spawns waves, tracks deaths
- Spawn system: `FEnemySpawnEntry` (archetype + profile + spawn point index)
- Spawn point actors: placed in level by designers, referenced by index
- Factory pattern: archetype enum → concrete class in `SpawnEnemy()`
- Per-wave squad coordinator: clean state per wave, new token pool
- Death tracking: `OnMemberDied` → decrement counter → `CheckWaveComplete()`
- Inter-wave pacing: configurable delay, health pickup between waves
- Victory/Game Over: delegates to GameMode

### Lecture 20: Wave Design & Pacing
- The 5-wave design philosophy: teach → combine → stress test
- Wave 1: Rushers only (learn melee combat, 1 token)
- Wave 2: Watchers + Rushers (learn ranged pressure, 2 tokens)
- Wave 3: Warden + Rushers (learn tanking/flanking, 2 tokens)
- Wave 4: Full squad composition (combined arms, 3 tokens)
- Wave 5: Gauntlet (9 enemies, full coordination, 3–4 tokens)
- Pacing curves: tension → release → escalation
- Health pickup placement: reward skill, prevent softlocking

### Lab 10: Encounter System
**Deliverable:** Complete encounter system with 5 playable waves. The full game loop: start → wave 1–5 → victory (or game over on player death).

**Exercises:**
1. Implement `AEncounterManager` with `StartEncounter()` and `AdvanceWave()`
2. Implement `SpawnWave()` with per-entry enemy spawning and profile application
3. Place 4–6 spawn point actors in the arena level
4. Create `UEncounterData` asset with all 5 waves matching GDD compositions
5. Wire death tracking and wave completion logic
6. Connect to GameMode: `OnPlayerDeath` → Game Over, `OnEncounterComplete` → Victory
7. Full playthrough test: verify wave escalation feels correct

---

# Phase 4: Polish & Delivery (Weeks 11–12)

*Goal: Add debug tools, UI, difficulty, combat feel polish, and documentation.*

---

## Week 11: Debug Tools & Core UI

### Learning Objectives
- Build a comprehensive AI debug HUD for real-time inspection
- Implement the combat HUD (health, stamina, ammo, wave, enemy count)
- Build enemy world-space health bars with stagger meters
- Create menu screens (main, pause, game over, victory)
- Understand UMG widget architecture and delegate-driven updates

### Lecture 21: AI Debug Visualization
- Why debug tools are mandatory: "if it isn't debuggable, it isn't shippable"
- `AAIDebugHUD` with `DrawHUD()` override for immediate-mode rendering
- Per-enemy overlays: state label, action + score, perception cone, health/stagger bars
- Token system display: active holders, queue status
- Flanking sector visualization: 8 sphere markers around the player
- Tactical situation readout
- Toggle system: per-layer flags, console command (`ToggleAIDebug`)
- 3D debug drawing: `DrawDebugLine`, `DrawDebugSphere`, `DrawDebugString`

### Lecture 22: UI Architecture with UMG
- `UUserWidget` architecture: `BindWidget`, `NativeConstruct`, delegate binding
- Combat HUD: `UProgressBar` for health/stamina, `UTextBlock` for ammo/wave/enemies
- Delegate-driven updates: bind to `OnHealthChanged`, `OnStaminaChanged`, `OnAmmoChanged`
- Enemy health bar: world-space `UWidgetComponent` with color gradient
- `UWidgetSwitcher` for multi-page menu system (main, pause, game over, victory)
- Button click delegates and game flow control

### Lab 11: Debug Tools & UI
**Deliverable:** Full AI debug overlay and combat HUD. Main menu, pause menu, game over screen, and victory screen.

**Exercises:**
1. Implement `AAIDebugHUD` with 6 toggleable overlay layers
2. Implement `UCombatHUDWidget` bound to player stat delegates
3. Implement `UEnemyHealthBarWidget` with color-interpolated health bar
4. Implement `UMainMenuWidget` with `UWidgetSwitcher` for 5 menu states
5. Wire menu delegates to GameMode (play, restart, quit)
6. Test: toggle debug during gameplay; verify HUD updates in real-time

---

## Week 12: Difficulty, Combat Feel & Final Delivery

### Learning Objectives
- Implement difficulty presets with global stat multipliers
- Apply game feel techniques: hitlag, screen shake, attack telegraphs
- Polish the combat loop: hit feedback, stagger response, death effects
- Write technical documentation and a development postmortem
- Prepare the final project submission with full test coverage

### Lecture 23: Difficulty Systems & Game Feel
- `UDifficultyManager` as a GameInstance subsystem (persists across levels)
- Three presets: Easy (forgiving), Normal (balanced), Hard (punishing)
- Difficulty multipliers: enemy HP/damage, max tokens, accuracy, player health
- Dynamic difficulty (optional): performance scoring → automatic adjustment
- Game feel polish: hitlag (`SetGlobalTimeDilation`), screen shake (camera offset), kill slow-mo
- Attack telegraphs: montage wind-up phases provide visual/audio warning
- VFX priorities from the GDD: hit sparks, slash trails, projectile trails, AOE ground effects
- The "juice" checklist: every interaction should provide multisensory feedback

### Lecture 24: Documentation, Optimization & Delivery
- Technical documentation: architecture overview, class diagrams, data flow
- Designer tuning guide: how to modify behaviors via Data Assets without code
- Debug tools reference: all commands, overlays, and workflows
- Performance optimization review: staggered ticking, AI LOD, perception intervals
- Profiling in UE5: Stat commands, Unreal Insights, common bottlenecks
- Postmortem structure: goals, achievements, challenges, lessons learned
- Portfolio presentation: video breakdown, code walkthrough, documentation quality

### Lab 12: Final Polish & Submission
**Deliverable:** The complete, polished game with difficulty system, combat feel, and comprehensive documentation.

**Exercises:**
1. Implement `UDifficultyManager` with 3 presets and dynamic difficulty (optional)
2. Apply hitlag and screen shake to the combat camera component
3. Final tuning pass: adjust action weights, cooldowns, and damage values
4. Full 5-wave playthrough: verify pacing, difficulty curve, and AI behavior variety
5. Write README with project overview, architecture diagram, and build instructions
6. Write a 1-page postmortem: what worked, what was challenging, what you'd do differently
7. Record a 3–5 minute portfolio video showcasing AI behaviors and debug tools

**FINAL PROJECT DUE: End of Week 12**

---

# Weekly Schedule Template

| Day | Activity | Duration |
|---|---|---|
| **Monday** | Lecture A | 90 min |
| **Wednesday** | Lecture B | 90 min |
| **Friday** | Lab (Hands-on) | 2–3 hours |

---

# Assessment Rubrics

## Lab Submissions (Weekly)

| Criteria | Excellent (A) | Good (B) | Adequate (C) | Incomplete (D/F) |
|---|---|---|---|---|
| **Functionality** | All features working correctly | Minor issues, mostly working | Core feature works, edge cases fail | Feature doesn't work |
| **Code Quality** | Clean, documented, follows UE5 conventions | Readable, minor style issues | Functional but messy | Hard to read, no comments |
| **Architecture** | Proper component separation, no coupling | Minor coupling, mostly modular | Some code in wrong location | Monolithic, tangled |

## Midterm: AI Archetype Demo

| Criteria | Weight | Description |
|---|---|---|
| Utility AI produces varied, context-appropriate decisions | 30% | Enemy selects different actions based on distance, health, player state |
| Actions execute correctly with animations and damage | 25% | Montages play, hit detection works, damage pipeline functional |
| Perception system detects player via sight and hearing | 20% | FOV cone, LOS checks, threat scoring visible in behavior |
| Code architecture is clean and extensible | 15% | Inherits from base properly, components are modular |
| Video demo clearly showcases behavior variety | 10% | 2-minute video with narration showing different situations |

## Final Project

| Criteria | Weight | Description |
|---|---|---|
| All 3 archetypes with distinct, readable behaviors | 20% | Each archetype feels unique; behaviors match GDD descriptions |
| Squad coordination visible in gameplay | 15% | Flanking, token management, tactical situations observed |
| Utility AI makes intelligent context-sensitive decisions | 15% | Actions change based on world state; no repetitive patterns |
| Combat feels responsive and satisfying | 15% | < 100ms input, hit feedback, stagger/death effects |
| Data-driven design (all values in Data Assets) | 10% | No magic numbers; parameters tunable without code changes |
| Debug tools functional and informative | 10% | AI overlay shows state, scores, perception, tokens |
| 5-wave encounter playable with proper pacing | 10% | Waves escalate; victory and game over work correctly |
| Documentation quality | 5% | README, postmortem, code comments |

---

# Supplemental Resources

## Required Reading
- Dave Mark — *"Embracing the Dark Art of Mathematical Modeling"* (GDC Talk, Utility AI)
- Bungie — *"Building a Better Centaur: AI at Bungie"* (GDC Talk, Halo AI)
- Alex Champandard — *"Behavior Trees: From Theory to Practice"*
- UE5 Documentation: Behavior Trees, NavMesh, Enhanced Input, UMG

## Recommended Reading
- Naughty Dog — *"The AI of The Last of Us"* (GDC Talk)
- Bobby Anguelov — *"Hierarchical Task Networks in Games"*
- Mark DeLoura (ed.) — *"Game Programming Gems"* (AI Chapters)
- Robert Nystrom — *"Game Programming Patterns"* (State, Component, Observer chapters)

## Video References
- GDC Vault: *"The AI of Halo"* series
- GDC Vault: *"Combat AI in God of War"*
- AI and Games YouTube Channel: *"How AI Works in..."* series

---

# Appendix: Key Systems Cheat Sheet

| System | Core Class | Purpose |
|---|---|---|
| Player | `APlayerCharacter` | Movement, combat, lock-on |
| Player Stats | `UPlayerStatsComponent` | Health, stamina, ammo |
| Camera | `UCombatCameraComponent` | Follow, collision, effects |
| Combat | `UCombatComponent` | Shared damage pipeline |
| Hit Detection | `UHitDetectionComponent` | Animation-driven sweeps |
| Stagger | `UCombatComponent` (integrated) | Accumulation + threshold |
| AI Character | `AAICharacterBase` | Base for all archetypes |
| AI Controller | `AAIControllerBase` | BT + BB lifecycle |
| Perception | `UAICombatPerceptionComponent` | Sight, hearing, threat |
| Utility AI | `UUtilityAIComponent` | Action scoring engine |
| Response Curves | `FResponseCurve` | Linear, Quadratic, Logistic, Step, Inverse |
| Considerations | `FUtilityConsideration` | World-state → normalized → curved score |
| Actions | `FUtilityAction` | Name, weight, considerations, cooldown |
| Rusher | `ARusherCharacter` | Aggressive melee (7 actions) |
| Watcher | `AWatcherCharacter` | Ranged supporter (6 actions) |
| Warden | `AWardenCharacter` | Heavy tank (7 actions) |
| Squad Coord | `ASquadCoordinator` | Group-level tactics |
| Attack Tokens | `UAttackTokenSystem` | Limits simultaneous attacks |
| Flanking | `UFlankingSystem` | 8-sector positioning |
| Cover | `UCoverSystem` + `ACoverNode` | Cover evaluation + occupation |
| Encounter | `AEncounterManager` | Wave lifecycle |
| Data Assets | `UEnemyProfileData` etc. | Designer-tunable parameters |
| Difficulty | `UDifficultyManager` | Preset multipliers |
| Debug | `AAIDebugHUD` | AI visualization overlay |
| HUD | `UCombatHUDWidget` | Player status display |
| Enemy UI | `UEnemyHealthBarWidget` | World-space health/stagger |
| Menus | `UMainMenuWidget` | Main/Pause/GameOver/Victory |

---

*Curriculum Version: 1.0*
*Last Updated: February 10, 2026*
*Course Designer: CombatAI Project*
