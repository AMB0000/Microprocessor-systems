# ENCE 3210 – Microprocessor Systems I  
## Lab 1 – C Programming & Arduino Implementation

This repository contains the solution for **Lab 1** of **ENCE 3210 – Microprocessor Systems I**.  
The lab focuses on fundamental C programming concepts and basic hardware interaction using the Arduino IDE.

---

## 📌 Objectives

The goals of this lab are to:
- Practice writing C functions and using arrays
- Perform mathematical computations using loops
- Work with global variables and memory placement
- Detect patterns and characters in arrays
- Control RGB LEDs and buttons using GPIO
- Implement simple embedded logic following lab specifications

---

## 🧰 Tools & Environment

- **Language:** C (Arduino-compatible)
- **IDE:** Arduino IDE
- **Board:** Arduino (e.g., Uno / Nano)
- **Libraries Used:**  
  - `<math.h>` (as allowed by lab hint)

---

##  Part 1a

### This part defines a function that takes an integer input and returns twice the square of that number. The function is reusable and separates the calculation logic from the rest of the program, which follows good C programming practice.
```C



int twice_square(int x)
{
    return 2 * x * x;
}

```

##  Part 1b

### In this part, the function from Part 1(a) is used to calculate twice the square of all integers from 0 to 511. Each result is stored in an array, demonstrating the use of loops and arrays for bulk data processing.

``` C

#define SIZE_1 512

int results_part1[SIZE_1];

void part1b(void)
{
    for (int i = 0; i < SIZE_1; i++)
    {
        results_part1[i] = twice_square(i);
    }
}

```

## Part 2

### This part calculates the cosine of every integer angle between 0 and 360 degrees. Since the cos() function works with radians, each degree value is first converted to radians. The computed cosine values are stored in an array for later use.

``` C

#include <math.h>

#define SIZE_2 361
#define PI 3.14159265358979323846

double cos_values[SIZE_2];

void part2(void)
{
    for (int deg = 0; deg <= 360; deg++)
    {
        double rad = deg * (PI / 180.0);
        cos_values[deg] = cos(rad);
    }
}


```

## Part 3

### Here, a global array of 50 integers is searched to find the second largest value. The result is stored in the memory location immediately following the array, demonstrating array traversal, comparison logic, and careful memory placement.

``` C

#include <limits.h>

#define SIZE_3 50

int arr_part3[SIZE_3 + 1] = {
    12, 45, 3, 89, 23, 67, 89, 10, 5, 44,
    22, 78, 34, 56, 90, 1, 8, 90, 33, 66,
    77, 55, 21, 43, 99, 88, 11, 2, 4, 6,
    7, 9, 13, 14, 15, 16, 17, 18, 19, 20,
    25, 26, 27, 28, 29, 30, 31, 32, 35, 36
};

void part3(void)
{
    int max = arr_part3[0];
    int second_max = INT_MIN;

    for (int i = 1; i < SIZE_3; i++)
    {
        int v = arr_part3[i];

        if (v > max)
        {
            second_max = max;
            max = v;
        }
        else if (v < max && v > second_max)
        {
            second_max = v;
        }
    }

    arr_part3[SIZE_3] = second_max;
}


```


## Part 4 

### This part scans a character array to detect the letters ‘R’, ‘G’, and ‘B’. The program counts how many times each letter appears and turns on the corresponding red, green, or blue LED if that letter is found in the array.

``` C
#define RED_LED 5
#define GREEN_LED 6
#define BLUE_LED 7

char letters_part4[] = "RRGBGGBRBRGRGB";

int r_count = 0;
int g_count = 0;
int b_count = 0;

void part4(void)
{
    r_count = 0;
    g_count = 0;
    b_count = 0;

    for (int i = 0; letters_part4[i] != '\0'; i++)
    {
        char c = letters_part4[i];

        if (c == 'R') r_count++;
        else if (c == 'G') g_count++;
        else if (c == 'B') b_count++;
    }

    if (r_count > 0) digitalWrite(RED_LED, HIGH);
    if (g_count > 0) digitalWrite(GREEN_LED, HIGH);
    if (b_count > 0) digitalWrite(BLUE_LED, HIGH);
}
```

## Part5 

### In this part, the program searches an integer array for a specific three-element pattern stored in another array. The number of pattern occurrences is counted. If the pattern is found at least once, the green LED turns on; otherwise, the red LED turns on at the end of the program.

