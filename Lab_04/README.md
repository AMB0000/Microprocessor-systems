## Lab 4: Microprocessors 1

### Question 1: 14-bit SAR ADC Simulation Results

The simulation was performed to test the accuracy of a 14-bit Successive Approximation Register (SAR) algorithm using a 3.0V reference voltage. The following table compares the target analog input voltage (Vin) against the reconstructed quantized voltage and the resulting 14-bit binary string.

| Input Voltage (Vin) | 14-Bit Binary Output | Quantized Result |
| :--- | :--- | :--- |
| 0.42 V | 00100011110101 | 0.419861 V |
| 0.83 V | 01000110110100 | 0.829834 V |
| 1.65 V | 10001100110011 | 1.649963 V |
| 2.752 V | 11101010110101 | 2.751892 V |

Note on Accuracy: The slight variance between the input voltage and the result represents the quantization error inherent in digital conversion, which is minimized by the 14-bit resolution.

#### Debugging Process
- Corrected bit depth from 12 to 14.
- Fixed Vref from 3.6V to 3.0V.
- Resolved logic errors in the successive approximation loop to ensure proper binary search convergence.

### Code

```c
#include <Arduino.h>

#define bitsize 14 // 14-bit resolution as required by lab specs

void setup() {
  Serial.begin(9600);
  
  // Configuration parameters
  float Vref = 3.0;        // Fixed: Reference voltage set to 3.0V 
  float Vin = 0.42;        // Change this to: 0.83, 1.65, or 2.752 for testing 
  float thresh;            // Current comparison threshold
  float step = Vref / 2.0; // The initial "half-way" step for binary search
  float quantized = 0;     // Reconstructed analog voltage
  int count;
  int bitval;
  int bits[bitsize];

  // Start the threshold at the middle of the reference range
  thresh = step;

  /*
   * SAR Algorithm: Binary Search through voltage ranges.
   * Compares input to threshold, sets bit, then narrows the search range.
   */
  for (count = 0; count < bitsize; count++) {
    if (Vin >= thresh) {
      bitval = 1;              // If input is higher, bit is 1
      step = step / 2.0;       // Halve the search range
      thresh = thresh + step;  // Move threshold UP to the middle of the next range
    } else {
      bitval = 0;              // If input is lower, bit is 0
      step = step / 2.0;       // Halve the search range
      thresh = thresh - step;  // Move threshold DOWN to the middle of the next range
    }

    // Store the bit in our array
    bits[count] = bitval;
    
    /*
     * Reconstruction (Quantization):
     * Sums up the voltage weight of each '1' bit to verify conversion accuracy.
     * Formula: bit * (Vref / 2^(bit_position + 1))
     */
    quantized += bitval * (Vref / pow(2, count + 1));
  }

  // Output results to Serial Monitor for documentation
  Serial.print("Target Vin: ");
  Serial.println(Vin);
  Serial.print("Quantized Result: ");
  Serial.println(quantized, 6); // Print with 6 decimal places for precision
  Serial.print("Binary String: ");
  for(int i = 0; i < bitsize; i++) {
    Serial.print(bits[i]);
  }
  Serial.println("\n----------------------------");
}

void loop() {
  // Static simulation; no repeating logic required.
}

```


## Question 2: Solar Charge Controller System Design

### Project Overview
This task involved designing the control logic for a Solar Charge Controller. The system manages power distribution between a solar panel and a battery to maintain the operation of a microcontroller (MCU). Due to hardware limitations, the system state is visualized through real-time Serial Monitor feedback.

### System Logic & Requirements
The controller operates based on the following prioritized logic gates:
- Primary Power Source (Solar): When the solar panel output exceeds 2.5V, the system routes solar power to the MCU.
- Backup Power Source (Battery): If solar output falls below 2.5V, the system automatically switches the MCU to battery power.
- Charging Function: The charging circuit is activated only when the battery voltage is lower than the solar panel voltage (Vbatt < Vsolar).
- Status Monitoring: All transitions and charging states are output to the Serial Monitor for verification.

### Block Diagram
The software architecture follows a modular design, separating analog data acquisition from the decision-making logic.

<p align="center">
  <img src="https://github.com/AMB0000/Microprocessor-systems/blob/d10fdbfddf546eded00795de633420597910b0d3/Lab_04/Pictures/IMG_0453.jpeg?raw=true" width="500">
</p>

### Test Results 

| Test Case | Solar Voltage (A0) | Battery Voltage (A1) | MCU Power Source | Charging Status |
| :--- | :--- | :--- | :--- | :--- |
| Peak Sun | 4.20 V | 3.10 V | SOLAR | ACTIVE |
| Low Sun / High Batt | 3.00 V | 3.80 V | SOLAR | IDLE |
| Dusk / Night | 1.10 V | 3.60 V | BATTERY | IDLE |
| Critical Power | 0.80 V | 0.20 V | BATTERY | ACTIVE |
| Threshold Limit | 2.51 V | 2.10 V | SOLAR | ACTIVE |

#### Logic Validation Criteria:
- Power Source Selection: The system correctly toggles to SOLAR whenever Vsolar > 2.5V; otherwise, it defaults to BATTERY.
- Charging Activation: Charging logic is only engaged when Vsolar > Vbatt, ensuring power only flows toward the lower potential.

