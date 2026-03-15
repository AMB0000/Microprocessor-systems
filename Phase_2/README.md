# DemoSat High Altitude Balloon Payload
## Phase 2: Alternative Design
**ENCE 3231 Microprocessors 1 — University of Denver | Winter Quarter 2026 | Ali Behbehani**

---

## 1. Overview

In Phase 1, I found that three of the four sensors in the original DemoSat kit are no longer available to buy. Phase 2 is my replacement design that:

- Uses parts that are still being made and sold
- Does the same job as the original kit
- Stays under the $147 budget and 200g weight limit
- Uses less power than the original
- Uses a custom PCB instead of the original shield

### Parts That Need to Be Replaced

| Original Part | What It Does | Status |
|---|---|---|
| HIH6130 | Humidity + temperature | Discontinued |
| MPL3115A2 | Pressure + altitude | Discontinued |
| MMA8452Q | Accelerometer | Discontinued |
| OpenLog DEV-13712 | Data logger | Replaced by incompatible version |
| DS18B20 | External temperature | Still available |
| Arduino Uno | Main controller | Still available |

---

## 2. My Replacement Choices

I replaced each discontinued part with something that works the same way and is still being made.

| Original Part | My Replacement | Why |
|---|---|---|
| HIH6130 | HIH-9120-021-001 | Same I2C interface, better accuracy |
| MPL3115A2 | DPS310 | I2C, very low power, compact |
| MMA8452Q | ADXL345 | Same I2C interface, more range |
| OpenLog DEV-13712 | OpenLog DEV-13712 | Re-sourced original compatible version |
| DS18B20 | DS18B20 | Unchanged |
| Arduino Uno | Arduino Uno R3 | Unchanged |

---

## 3. Component Details

### HIH-9120-021-001 — Humidity + Temperature
- Replaces: HIH6130
- Where to buy: Digi-Key
- Interface: I2C (address 0x27)
- Accuracy: ±1.7% humidity, ±0.5°C temperature
- Runs on 3.3V
- Cost: $12.00 | Weight: ~2g

### DPS310 — Barometric Pressure
- Replaces: MPL3115A2
- Where to buy: Adafruit #4494
- Interface: I2C (address 0x77)
- Pressure range: 300–1200 hPa
- Very low power: ~1.7µA
- Cost: $6.95 | Weight: ~2g

### ADXL345 — 3-Axis Accelerometer
- Replaces: MMA8452Q
- Where to buy: Adafruit #1231
- Interface: I2C (address 0x53)
- Range: ±2g to ±16g, 13-bit resolution
- Power: ~140µA @ 3.3V
- Cost: $9.95 | Weight: ~2g

### OpenLog DEV-13712 — Data Logger
- Replaces: incompatible OpenLog variant
- Where to buy: SparkFun
- Interface: UART (TX → D1)
- Logs all Serial data directly to microSD card
- Cost: $24.95 | Weight: ~10g

### Parts I Kept

| Part | Reason |
|---|---|
| Arduino Uno | Still works, no reason to change |
| DS18B20 waterproof probe | Still available |
| 4x LEDs + 330Ω resistors | No change needed |
| 2x 9V battery | Enough power for the flight |
| Rocker switch + barrel jack | No change needed |

---

## 4. How the System Works

### Block Diagram
```
                    ┌──────────────┐
                    │  9V Battery  │
                    │     x2       │
                    └──────┬───────┘
                           │ VIN (through switch)
                    ┌──────▼───────┐
                    │  Arduino Uno │
                    │              │
                    │  I2C: A4/A5  │
                    │  1-Wire: D6  │
                    │  UART: D1    │
                    │  GPIO: D2-D5 │
                    └──┬────────┬──┘
                       │        │
              I2C Bus  │        │  UART
                       │        │
         ┌─────────────┤    ┌───▼──────┐
         │             │    │ OpenLog  │
   ┌─────▼───┐  ┌──────▼─┐  │ DEV-13712│
   │HIH-9120 │  │ DPS310 │  └──────────┘
   │0x27     │  │ 0x77   │
   └─────────┘  └────────┘
   ┌─────────┐
   │ ADXL345 │
   │ 0x53    │
   └─────────┘

      1-Wire (D6)
           │
    ┌──────▼──────┐
    │   DS18B20   │
    │  Ext. Temp  │
    └─────────────┘

      GPIO (D2-D5)
           │
    ┌──────▼──────┐
    │  4x LEDs    │
    │  330Ω each  │
    └─────────────┘
```

### Pin Map

| Arduino Pin | Connected To | Protocol |
|---|---|---|
| A4 (SDA) | HIH-9120, DPS310, ADXL345 | I2C |
| A5 (SCL) | HIH-9120, DPS310, ADXL345 | I2C |
| D1 (TX) | OpenLog RXI | UART |
| D6 | DS18B20 data | 1-Wire |
| D2–D5 | LEDs x4 via 330Ω | GPIO |

### I2C Addresses

