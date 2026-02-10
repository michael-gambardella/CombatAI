# CombatAI — Third-Person Combat AI System

A UE5 portfolio project demonstrating sophisticated enemy AI for a third-person action game.

## Project Overview

CombatAI is a single-player arena combat experience featuring three distinct enemy archetypes, a hybrid Utility AI + Behavior Tree decision-making system, and coordinated group tactics. Built in **Unreal Engine 5.4** with **C++** and **Blueprints**.

### Key Features

- **Hybrid AI Architecture**: Utility AI selects *what* to do; Behavior Trees execute *how*
- **3 Enemy Archetypes**: Rusher (aggressive melee), Watcher (ranged suppressor), Warden (heavy tank)
- **20 Unique AI Actions** across all archetypes with configurable response curves
- **Squad Coordination**: Attack token system, 8-sector flanking, cover evaluation, aggro tracking
- **6 Tactical Situations**: Squad-level awareness (surrounded, ally down, player low health, etc.)
- **Designer-Friendly**: All parameters exposed as UE5 Data Assets — no code changes needed to tune
- **Custom Perception**: Sight (FOV + LOS), hearing, and threat assessment with memory
- **Full Combat System**: 3-hit combo, heavy charge, dodge with i-frames, parry, ranged, lock-on
- **5-Wave Encounter**: Escalating difficulty with mixed enemy compositions
- **Debug Tools**: Full AI visualization HUD with per-enemy state, scores, and coordination overlay

## Architecture

```
CombatAI/
├── Source/CombatAI/
│   ├── Core/                  GameMode, GameState, PlayerController
│   ├── Player/                PlayerCharacter, Stats, AnimInstance
│   ├── Camera/                CombatCamera with effects
│   ├── Combat/                Shared damage pipeline, hit detection, stagger
│   ├── AI/
│   │   ├── Core/              Base character, controller, perception
│   │   ├── UtilityAI/         Response curves, considerations, actions, evaluator
│   │   ├── Coordination/      Squad coordinator, tokens, flanking, cover
│   │   ├── Archetypes/        Rusher, Watcher, Warden implementations
│   │   └── Debug/             AI visualization HUD
│   ├── Data/                  Data Assets (profiles, encounters, difficulty)
│   ├── Encounter/             Wave manager
│   └── UI/                    HUD, enemy health bars, menus
├── docs/
│   ├── GAME_OVERVIEW.md       Full game design document
│   ├── BACKLOG.md             Epics, stories, sprints, test plan
│   └── tests/                 Per-sprint test reports
└── CombatAI.uproject
```

## Technical Highlights

| System | Approach | Why |
|---|---|---|
| AI Decision-Making | Hybrid Utility AI + BT | Utility handles dynamic scoring; BT handles reliable execution |
| Perception | Custom FOV/LOS/Hearing | Full control over threat assessment and memory |
| Group Tactics | Token + Sector system | Prevents dog-piling; creates cinematic encounters |
| Damage Pipeline | Shared CombatComponent | Player and AI use identical damage interface |
| Data-Driven Design | UE5 Data Assets | Designers tweak without code; hot-reload supported |
| Response Curves | 5 curve types | Fine-tuned AI behavior shaping (Linear, Quadratic, Logistic, Step, Inverse) |

## Development Stats

- **8 sprints** across 16 weeks
- **~8,550 lines** of C++ code
- **78 source files** (24 classes, 8 structs, 8 enums)
- **168 test cases** across 8 test reports — all passing
- **279 story points** delivered

## Building

1. Clone this repository
2. Open `CombatAI.uproject` in Unreal Engine 5.4
3. Build the project (Development Editor configuration)
4. Open the arena level and press Play

## Documentation

- [Game Overview](docs/GAME_OVERVIEW.md) — Complete game design document
- [Backlog](docs/BACKLOG.md) — Full agile backlog with 90 user stories
- [Sprint Test Reports](docs/tests/) — Per-sprint test results and metrics

## License

Portfolio project. All rights reserved.
