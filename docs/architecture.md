# Architecture

Everything here is a **design intent**, not a verified result. Pin assignments must be checked against the datasheets during schematic capture.

## 1. Subsystems

| Subsystem | Part | Interface | Role |
|---|---|---|---|
| MCU | ESP32-S3-WROOM-1-N16 | n/a | Control loop, display, USB HID |
| Motor driver | TMC6300-LA | 6 PWM inputs | 3-phase gate-driven output stage |
| Motor | 2804 gimbal BLDC | 3 phases | Produces haptic torque |
| Angle sensor | MT6701 | SSI (SPI-compatible, read-only) | 14-bit absolute angle |
| Display | GC9A01 240x240 | SPI | Mode and status UI |
| Host link | Native USB | USB 2.0 FS | HID keyboard and consumer control |

## 2. Provisional pin plan (ESP32-S3)

Strapping pins (GPIO0, 3, 45, 46) and USB pins (GPIO19, 20) are avoided for general I/O.

| Signal | GPIO | Notes |
|---|---|---|
| PWM UH / UL | 4 / 5 | MCPWM, complementary with dead time |
| PWM VH / VL | 6 / 7 | MCPWM |
| PWM WH / WL | 15 / 16 | MCPWM |
| Encoder CLK / DATA / CSN | 12 / 13 / 10 | Dedicated SPI host |
| LCD SCK / MOSI | 18 / 17 | Second SPI host |
| LCD CS / DC / RST / BL | 8 / 21 / 14 / 2 | BL via PWM for dimming |
| USB D- / D+ | 19 / 20 | Native USB |

## 3. Power

```
VBUS 5V -> TVS -> resettable fuse -> bulk cap -> 3.3V LDO -> MCU, encoder, LCD logic
                                   \-> TMC6300 VM (motor supply, 5V)
```

### Budget (estimates, to be measured)

| Load | Estimate |
|---|---|
| ESP32-S3 (no radio) | ~80 mA |
| LCD with backlight | ~50 mA |
| Encoder | ~15 mA |
| **Logic subtotal** | **~145 mA** |
| Remaining for motor under 500 mA USB limit | **~350 mA, enforced in firmware** |

The LDO must be rated for comfortable headroom above the logic load (at least 500 mA, ideally 1 A). Add inrush limiting or enough bulk capacitance to avoid USB brown-out during motor start.

## 4. PCB plan

- 4 layers: Signal / GND / 3V3 and motor power / Signal
- Round outline sized to the enclosure, with mounting holes on a defined bolt circle
- Solid GND plane directly under the encoder and its traces
- Motor phase traces kept short and away from SSI and SPI lines
- Decoupling placed at each IC supply pin, with a bulk capacitor near the driver
- USB differential pair routed with controlled impedance per the fabricator stackup
- Test points on 3V3, 5V, GND, motor phases and SSI lines
- Fabrication target: JLCPCB or PCBWay standard 4-layer process; confirm minimum trace, clearance and via sizes against their capabilities before routing

## 5. Mechanical plan

- Motor mounted to a printed back plate; board and display stacked in front
- Magnet fixed to the rear of the motor shaft; encoder on the board at a gap set by standoffs (verify gap against the MT6701 datasheet)
- Knob press-fit or set-screwed onto the shaft
- Shell in two printed parts joined with M2/M3 screws
- Cable exit for USB-C on the rear

## 6. Firmware architecture

| Layer | Responsibility |
|---|---|
| `haptics.h` | Pure functions: torque as a function of angle and velocity (host-testable) |
| Motor and sensor drivers | SimpleFOC-based FOC loop (planned) |
| UI | Display of current mode |
| HID | Maps knob events to host shortcuts |

The control loop must stay deterministic. Display updates and USB handling run at lower priority and must never block the haptic loop.

## 7. Open questions

1. Is gimbal motor torque at 5V and the available current enough for a convincing detent?
2. Should the motor supply be boosted above 5V for more torque (TMC6300 supply limit must be respected)?
3. Is the MT6701 SSI interface or I2C the better fit for the update rate?
