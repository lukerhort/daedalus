// flight.cpp — see flight.h. Phase 1 scope: read sensors, log a CSV
// record per step. State machine and estimator arrive in Phase 3.

#include "flight.h"
#include <cstdio>  // snprintf — available on both Teensy and desktop

namespace daedalus {

const char* to_string(FlightState s) {
    switch (s) {
        case FlightState::PAD:     return "PAD";
        case FlightState::ARMED:   return "ARMED";
        case FlightState::BOOST:   return "BOOST";
        case FlightState::COAST:   return "COAST";
        case FlightState::APOGEE:  return "APOGEE";
        case FlightState::DESCENT: return "DESCENT";
        case FlightState::LANDED:  return "LANDED";
    }
    return "?";
}

FlightController::FlightController(IBarometer& baro, IImu& imu, IGps& gps,
                                   IClock& clock, ILogger& logger)
    : baro_(baro), imu_(imu), gps_(gps), clock_(clock), logger_(logger) {}

void FlightController::step() {
    BaroSample baro{};
    ImuSample  imu{};
    baro_.read(baro);
    imu_.read(imu);
    // GPS is polled at full rate but only produces new fixes at ~10 Hz;
    // the driver returns its latest sample.
    GpsSample gps{};
    gps_.read(gps);

    char line[160];
    snprintf(line, sizeof(line), "%lu,%s,%.2f,%.2f,%.3f,%.3f,%.3f,%d",
             static_cast<unsigned long>(clock_.now_us()),
             to_string(state_),
             static_cast<double>(baro.pressure_pa),
             static_cast<double>(baro.temperature_c),
             static_cast<double>(imu.accel_mps2[0]),
             static_cast<double>(imu.accel_mps2[1]),
             static_cast<double>(imu.accel_mps2[2]),
             gps.fix ? gps.num_sats : 0);
    logger_.write_line(line);
}

}  // namespace daedalus