### Code

```c
/*
 * ENGR 3650 - Microprocessors 1
 * Lab 4 - Question 2: Solar Charge Controller Logic

void setup() {
  Serial.begin(9600); 
  Serial.println("--- Solar Charge Controller Initialized ---");
}

void loop() {
  // Read analog voltages (0-1023) and convert to 0-5.0V range
  float solarV = analogRead(A0) * (5.0 / 1023.0); 
  float batteryV = analogRead(A1) * (5.0 / 1023.0);

  // 1. Power Source Selection Logic
  // MCU is powered by Solar if Vin > 2.5V; otherwise, use Battery.
  if (solarV > 2.5) {
    Serial.print("MCU Power Source: SOLAR   | ");
  } else {
    Serial.print("MCU Power Source: BATTERY | ");
  }

  // 2. Charging Activation Logic
  // Charging circuit is active if Solar Voltage > Battery Voltage.
  if (solarV > batteryV) {
    Serial.println("Charging Status: ACTIVE");
  } else {
    Serial.println("Charging Status: IDLE");
  }

  // Output raw data for verification against the test table
  Serial.print("Solar (A0): "); Serial.print(solarV); Serial.print("V | ");
  Serial.print("Battery (A1): "); Serial.print(batteryV); Serial.println("V");
  Serial.println("----------------------------------------------------------");

  delay(1000); // Sample every 1 second
}
```


## Question 3: Improved Solar Charge Controller 

### Project Overview
This task addresses the major limitation identified in Question 2. In the previous design, the charging process terminates the exact moment the battery voltage equals the solar voltage. This prevents a standard lithium-ion battery from ever reaching its true full capacity of 4.2V if the solar panel output fluctuates or drops slightly. To solve this, a hysteresis logic loop (state machine) is implemented to keep the charging state active until the battery is completely full.

### Block Diagram

<p align="center">
  <img src="" width="500" title="Q3 State Machine Diagram">
</p>


State 1: IDLE
- The system rests here when the battery is full or solar energy is insufficient.
- Transition to ACTIVE: Occurs only if Solar Voltage > Battery Voltage AND Battery Voltage < 4.2V.

State 2: ACTIVE (Charging)
- The system stays in this state to force the battery to charge fully.
- Transition to IDLE: Occurs if Battery Voltage reaches 4.2V (Full) OR if Solar Voltage drops below Battery Voltage (to prevent reverse current flow).

### Test Results & Output Validation
The table below demonstrates how the hysteresis logic behaves differently from Question 2. Notice how the system maintains the ACTIVE state even when the voltage difference is marginal, ensuring a full charge.

| Test Case | Solar Voltage | Battery Voltage | Previous State | MCU Source | Charging Status |
| --- | --- | --- | --- | --- | --- |
| Initial Charge | 4.50 V | 3.00 V | IDLE | SOLAR | ACTIVE |
| Mid Charge | 3.80 V | 3.50 V | ACTIVE | SOLAR | ACTIVE |
| Cloud Cover | 3.20 V | 3.50 V | ACTIVE | SOLAR | IDLE |
| Full Capacity | 4.80 V | 4.20 V | ACTIVE | SOLAR | IDLE |
| Night Mode | 0.50 V | 3.80 V | IDLE | BATTERY | IDLE |

### Code Implementation

```c
/*
 * ENGR 3650 - Microprocessors 1
 * Lab 4 - Question 3: Improved Solar Charge Controller
 * Objective: Implement state-based hysteresis to ensure 4.2V full charge.
 */

bool isCharging = false;
const float BATT_FULL = 4.2; // Full charge threshold for Lithium-ion

void setup() {
  Serial.begin(9600);
  Serial.println("--- Q3: Hysteresis Controller Initialized ---");
}

void loop() {
  // Read analog voltages and convert to 0-5.0V range
  float solarV = analogRead(A0) * (5.0 / 1023.0); 
  float batteryV = analogRead(A1) * (5.0 / 1023.0);

  // 1. Power Source Selection Logic
  if (solarV > 2.5) {
    Serial.print("MCU Power Source: SOLAR   | ");
  } else {
    Serial.print("MCU Power Source: BATTERY | ");
  }

  // 2. Improved Charging Activation Logic (Hysteresis)
  if (isCharging == false) {
    // Start charging condition
    if (solarV > batteryV && batteryV < BATT_FULL) {
      isCharging = true;
    }
  } else {
    // Stop charging condition
    if (batteryV >= BATT_FULL || solarV <= batteryV) {
      isCharging = false;
    }
  }

  // 3. Status Output
  if (isCharging == true) {
    Serial.println("Charging Status: ACTIVE (Target: 4.2V)");
  } else {
    Serial.println("Charging Status: IDLE");
  }

  // Output raw data for debugging
  Serial.print("Solar (A0): "); Serial.print(solarV); Serial.print("V | ");
  Serial.print("Battery (A1): "); Serial.print(batteryV); Serial.println("V");
  Serial.println("----------------------------------------------------------");

  delay(1000); // Sample every 1 second
}

