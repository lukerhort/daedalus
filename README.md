# Daedalus

Custom flight computer for a Level 1 high-power rocket, built together
with the test infrastructure that validates it: a SITL
(Software-In-The-Loop) regression suite running in CI on every commit,
and a HITL (Hardware-In-The-Loop) bench with automated fault injection.

## Architecture

All flight logic lives behind a HAL (Hardware Abstraction Layer).
The same state machine and estimator compile for two targets:

```
                lib/flight/          platform-free flight logic
                     |
                lib/hal/hal.h        abstract sensor/actuator interfaces
                /           \
   src/ (Teensy 4.1)     sitl/ (desktop)
   real drivers,         simulated sensors fed by
   PlatformIO build      OpenRocket trajectories, g++ + pytest
```

Rule: no flight logic in driver code, no Arduino.h above the HAL line.

## Layout

| Path | Contents |
|---|---|
| `lib/hal/` | HAL interfaces + sample structs (platform-free) |
| `lib/flight/` | State machine, estimator, logging format (platform-free) |
| `src/` | Teensy composition root + real drivers |
| `sitl/` | Desktop sim + pytest regression suite (Phase 2) |
| `tools/` | Log plotting / analysis scripts |
| `docs/` | Debugging log highlights, architecture notes |

## Status

Phase 1 — sensor bring-up on the bench.
