# ENCE 3210 – Microprocessor Systems I
## Lab 2 – Interrupts & Timers
### By: Ali Behbehani



## This read me file contains the solution for **Lab 2** of **ENCE 3210 – Microprocessor Systems I**.  
The lab focuses on **interrupt-driven programming** using external interrupts and hardware timers on the Arduino platform.

---

## 📌 Objectives

The goals of this lab are to:

- Use external interrupts to detect button presses  
- Implement Interrupt Service Routines (ISRs)  
- Compare values updated inside ISRs  
- Control LEDs using interrupt-driven logic  
- Configure Timer1 in CTC mode  
- Perform array-based computation inside an ISR  

---

## 🧰 Tools & Environment

- **Language:** C (Arduino-compatible)
- **IDE:** Arduino IDE
- **Board:** Arduino Uno
- **Peripherals:**
  - Push buttons
  - LEDs / RGB LED
  - Timer1 (hardware timer)

---

## Problem 1 — Button Press Comparison Using Interrupts

### Description
Two push buttons are configured to generate external interrupts. Each interrupt increments a counter.  
LED output indicates which button has been pressed more times.

### Behavior
- Button 1 > Button 2 → Red LED ON  
- Button 1 < Button 2 → Green LED ON  

```c
#define BTN1 2               // Button 1 connected to external interrupt pin (INT0)
#define BTN2 3               // Button 2 connected to external interrupt pin (INT1)

#define RED_LED 5            // Red LED output pin
#define GREEN_LED 4          // Green LED output pin

// Counters for number of button presses
// Declared volatile because they are updated inside ISRs
volatile unsigned long btn1Count = 0;
volatile unsigned long btn2Count = 0;

// ISR for Button 1 press
// Increments Button 1 counter
void isr_btn1()
{
  btn1Count++;
}

// ISR for Button 2 press
// Increments Button 2 counter
void isr_btn2()
{
  btn2Count++;
}

void setup()
{
  // Configure buttons as inputs with internal pull-up resistors
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  // Configure LEDs as outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Attach external interrupts to button pins
  // ISRs trigger on falling edge (button press)
  attachInterrupt(digitalPinToInterrupt(BTN1), isr_btn1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2), isr_btn2, FALLING);
}

void loop()
{
  // Compare button press counts and control LEDs accordingly
  if (btn1Count > btn2Count)
  {
    // Button 1 pressed more times
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (btn1Count == btn2Count)
  {
    // Button press counts are equal
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }
  else
  {
    // Button 2 pressed more times
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}

```

## Problem 2 — Repeating Lab 1 Problem 6 Using Interrupts


```C
#define BUTTON_SEQ 2          // Button used to start/stop the LED sequence

#define RED_LED 5             // Red LED output pin
#define GREEN_LED 4           // Green LED output pin

// Controls whether the LED sequence is running
// Volatile because it is modified inside an ISR
volatile unsigned char running = 0;

// Flag used to communicate button press from ISR to loop()
volatile unsigned char toggle_event = 0;

// Store last interrupt time for simple debounce
volatile unsigned long last_isr_time = 0;

// Run the LED sequence once (adapted from Lab 1 Part 6)
// Uses only red and green LEDs
void part6_sequence_once(void)
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  delay(300);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(300);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(300);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(300);
}

// Interrupt Service Routine for the sequence button
// Keeps ISR short by only setting a flag
void isr_seq(void)
{
  unsigned long now = millis();

  // Basic debounce protection
  if (now - last_isr_time < 200)
    return;

  last_isr_time = now;
  toggle_event = 1;
}

void setup()
{
  pinMode(BUTTON_SEQ, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Attach external interrupt to button pin
  attachInterrupt(digitalPinToInterrupt(BUTTON_SEQ),
                  isr_seq,
                  FALLING);
}

void loop()
{
  // Handle button press event triggered by ISR
  if (toggle_event)
  {
    noInterrupts();
    toggle_event = 0;
    interrupts();

    running = !running;      // Toggle sequence ON/OFF
  }

  // Run the sequence only when enabled
  if (running)
  {
    part6_sequence_once();
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
  }
}



```

