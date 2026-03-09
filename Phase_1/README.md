# DemoSat High Altitude Balloon Payload
## Phase 1: System Evaluation
**ENCE 3231 Microprocessors 1 — University of Denver | Winter Quarter 2026 | Ali Behbehani |**

---

## 1. Project Overview

For this project, I was tasked with designing, building, and launching a payload to near-space altitudes (up to 100,000 ft) using the DemoSat High Altitude Balloon Payload kit. **In Phase 1, my goal was to evaluate the current system — identifying all components, estimating power consumption and weight, and determining whether the existing design is still feasible given current component availability.**

The kit I was given is composed of six modules:

- **Soldering 101 Kit** – Tools I used to assemble the shield and sensor PCB
- **Arduino Kit** – Arduino Uno microcontroller, breadboard, LEDs, resistors, jumper wires, potentiometer
- **Shield Kit** – Custom PCB that stacks on the Arduino, housing sensor headers and DIP sockets
- **Sensor Kit** – HIH6130 (humidity/temp), DS18B20 (external temp), MPL3115A2 (pressure), MMA8452Q (accelerometer), OpenLog (data logger), LEDs
- **Structure Kit** – Foam core, insulation, aluminum tape, velcro, stickers, flight string, xacto knife
- **Power Switch Kit** – Rocker switch, 9V barrel connector, stranded wire, heat shrink

---

## 2. System Architecture & Block Diagram


### 2.1 Block Diagram

