// hal.h — Hardware Abstraction Layer (HAL) interfaces for Daedalus.
//
// THE RULE: everything in lib/flight/ talks to hardware ONLY through
// these interfaces. No file in lib/hal/ or lib/flight/ may include
// Arduino.h or any driver library. The Teensy build implements these
// interfaces with real drivers (src/drivers/); the SITL build
// implements them with simulated sensors (sitl/sim/). Same flight
// logic, two worlds.
//
// Design choice: C++ abstract interfaces (pure virtual) rather than
// #ifdef platform switches or templates.
//   - vs #ifdef: flight logic stays a single readable code path; the
//     compiler type-checks both implementations all the time.
//   - vs templates: virtual dispatch costs one pointer indirection per
//     call. At a 100 Hz loop rate on a 600 MHz Cortex-M7 that is
//     measured in nanoseconds — irrelevant. Templates would buy nothing
//     and cost readability.
//   - It also makes fault injection trivial later (Phase 5): wrap any
//     interface in a decorator that drops reads or returns stuck values.

#pragma once
#include <cstddef>
#include <cstdint>

namespace daedalus {

// ---- Sample structs ----------------------------------------------------
// Units are encoded in the field names. A float named "pressure" caused
// a Mars probe crash; a float named "pressure_pa" cannot be misread.

struct BaroSample {
    float    pressure_pa   = 0.0f;  // static pressure, Pascals
    float    temperature_c = 0.0f;  // die temperature, Celsius
    uint32_t t_us          = 0;     // timestamp, microseconds (IClock domain)
    bool     valid         = false; // false => sensor read failed; do not use
};

struct ImuSample {
    float    accel_mps2[3] = {0, 0, 0}; // body-frame accel, m/s^2 (x, y, z)
    float    gyro_rps[3]   = {0, 0, 0}; // body-frame angular rate, rad/s
    float    mag_ut[3]     = {0, 0, 0}; // magnetic field, microtesla
    uint32_t t_us          = 0;
    bool     valid         = false;
};

struct GpsSample {
    double   lat_deg  = 0.0;
    double   lon_deg  = 0.0;
    float    alt_msl_m = 0.0f;   // altitude above mean sea level, meters
    uint8_t  num_sats = 0;
    bool     fix      = false;   // false => position fields are garbage
    uint32_t t_us     = 0;
};

// ---- Hardware interfaces -----------------------------------------------
// Every read() returns a sample with a `valid` flag instead of blocking
// or throwing: the 100 Hz loop must never stall on a sick sensor, and
// "sensor returned garbage" is a first-class state the flight logic is
// required to handle (this is what makes fault injection meaningful).

class IBarometer {
public:
    virtual ~IBarometer() = default;
    virtual bool read(BaroSample& out) = 0;
};

class IImu {
public:
    virtual ~IImu() = default;
    virtual bool read(ImuSample& out) = 0;
};

class IGps {
public:
    virtual ~IGps() = default;
    virtual bool read(GpsSample& out) = 0;
};

class IClock {
public:
    virtual ~IClock() = default;
    virtual uint32_t now_us() = 0;  // monotonic microseconds since boot
};

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual bool write_line(const char* line) = 0;  // one CSV record
    virtual void flush() = 0;
};

class IPyro {  // Phase 5 — declared now so the shape is settled early
public:
    virtual ~IPyro() = default;
    virtual bool continuity(uint8_t channel) = 0;
    virtual void fire(uint8_t channel) = 0;  // no-op unless hardware-armed
};

}  // namespace daedalus