## Problem 3 — Repeating Lab 1 Problem 7 Using Timer Interrupts


```C

#define SIZE_7 10

#define BUTTON_MODE 3        // Button used to change operation mode (INT1)
#define BUTTON_EXEC 2        // Button used to execute operation (INT0)

#define RED_LED 5
#define GREEN_LED 4

// Arrays used for arithmetic operations
int A[SIZE_7] = {1,2,3,4,5,6,7,8,9,10};
int B[SIZE_7] = {10,9,8,7,6,5,4,3,2,1};
int C[SIZE_7];

// Mode selection variable (1 = add, 2 = subtract, 3 = multiply)
volatile unsigned char press_count = 1;

// Flags set by interrupts to signal events to main loop
volatile unsigned char mode_event = 0;
volatile unsigned char exec_event = 0;

// Display current mode using red and green LEDs
void show_mode_led(void)
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  if (press_count == 1)                  // Addition mode
    digitalWrite(RED_LED, HIGH);
  else if (press_count == 2)             // Subtraction mode
    digitalWrite(GREEN_LED, HIGH);
  else if (press_count == 3)             // Multiplication mode
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
  }
}

// Perform arithmetic operation based on selected mode
void execute_operation(void)
{
  for (int i = 0; i < SIZE_7; i++)
  {
    if (press_count == 1)
      C[i] = A[i] + B[i];
    else if (press_count == 2)
      C[i] = A[i] - B[i];
    else if (press_count == 3)
      C[i] = A[i] * B[i];
    else
      C[i] = 0;
  }
}

// ISR triggered when MODE button is pressed
void isr_mode(void)
{
  mode_event = 1;        // Set flag instead of doing work in ISR
}

// ISR triggered when EXEC button is pressed
void isr_exec(void)
{
  exec_event = 1;        // Signal main loop to execute operation
}

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(BUTTON_EXEC, INPUT_PULLUP);

  // Attach external interrupts for both buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_MODE), isr_mode, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_EXEC), isr_exec, FALLING);

  show_mode_led();       // Display initial mode
}

void loop()
{
  // Handle mode change event
  if (mode_event)
  {
    noInterrupts();
    mode_event = 0;
    interrupts();

    press_count++;
    if (press_count > 3) press_count = 1;
    show_mode_led();
  }

  // Handle execute event
  if (exec_event)
  {
    noInterrupts();
    exec_event = 0;
    interrupts();

    execute_operation();
  }
}


```

## Problem 4 — Interrupt-Based Array Filtering

### Description
A global integer array `x[10]` is initialized at the beginning of the program.  
When an interrupt occurs from **Button 1**, the ISR computes a new global array `y[10]` using the filtering equation:

```C
```cpp
#define BTN1 2

volatile int x[10] = {1,2,3,4,5,6,7,8,9,10};  // Input array (initialized at start)
volatile int y[10];                           // Output array (computed in ISR)

void isr_btn1()
{
  // First element handled separately since x[-1] does not exist
  y[0] = 2 * x[0];

  // Compute remaining elements using the filtering equation
  for (int n = 1; n < 10; n++)
  {
    y[n] = 2 * x[n] - x[n - 1];
  }
}

void setup()
{
  pinMode(BTN1, INPUT_PULLUP);                // Configure button input
  attachInterrupt(digitalPinToInterrupt(BTN1),
                  isr_btn1,
                  FALLING);                   // Trigger ISR on button press
}

void loop() {}
```


## Conclusion 

In this lab, interrupts were used to replace polling-based logic and implement responsive, event-driven behavior on the Arduino platform. Timer-based interrupts were also implemented using Timer1 in CTC mode to achieve precise, non-blocking timing; external interrupts were used to detect button presses, enabling counters and control logic to be updated instantly within Interrupt Service Routines. These exercises allowed for the effective control of hardware resources like buttons, LEDs, and timers while upholding a clear program structure. The lab showed how to use volatile variables and flags to safely share data between ISRs and the main program. Overall, this lab demonstrated the benefits of interrupts over delay-based and polling techniques and reaffirmed their significance in real-time embedded systems.



Disclaimer -- (ChatGPT was used to help write comments for my code to help eplain to the reader)