![DemoSat System Block Diagram](https://github.com/AMB0000/Microprocessor-systems/blob/5559a99dc1355a7c41e5c876dde68455c18d4cff/Phase_1/Images/Screenshot%202026-03-09%20141327.png)


### 2.2 Signal & Data Flow

| Connection | Protocol | Arduino Pins |
|---|---|---|
| HIH6130 → MCU | I²C | A4 (SDA), A5 (SCL) |
| MPL3115A2 → MCU | I²C (shared bus) | A4 (SDA), A5 (SCL) |
| MMA8452Q → MCU | I²C (shared bus) | A4 (SDA), A5 (SCL) |
| DS18B20 → MCU | 1-Wire | D6 |
| MCU → OpenLog | UART | D0 (TX), D1 (RX) |
| OpenLog → SD Card | SPI | Internal to OpenLog |
| MCU → LEDs (×6) | GPIO | D2–D7 |
| MCU ↔ PC | UART via USB | D0/D1 |

> I share the I²C SDA/SCL bus (A4/A5) across three sensors. Since each device has a unique I²C address, there are no address conflicts. The DS18B20 uses a separate 1-Wire protocol on its own dedicated digital pin.

---

## 3. Component Identification

After going through the full kit, I catalogued all electronic components, their roles, communication interfaces, and current availability:

| Module | Component | Qty | Part / IC | Interface | Availability |
|---|---|---|---|---|---|
| Power | 9V Battery | 2 | Generic 9V Alkaline | — | ✅ Available |
| Power | Rocker Switch | 1 | Generic SPST | — | ✅ Available |
| Power | 9V Barrel Connector | 1 | Generic 2.1mm | — | ✅ Available |
| Microcontroller | Arduino Uno | 1 | ATmega328P (5V, 16MHz) | GPIO/I2C/SPI/UART | ✅ Available |
| Shield | Custom Shield PCB | 1 | DIP/Header board | Stackable | ✅ Available |
| Sensor | Humidity + Temp (internal) | 1 | HIH6130 (Honeywell) | I²C | ⚠️ Discontinued |
| Sensor | External Temperature | 1 | DS18B20 (waterproof probe) | 1-Wire | ✅ Available |
| Sensor | Pressure + Altitude | 1 | MPL3115A2 (NXP) | I²C | ⚠️ Discontinued |
| Sensor | 3-Axis Accelerometer | 1 | MMA8452Q (NXP) | I²C | ⚠️ Discontinued |
| Storage | OpenLog Data Logger | 1 | SparkFun DEV-13712 | UART | ⚠️ EOL Variant |
| Storage | SD Card + Adapter | 1 | Generic microSD | SPI (via OpenLog) | ✅ Available |
| Indicators | LED – Red | 1 | 5mm THT LED | GPIO | ✅ Available |
| Indicators | LED – Blue | 1 | 5mm THT LED | GPIO | ✅ Available |
| Indicators | LED – Green | 1 | 5mm THT LED | GPIO | ✅ Available |
| Indicators | LED – Yellow | 1 | 5mm THT LED | GPIO | ✅ Available |
| Indicators | LED – Orange (ext) | 1 | 5mm THT LED | GPIO | ✅ Available |
| Indicators | Current-Limiting Resistors | 6 | 330Ω THT | — | ✅ Available |

---

## 4. Power Consumption Estimate

### 4.1 Assumptions

To estimate power consumption, I made the following assumptions:

- I am powering the system from a 9V battery through the Arduino's onboard linear regulator (~85% efficient at low loads)
- The Arduino draws ~46 mA when actively running code at 16 MHz with USB disconnected
- I am sampling sensors periodically; the figures I used represent typical active/conversion current from each datasheet
- I assumed all 4 LEDs are ON simultaneously at 5V through 330Ω resistors → I = (5V – 2V) / 330Ω ≈ 9 mA per LED
- The OpenLog draws ~11 mA during active logging to the SD card

### 4.2 Power Consumption Table

| Component | Mode | Current | Voltage | Power |
|---|---|---|---|---|
| Arduino Uno (ATmega328P) | Active | ~46 mA | 5V (regulated) | 0.23 W |
| HIH6130 Humidity Sensor | Conversion | ~1.2 mA | 3.3V | 0.004 W |
| DS18B20 Ext. Temperature | Converting | ~1.5 mA | 3.3–5V | 0.008 W |
| MPL3115A2 Pressure | Active | ~2.0 mA | 3.3V | 0.007 W |
| MMA8452Q Accelerometer | Active | ~0.2 mA | 3.3V | 0.001 W |
| OpenLog Data Logger | Logging | ~11 mA | 3.3–5V | 0.055 W |
| 4× LEDs (330Ω, 5V) | On | ~4 × 9 mA | 5V | 0.180 W |
| **TOTAL (estimated)** | | **~76 mA** | **~8.6V avg** | **~0.49 W** |

### 4.3 Battery Life Estimate

With a total draw of ~76 mA from the 5V rail, and accounting for regulator efficiency (~85%), I calculated that the 9V battery supplies approximately **90 mA**.

A standard 9V alkaline battery has a capacity of ~550 mAh, so my estimated runtime is:

$$\text{Runtime} = \frac{550 \text{ mAh}}{90 \text{ mA}} \approx \textbf{6.1 hours}$$

Since a typical DemoSat flight lasts 3–4 hours (ascent + descent), I have sufficient power margin. The kit includes two 9V batteries — if I wire them in parallel, I can extend the runtime to ~12 hours.

---

## 5. Maximum Weight Estimate

### 5.1 Weight Table

I estimated each component's weight using manufacturer datasheets and standard part specifications:

| Component | Qty | Unit Weight | Subtotal |
|---|---|---|---|
| Arduino Uno (PCB + headers) | 1 | ~25 g | 25 g |
| Custom Shield PCB | 1 | ~10 g | 10 g |
| 9V Battery (Energizer) | 2 | ~45 g each | 90 g |
| HIH6130 breakout | 1 | ~2 g | 2 g |
| DS18B20 probe + cable | 1 | ~8 g | 8 g |
| MPL3115A2 breakout | 1 | ~2 g | 2 g |
| MMA8452Q breakout | 1 | ~2 g | 2 g |
| OpenLog + SD Card | 1 | ~5 g | 5 g |
| LEDs, resistors, wiring | ~16 pcs | ~3 g total | 3 g |
| Structure (foam, tape, tube) | — | ~30 g | 30 g |
| **TOTAL (estimated maximum)** | | | **~177 g** |

### 5.2 Weight Analysis

My estimated total comes to **~177 g**, which is within the **200 g maximum payload weight** requirement. This gives me a margin of approximately **23 grams** for additional mounting hardware or connectors.

I also noted that my two 9V batteries (90 g combined) account for ~51% of the total weight. This is something I plan to address in Phase 2 — switching to a lighter battery solution would significantly improve the design.

---

## 6. Feasibility Assessment

### 6.1 Component Availability

After researching each component on DigiKey and Mouser, I found that several key parts are no longer available:

| Part | Function | Availability Status | Risk Level |
|---|---|---|---|
| HIH6130 | Honeywell HumidIcon (humidity + temp) | Discontinued / very limited stock | 🔴 High – I need a replacement |
| MPL3115A2 | NXP Pressure + Altitude | EOL; low availability on DigiKey/Mouser | 🔴 High – I need a replacement |
| MMA8452Q | NXP 3-axis Accelerometer | EOL; scarce in breakout form | 🔴 High – I need a replacement |
| OpenLog DEV-13712 | SparkFun Data Logger | Replaced by newer OpenLog variants | 🟡 Medium – newer version available |
| DS18B20 | Maxim/Analog Devices Temp Probe | Widely available | 🟢 Low – no issue |
| Arduino Uno | ATmega328P board | Widely available | 🟢 Low – no issue |

### 6.2 Key Findings

- **HIH6130 (Honeywell):** Discontinued. The breakout board (SparkFun SEN-11295) I would need is no longer produced, and legacy stock is available only at elevated prices.
- **MPL3115A2 (NXP):** End-of-life. I found dwindling availability on major distributors.
- **MMA8452Q (NXP):** End-of-life. NXP has moved to the FXOS8700 and LIS3DH families, so I cannot easily source a breakout board.
- **SparkFun OpenLog DEV-13712:** The original version in the kit is discontinued; a newer Artemis-based variant exists, but it has a different pinout and firmware.
- **DS18B20 and Arduino Uno:** Both are widely available and present no procurement risk for me.

### 6.3 Overall Feasibility Conclusion

> **I concluded that the existing DemoSat design is NOT feasible to replicate with off-the-shelf components at this time.** Three of the four sensor ICs I need are discontinued, meaning I cannot reproduce the system at the original $147 price point. This is what motivates my **Phase 2 redesign** — where I will propose a fully available, functionally equivalent alternative.

---

## 7. Phase 1 Summary

| Parameter | Result |
|---|---|
| Estimated Total Current | ~76 mA @ 5V rail |
| Estimated Power Draw | ~0.49 W |
| Estimated Battery Life | ~6.1 hours (single 9V battery) |
| Estimated Electronics Weight | ~177 g (within 200 g limit) |
| Weight Margin Remaining | ~23 g |
| Unavailable Components | HIH6130, MPL3115A2, MMA8452Q, OpenLog DEV-13712 |
| Overall Feasibility | ❌ Current design NOT reproducible — Phase 2 redesign required |

---

*In Phase 2, I will propose a fully available, cost-equivalent or lower-cost redesign that maintains all the functionality I need: internal/external temperature sensing, 3-axis acceleration, pressure, humidity, LED indicators, battery-powered operation, and SD card data storage.*


*Phase 2 will propose a fully available, cost-equivalent or lower-cost redesign that maintains all required functionality: internal/external temperature sensing, 3-axis acceleration, pressure, humidity, LED indicators, battery-powered operation, and SD card data storage.*

