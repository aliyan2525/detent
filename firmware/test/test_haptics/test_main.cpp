#include <unity.h>
#include "haptics.h"
using namespace haptics;

void setUp() {}
void tearDown() {}

void test_detent_zero_at_centre() {
  Config c;
  TEST_ASSERT_EQUAL_FLOAT(0.0f, torque(Mode::Detent, 0.0f, 0.0f, c));
}

void test_detent_restoring_sign() {
  Config c;
  float just_above = c.detent_angle_rad * 0.2f;
  TEST_ASSERT_TRUE(torque(Mode::Detent, just_above, 0.0f, c) < 0.0f);
  TEST_ASSERT_TRUE(torque(Mode::Detent, -just_above, 0.0f, c) > 0.0f);
}

void test_torque_is_clamped() {
  Config c;
  c.stiffness = 1000.0f;
  float t = torque(Mode::Detent, c.detent_angle_rad * 0.4f, 0.0f, c);
  TEST_ASSERT_TRUE(fabsf(t) <= c.max_torque + 1e-6f);
}

void test_scrub_opposes_velocity() {
  Config c;
  TEST_ASSERT_TRUE(torque(Mode::Scrub, 0.0f, 10.0f, c) < 0.0f);
  TEST_ASSERT_TRUE(torque(Mode::Scrub, 0.0f, -10.0f, c) > 0.0f);
}

void test_bounded_walls() {
  Config c;
  TEST_ASSERT_TRUE(torque(Mode::Bounded, c.min_angle_rad - 0.1f, 0.0f, c) > 0.0f);
  TEST_ASSERT_TRUE(torque(Mode::Bounded, c.max_angle_rad + 0.1f, 0.0f, c) < 0.0f);
  TEST_ASSERT_EQUAL_FLOAT(0.0f, torque(Mode::Bounded, 3.0f, 0.0f, c));
}

void test_free_is_zero() {
  Config c;
  TEST_ASSERT_EQUAL_FLOAT(0.0f, torque(Mode::Free, 1.0f, 5.0f, c));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_detent_zero_at_centre);
  RUN_TEST(test_detent_restoring_sign);
  RUN_TEST(test_torque_is_clamped);
  RUN_TEST(test_scrub_opposes_velocity);
  RUN_TEST(test_bounded_walls);
  RUN_TEST(test_free_is_zero);
  return UNITY_END();
}