``` C

int data_part5[] = {1, 2, 3, 1, 2, 3, 4, 1, 2, 3};
int pattern_part5[3] = {1, 2, 3};

int pattern_count = 0;

void part5(void)
{
    pattern_count = 0;

    int n = (int)(sizeof(data_part5) / sizeof(data_part5[0]));

    for (int i = 0; i <= n - 3; i++)
    {
        if (data_part5[i] == pattern_part5[0] &&
            data_part5[i + 1] == pattern_part5[1] &&
            data_part5[i + 2] == pattern_part5[2])
        {
            pattern_count++;
        }
    }

    if (pattern_count > 0)
        digitalWrite(GREEN_LED, HIGH);
    else
        digitalWrite(RED_LED, HIGH);
}

```

## Part 6a

### This part implements a predefined RGB LED sequence:
R → RG → RGB → GB → B → RB.
Each step is displayed with a short delay, demonstrating sequential control of multiple outputs.

``` C

void set_rgb(int r, int g, int b)
{
    digitalWrite(RED_LED, r);
    digitalWrite(GREEN_LED, g);
    digitalWrite(BLUE_LED, b);
}

void part6_sequence_once(void)
{
    set_rgb(HIGH, LOW,  LOW);
    delay(300);

    set_rgb(HIGH, HIGH, LOW);
    delay(300);

    set_rgb(HIGH, HIGH, HIGH);
    delay(300);

    set_rgb(LOW,  HIGH, HIGH);
    delay(300);

    set_rgb(LOW,  LOW,  HIGH);
    delay(300);

    set_rgb(HIGH, LOW,  HIGH);
    delay(300);
}
```

## Part 6b

### The LED sequence from Part 6(a) is repeated continuously while a button is active. When the button is pressed again, the sequence stops. This part demonstrates basic button input handling and state control.
``` C

#define BUTTON_SEQ 2

int pressed_edge(int pin)
{
    static int last_state = LOW;
    int now = digitalRead(pin);

    int edge = (now == HIGH && last_state == LOW);
    last_state = now;

    if (edge) delay(25);
    return edge;
}

void part6b(void)
{
    static int running = 0;

    if (pressed_edge(BUTTON_SEQ))
    {
        running = !running;
    }

    if (running)
    {
        part6_sequence_once();
    }
}

```

## Part 7a

### Three arrays of size 10 are created. The first two arrays contain integer values, while the third array is reserved to store the results of arithmetic operations.

```C

#define SIZE_7 10

int A[SIZE_7] = {1,2,3,4,5,6,7,8,9,10};
int B[SIZE_7] = {10,9,8,7,6,5,4,3,2,1};
int C[SIZE_7];
```

## Part 7b

### When the button is pressed once, the program selects the addition operation. The red LED turns on to indicate that addition mode is active.

```C
#define BUTTON_MODE 3

int press_count = 0;

void show_mode_led(void)
{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    if (press_count == 1) digitalWrite(RED_LED, HIGH);
    else if (press_count == 2) digitalWrite(GREEN_LED, HIGH);
    else if (press_count == 3) digitalWrite(BLUE_LED, HIGH);
}

void part7b(void)
{
    if (pressed_edge(BUTTON_MODE))
    {
        press_count++;
        if (press_count > 3) press_count = 1;
        show_mode_led();
    }
}

```

## Part 7c

### When the button is pressed twice, the program selects the subtraction operation. The green LED turns on to indicate subtraction mode.

```C


void part7c(void)
{
    show_mode_led();
}

```

## Part 7d

### When the button is pressed three times, the program selects the multiplication operation. The blue LED turns on to indicate multiplication mode.

```C

void part7d(void)
{
    show_mode_led();
}

```

## Part 7e

### When a second button is pressed, the selected arithmetic operation is applied element-by-element to the first two arrays. The results are stored in the third array. All LEDs turn off during computation, and once all results are saved, all LEDs turn on to signal completion.

```C

#define BUTTON_EXEC 4

void leds_off(void)
{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
}

void leds_all_on(void)
{
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
}

void execute_operation(void)
{
    leds_off();

    for (int i = 0; i < SIZE_7; i++)
    {
        if (press_count == 1) C[i] = A[i] + B[i];
        else if (press_count == 2) C[i] = A[i] - B[i];
        else if (press_count == 3) C[i] = A[i] * B[i];
        else C[i] = 0;
    }

    leds_all_on();
}

void part7e(void)
{
    if (pressed_edge(BUTTON_EXEC))
    {
        execute_operation();
    }
}

```
## Conclusion 

This lab demonstrated core C programming concepts applied to a microprocessor-based environment. Through the use of functions, arrays, loops, and global variables, each task followed the lab specifications and reinforced structured problem-solving. Basic interaction with hardware components such as RGB LEDs and push buttons was also implemented, providing practical experience with embedded system logic and control.