| Device | Address |
|---|---|
| HIH-9120-021-001 | 0x27 |
| DPS310 | 0x77 |
| ADXL345 | 0x53 |

No address conflicts.

---

## 5. Bill of Materials

| # | Part | Description | Supplier | Price | Qty | Total | Weight |
|---|---|---|---|---|---|---|---|
| 1 | Arduino Uno R3 | Main controller | Arduino | $28.00 | 1 | $28.00 | 25g |
| 2 | HIH-9120-021-001 | Humidity + temp | Digi-Key | $12.00 | 1 | $12.00 | 2g |
| 3 | DPS310 | Pressure | Adafruit #4494 | $6.95 | 1 | $6.95 | 2g |
| 4 | ADXL345 | Accelerometer | Adafruit #1231 | $9.95 | 1 | $9.95 | 2g |
| 5 | DS18B20 | External temp | Adafruit #381 | $4.95 | 1 | $4.95 | 8g |
| 6 | OpenLog DEV-13712 | Data logger | SparkFun | $24.95 | 1 | $24.95 | 10g |
| 7 | 5mm LEDs | Status indicators | SparkFun | $0.50 | 4 | $2.00 | 1g |
| 8 | 330Ω resistors | LED current limiting | SparkFun | $0.05 | 4 | $0.20 | <1g |
| 9 | 4.7kΩ resistors | I2C + 1-Wire pull-ups | SparkFun | $0.05 | 3 | $0.15 | <1g |
| 10 | 9V battery | Power | Local | $4.00 | 2 | $8.00 | 90g |
| 11 | Rocker switch | Power switch | Local | $1.50 | 1 | $1.50 | 5g |
| 12 | Barrel jack | Power connector | Amazon | $1.00 | 1 | $1.00 | 3g |
| 13 | Custom PCB | 2-layer shield | JLCPCB | $10.00 | 1 | $10.00 | 12g |
| 14 | Pin headers | Wiring | Amazon | $2.00 | — | $2.00 | 3g |
| | | | | | **Total** | **$111.65** | **~163g** |

### Budget

| | Amount |
|---|---|
| Original kit | $147.00 |
| My design | $111.65 |
| Savings | $35.35 |

### Weight

| | Weight |
|---|---|
| Limit | 200g |
| My design | ~163g |
| Margin left | ~37g |

---

## 6. Power Analysis

### Current Draw

| Part | Current | Voltage | Power |
|---|---|---|---|
| Arduino Uno | ~46 mA | 5V | 0.230W |
| HIH-9120 | ~0.57 mA | 3.3V | 0.002W |
| DPS310 | ~0.002 mA | 3.3V | 0.000W |
| ADXL345 | ~0.14 mA | 3.3V | 0.001W |
| DS18B20 | ~1.5 mA | 3.3V | 0.005W |
| OpenLog | ~20 mA | 5V | 0.100W |
| 4x LEDs | ~36 mA | 5V | 0.180W |
| **Total (LEDs on)** | **~105 mA** | | **~0.518W** |
| **Total (LEDs off)** | **~69 mA** | | **~0.338W** |

### Battery Life

With 2x 9V batteries (1100 mAh total):

Runtime = 1100 mAh / 81 mA ≈ **13.6 hours**

A DemoSat flight is 3–4 hours, so this is plenty.

---

## 7. PCB Design

The PCB is an Arduino Uno shield that stacks directly on top of the Arduino.

| Spec | Value |
|---|---|
| Size | 69.5 × 54.0 mm |
| Layers | 2 |
| Thickness | 1.6 mm |
| Min trace width | 0.2 mm |
| Finish | HASL lead-free |
| Manufacturer | JLCPCB / OSHPark |
| Cost | ~$10 for 5 boards |

Key design choices:
- 100nF decoupling cap on each sensor VCC pin
- 4.7kΩ pull-up resistors on SDA and SCL
- 4.7kΩ pull-up on DS18B20 data line
- Ground plane on bottom copper layer

---

## 8. Does It Work?

| Function | Original Part | My Part | Works? |
|---|---|---|---|
| Humidity | HIH6130 | HIH-9120-021-001 | ✅ |
| Internal temp | HIH6130 | HIH-9120-021-001 | ✅ |
| Pressure + altitude | MPL3115A2 | DPS310 | ✅ |
| External temp | DS18B20 | DS18B20 | ✅ |
| Acceleration | MMA8452Q | ADXL345 | ✅ |
| Data logging | OpenLog | OpenLog (re-sourced) | ✅ |
| LEDs | 4x LEDs | 4x LEDs | ✅ |
| Battery power | 2x 9V | 2x 9V | ✅ |

---



## 9. Summary

| | Result |
|---|---|
| Total cost | $111.65 |
| Weight | ~163g |
| Max current (LEDs on) | ~105mA |
| Logging current | ~69mA |
| Battery life | ~13.6 hours |
| Discontinued parts | 0 |
| Under budget | ✅ |
| Under weight limit | ✅ |

