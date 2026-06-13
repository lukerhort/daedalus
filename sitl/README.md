# SITL (Software-In-The-Loop) harness — built in Phase 2

Desktop build of `lib/flight` + `lib/hal` with simulated sensors.

- `sim/`   — C++ sensor simulators implementing the HAL interfaces,
             fed by OpenRocket trajectory CSV exports, plus a desktop
             `main.cpp` and plain Makefile (deliberately NOT a
             PlatformIO env — the desktop toolchain must never see
             Arduino headers, which is what proves the HAL boundary).
- `tests/` — Python `pytest` regression suite. Each test launches the
             SITL binary against a scenario and asserts on its output
             (state transitions, apogee call timing, etc.).

Phase 2 exit: green GitHub Actions badge running this suite on every commit.
