# DemoSat High Altitude Balloon Payload
## Phase 2: Alternative Design
**ENCE 3231 Microprocessors 1 — University of Denver | Winter Quarter 2026 | Ali Behbehani**

---

## 1. Overview

In Phase 1, I identified that three of the four core sensors in the original DemoSat kit are discontinued, and the data logger has been replaced by a newer incompatible variant. The goal of Phase 2 is to design a fully functional replacement system that:

- Replaces all unavailable components with in-production alternatives
- Maintains identical functionality to the original kit
- Stays within the **$147 budget** and **200 g weight limit**
- Reduces power consumption where possible
- Uses a custom PCB to replace the original shield

### Discontinued Parts from Phase 1

| Original Part | Function | Status |
|---|---|---|
| HIH6130 (Honeywell) | Internal humidity + temperature | 🔴 Discontinued |
| MPL3115A2 (NXP) | Barometric pressure + altitude | 🔴 End-of-life |
| MMA8452Q (NXP) | 3-axis accelerometer | 🔴 End-of-life |
| OpenLog DEV-13712 (SparkFun) | UART data logger | 🟡 Replaced by incompatible variant |
| DS18B20 (external temp) | External temperature | 🟢 Keep — still available |
| Arduino Uno | Microcontroller | 🟢 Keep — still available |

---

## 2. Design Philosophy

The key insight driving my replacement strategy is sensor consolidation. The **BME280** is a single, widely available, low-cost sensor that simultaneously measures humidity, barometric pressure, and temperature — replacing both the HIH6130 and MPL3115A2 in one chip. This reduces part count, cost, PCB footprint, and weight all at once.

For the accelerometer, I selected the **ADXL345** — a direct functional replacement for the MMA8452Q, using the same I2C interface with excellent Arduino library support.

For data logging, I replaced the OpenLog with a simple **MicroSD SPI module**, which is lighter, cheaper, more reliable, and does not depend on a specific firmware version.

| Original Part | Replaced By | Reason |
|---|---|---|
| HIH6130 (humidity + temp) | **BME280** | Single chip replaces two sensors |
| MPL3115A2 (pressure) | **BME280** | Consolidated into same chip |
| MMA8452Q (accelerometer) | **ADXL345** | Same I2C interface, widely available |
| OpenLog DEV-13712 | **MicroSD SPI module** | Simpler, lighter, no firmware dependency |
| DS18B20 | **DS18B20** | Unchanged — still available |
| Arduino Uno | **Arduino Uno** | Unchanged |

---

## 3. Component Selection

### BME280 — Humidity, Pressure & Internal Temperature
- **Replaces:** HIH6130 + MPL3115A2
- **Source:** Adafruit #2652
- **Interface:** I2C (address 0x76)
- **Specs:** Humidity ±3%, Pressure ±1 hPa, Temperature ±1°C
- **Why:** In production, excellent `Adafruit_BME280` library support, lower power than both sensors it replaces combined
- **Cost:** $14.95 | **Weight:** ~2 g

### ADXL345 — 3-Axis Accelerometer
- **Replaces:** MMA8452Q
- **Source:** Adafruit #1231 or SparkFun SEN-09836
- **Interface:** I2C (address 0x53)
- **Specs:** ±2g / ±4g / ±8g / ±16g selectable range, 13-bit resolution
- **Why:** Pin and software compatible with I2C, widely available, excellent library support, lower power than MMA8452Q
- **Cost:** $9.95 | **Weight:** ~2 g

### MicroSD SPI Module — Data Storage
- **Replaces:** OpenLog DEV-13712
- **Source:** Amazon / SparkFun
- **Interface:** SPI
- **Why:** No firmware dependency, lighter than OpenLog, logs CSV data directly via Arduino SD library (built-in), widely available
- **Cost:** $5.99 | **Weight:** ~4 g

### Retained Components

| Component | Reason Kept |
|---|---|
| Arduino Uno | Meets all I/O requirements, familiar platform |
| DS18B20 (waterproof probe) | Still in production, performs well |
| 6× LEDs + 330Ω resistors | No change needed |
| 9V battery (×1) | Sufficient for flight duration — see power analysis |
| Rocker switch + barrel connector | No change needed |

