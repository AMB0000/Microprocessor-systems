# Introduction


##  Introduction
In this project, I designed and built a Digital Signal Processing (DSP) shield for the Arduino Uno as part of my microprocessors course. The goal was to go through the full engineering process — from learning the theory, to designing the hardware, manufacturing it, assembling it, and finally testing it with real code and lab equipment.

---

##  What is DSP?
Digital Signal Processing (DSP) is the manipulation of signals — such as sound, voltage, or sensor data — using digital techniques. Instead of working with continuous analog signals, DSP converts them into discrete digital values that a microprocessor can process. This allows us to filter, analyze, and generate signals with high precision. In this project, I applied DSP concepts by using the Arduino Uno to generate and observe waveforms through a Digital-to-Analog Converter (DAC) built into the shield.

---

##  What is the DSP Shield?
The DSP shield is a custom PCB (Printed Circuit Board) that sits on top of the Arduino Uno and expands its capabilities. It was designed to allow students to experiment with real embedded systems concepts including:

- **Interrupt routines** — responding to events in real time
- **Timers** — controlling the timing of operations
- **Mixed signals** — working with both analog and digital signals through ADCs (Analog-to-Digital Converters) and DACs (Digital-to-Analog Converters)
- **Digital communications** — using protocols such as I2C, SPI, UART, and CAN to communicate between components

---

##  My Process
I began by studying DSP theory and understanding how signals are represented and processed digitally. From there, I used **KiCad** — a professional PCB design software — to draw the schematics and lay out the board. Once the design was complete, I ordered the manufactured PCB from **PCBWay**, a PCB fabrication service. After receiving the boards, I hand-soldered all the components onto the board in class. Finally, I programmed the Arduino to run test code and used a waveform analyzer to observe the output signals on screen and verify the board was working correctly.
