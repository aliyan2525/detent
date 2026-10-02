// Pure haptic-feel functions. No hardware dependencies, so they can be
// unit-tested on a host machine.
#pragma once
#include <math.h>

namespace haptics {

enum class Mode { Detent, Scrub, Bounded, Free };

struct Config {
  float detent_angle_rad = 0.2617994f; // 15 degrees between clicks
  float stiffness        = 2.0f;       // torque per radian of error (normalised)
  float deadband_rad     = 0.01f;      // no torque very close to a detent centre
  float damping          = 0.02f;      // torque per rad/s for Scrub
  float min_angle_rad    = 0.0f;       // Bounded mode limits
  float max_angle_rad    = 6.2831853f;
  float wall_stiffness   = 6.0f;
  float max_torque       = 0.5f;       // normalised, clamped for safety
};

inline float clampf(float v, float lo, float hi) {
  return v < lo ? lo : (v > hi ? hi : v);
}

// Returns normalised torque in [-max_torque, +max_torque].
inline float torque(Mode mode, float angle_rad, float velocity_rad_s, const Config& c) {
  float t = 0.0f;
  switch (mode) {
    case Mode::Detent: {
      float nearest = roundf(angle_rad / c.detent_angle_rad) * c.detent_angle_rad;
      float err = angle_rad - nearest;
      if (fabsf(err) > c.deadband_rad) t = -c.stiffness * err;
      break;
    }
    case Mode::Scrub:
      t = -c.damping * velocity_rad_s;
      break;
    case Mode::Bounded:
      if (angle_rad < c.min_angle_rad)      t = c.wall_stiffness * (c.min_angle_rad - angle_rad);
      else if (angle_rad > c.max_angle_rad) t = -c.wall_stiffness * (angle_rad - c.max_angle_rad);
      break;
    case Mode::Free:
      t = 0.0f;
      break;
  }
  return clampf(t, -c.max_torque, c.max_torque);
}

} // namespace haptics