> **Battery note:** My Phase 1 design used two 9V batteries (90 g). In Phase 2 I reduced to one 9V battery (45 g), saving 45 g of weight while still providing over 5 hours of runtime — well above the 3–4 hour flight window.

---

## 4. System Architecture

### 4.1 Block Diagram
```
                        ┌─────────────────────┐
                        │     9V Battery      │
                        │   (Alkaline, 1×)    │
                        └──────────┬──────────┘
                                   │ VIN (via rocker switch)
                        ┌──────────▼──────────┐
                        │     Arduino Uno      │
                        │  ATmega328P @ 16MHz  │
                        │                      │
                        │  I2C:  A4(SDA)/A5(SCL)│
                        │  1-Wire: D6          │
                        │  SPI:  D10–D13       │
                        │  GPIO: D2–D7 (LEDs)  │
                        └──┬───────────────┬───┘
                           │               │
               ────────────┤               ├────────────
              I2C Bus (SDA/SCL)         SPI Bus
                           │               │
          ┌────────────────┤         ┌─────┴──────────┐
          │                │         │  MicroSD Module │
    ┌─────▼──────┐  ┌──────▼──────┐  │  Data logging  │
    │   BME280   │  │   ADXL345   │  │  CS → D10      │
    │            │  │             │  └────────────────┘
    │ Humidity   │  │ Accel X/Y/Z │
    │ Pressure   │  │ ±2g–±16g    │
    │ Int. Temp  │  │ I2C: 0x53   │
    │ I2C: 0x76  │  └─────────────┘
    └────────────┘

         1-Wire (D6)
              │
    ┌─────────▼──────────┐
    │      DS18B20       │
    │  External Temp     │
    │  Waterproof probe  │
    └────────────────────┘

         GPIO (D2–D7)
              │
    ┌─────────▼──────────┐
    │   6× LED Indicators│
    │   330Ω current lim │
    └────────────────────┘
```

### 4.2 Pin Assignment Table

| Arduino Pin | Connected To | Protocol |
|---|---|---|
| A4 (SDA) | BME280 SDA, ADXL345 SDA | I2C |
| A5 (SCL) | BME280 SCL, ADXL345 SCL | I2C |
| D6 | DS18B20 data | 1-Wire |
| D10 | MicroSD CS | SPI |
| D11 (MOSI) | MicroSD MOSI | SPI |
| D12 (MISO) | MicroSD MISO | SPI |
| D13 (SCK) | MicroSD SCK | SPI |
| D2–D7 | LEDs (×6) via 330Ω | GPIO |

### 4.3 I2C Address Map

| Device | I2C Address | Notes |
|---|---|---|
| BME280 | 0x76 | SDO pin to GND |
| ADXL345 | 0x53 | SDO/ALT pin to GND |

No address conflicts on the shared I2C bus.

---

## 5. Bill of Materials

