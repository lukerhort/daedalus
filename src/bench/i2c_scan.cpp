// bench/i2c_scan.cpp — I2C bus scanner. Bench tool, not flight code.
// Pings every address; prints which ones answer (ACK). Confirms a sensor
// is alive and at what address before writing its driver.
#include <Arduino.h>
#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}   // bench tool: OK to wait for monitor
    Serial.println("I2C scan, addresses 0x01-0x7E");
}

void loop() {
    uint8_t found = 0;
    for (uint8_t addr = 1; addr < 127; ++addr) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {   // 0 = device acknowledged
            Serial.printf("  found 0x%02X\n", addr);
            ++found;
        }
    }
    Serial.println(found ? "---" : "  none found");
    delay(2000);
}