// main.cpp — Teensy 4.1 composition root.
//
// This is the ONLY layer that knows it is running on a Teensy. It owns
// Arduino.h, constructs concrete HAL implementations (src/drivers/,
// added as sensors are brought up), wires them into FlightController,
// and runs the loop. Nothing here contains flight logic — if a line in
// this file makes a flight decision, it is in the wrong file.
//
// Week 1 milestone scope: heartbeat blink + a disciplined 100 Hz tick
// printed over USB serial. Sensor drivers slot in during Phase 1.

#include <Arduino.h>

constexpr uint32_t LOOP_PERIOD_US = 10000;  // 100 Hz
elapsedMicros since_tick;
uint32_t tick_count = 0;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    // Don't block waiting for Serial: the flight computer must boot and
    // run with no USB host attached (it will, on the pad).
}

void loop() {
    if (since_tick >= LOOP_PERIOD_US) {
        since_tick -= LOOP_PERIOD_US;  // subtract, don't reset: no drift
        ++tick_count;

        // 1 Hz heartbeat derived from the 100 Hz tick — proves the
        // scheduler is honest, not just that the LED works.
        if (tick_count % 100 == 0) {
            digitalToggleFast(LED_BUILTIN);
            Serial.printf("tick=%lu t=%lu us\n",
                          tick_count, static_cast<unsigned long>(micros()));
        }

        // Phase 1: FlightController::step() gets called here once the
        // first driver (BMP390) exists.
    }
}