| # | Component | Part | Supplier | Unit Price | Qty | Total | Weight |
|---|---|---|---|---|---|---|---|
| 1 | Microcontroller | Arduino Uno R3 | Arduino | $28.00 | 1 | $28.00 | 25 g |
| 2 | Humidity + Pressure + Temp | BME280 Breakout | Adafruit #2652 | $14.95 | 1 | $14.95 | 2 g |
| 3 | 3-Axis Accelerometer | ADXL345 Breakout | Adafruit #1231 | $9.95 | 1 | $9.95 | 2 g |
| 4 | External Temperature | DS18B20 waterproof probe | Adafruit #381 | $4.95 | 1 | $4.95 | 8 g |
| 5 | Data Storage | MicroSD SPI module | Amazon | $5.99 | 1 | $5.99 | 4 g |
| 6 | MicroSD Card | 8 GB Class 10 | Amazon | $6.99 | 1 | $6.99 | 1 g |
| 7 | LED indicators | 5mm LEDs (×6, assorted) | SparkFun | $0.50 | 6 | $3.00 | 1 g |
| 8 | Current-limiting resistors | 330Ω 1/4W (×6) | SparkFun | $0.05 | 6 | $0.30 | <1 g |
| 9 | Pull-up resistor | 4.7kΩ 1/4W (DS18B20) | SparkFun | $0.05 | 1 | $0.05 | <1 g |
| 10 | Battery | 9V Alkaline (Energizer) | Local | $4.00 | 1 | $4.00 | 45 g |
| 11 | Power switch | SPST rocker switch | Local | $1.50 | 1 | $1.50 | 5 g |
| 12 | Barrel connector | 2.1mm barrel jack | Amazon | $1.00 | 1 | $1.00 | 3 g |
| 13 | Custom PCB | 2-layer shield, ~60×55mm | JLCPCB | $10.00 | 1 | $10.00 | 12 g |
| 14 | Headers + wiring | 2.54mm pin headers | Amazon | $2.00 | — | $2.00 | 3 g |
| | | | | | **Total** | **$92.68** | **~111 g** |

### Budget Summary

| | Amount |
|---|---|
| Original kit cost | $147.00 |
| This design | $92.68 |
| **Savings** | **$54.32 (37%)** |
| Budget remaining | $54.32 |

### Weight Summary

| | Weight |
|---|---|
| Maximum allowed | 200 g |
| This design (electronics) | ~111 g |
| **Margin remaining** | **~89 g** |

> The ~89 g margin covers the foam structure, insulation, aluminum tape, and flight hardware from the Structure Kit.

---

## 6. Power Consumption Analysis

### 6.1 Assumptions
- System powered by 1× 9V alkaline battery through Arduino's onboard LDO regulator (~85% efficient)
- All 6 LEDs on simultaneously through 330Ω at 5V → I = (5V − 2V) / 330Ω ≈ 9 mA each
- MicroSD draw measured during active write cycle

### 6.2 Power Table

| Component | Mode | Current | Voltage | Power |
|---|---|---|---|---|
| Arduino Uno (ATmega328P) | Active | ~46 mA | 5V | 0.230 W |
| BME280 | Forced mode 1Hz | ~0.5 mA | 3.3V | 0.002 W |
| ADXL345 | Measurement mode | ~0.14 mA | 3.3V | 0.001 W |
| DS18B20 | Converting | ~1.5 mA | 3.3V | 0.005 W |
| MicroSD module | Write cycle | ~30 mA | 3.3V | 0.099 W |
| 6× LEDs (330Ω, 5V) | All on | ~54 mA | 5V | 0.270 W |
| **TOTAL (active)** | | **~132 mA** | | **~0.607 W** |
| **TOTAL (logging, LEDs off)** | | **~78 mA** | | **~0.337 W** |

### 6.3 Battery Life Estimate

Accounting for LDO regulator efficiency (~85%), the 9V battery supplies approximately **92 mA** at the input when the system draws 78 mA at 5V during normal logging.

$$\text{Runtime} = \frac{550 \text{ mAh}}{92 \text{ mA}} \approx \textbf{5.9 hours}$$

A typical DemoSat flight lasts **3–4 hours** — this design has sufficient margin on a single battery.

### 6.4 Comparison: Phase 1 vs Phase 2

| Metric | Phase 1 (original) | Phase 2 (this design) | Change |
|---|---|---|---|
| Active current (5V rail) | ~76 mA | ~78 mA | +3% (6 LEDs vs 4) |
| Battery count | 2× 9V | 1× 9V | **−50% batteries** |
| Battery weight | 90 g | 45 g | **−45 g saved** |
| Estimated runtime | ~6.1 hrs (×2 batteries) | ~5.9 hrs (×1 battery) | Comparable |
| Unavailable parts | 4 | 0 | **✅ Fully sourceable** |

> The slight current increase is because I count all 6 LEDs (matching the original kit) vs the 4 LEDs assumed in Phase 1. The major win is cutting from two batteries to one — saving 45 g while maintaining flight-duration coverage.

