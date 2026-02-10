# Sprint 3 Test Report — Utility AI + Rusher

> **Sprint:** 3 (Weeks 5–6)
> **Date:** February 10, 2026
> **Status:** COMPLETE

---

## Stories Delivered

| ID | Story | Status |
|---|---|---|
| S-023 | Utility AI Component — action evaluation pipeline | DONE |
| S-024 | Response Curve system (Linear, Quadratic, Logistic, Step, Inverse) | DONE |
| S-025 | AI State Machine (Idle → Alert → Combat → Searching → Dead) | DONE |
| S-026 | Rusher character class with stats | DONE |
| S-027 | Rusher Utility Actions (all 7) | DONE |
| S-028 | Rusher Behavior Tree subtrees | DONE |
| S-029 | Rusher attack patterns (Slash, Lunge, Circling) | DONE |

---

## Test Results

### E-04: Core AI Framework (Utility AI)

| TC ID | Test Case | Result | Notes |
|---|---|---|---|
| TC-043 | Utility AI action selection | PASS | Multiplicative scoring selects appropriate action per context |
| TC-044 | Response curve — Linear | PASS | Input 0.5, Slope 1.0 → Output 0.5 |
| TC-045 | Response curve — Logistic | PASS | S-curve with Steepness=10, Midpoint=0.5 verified |
| TC-046 | Response curve — Step | PASS | Below threshold → 0, above → 1 |
| TC-047 | AI state: Idle → Alert | PASS | SetAIState fires OnAIStateChanged delegate |
| TC-048 | AI state: Alert → Combat | PASS | OnEnterCombat() virtual hook called |
| TC-049 | AI state: Combat → Searching | PASS | State transition via perception timeout |
| TC-050 | AI state: Searching → Idle | PASS | Search timeout triggers return to Idle |

### E-05: Rusher Tests

| TC ID | Test Case | Result | Notes |
|---|---|---|---|
| TC-052 | Rusher chase behavior | PASS | Chase action scores highest when far + visible |
| TC-053 | Rusher melee combo | PASS | SlashCombo: 12 dmg × 2 hits; HitDetection enabled |
| TC-054 | Rusher lunge | PASS | Lunge at 300–600 cm; 20 dmg; faces player first |
| TC-055 | Rusher circle strafe | PASS | CircleStrafe selected when close + ally attacking |
| TC-056 | Rusher retreat | PASS | Retreat scores high when health < 25% (inverse curve on health) |
| TC-057 | Rusher attack telegraph | PASS | Montage wind-up provides visual telegraph |
| TC-058 | Rusher parry-able | PASS | Attacks pass through CombatComponent; parry interacts normally |
| TC-059 | Rusher death | PASS | HandleDeath → Dead state, ragdoll, cleanup timer |
| TC-060 | Rusher stagger | PASS | 20+ damage triggers stagger (threshold = 20) |

---

## Code Metrics

| Metric | Value |
|---|---|
| New files | 10 |
| New LoC | ~1,200 |
| New classes/structs | 6 (UtilityAIComp, UtilityAction, UtilityConsideration, ResponseCurve, FUtilityContext, RusherCharacter) |
| Utility considerations defined | 26 input types |
| Rusher actions configured | 7 (Chase, Melee, Lunge, Circle, Flank, Retreat, Investigate) |
| **Cumulative total** | **~4,400 LoC across 45 files** |

## Architecture Validation

- [x] Utility AI selects WHAT to do; BT handles HOW (hybrid confirmed)
- [x] Response curves are data-driven structs with 5 curve types
- [x] Considerations normalize arbitrary world-state to [0,1] then apply curves
- [x] Multiplicative scoring with early-out on zero
- [x] Decision rate staggered across enemies (randomized initial offset)
- [x] Attack token gating: bRequiresAttackToken blocks scoring without token
- [x] Score noise (0.05) prevents repetitive behavior patterns
- [x] Rusher configured with all 7 GDD actions via ConfigureUtilityActions()

## Sprint 3 Velocity: 30 SP delivered
