# Third-Person Combat AI System — Game Overview

> **Project Codename:** *CombatAI*
> **Genre:** Third-Person Action / Action-Adventure
> **Engine:** Unreal Engine 5 (C++ & Blueprints)
> **Target Platform:** PC (Windows)
> **Development Timeline:** 3–4 Months
> **Team Size:** Solo Developer (Portfolio Project)
> **Target Audience:** Game industry recruiters/studios (Retro Studios / Nintendo)

---

## Table of Contents

1. [Executive Summary](#1-executive-summary)
2. [Design Pillars](#2-design-pillars)
3. [Game Concept](#3-game-concept)
4. [World & Setting](#4-world--setting)
5. [Player Character](#5-player-character)
6. [Camera System](#6-camera-system)
7. [Core Combat Loop](#7-core-combat-loop)
8. [Enemy Archetypes](#8-enemy-archetypes)
9. [AI Architecture](#9-ai-architecture)
10. [Group Coordination & Tactics](#10-group-coordination--tactics)
11. [Designer-Friendly Tooling](#11-designer-friendly-tooling)
12. [Level Design & Encounter Design](#12-level-design--encounter-design)
13. [Difficulty & Tuning](#13-difficulty--tuning)
14. [UI & HUD](#14-ui--hud)
15. [Audio & VFX](#15-audio--vfx)
16. [Technical Architecture](#16-technical-architecture)
17. [Development Roadmap](#17-development-roadmap)
18. [Quality Bar & Polish Targets](#18-quality-bar--polish-targets)
19. [Documentation & Collaboration Philosophy](#19-documentation--collaboration-philosophy)
20. [References & Inspirations](#20-references--inspirations)

---

## 1. Executive Summary

**CombatAI** is a focused third-person action game built as a portfolio piece to demonstrate sophisticated enemy AI systems, polished combat feel, and designer-friendly workflows. The project centers on a single arena-style combat environment where the player faces waves of diverse enemy archetypes that coordinate tactically—flanking, suppressing, retreating, and adapting to player behavior in real time.

The goal is not to ship a full game, but to demonstrate **mastery of 3D combat AI** at a level consistent with studios like Retro Studios working on action-adventure titles. Every system is built with **designer collaboration** in mind: data-driven parameters, visual debugging tools, and clear documentation at every layer.

### Key Deliverables

- A playable third-person combat scenario with responsive, weighty combat
- Three distinct enemy archetypes with unique AI behaviors
- A robust AI framework (Behavior Trees + Utility AI hybrid)
- Group AI coordination system (squad tactics, threat management)
- Designer-exposed tuning parameters (no code changes required)
- Comprehensive documentation and debug visualization tools

---

## 2. Design Pillars

### Pillar 1: **Readable AI Behaviors**
Every enemy action should be *understandable* to the player at a glance. Enemies telegraph attacks, display clear intent through animation and movement, and react visibly to player actions. The player should never feel cheated—they should always be able to identify *why* an enemy did what it did.

### Pillar 2: **Tactical Depth Through Cooperation**
Individual enemies are manageable. Groups are dangerous. The emergent difficulty comes from enemies **working together**: flanking, suppressing, staggering attacks, and exploiting player mistakes. This mirrors the design philosophy of games like *Halo*, *Metroid Prime*, and *The Last of Us*.

### Pillar 3: **Polished Combat Feel**
Responsive input, satisfying hit feedback (hitlag, camera shake, VFX), clear readability, and tight animation-driven gameplay. Every attack, dodge, and impact should *feel good*. This is non-negotiable—combat AI means nothing if the moment-to-moment gameplay doesn't feel right.

### Pillar 4: **Designer-Driven Iteration**
All behavioral parameters live in data assets, not hard-coded values. The AI system is built so a hypothetical designer can tweak aggression, engagement ranges, coordination patterns, and difficulty curves without touching a single line of code. This demonstrates the **collaborative workflow** studios like Retro Studios value.

---

## 3. Game Concept

### High Concept
A third-person melee/ranged combat game where the player faces increasingly coordinated enemy squads in a compact arena. The focus is on enemy AI quality—not on content breadth. Think of it as a **combat encounter playground** that showcases what smart enemies look like.

### Core Fantasy
The player is a skilled combatant who must read enemy formations, prioritize threats, and exploit openings. Victory comes from understanding enemy behavior patterns and turning their coordination against them.

### Scope & Boundaries

| In Scope | Out of Scope |
|---|---|
| 1 playable arena (modular, reconfigurable) | Open world / exploration |
| 3 enemy archetypes | Story / narrative / cutscenes |
| 3–5 encounter waves | Inventory / loot / RPG progression |
| Full AI framework with debug tools | Multiplayer |
| Player melee + ranged combat | Character customization |
| HUD with health/threat indicators | Save/load system |
| Polish pass (VFX, juice, audio) | Multiple player characters |

---

## 4. World & Setting

### Arena Environment
The game takes place in a **modular arena** — a ruined, industrial-sci-fi facility that provides varied combat geometry:

- **Open zones** — Large clearings where ranged enemies can establish sight lines and the player must manage spacing
- **Cover clusters** — Scattered half-walls, pillars, and barriers that both AI and the player can use for cover
- **Elevated platforms** — Multi-level geometry allowing verticality (ranged enemies seek high ground)
- **Chokepoints** — Narrow passages where melee rushers are funneled but also become more dangerous
- **Flanking routes** — Side corridors that smart enemies will exploit to approach from unexpected angles

### Environmental Storytelling
Though narrative isn't a focus, the arena should feel *lived-in*. Destroyed equipment, scorch marks, and scattered debris suggest a facility where combat has taken place before. This provides visual grounding without requiring story assets.

### Lighting & Atmosphere
- Moody ambient lighting with strong directional light for readability
- Enemy-specific visual elements (glow colors, silhouette shapes) remain distinguishable at distance
- Fog/particle effects for atmosphere that don't obscure gameplay-critical information

---

## 5. Player Character

### Movement

| Parameter | Value | Notes |
|---|---|---|
| Walk Speed | 400 cm/s | Default navigation speed |
| Sprint Speed | 700 cm/s | Hold to sprint; stamina cost |
| Dodge Roll | 600 cm/s, 0.5s duration | I-frames during active frames (0.1–0.35s) |
| Jump Height | 300 cm | Single jump; used for platform traversal |
| Turn Rate | Instant (with animation blending) | Responsive feel priority |

### Combat Abilities

#### Melee System
- **Light Attack Chain** — 3-hit combo string (quick, moderate damage, short range)
- **Heavy Attack** — Charged attack with longer wind-up, higher damage, and stagger capability
- **Dodge** — Directional roll with invincibility frames; cancels out of most actions
- **Parry** — Timed block that staggers enemies and opens a counter-attack window (tight timing)
- **Stagger System** — Enemies have a stagger meter; consistent pressure breaks their stance

#### Ranged System
- **Ranged Attack** — Projectile-based attack (energy shot) for engaging distant enemies
- **Limited Ammo / Cooldown** — Prevents ranged-only play; encourages mixing melee and ranged
- **Lock-On Assist** — Soft lock-on to targeted enemy; switchable between targets

### Player Stats

| Stat | Base Value | Notes |
|---|---|---|
| Health | 100 HP | No regeneration (health pickups only) |
| Stamina | 100 SP | Regenerates over time; dodge/sprint consume stamina |
| Stagger Resist | Medium | Player can be staggered but recovers quickly |
| Ranged Ammo | 12 shots | Replenished by defeating enemies or pickups |

### Input Mapping (Gamepad Primary)

| Action | Gamepad | Keyboard/Mouse |
|---|---|---|
| Move | Left Stick | WASD |
| Camera | Right Stick | Mouse |
| Light Attack | X / Square | Left Click |
| Heavy Attack | Y / Triangle (Hold) | Right Click (Hold) |
| Dodge | B / Circle | Space |
| Parry | LB / L1 | Q |
| Ranged Attack | RT / R2 | Middle Click |
| Lock-On Toggle | RS Click / R3 | Tab |
| Sprint | LS Click / L3 | Shift |

---

## 6. Camera System

### Third-Person Camera
- **Over-the-shoulder** default position, offset slightly right
- Smooth follow with adjustable lag (look-ahead for movement direction)
- Collision avoidance: camera pulls forward when geometry occludes
- Lock-on mode: camera orbits the locked target, player strafes

### Combat Camera Behaviors
- **Hit Impact** — Subtle screen shake on melee hits (intensity scales with attack strength)
- **Hitlag / Freeze Frame** — 2–4 frame pause on heavy hits for impact weight
- **Kill Camera** — Brief slow-motion on final enemy in a wave (optional, togglable)
- **Threat Framing** — Camera subtly adjusts FOV and position when surrounded by multiple enemies

### Camera Parameters (Designer-Tunable)

| Parameter | Default | Range |
|---|---|---|
| Follow Distance | 350 cm | 200–600 cm |
| Follow Height | 120 cm | 50–200 cm |
| Horizontal Offset | 60 cm | 0–120 cm |
| FOV | 75° | 60°–100° |
| Look-Ahead Strength | 0.3 | 0–1 |
| Shake Intensity (Light) | 2.0 | 0–10 |
| Shake Intensity (Heavy) | 5.0 | 0–10 |
| Hitlag Duration | 0.05s | 0–0.15s |

---

## 7. Core Combat Loop

### Moment-to-Moment Flow

```
[ASSESS] → Read enemy positions and intent
    ↓
[PRIORITIZE] → Identify most dangerous threat (Rusher closing in? Sniper exposed?)
    ↓
[ENGAGE] → Close distance or use ranged attack
    ↓
[REACT] → Dodge incoming attacks, parry when possible
    ↓
[PUNISH] → Exploit stagger windows and recovery frames
    ↓
[REPOSITION] → Manage spacing for the next engagement
    ↓
[REPEAT]
```

### Combat Cadence
The rhythm of combat alternates between:

1. **Offensive Windows** — When enemies are recovering, staggered, or between coordinated attacks
2. **Defensive Moments** — When multiple enemies attack simultaneously, requiring dodges and positioning
3. **Tactical Decisions** — Choosing which enemy to engage first based on archetype and positioning

### Damage Model

| Attack | Damage | Stagger | Notes |
|---|---|---|---|
| Player Light Attack | 10 | Low | Fast, combo-able |
| Player Heavy Attack | 25 | High | Slow wind-up, breaks guard |
| Player Ranged | 15 | Medium | Moderate speed projectile |
| Player Parry Counter | 30 | Max | Requires precise timing |
| Rusher Slash | 12 | Low | Fast but telegraphed |
| Rusher Lunge | 20 | Medium | Gap-closing, dodge-able |
| Sniper Shot | 18 | High | Long wind-up, high accuracy |
| Sniper Overwatch Burst | 8 per hit | Low | Suppressive, less accurate |
| Tank Slam | 30 | Max | Slow, AOE ground effect |
| Tank Charge | 25 | High | Linear path, dodge-able |
| Tank Shield Bash | 15 | High | Close range, fast |

---

## 8. Enemy Archetypes

### 8.1 Melee Rusher — *"The Blade"*

> *Aggressive, fast, expendable. Rushers close distance quickly and attack relentlessly. Individually manageable but dangerous in packs.*

**Visual Design:**
- Lean, agile humanoid silhouette
- Glowing red energy accents (aggressive = warm color)
- Dual blades or clawed weapons
- Fast, twitchy idle animations

**Core Behavior:**
- Sprints toward the player using the shortest navigable path
- Attacks in quick 2–3 hit combos with short recovery windows
- Uses **lunges** to close gaps when at medium range
- Circles and repositions between attack strings
- **Retreats briefly** when staggered or at low health

**AI Decision Factors:**
| Factor | Weight | Description |
|---|---|---|
| Distance to Player | High | Wants to be in melee range (0–200 cm) |
| Player Facing | Medium | Prefers attacking from flanks/rear |
| Own Health | Medium | Retreats below 25% HP |
| Ally Count | Low | More aggressive when allies are alive |
| Player Stagger State | High | Rushes immediately if player is staggered |

**Stats:**

| Stat | Value |
|---|---|
| Health | 40 HP |
| Move Speed | 650 cm/s |
| Attack Speed | Fast (0.4s between swings) |
| Stagger Threshold | Low (20 damage) |
| Aggression | Very High |
| Detection Range | 2500 cm |

**Attack Patterns:**

1. **Slash Combo** — 2-hit close-range combo; 0.8s total; 12 damage per hit
2. **Lunge Strike** — Gap-closer from 300–600 cm; 0.6s charge, 20 damage; dodge window at 0.3–0.5s
3. **Circling Slash** — Strafes 90° then attacks; used when player is blocking/facing directly

**Designer-Tunable Parameters:**
- `AggressionLevel` (0–1): How eagerly it closes distance
- `FlankPreference` (0–1): Tendency to circle to player's flank vs. direct charge
- `RetreatHealthThreshold` (0–1): HP percentage triggering retreat
- `AttackCooldown` (seconds): Minimum time between attack strings
- `LungeRange` (cm): Distance at which lunge attack activates
- `CircleSpeed` (cm/s): Speed while strafing around the player
- `ComboLength` (int): Number of hits in a combo string

---

### 8.2 Ranged Supporter — *"The Watcher"*

> *Calculated and methodical. Watchers maintain distance, suppress the player with projectiles, and reposition when threatened. They make Rushers more dangerous by limiting the player's movement options.*

**Visual Design:**
- Taller, slender silhouette with a distinct weapon (energy rifle/staff)
- Glowing blue/cyan accents (calculated = cool color)
- Elevated posture; looks down sights frequently
- Smooth, deliberate movement animations

**Core Behavior:**
- Maintains **optimal engagement range** (1500–2500 cm from player)
- Takes **aimed shots** with clear wind-up (laser sight / charging VFX)
- Switches to **suppressive fire** when allies are closing in on the player
- Seeks **elevated positions** and **cover** proactively
- **Retreats** when player closes within 500 cm

**AI Decision Factors:**
| Factor | Weight | Description |
|---|---|---|
| Distance to Player | Very High | Wants to stay at range (1500–2500 cm) |
| Line of Sight | Very High | Needs clear LOS to fire |
| Cover Availability | High | Prefers positions near cover |
| Elevation | Medium | Seeks high ground when available |
| Ally Engagement | High | Suppressive fire when allies are in melee |
| Own Health | Medium | Increased retreat urgency at low health |

**Stats:**

| Stat | Value |
|---|---|
| Health | 30 HP |
| Move Speed | 400 cm/s |
| Attack Speed | Slow (1.5s between shots) |
| Stagger Threshold | Very Low (10 damage) |
| Aggression | Low |
| Detection Range | 4000 cm |
| Optimal Range | 1500–2500 cm |

**Attack Patterns:**

1. **Aimed Shot** — 1.2s wind-up with visible laser sight; 18 damage; high accuracy; single target
2. **Overwatch Burst** — 3-round burst at reduced accuracy; 8 damage per hit; used during ally engagement
3. **Retreat Shot** — Fires while backpedaling; reduced accuracy; used when player closes in

**Designer-Tunable Parameters:**
- `OptimalRangeMin` / `OptimalRangeMax` (cm): Preferred engagement distance band
- `RetreatDistance` (cm): Distance threshold triggering retreat behavior
- `ShotWindUpTime` (seconds): Telegraph duration before firing
- `SuppressionMode` (bool): Enable/disable suppressive fire behavior
- `CoverSearchRadius` (cm): How far to look for cover when repositioning
- `ElevationPreference` (0–1): How strongly it seeks high ground
- `AccuracyBase` (0–1): Base shot accuracy
- `AccuracyMoving` (0–1): Accuracy while the target is moving

---

### 8.3 Tank — *"The Warden"*

> *Slow, armored, dominant. Wardens are battlefield anchors that command space. They soak damage, disrupt the player's positioning with area attacks, and protect weaker allies.*

**Visual Design:**
- Large, heavy silhouette (1.5x player scale)
- Glowing amber/orange accents (dominant = warm neutral)
- Massive shield or armored plating on front
- Heavy, deliberate movement; ground trembles with footsteps
- Visible weak point on back (glowing core)

**Core Behavior:**
- Advances **slowly but relentlessly** toward the player
- Uses **shield** to block frontal attacks (player must flank or stagger)
- Performs **area-of-effect attacks** that deny space
- **Charges** in a straight line to close distance and disrupt player positioning
- Acts as a **bodyguard** for nearby Watchers when able
- Has a **critical weak point** on back — exposed during certain attack animations

**AI Decision Factors:**
| Factor | Weight | Description |
|---|---|---|
| Distance to Player | High | Wants to be in close-medium range (0–500 cm) |
| Player Position (Frontal) | Very High | Always faces the player; rotates to deny flank |
| Ally Proximity | High | Positions between player and Watchers |
| Charge Opportunity | Medium | Charges when player is in a line at 500–1200 cm |
| Own Health | Low | Tanks don't retreat easily |
| Weak Point Exposure | Very High | Turns to protect back when possible |

**Stats:**

| Stat | Value |
|---|---|
| Health | 150 HP |
| Frontal Armor | 50% damage reduction |
| Weak Point Multiplier | 2x damage |
| Move Speed | 250 cm/s |
| Charge Speed | 800 cm/s |
| Stagger Threshold | Very High (60 damage) |
| Aggression | Medium |
| Detection Range | 2000 cm |

**Attack Patterns:**

1. **Ground Slam** — 1.5s wind-up; AOE in 300 cm radius; 30 damage; stagger on hit; dodge window at 0.8–1.2s
2. **Shield Charge** — Charges 800 cm in a line; 25 damage + knockdown on hit; 1.0s recovery if misses
3. **Shield Bash** — Quick close-range swipe; 15 damage; pushes player back 200 cm
4. **Stomp Wave** — Below 30% HP only; 360° AOE shockwave; 20 damage; forces player to jump or dodge

**Designer-Tunable Parameters:**
- `FrontalArmorReduction` (0–1): Damage reduction from frontal attacks
- `WeakPointMultiplier` (float): Damage multiplier for back weak point
- `ChargeDistance` (cm): Max charge distance
- `ChargeCooldown` (seconds): Minimum time between charges
- `SlamRadius` (cm): Ground slam AOE radius
- `BodyguardRange` (cm): Distance within which Tank will protect allies
- `TurnSpeed` (degrees/s): How fast the Tank rotates to face player
- `EnrageHealthThreshold` (0–1): HP percentage that triggers Stomp Wave ability

---

### 8.4 Enemy Archetype Comparison Matrix

| Attribute | Rusher | Watcher | Warden |
|---|---|---|---|
| Role | Aggressive DPS | Ranged Support | Frontline Tank |
| Health | Low (40) | Very Low (30) | Very High (150) |
| Speed | Fast (650) | Medium (400) | Slow (250) |
| Damage | Moderate | Moderate | High |
| Range | Melee | Long | Close-Medium |
| Threat Solo | Low | Low | Medium |
| Threat in Group | High | Very High | Very High |
| Counter Strategy | Parry + combo | Close gap quickly | Flank to weak point |
| Color Code | Red | Blue/Cyan | Amber/Orange |

---

## 9. AI Architecture

### 9.1 Hybrid Approach: Behavior Trees + Utility AI

The AI system uses a **hybrid architecture** combining the structure of Behavior Trees with the flexibility of Utility AI scoring:

```
┌─────────────────────────────────────────────────┐
│                  AI BRAIN (Per Enemy)            │
│                                                  │
│  ┌──────────────┐    ┌────────────────────────┐ │
│  │  Perception   │───▶│  Utility AI Evaluator  │ │
│  │  System       │    │  (Action Scoring)      │ │
│  └──────────────┘    └──────────┬─────────────┘ │
│                                 │                │
│                     ┌───────────▼──────────────┐ │
│                     │  Behavior Tree Executor  │ │
│                     │  (Action Execution)      │ │
│                     └───────────┬──────────────┘ │
│                                 │                │
│                     ┌───────────▼──────────────┐ │
│                     │  Animation / Movement    │ │
│                     │  (Motor Output)          │ │
│                     └──────────────────────────┘ │
│                                                  │
│  ┌──────────────┐                               │
│  │  Group AI     │◀── Squad Coordinator         │
│  │  Influence    │    (Shared across squad)      │
│  └──────────────┘                               │
└─────────────────────────────────────────────────┘
```

**Why Hybrid?**
- **Behavior Trees** provide clear, debuggable execution flow — ideal for designers to understand and modify
- **Utility AI** provides nuanced decision-making without explosion of conditional branches — ideal for context-sensitive choices
- The **Utility layer picks WHAT** to do; the **Behavior Tree handles HOW** to do it

### 9.2 Perception System

Each AI agent maintains a perception model of the world:

#### Sight
- **FOV Cone**: Configurable angle (default 120°) and range (archetype-dependent)
- **Line-of-sight checks**: Raycast-based, with configurable frequency (default every 0.2s)
- **Peripheral detection**: Wider angle (270°) but requires more stimuli to trigger full awareness
- **Last Known Position**: If LOS is broken, AI remembers last seen position and investigates

#### Hearing
- **Sound events**: Player attacks, footsteps (sprint), and projectile impacts generate sound stimuli
- **Sound propagation**: Radius-based; attenuated by distance; blocked by geometry (simplified)
- **Investigation behavior**: AI moves toward sound source if player is not currently visible

#### Threat Assessment
- **Threat Level**: Continuously scored value (0–100) based on:
  - Player proximity
  - Player current action (attacking = high threat, idle = low threat)
  - Damage received from player recently
  - Ally casualties
- **Threat Memory**: Decays over time; doesn't reset instantly when player disengages

#### Perception Data Structure

```cpp
USTRUCT(BlueprintType)
struct FPerceptionData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly) bool bCanSeePlayer;
    UPROPERTY(BlueprintReadOnly) bool bCanHearPlayer;
    UPROPERTY(BlueprintReadOnly) FVector LastKnownPlayerPosition;
    UPROPERTY(BlueprintReadOnly) float TimeSinceLastSeen;
    UPROPERTY(BlueprintReadOnly) float ThreatLevel;          // 0–100
    UPROPERTY(BlueprintReadOnly) float DistanceToPlayer;
    UPROPERTY(BlueprintReadOnly) bool bPlayerIsAttacking;
    UPROPERTY(BlueprintReadOnly) bool bPlayerIsStaggered;
    UPROPERTY(BlueprintReadOnly) int32 NearbyAllyCount;
    UPROPERTY(BlueprintReadOnly) float DamageReceivedRecently;
};
```

### 9.3 Utility AI — Action Scoring

Each AI archetype evaluates a set of possible **actions** every decision tick. Each action has a **utility score** computed from weighted **considerations** (response curves applied to world state inputs).

#### Action Evaluation Pipeline

```
For each candidate action:
    score = 1.0
    For each consideration:
        input = normalize(world_state_value, min, max)  // 0–1
        response = apply_curve(input, curve_type, parameters)
        score *= response  // Multiplicative combination
    score *= action_weight  // Global weight modifier
    
Select action with highest score (with optional randomization for variety)
```

#### Response Curve Types
- **Linear** — Direct proportional mapping
- **Quadratic** — Accelerating or decelerating curve
- **Logistic** — S-curve with configurable steepness and midpoint
- **Step** — Binary threshold (below X = 0, above X = 1)
- **Inverse** — Higher input = lower score

#### Rusher Action Table

| Action | Considerations | Notes |
|---|---|---|
| Chase Player | Distance (inverse logistic), Player Visible (step) | Default behavior when far |
| Melee Attack | Distance (step, < 200cm), Cooldown Ready (step) | Primary damage action |
| Lunge Attack | Distance (logistic, 300–600cm), Player Stationary (linear) | Gap closer |
| Circle Strafe | Distance (step, < 300cm), Ally Attacking (step), Cooldown Active (step) | Wait for opening |
| Flank | Group Request (step), Player Facing (inverse linear) | Coordinated behavior |
| Retreat | Own Health (inverse quadratic), Damage Received (linear) | Self-preservation |
| Investigate | Last Known Position Valid (step), Can't See Player (step) | Search behavior |

#### Watcher Action Table

| Action | Considerations | Notes |
|---|---|---|
| Aimed Shot | Distance (logistic, 1500–2500), LOS (step), Cooldown Ready (step) | Primary attack |
| Overwatch Burst | Ally In Melee (step), LOS (step) | Suppressive fire |
| Seek Cover | Exposed (step), Player Facing (linear), Threat Level (linear) | Defensive repositioning |
| Seek High Ground | Elevation Available (step), Not At Elevation (step) | Tactical positioning |
| Retreat | Distance (inverse logistic, < 500), Health (inverse quadratic) | Flee when pressured |
| Reposition | LOS Blocked (step), Current Position Quality (inverse) | Find new angle |

#### Warden Action Table

| Action | Considerations | Notes |
|---|---|---|
| Advance | Distance (logistic, > 500), Can See Player (step) | Close distance slowly |
| Ground Slam | Distance (step, < 300), Cooldown Ready (step) | AOE zone denial |
| Shield Charge | Distance (logistic, 500–1200), Clear Path (step), Cooldown Ready (step) | Disruption |
| Shield Bash | Distance (step, < 150), Player Attacking Front (step) | Punish aggression |
| Stomp Wave | Health Below Enrage (step), Surrounded (step) | Desperation move |
| Protect Ally | Ally Nearby (logistic), Ally Under Threat (linear) | Bodyguard behavior |
| Face Player | Player Behind (step) | Protect weak point |

### 9.4 Behavior Tree Structure

Each Utility-selected action maps to a **Behavior Tree subtree** that handles execution:

#### Example: Rusher Behavior Tree (Simplified)

```
[Root - Selector]
├── [Sequence: Death]
│   ├── [Condition: Health <= 0]
│   └── [Task: Play Death Animation → Destroy]
│
├── [Sequence: Staggered]
│   ├── [Condition: IsStaggered]
│   └── [Task: Play Stagger Recovery → Wait → Resume]
│
├── [Sequence: Execute Utility Action]
│   ├── [Task: Get Highest Utility Action]
│   └── [Selector: Action Router]
│       ├── [Sequence: Chase]
│       │   ├── [Condition: Action == Chase]
│       │   └── [Task: MoveTo Player → Update Path Every 0.3s]
│       │
│       ├── [Sequence: Melee Attack]
│       │   ├── [Condition: Action == MeleeAttack]
│       │   ├── [Task: Face Player]
│       │   └── [Task: Play Attack Montage → Apply Damage on Notify]
│       │
│       ├── [Sequence: Lunge]
│       │   ├── [Condition: Action == Lunge]
│       │   ├── [Task: Face Player]
│       │   └── [Task: Root Motion Lunge → Damage at Apex]
│       │
│       ├── [Sequence: Circle Strafe]
│       │   ├── [Condition: Action == CircleStrafe]
│       │   └── [Task: Strafe Around Player → Timer → Re-evaluate]
│       │
│       ├── [Sequence: Retreat]
│       │   ├── [Condition: Action == Retreat]
│       │   └── [Task: MoveTo Away From Player → Timer → Re-evaluate]
│       │
│       └── [Sequence: Investigate]
│           ├── [Condition: Action == Investigate]
│           └── [Task: MoveTo Last Known Position → Look Around → Resume]
```

### 9.5 AI State Machine (High-Level)

Overlaying the Behavior Tree is a simple state machine that governs high-level AI phases:

```
[IDLE] ──(perception trigger)──▶ [ALERT]
  ▲                                 │
  │                        (confirm sighting)
  │                                 ▼
  └──(lost player, timeout)── [COMBAT] ──(health = 0)──▶ [DEAD]
                                 │  ▲
                      (lost LOS) │  │ (re-acquired)
                                 ▼  │
                            [SEARCHING]
```

| State | Description | Behavior |
|---|---|---|
| **Idle** | No stimuli detected | Patrol or stand guard; low tick rate |
| **Alert** | Stimulus detected, not confirmed | Turn toward stimulus; heightened perception |
| **Combat** | Player confirmed | Full AI decision loop; Utility + BT active |
| **Searching** | Lost track of player | Move to last known position; sweep area |
| **Dead** | Health depleted | Ragdoll / death animation; cleanup |

---

## 10. Group Coordination & Tactics

### 10.1 Squad Coordinator

A central **Squad Coordinator** component manages group-level tactics. It does not control individual AI directly but provides **requests and information** that influence individual Utility scores.

```
┌───────────────────────────────────────┐
│          SQUAD COORDINATOR            │
│                                       │
│  ┌─────────┐  ┌──────────────────┐   │
│  │ Threat   │  │ Role Assignment  │   │
│  │ Manager  │  │ System           │   │
│  └────┬────┘  └────────┬─────────┘   │
│       │                │              │
│  ┌────▼────┐  ┌────────▼─────────┐   │
│  │ Attack  │  │ Position         │   │
│  │ Tokens  │  │ Manager          │   │
│  └────┬────┘  └────────┬─────────┘   │
│       │                │              │
│  ┌────▼────────────────▼─────────┐   │
│  │    Tactical Evaluator         │   │
│  │    (Flanking, Cover, etc.)    │   │
│  └───────────────────────────────┘   │
└───────────────────────────────────────┘
```

### 10.2 Attack Token System

To prevent "dogpiling" (all enemies attacking simultaneously), an **Attack Token** system limits concurrent attacks:

- The Squad Coordinator maintains a pool of **attack tokens** (default: 2–3)
- An enemy must **request and receive** a token before initiating an attack
- Tokens are **returned** after the attack completes (or after a timeout)
- Priority is influenced by: proximity, archetype role, time since last attack

**Designer-Tunable:**
- `MaxConcurrentAttacks` (int): How many enemies can attack at once
- `TokenPriority_Distance` (0–1): Weight for distance-based priority
- `TokenPriority_TimeSinceAttack` (0–1): Weight for fairness (longer wait = higher priority)
- `TokenHoldDuration` (seconds): Max time an enemy can hold a token

### 10.3 Flanking System

The Squad Coordinator assigns **approach vectors** to prevent all enemies from approaching from the same direction:

#### Flanking Algorithm
1. Divide the space around the player into **sectors** (8 sectors, 45° each)
2. Track which sectors are **occupied** by allies
3. When assigning a new enemy an engagement position, prefer **unoccupied sectors**
4. Prefer **rear sectors** (behind player facing direction) when available
5. Update assignments dynamically as the player rotates and moves

```
        [REAR-LEFT]  [REAR]  [REAR-RIGHT]
             ╲         │         ╱
              ╲        │        ╱
    [LEFT] ────── [PLAYER] ────── [RIGHT]
              ╱        │        ╲
             ╱         │         ╲
      [FRONT-LEFT] [FRONT] [FRONT-RIGHT]
```

**Designer-Tunable:**
- `FlankPreference` (0–1): How strongly enemies prefer non-frontal sectors
- `SectorReassignmentInterval` (seconds): How often positions are re-evaluated
- `MinSectorSpacing` (cm): Minimum distance between enemies in the same sector

### 10.4 Cover System

AI agents can utilize **cover nodes** placed in the environment:

#### Cover Node Properties
- **Position** & **Forward Direction** (the direction the cover protects from)
- **Height**: Low (crouch) or High (standing)
- **Width**: Narrow (1 agent) or Wide (2 agents)
- **Occupied By**: Reference to occupying AI (or null)

#### Cover Evaluation Scoring

```
cover_score = 
    w1 * protects_from_player_direction +
    w2 * within_optimal_range +
    w3 * not_occupied +
    w4 * has_line_of_sight_to_fire +
    w5 * distance_to_current_position (inverse — prefer closer)
```

**Used primarily by:** Watchers (proactive cover seeking) and Rushers (brief cover during retreat)

### 10.5 Aggro Management

The Squad Coordinator tracks **who the player is focused on** and coordinates responses:

- **Aggro Target**: The enemy the player is currently attacking or facing
- **Aggro Holder**: Gets defensive/evasive behavior bonus
- **Non-Aggro Allies**: Get offensive behavior bonus (flank, close distance)
- If the aggro target **dies**, the next closest enemy becomes the new perceived target

### 10.6 Tactical Situations

The Squad Coordinator recognizes and responds to tactical situations:

| Situation | Detection | Response |
|---|---|---|
| **Player Surrounded** | 3+ enemies in different sectors | All enemies attack aggressively; reduced token requirement |
| **Ally Down** | Ally killed | Nearby allies briefly increase aggression; Watchers reposition |
| **Player at Range** | Player far from all enemies | Rushers sprint; Watchers fire freely; Tank charges |
| **Player in Cover** | Player near cover geometry | Rushers flank; Watchers reposition for angle; Tank advances |
| **Player Low Health** | Player HP below 25% | All enemies increase aggression |
| **Squad Depleted** | 1 enemy remaining | Remaining enemy becomes desperate (increased aggro + new attacks) |

---

## 11. Designer-Friendly Tooling

### 11.1 Data Asset Architecture

All AI parameters are stored in **UE5 Data Assets** (or equivalent scriptable objects), not hard-coded:

```
/Content/Data/AI/
├── EnemyProfiles/
│   ├── DA_Rusher_Default.uasset
│   ├── DA_Rusher_Elite.uasset
│   ├── DA_Watcher_Default.uasset
│   ├── DA_Watcher_Sniper.uasset
│   ├── DA_Warden_Default.uasset
│   └── DA_Warden_Heavy.uasset
├── UtilityActions/
│   ├── DA_Action_Chase.uasset
│   ├── DA_Action_MeleeAttack.uasset
│   ├── DA_Action_AimedShot.uasset
│   ├── DA_Action_SeekCover.uasset
│   └── ...
├── SquadProfiles/
│   ├── DA_Squad_EarlyEncounter.uasset
│   ├── DA_Squad_MidEncounter.uasset
│   └── DA_Squad_BossEncounter.uasset
├── ResponseCurves/
│   ├── DA_Curve_Linear.uasset
│   ├── DA_Curve_Logistic.uasset
│   └── DA_Curve_InverseQuadratic.uasset
└── DifficultyProfiles/
    ├── DA_Difficulty_Easy.uasset
    ├── DA_Difficulty_Normal.uasset
    └── DA_Difficulty_Hard.uasset
```

#### Enemy Profile Data Asset (Example)

```cpp
UCLASS(BlueprintType)
class UEnemyProfileData : public UDataAsset
{
    GENERATED_BODY()
    
public:
    // -- Identity --
    UPROPERTY(EditAnywhere, Category = "Identity")
    FName EnemyName;

    UPROPERTY(EditAnywhere, Category = "Identity")
    EEnemyArchetype Archetype;  // Rusher, Watcher, Warden
    
    // -- Stats --
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "500"))
    float MaxHealth = 40.0f;
    
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "100", ClampMax = "1000"))
    float MoveSpeed = 650.0f;
    
    UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0", ClampMax = "100"))
    float StaggerThreshold = 20.0f;
    
    // -- Perception --
    UPROPERTY(EditAnywhere, Category = "Perception", meta = (ClampMin = "500", ClampMax = "5000"))
    float DetectionRange = 2500.0f;
    
    UPROPERTY(EditAnywhere, Category = "Perception", meta = (ClampMin = "30", ClampMax = "360"))
    float FieldOfView = 120.0f;
    
    // -- Behavior --
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0", ClampMax = "1"))
    float AggressionLevel = 0.8f;
    
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0", ClampMax = "1"))
    float FlankPreference = 0.5f;
    
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0", ClampMax = "1"))
    float RetreatHealthThreshold = 0.25f;
    
    UPROPERTY(EditAnywhere, Category = "Behavior", meta = (ClampMin = "0.1", ClampMax = "5.0"))
    float AttackCooldown = 1.0f;
    
    // -- Utility Actions --
    UPROPERTY(EditAnywhere, Category = "Utility AI")
    TArray<UUtilityActionData*> AvailableActions;
    
    // -- Visual --
    UPROPERTY(EditAnywhere, Category = "Visual")
    USkeletalMesh* Mesh;
    
    UPROPERTY(EditAnywhere, Category = "Visual")
    UAnimBlueprint* AnimBlueprint;
    
    UPROPERTY(EditAnywhere, Category = "Visual")
    FLinearColor AccentColor;
};
```

### 11.2 Visual Debug Tools

#### In-Game Debug HUD (Toggle with ` key)

| Overlay | Information Displayed |
|---|---|
| **AI State** | Current state (Idle/Alert/Combat/etc.) above each enemy's head |
| **Current Action** | Active Utility Action name and score |
| **Perception** | FOV cone visualization; sight lines to player; hearing radius |
| **Threat Level** | Color-coded threat bar (green → yellow → red) |
| **Attack Tokens** | Token holders highlighted; token queue displayed |
| **Squad Sectors** | Flanking sector visualization around player |
| **Cover Nodes** | Cover positions color-coded (green = available, red = occupied, gray = invalid angle) |
| **Pathfinding** | NavMesh visualization; current path displayed per enemy |
| **Utility Scores** | Bar chart of all action scores for selected enemy |

#### Debug Commands (Console)

```
ai.debug.all           -- Toggle all debug overlays
ai.debug.perception    -- Toggle perception visualization
ai.debug.utility       -- Toggle utility score display
ai.debug.squad         -- Toggle squad coordination overlay
ai.debug.cover         -- Toggle cover node visualization
ai.debug.tokens        -- Toggle attack token display
ai.debug.select [id]   -- Select specific enemy for detailed inspection
ai.debug.pause         -- Freeze all AI for inspection
ai.debug.step          -- Advance AI by one tick (when paused)
ai.debug.log [id]      -- Dump decision log for enemy to output log
ai.spawn [archetype] [count] -- Spawn enemies on the fly
ai.difficulty [preset]  -- Switch difficulty preset at runtime
```

### 11.3 Blueprint Integration

- All AI decision-making events fire **Blueprint-assignable delegates**
- Key events exposed:
  - `OnStateChanged(EAIState OldState, EAIState NewState)`
  - `OnActionSelected(UUtilityActionData* Action, float Score)`
  - `OnAttackTokenGranted()` / `OnAttackTokenReturned()`
  - `OnPerceptionUpdated(FPerceptionData Data)`
  - `OnDamageReceived(float Amount, AActor* Source)`
  - `OnStaggered()` / `OnStaggerRecovered()`
  - `OnDeath()`
- Designers can hook into any of these events in Blueprint for custom responses, VFX triggers, or encounter scripting

### 11.4 Runtime Parameter Tweaking

- All Data Asset values are accessible through an **in-game tweaking panel** (ImGui or UMG-based)
- Changes take effect **immediately** — no restart or recompile needed
- Supports **save/load** of parameter snapshots for A/B testing
- Hot-reload of Data Assets during PIE (Play In Editor) sessions

---

## 12. Level Design & Encounter Design

### 12.1 Arena Layout

The arena is designed as a **single interconnected space** with distinct zones:

```
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│    [ELEVATED PLATFORM]          [ELEVATED PLATFORM]         │
│    ┌─────────────┐              ┌──────────────┐            │
│    │  Sniper      │   RAMP      │   Sniper      │           │
│    │  Perch       │◄───────     │   Perch       │           │
│    └─────────────┘              └──────────────┘            │
│                                                             │
│         ┌───┐                        ┌───┐                  │
│         │CVR│    OPEN ARENA          │CVR│                  │
│         └───┘                        └───┘                  │
│                                                             │
│    ┌───┐          ┌──────┐          ┌───┐                   │
│    │CVR│          │ CVR  │          │CVR│                    │
│    └───┘          │ (lg) │          └───┘                    │
│                   └──────┘                                  │
│  ┌──────────┐                           ┌──────────┐       │
│  │ FLANK    │                           │ FLANK    │       │
│  │ CORRIDOR │        [PLAYER            │ CORRIDOR │       │
│  │          │         START]            │          │       │
│  └──────────┘                           └──────────┘       │
│                                                             │
│         ┌───┐                        ┌───┐                  │
│         │CVR│    CHOKEPOINT          │CVR│                  │
│         └───┘    ┌──────────┐        └───┘                  │
│                  │          │                               │
│    [SPAWN ZONE]  │  NARROW  │  [SPAWN ZONE]                │
│                  │ PASSAGE  │                               │
│                  └──────────┘                               │
└─────────────────────────────────────────────────────────────┘

CVR = Cover Position
```

### 12.2 Encounter Wave Design

The game features **5 escalating encounter waves** that introduce and combine archetypes:

#### Wave 1: Introduction — Rushers Only
| Enemy | Count | Purpose |
|---|---|---|
| Rusher | 3 | Teach melee combat, dodging, and attack patterns |

**Designer Notes:** No coordination complexity. Let the player learn the controls and feel the combat. Rushers attack one at a time (1 attack token).

#### Wave 2: Ranged Pressure — Watchers Enter
| Enemy | Count | Purpose |
|---|---|---|
| Watcher | 2 | Introduce ranged threats; teach gap-closing |
| Rusher | 2 | Demonstrate Rusher + Watcher synergy |

**Designer Notes:** Watchers fire while Rushers engage. Player must decide: deal with ranged pressure or melee threats first. 2 attack tokens.

#### Wave 3: The Wall — Tank Introduction
| Enemy | Count | Purpose |
|---|---|---|
| Warden | 1 | Introduce tank archetype; teach flanking for weak point |
| Rusher | 2 | Rushers distract while Tank advances |

**Designer Notes:** Tank acts as primary threat. Rushers make flanking the Tank's weak point harder. 2 attack tokens.

#### Wave 4: Full Squad — Combined Arms
| Enemy | Count | Purpose |
|---|---|---|
| Warden | 1 | Anchors the formation |
| Watcher | 2 | Elevated positions; suppressive fire |
| Rusher | 3 | Aggressive flanking |

**Designer Notes:** Full tactical AI on display. Watchers on high ground. Tank protecting Watchers. Rushers flanking. Player must use all skills. 3 attack tokens.

#### Wave 5: Gauntlet — Stress Test
| Enemy | Count | Purpose |
|---|---|---|
| Warden | 2 | Two tanks create zone control nightmare |
| Watcher | 3 | Overlapping fields of fire |
| Rusher | 4 | Constant flanking pressure |

**Designer Notes:** This wave is the showcase. 9 enemies with full coordination. Demonstrates AI system scalability. 3–4 attack tokens. Health pickups available.

### 12.3 Encounter Scripting Data

Each wave is defined in an **Encounter Data Asset**:

```cpp
USTRUCT(BlueprintType)
struct FEncounterWave
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere) FText WaveName;
    UPROPERTY(EditAnywhere) TArray<FEnemySpawnEntry> Enemies;
    UPROPERTY(EditAnywhere) USquadProfileData* SquadProfile;
    UPROPERTY(EditAnywhere) int32 AttackTokenCount = 2;
    UPROPERTY(EditAnywhere) float TimeBetweenSpawns = 0.5f;
    UPROPERTY(EditAnywhere) bool bSpawnHealthPickup = false;
    UPROPERTY(EditAnywhere) float DelayAfterPreviousWave = 3.0f;
};
```

---

## 13. Difficulty & Tuning

### 13.1 Difficulty Presets

Three difficulty levels modify AI parameters globally via **Difficulty Profile Data Assets**:

| Parameter | Easy | Normal | Hard |
|---|---|---|---|
| Enemy Health Multiplier | 0.7x | 1.0x | 1.3x |
| Enemy Damage Multiplier | 0.7x | 1.0x | 1.2x |
| Max Attack Tokens | 1 | 2–3 | 3–4 |
| AI Reaction Time | +0.3s delay | Base | -0.1s faster |
| Flanking Aggressiveness | 0.3 | 0.6 | 0.9 |
| Ranged Accuracy | 0.5 | 0.7 | 0.85 |
| Stagger Duration (Enemy) | +30% | Base | -20% |
| Player Health | 130 HP | 100 HP | 80 HP |
| Player Stamina Regen | +25% | Base | -15% |
| Health Pickup Frequency | High | Medium | Low |

### 13.2 Dynamic Difficulty Adjustments (Optional)

An optional **dynamic difficulty** layer tracks player performance and adjusts in real-time:

**Performance Metrics Tracked:**
- Deaths per wave
- Average time to clear a wave
- Damage taken per wave
- Parry success rate
- Dodge success rate

**Adjustment Levers:**
- Subtly increase/decrease attack token count
- Adjust enemy reaction time
- Modify ranged accuracy
- Increase/decrease health pickup frequency

**Implementation Note:** All dynamic adjustments are **invisible** to the player and bounded by configurable min/max ranges. This system is togglable and defaults to OFF for the portfolio demo (to show raw AI behavior).

---

## 14. UI & HUD

### 14.1 In-Game HUD Elements

```
┌──────────────────────────────────────────────────────┐
│ [WAVE 3/5]                              [ENEMIES: 4] │
│                                                      │
│                                                      │
│                                                      │
│                                                      │
│                          +                           │
│                       [RETICLE]                      │
│                                                      │
│                                                      │
│                                                      │
│                                                      │
│                                                      │
│                    [LOCK-ON INDICATOR]                │
│                    Enemy Name | HP Bar               │
│                                                      │
│ [HEALTH BAR ████████░░]     [STAMINA ██████████░░]   │
│ [AMMO: ◆◆◆◆◆◆◆◆◇◇◇◇]                              │
└──────────────────────────────────────────────────────┘
```

### 14.2 Enemy UI Indicators
- **Health bar** above each enemy (visible when damaged)
- **Stagger meter** below health bar (fills as player lands hits)
- **Attack telegraph** — Red flash/glow preceding attacks
- **Weak point indicator** — Pulsing glow on Warden's back when exposed
- **Aggro indicator** — Subtle icon showing which enemy has player's attention

### 14.3 Menus
- **Main Menu** — Start / Difficulty Select / Controls / Quit
- **Pause Menu** — Resume / Restart Wave / Restart All / Difficulty / Quit
- **Wave Complete Screen** — Stats (time, damage taken, parries, etc.) / Continue
- **Game Over Screen** — Stats / Retry / Quit
- **Victory Screen** — Full run stats / Replay

---

## 15. Audio & VFX

### 15.1 Audio Design Priorities

| Category | Examples | Priority |
|---|---|---|
| **Player Combat** | Sword swings, hit impacts, dodge whoosh | Critical |
| **Enemy Feedback** | Attack telegraphs (audio cue), hurt sounds, death | Critical |
| **UI Audio** | Menu navigation, wave announcements | Medium |
| **Ambient** | Arena atmosphere, environmental hum | Low |
| **Music** | Dynamic combat music (intensity layers) | Low |

### 15.2 VFX Priorities

| Effect | Description | Priority |
|---|---|---|
| **Hit Sparks** | Particles on melee impact | Critical |
| **Slash Trails** | Weapon swing arc visualization | Critical |
| **Projectile Trail** | Energy trail for ranged attacks (both player and Watcher) | Critical |
| **Attack Telegraph** | Red glow/flash before enemy attacks | Critical |
| **Stagger Effect** | Brief visual disruption on stagger | High |
| **Ground Slam AOE** | Expanding ring/crack on Warden slam | High |
| **Charge Trail** | Dust/sparks behind charging Warden | High |
| **Weak Point Glow** | Pulsing glow on Warden's back | High |
| **Shield Impact** | Sparks/deflection on frontal Warden hits | Medium |
| **Death Dissolve** | Enemies dissolve on death | Medium |
| **Health Pickup** | Glowing orb with particle aura | Low |
| **Environment** | Dust particles, ambient fog | Low |

### 15.3 Screen Effects

- **Damage Vignette** — Red screen edges when player is hit
- **Low Health Warning** — Pulsing red vignette + heartbeat audio
- **Stagger Screen Shake** — Brief shake when player is staggered
- **Hit Stop** — 2–4 frame freeze on heavy melee hits (player attacks)
- **Kill Slow-Mo** — Optional brief slow-motion on final kill of a wave

---

## 16. Technical Architecture

### 16.1 Project Structure

```
/Source/CombatAI/
├── Core/
│   ├── CombatAIGameMode.h/.cpp
│   ├── CombatAIGameState.h/.cpp
│   └── CombatAIPlayerController.h/.cpp
│
├── Player/
│   ├── PlayerCharacter.h/.cpp
│   ├── PlayerCombatComponent.h/.cpp
│   ├── PlayerStatsComponent.h/.cpp
│   └── PlayerAnimInstance.h/.cpp
│
├── AI/
│   ├── Core/
│   │   ├── AICharacterBase.h/.cpp         // Base enemy class
│   │   ├── AIControllerBase.h/.cpp        // Base AI controller
│   │   └── CombatAIPerceptionComponent.h/.cpp   // Custom perception
│   │
│   ├── UtilityAI/
│   │   ├── UtilityAIComponent.h/.cpp      // Main Utility AI brain
│   │   ├── UtilityAction.h/.cpp           // Base action class
│   │   ├── UtilityConsideration.h/.cpp    // Base consideration
│   │   ├── ResponseCurve.h/.cpp           // Curve evaluation
│   │   └── Actions/                       // Specific action implementations
│   │       ├── Action_Chase.h/.cpp
│   │       ├── Action_MeleeAttack.h/.cpp
│   │       ├── Action_AimedShot.h/.cpp
│   │       ├── Action_SeekCover.h/.cpp
│   │       ├── Action_Retreat.h/.cpp
│   │       └── ...
│   │
│   ├── BehaviorTree/
│   │   ├── Tasks/                         // BT task nodes
│   │   ├── Decorators/                    // BT decorator nodes
│   │   └── Services/                      // BT service nodes
│   │
│   ├── Coordination/
│   │   ├── SquadCoordinator.h/.cpp        // Group AI manager
│   │   ├── AttackTokenSystem.h/.cpp       // Token management
│   │   ├── FlankingSystem.h/.cpp          // Sector-based flanking
│   │   └── CoverSystem.h/.cpp            // Cover evaluation
│   │
│   ├── Archetypes/
│   │   ├── RusherCharacter.h/.cpp
│   │   ├── WatcherCharacter.h/.cpp
│   │   └── WardenCharacter.h/.cpp
│   │
│   └── Debug/
│       ├── AIDebugHUD.h/.cpp              // Debug overlay
│       ├── AIDebugComponent.h/.cpp        // Per-enemy debug data
│       └── AIDebugCommands.h/.cpp         // Console commands
│
├── Combat/
│   ├── CombatComponent.h/.cpp             // Shared combat interface
│   ├── DamageSystem.h/.cpp                // Damage calculation
│   ├── StaggerSystem.h/.cpp               // Stagger mechanics
│   └── HitDetection.h/.cpp               // Collision-based hit detection
│
├── Data/
│   ├── EnemyProfileData.h/.cpp            // Data asset definitions
│   ├── UtilityActionData.h/.cpp
│   ├── SquadProfileData.h/.cpp
│   ├── EncounterData.h/.cpp
│   ├── DifficultyProfileData.h/.cpp
│   └── ResponseCurveData.h/.cpp
│
├── Encounter/
│   ├── EncounterManager.h/.cpp            // Wave spawning & progression
│   └── SpawnPoint.h/.cpp                  // Spawn location actors
│
├── Camera/
│   ├── CombatCameraComponent.h/.cpp       // Third-person camera
│   └── CameraEffects.h/.cpp              // Shake, hitlag, slow-mo
│
└── UI/
    ├── CombatHUD.h/.cpp                   // Main HUD widget
    ├── EnemyHealthBar.h/.cpp              // Enemy overhead UI
    ├── WaveIndicator.h/.cpp               // Wave progress display
    └── DebugPanel.h/.cpp                  // Runtime tuning panel
```

### 16.2 Key Technical Decisions

| Decision | Choice | Rationale |
|---|---|---|
| **Language** | C++ with Blueprint exposure | Performance for AI; designer accessibility via BP |
| **AI Framework** | Custom Utility AI + UE5 Behavior Trees | Hybrid gives best of both worlds |
| **Pathfinding** | UE5 NavMesh (Recast) | Industry standard; supports dynamic obstacles |
| **Perception** | Custom (not UE5 AIPerception) | More control over threat scoring and memory |
| **Hit Detection** | Animation-driven collision volumes | Precise, frame-accurate hit windows |
| **Animation** | Animation Blueprints + Montages | Blend trees for locomotion; montages for actions |
| **UI** | UMG (Unreal Motion Graphics) | Native UE5; designer-friendly |
| **Debug Tools** | ImGui integration + UE5 Debug Drawing | Lightweight, runtime togglable |
| **Version Control** | Git + Git LFS | Industry standard; LFS for binary assets |

### 16.3 Performance Targets

| Metric | Target | Notes |
|---|---|---|
| Frame Rate | 60 FPS (stable) | On mid-range PC hardware |
| AI Tick Rate | 10 Hz (perception), 5 Hz (utility) | Staggered across enemies |
| Max Active Enemies | 12 | With full AI, VFX, and animations |
| NavMesh Updates | < 1ms per frame | Minimal dynamic obstacles |
| Memory (AI per enemy) | < 2 KB | Data-driven; shared assets |

### 16.4 AI Optimization Strategies

- **Staggered ticking**: Not all enemies evaluate on the same frame; spread across multiple frames
- **LOD for AI**: Distant/off-screen enemies reduce tick rate and disable perception raycasts
- **Spatial hashing**: For quick neighbor queries (who's near who)
- **Object pooling**: Enemies are pooled and recycled between waves
- **Async pathfinding**: Navigation queries run on worker threads

---

## 17. Development Roadmap

### Phase 1: Foundation (Weeks 1–4)

| Week | Focus | Deliverables |
|---|---|---|
| 1 | Project Setup & Player | UE5 project, player character movement, camera |
| 2 | Player Combat | Melee combo, dodge, ranged attack, animation setup |
| 3 | Core AI Framework | AI base classes, perception system, basic BT |
| 4 | Utility AI System | Utility evaluator, response curves, action scoring |

**Milestone:** Player can move, attack, and fight a basic AI enemy that chases and attacks.

### Phase 2: Enemy Archetypes (Weeks 5–8)

| Week | Focus | Deliverables |
|---|---|---|
| 5 | Rusher AI | Full Rusher behavior: chase, combo, lunge, retreat |
| 6 | Watcher AI | Aimed shot, cover seeking, repositioning, suppression |
| 7 | Warden AI | Shield blocking, charge, slam, weak point, bodyguard |
| 8 | Data Assets & Tuning | All enemy profiles as data assets; initial parameter tuning |

**Milestone:** All three archetypes functional with unique behaviors and data-driven stats.

### Phase 3: Group Coordination (Weeks 9–11)

| Week | Focus | Deliverables |
|---|---|---|
| 9 | Squad Coordinator | Attack tokens, flanking sectors, aggro management |
| 10 | Tactical Behaviors | Cover system, combined tactics, situation recognition |
| 11 | Encounter System | Wave spawning, progression, encounter data assets |

**Milestone:** Enemies coordinate as squads; 5 waves playable.

### Phase 4: Polish & Documentation (Weeks 12–16)

| Week | Focus | Deliverables |
|---|---|---|
| 12 | Debug Tools | Full debug HUD, console commands, visual overlays |
| 13 | Combat Feel | Hitlag, screen shake, VFX, audio integration |
| 14 | UI & Juice | HUD, wave transitions, kill effects, menus |
| 15 | Tuning Pass | Difficulty balancing, playtesting, parameter refinement |
| 16 | Documentation & Packaging | Technical docs, video breakdown, portfolio prep |

**Milestone:** Polished, portfolio-ready demo with full documentation.

### Contingency Buffer
- Weeks 1–11 include **2 buffer days per phase** for unexpected issues
- Phase 4 can be compressed if earlier phases run long (polish is scalable)
- Core AI systems (Phases 1–3) are the non-negotiable priority

---

## 18. Quality Bar & Polish Targets

### Must-Have (Ship Blockers)
- [ ] All 3 enemy archetypes with distinct, readable behaviors
- [ ] Utility AI producing intelligent, context-appropriate decisions
- [ ] Squad coordination visible in gameplay (flanking, token management)
- [ ] Combat feels responsive (< 100ms input-to-action)
- [ ] No AI getting stuck on geometry or in degenerate states
- [ ] Data assets for all tunable parameters (no magic numbers in code)
- [ ] Debug visualization for all major AI systems
- [ ] 60 FPS with 9+ active enemies

### Should-Have (Polish)
- [ ] Hit stop, screen shake, and impact VFX on all attacks
- [ ] Attack telegraph VFX clearly readable for all enemy attacks
- [ ] Audio feedback for all combat interactions
- [ ] Wave transition screens with stats
- [ ] In-game parameter tweaking panel

### Nice-to-Have (Stretch)
- [ ] Dynamic difficulty system
- [ ] Slow-motion kill cam on last enemy
- [ ] Environmental destruction (breakable cover)
- [ ] Elite enemy variants (data asset reskins with modified parameters)
- [ ] Replay system for AI behavior review

---

## 19. Documentation & Collaboration Philosophy

### 19.1 Documentation Deliverables

| Document | Description | Audience |
|---|---|---|
| **This Game Overview** | Full design vision and technical specification | All team members |
| **AI Architecture Doc** | Deep dive on Utility AI + BT hybrid system | Engineers |
| **Designer Tuning Guide** | How to modify enemy behaviors via Data Assets | Designers |
| **Debug Tools Reference** | All debug commands, overlays, and workflows | QA & Designers |
| **Code Architecture Doc** | Module dependencies, class hierarchy, data flow | Engineers |
| **Postmortem / Breakdown** | Development learnings and technical decisions explained | Portfolio reviewers |

### 19.2 Code Documentation Standards
- **Every public function** has a doc comment explaining purpose, parameters, and return value
- **Every class** has a header comment explaining its role in the system
- **Complex algorithms** (utility scoring, flanking assignment) have inline comments explaining logic
- **Data Assets** have tooltip descriptions on every exposed property
- **Blueprint nodes** have descriptive names and category organization

### 19.3 Collaboration Mindset

This project is built as if working on a team, even as a solo developer:

- **Clear separation of concerns**: AI logic doesn't reach into player code; communication happens through interfaces and events
- **Designer-first parameter exposure**: Nothing requires code changes to tweak gameplay feel
- **Readable code over clever code**: Prioritize clarity; future-you (or a teammate) should understand any function in 30 seconds
- **Debug tools are not optional**: If a system isn't debuggable, it's not shippable. Every AI system has visual debug support
- **Data-driven everything**: Enemy stats, utility curves, encounter compositions, difficulty settings — all in editable assets
- **Blueprint-friendly interfaces**: Key events and queries are exposed to Blueprint so designers can extend behaviors without engineering support
- **Consistent naming conventions**: `UE5 standard` — `PascalCase` for types and functions, `bPrefixed` for bools, `E` prefix for enums, `F` prefix for structs

### 19.4 Why This Demonstrates Collaboration

Retro Studios (and Nintendo broadly) values engineers who:

1. **Build systems others can use** — The Data Asset architecture and Blueprint exposure demonstrate this
2. **Communicate clearly** — The documentation suite shows technical writing ability
3. **Think about the whole team** — Debug tools, designer tuning panels, and event-driven architecture show awareness of cross-discipline workflows
4. **Iterate quickly** — Runtime tweaking, hot-reload support, and modular encounter design enable fast iteration loops
5. **Prioritize quality** — The polish targets and quality bar show attention to the final player experience

---

## 20. References & Inspirations

### Game References

| Game | Relevance |
|---|---|
| **Metroid Prime (Series)** | Lock-on combat, enemy variety, polished feel — Retro Studios' flagship |
| **Halo (Bungie/343)** | Gold standard for squad AI, tactical enemy behaviors, attack tokens |
| **The Last of Us (Naughty Dog)** | Companion and enemy AI coordination, cover usage, flanking |
| **God of War (2018)** | Third-person melee combat feel, enemy readability, stagger systems |
| **Batman: Arkham Series** | Group combat management, attack token system, enemy archetypes |
| **Dark Souls** | Attack telegraphing, punish windows, combat weight |
| **Doom Eternal** | Enemy role diversity, resource management in combat, aggression pacing |

### Technical References

| Resource | Topic |
|---|---|
| **"Building a Better Centaur" (GDC, Bungie)** | Halo AI architecture and design philosophy |
| **"The AI of The Last of Us" (GDC, Naughty Dog)** | Squad AI and contextual behaviors |
| **"Utility AI" (Dave Mark, GDC)** | Infinite Axis Utility System theory and implementation |
| **"Behavior Trees in Game AI" (Alex Champandard)** | BT fundamentals and advanced patterns |
| **"AI-Driven Dynamic Dialog" (Elan Ruskin, GDC)** | Data-driven AI systems philosophy |
| **UE5 AI Documentation** | Behavior Trees, EQS, NavMesh, AI Perception |

### Art & Audio References (Placeholder Sources)

| Asset Type | Suggested Source | Notes |
|---|---|---|
| Character Models | Mixamo / Marketplace | Placeholder humanoid meshes |
| Animations | Mixamo / Motion capture packs | Combat animation sets |
| VFX | Niagara Marketplace packs | Hit sparks, projectiles, AOE effects |
| Audio | Freesound.org / SONNISS GDC packs | Combat SFX, UI sounds |
| Environment | UE5 Marketplace modular kits | Sci-fi industrial assets |

---

## Appendix A: Glossary

| Term | Definition |
|---|---|
| **Aggro** | Which enemy the player is currently focused on / attacking |
| **Attack Token** | Permission token required to initiate an attack; limits simultaneous attacks |
| **Behavior Tree (BT)** | Tree-structured AI execution framework; handles *how* actions are performed |
| **Consideration** | A single input factor in a Utility AI evaluation (e.g., distance to player) |
| **Data Asset** | UE5 asset type for storing structured data; editable in the editor without code |
| **EQS** | Environment Query System — UE5's spatial query tool for AI |
| **Hitlag** | Brief frame pause on impact for combat weight |
| **I-Frames** | Invincibility frames during dodge roll |
| **NavMesh** | Navigation mesh; defines walkable area for AI pathfinding |
| **Response Curve** | Mathematical curve mapping an input value (0–1) to a score (0–1) |
| **Stagger** | Temporary stunned state from accumulated damage |
| **Utility AI** | AI decision-making system that scores actions based on weighted considerations |

---

## Appendix B: Risk Assessment

| Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|
| Scope creep (too many features) | High | High | Strict scope boundaries; In/Out table; cut list ready |
| AI getting stuck on geometry | Medium | High | Extensive NavMesh testing; stuck detection + teleport fallback |
| Combat feeling unresponsive | Medium | Very High | Input buffering; animation canceling; frequent playtesting |
| AI behaviors looking scripted | Medium | Medium | Utility AI randomization; varied response curves; parameter noise |
| Performance with 9+ enemies | Low | High | Staggered ticking; AI LOD; profile early and often |
| Animation quality (solo dev) | High | Medium | Use marketplace/Mixamo assets; focus on timing over fidelity |
| Difficulty balancing | High | Medium | Data-driven difficulty; playtest frequently; get external feedback |

---

*Document Version: 1.0*
*Last Updated: February 10, 2026*
*Author: Portfolio Project — CombatAI*