---

## 7. Custom PCB Design

### 7.1 Overview

The custom PCB replaces the original Shield Kit. It is designed as an Arduino Uno shield — stacking directly on top of the Arduino via standard headers — keeping the assembly compact and eliminating loose wiring.

### 7.2 PCB Specifications

| Parameter | Value |
|---|---|
| Form factor | Arduino Uno shield (68.6 × 53.3 mm) |
| Layers | 2 (top + bottom copper) |
| PCB thickness | 1.6 mm |
| Min trace width | 0.2 mm |
| Surface finish | HASL lead-free |
| Recommended manufacturer | JLCPCB |
| Estimated cost | ~$10 for 5 boards including shipping |

### 7.3 Component Placement
```
┌──────────────────────────────────────────┐
│  [Barrel Jack]    [Rocker Switch]        │
│                                          │
│  ┌──────────┐    ┌──────────┐           │
│  │  BME280  │    │  ADXL345 │           │
│  │  (I2C)   │    │  (I2C)   │           │
│  └──────────┘    └──────────┘           │
│                                          │
│  [DS18B20 connector — 3 pin]            │
│                                          │
│  [MicroSD module header — 6 pin SPI]    │
│                                          │
│  [LED ×6]  [R ×6 330Ω]  [R 4.7kΩ]     │
│                                          │
│  Arduino Uno shield headers (below PCB) │
└──────────────────────────────────────────┘
```

### 7.4 Key Design Rules

- **Decoupling caps:** 100nF ceramic cap on each sensor VCC pin (BME280, ADXL345, MicroSD), placed as close to the pin as possible
- **I2C pull-ups:** One pair of 4.7kΩ resistors on SDA and SCL to 3.3V — only one set for the whole bus
- **DS18B20 pull-up:** 4.7kΩ between data line and 3.3V
- **Ground plane:** Solid copper pour on bottom layer to reduce noise
- **SPI routing:** Keep D10–D13 traces short; CS line (D10) must be driven low before every transaction

### 7.5 Recommended EDA Tool

| Tool | Cost | Notes |
|---|---|---|
| **EasyEDA** | Free | Browser-based, integrates directly with JLCPCB for ordering |
| **KiCad** | Free | Industry standard, good for learning transferable skills |

---

## 8. Functionality Verification

| Required Function | Original Part | New Implementation | Status |
|---|---|---|---|
| Internal temperature | HIH6130 | BME280 | ✅ |
| Humidity | HIH6130 | BME280 | ✅ |
| Barometric pressure + altitude | MPL3115A2 | BME280 | ✅ |
| External temperature | DS18B20 | DS18B20 (unchanged) | ✅ |
| 3-axis acceleration | MMA8452Q | ADXL345 | ✅ |
| Data storage | OpenLog DEV-13712 | MicroSD SPI module | ✅ |
| LED indicators | 6× LEDs | 6× LEDs (unchanged) | ✅ |
| Battery-powered operation | 2× 9V | 1× 9V | ✅ |

---

## 9. Arduino Firmware

### Required Libraries (install via Arduino Library Manager)
- `Adafruit BME280 Library`
- `Adafruit Unified Sensor`
- `Adafruit ADXL345`
- `OneWire`
- `DallasTemperature`
- `SD` (built-in)

