// flight.h — top-level flight logic for Daedalus.
//
// This file and everything else in lib/flight/ is PLATFORM-FREE:
// standard C++17 only, hardware reached exclusively through the HAL
// interfaces. This is the code that compiles unchanged for both the
// Teensy and the SITL desktop harness.
//
// Dependency injection happens at the composition root (src/main.cpp
// for flight, sitl/sim/main.cpp for simulation): whoever owns main()
// constructs the concrete sensors and hands them in by reference.
// FlightController never knows or cares which world it is in.

#pragma once
#include "../hal/hal.h"

namespace daedalus {

// Flight state machine states, per project brief Phase 3.
enum class FlightState : uint8_t {
    PAD,
    ARMED,
    BOOST,
    COAST,
    APOGEE,
    DESCENT,
    LANDED,
};

const char* to_string(FlightState s);

class FlightController {
public:
    FlightController(IBarometer& baro, IImu& imu, IGps& gps,
                     IClock& clock, ILogger& logger);

    // Called at 100 Hz by the owner of the loop. Reads sensors, will
    // later run the estimator and state machine (Phase 3), and logs.
    void step();

    FlightState state() const { return state_; }

private:
    IBarometer& baro_;
    IImu&       imu_;
    IGps&       gps_;
    IClock&     clock_;
    ILogger&    logger_;

    FlightState state_ = FlightState::PAD;

    // Phase 3: complementary-filter estimator state lands here.
};

}  // namespace daedalus
