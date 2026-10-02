// DETENT firmware entry point.
// STATUS: skeleton. The hardware layer (FOC, encoder, display, USB HID) is not
// implemented yet and has NOT been run on hardware.
#include <Arduino.h>
#include "haptics.h"

static haptics::Config cfg;
static haptics::Mode mode = haptics::Mode::Detent;

// TODO: replace with the MT6701 SSI read.
static float readAngleRad()    { return 0.0f; }
// TODO: replace with a filtered velocity estimate.
static float readVelocityRadS(){ return 0.0f; }
// TODO: replace with SimpleFOC torque command through the TMC6300.
static void  applyTorque(float /*t*/) {}

void setup() {
  Serial.begin(115200);
  // TODO: init encoder, driver, FOC alignment, display, USB HID.
}

void loop() {
  float angle = readAngleRad();
  float vel   = readVelocityRadS();
  applyTorque(haptics::torque(mode, angle, vel, cfg));
  // Target: this loop runs at 1 kHz or faster; UI and HID run at lower priority.
}