### sensor_logger.ino
```cpp
/*
 * DemoSat Payload — Sensor Logger
 * ENCE 3231 Microprocessors 1 | Winter Quarter 2026
 * University of Denver | Ali Behbehani
 *
 * Sensors:
 *   BME280   (I2C 0x76) — humidity, pressure, internal temp
 *   ADXL345  (I2C 0x53) — 3-axis acceleration
 *   DS18B20  (1-Wire D6) — external temperature
 *   MicroSD  (SPI D10)  — CSV data logging
 *   6x LEDs  (D2–D7)    — status indicators
 */

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_ONE_WIRE  6
#define PIN_SD_CS     10
#define PIN_LED_1     2
#define PIN_LED_2     3
#define PIN_LED_3     4
#define PIN_LED_4     5
#define PIN_LED_5     7
#define PIN_LED_6     8

Adafruit_BME280 bme;
Adafruit_ADXL345_Unified adxl = Adafruit_ADXL345_Unified(12345);
OneWire oneWire(PIN_ONE_WIRE);
DallasTemperature ds18b20(&oneWire);

File logFile;
unsigned long lastLog = 0;
const unsigned long LOG_INTERVAL = 1000;

void errorHalt(const char* msg) {
  Serial.print("ERROR: "); Serial.println(msg);
  while (true) {
    digitalWrite(PIN_LED_1, HIGH); delay(200);
    digitalWrite(PIN_LED_1, LOW);  delay(200);
  }
}

void setup() {
  Serial.begin(9600);
  for (int p = 2; p <= 8; p++) pinMode(p, OUTPUT);

  if (!bme.begin(0x76))       errorHalt("BME280 not found");
  if (!adxl.begin())          errorHalt("ADXL345 not found");
  adxl.setRange(ADXL345_RANGE_2_G);
  ds18b20.begin();
  if (!SD.begin(PIN_SD_CS))   errorHalt("SD init failed");

  logFile = SD.open("log.csv", FILE_WRITE);
  if (!logFile) errorHalt("Cannot open log.csv");
  logFile.println("time_ms,ext_temp_C,int_temp_C,humidity_pct,pressure_hPa,ax_g,ay_g,az_g");
  logFile.close();

  // All LEDs on = system ready
  for (int p = 2; p <= 8; p++) digitalWrite(p, HIGH);
  delay(1000);
  for (int p = 2; p <= 8; p++) digitalWrite(p, LOW);
  digitalWrite(PIN_LED_2, HIGH); // green = logging
  Serial.println("Ready. Logging started.");
}

void loop() {
  if (millis() - lastLog < LOG_INTERVAL) return;
  lastLog = millis();

  ds18b20.requestTemperatures();
  float extTemp  = ds18b20.getTempCByIndex(0);
  float intTemp  = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;

  sensors_event_t event;
  adxl.getEvent(&event);
  float ax = event.acceleration.x / 9.81;
  float ay = event.acceleration.y / 9.81;
  float az = event.acceleration.z / 9.81;

  Serial.print("ExtT="); Serial.print(extTemp);
  Serial.print(" IntT="); Serial.print(intTemp);
  Serial.print(" Hum="); Serial.print(humidity);
  Serial.print(" Pres="); Serial.print(pressure);
  Serial.print(" Ax="); Serial.print(ax);
  Serial.print(" Ay="); Serial.print(ay);
  Serial.print(" Az="); Serial.println(az);

  logFile = SD.open("log.csv", FILE_WRITE);
  if (logFile) {
    logFile.print(millis());   logFile.print(",");
    logFile.print(extTemp);    logFile.print(",");
    logFile.print(intTemp);    logFile.print(",");
    logFile.print(humidity);   logFile.print(",");
    logFile.print(pressure);   logFile.print(",");
    logFile.print(ax);         logFile.print(",");
    logFile.print(ay);         logFile.print(",");
    logFile.println(az);
    logFile.close();
    digitalWrite(PIN_LED_2, LOW); delay(50);
    digitalWrite(PIN_LED_2, HIGH);
  } else {
    digitalWrite(PIN_LED_1, HIGH); delay(100);
    digitalWrite(PIN_LED_1, LOW);
  }
}
```

---

## 10. Phase 2 Summary

| Parameter | Result |
|---|---|
| Total cost | $92.68 (saves $54.32 vs original) |
| Electronics weight | ~111 g (89 g margin remaining) |
| Active power draw | ~132 mA (all LEDs on) |
| Logging power draw | ~78 mA (LEDs off) |
| Estimated battery runtime | ~5.9 hours (1× 9V) |
| Unavailable parts | **0 — fully sourceable** |
| Budget status | ✅ Under $147 |
| Weight status | ✅ Under 200 g |

---

*Builds on [Phase 1 System Evaluation](../Phase_1/README.md)*
