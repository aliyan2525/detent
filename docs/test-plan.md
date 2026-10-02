# Test Plan

All tests are **planned**. Record results in `design-log.md` when done.

## Stage 0: before power
- [ ] Visual inspection under magnification (solder bridges, orientation)
- [ ] Resistance check between 5V/GND and 3V3/GND (no short)

## Stage 1: power
- [ ] Apply USB power with a current-limited source or USB meter
- [ ] Measure 3V3 rail and ripple
- [ ] Record idle current

## Stage 2: MCU and USB
- [ ] Flash a blink program over USB
- [ ] Enumerate as HID on a host

## Stage 3: sensors and display
- [ ] Read raw encoder angle and check monotonic output over a full turn
- [ ] Check angle noise with the knob held still
- [ ] Draw to the display

## Stage 4: motor
- [ ] Spin open-loop at low current
- [ ] Run FOC with sensor alignment
- [ ] Verify current stays below the firmware limit

## Stage 5: haptics
- [ ] Detent mode: count clicks per revolution matches configuration
- [ ] Subjective feel test and notes
- [ ] Scrub mode and bounded mode

## Pass criteria (targets, to be revised)
- Total USB current below 500 mA at all times
- No reset or brown-out during motor start
- Haptic loop at or above 1 kHz
