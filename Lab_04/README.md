## Lab 4: Microprocessors 1

### Question 1: 14-bit SAR ADC Simulation Results

The simulation was performed to test the accuracy of a 14-bit Successive Approximation Register (SAR) algorithm using a 3.0V reference voltage. The following table compares the target analog input voltage ($V_{in}$) against the reconstructed quantized voltage and the resulting 14-bit binary string.

| Input Voltage ($V_{in}$) | 14-Bit Binary Output | Quantized Result |
| :--- | :--- | :--- |
| **0.42 V** | `00100011110101` | 0.419861 V |
| **0.83 V** | `01000110110100` | 0.829834 V |
| **1.65 V** | `10001100110011` | 1.649963 V |
| **2.752 V** | `11101010110101` | 2.751892 V |

**Note on Accuracy:** The slight variance between the input voltage and the result represents the quantization error inherent in digital conversion, which is minimized by the 14-bit resolution.

#### Debugging Process
- Corrected bit depth from 12 to 14.
- Fixed Vref from 3.6V to 3.0V.
- Resolved logic errors in the successive approximation loop to ensure proper binary search convergence.

### Code

``` C
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

  /* * SAR Algorithm: Binary Search through voltage ranges.
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
    
    /* * Reconstruction (Quantization):
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
* **Primary Power Source (Solar)**: When the solar panel output exceeds **2.5V**, the system routes solar power to the MCU.
* **Backup Power Source (Battery)**: If solar output falls below **2.5V**, the system automatically switches the MCU to battery power.
* **Charging Function**: The charging circuit is activated only when the battery voltage is lower than the solar panel voltage ($V_{batt} < V_{solar}$).
* **Status Monitoring**: All transitions and charging states are output to the Serial Monitor for verification.

### Block Diagram
The software architecture follows a modular design, separating analog data acquisition from the decision-making logic.


### Test Results 

| Test Case | Solar Voltage (A0) | Battery Voltage (A1) | MCU Power Source | Charging Status |
| :--- | :--- | :--- | :--- | :--- |
| **Peak Sun** | 4.20 V | 3.10 V | **SOLAR** | **ACTIVE** |
| **Low Sun / High Batt** | 3.00 V | 3.80 V | **SOLAR** | **IDLE** |
| **Dusk / Night** | 1.10 V | 3.60 V | **BATTERY** | **IDLE** |
| **Critical Power** | 0.80 V | 0.20 V | **BATTERY** | **ACTIVE** |
| **Threshold Limit** | 2.51 V | 2.10 V | **SOLAR** | **ACTIVE** |

#### **Logic Validation Criteria:**
* **Power Source Selection**: The system correctly toggles to **SOLAR** whenever $V_{solar} > 2.5V$; otherwise, it defaults to **BATTERY**.
* **Charging Activation**: Charging logic is only engaged when $V_{solar} > V_{batt}$, ensuring power only flows toward the lower potential.

### Code
```c
/* * ENCE 3210 – Microprocessors 1
 * Lab 4 - Question 2: Solar Charge Controller Logic
 */

void setup() {
  Serial.begin(9600);
  Serial.println("System Initialized: Solar Charge Controller");
}

void loop() {
  // Read analog voltages (0 - 1023) and map to 0 - 5.0V
  float solarV = analogRead(A0) * (5.0 / 1023.0);
  float batteryV = analogRead(A1) * (5.0 / 1023.0);

  // Requirement: Power Source Logic
  if (solarV > 2.5) {
    Serial.print("MCU Source: SOLAR | ");
  } else {
    Serial.print("MCU Source: BAT
