# Design Stage

## System Planning

The design stage is the most important part of the project because it defines how the entire system will function. During this phase, all hardware components and their interactions were carefully planned.

A block diagram was created to visualize how signals move through the system. The microprocessor was selected as the central controller responsible for handling all operations.

## Component Selection

Each component was selected based on functionality and compatibility:

• IMU Sensor — to detect motion and orientation  
• OLED Display — to show processed data visually  
• Potentiometers — to provide adjustable analog inputs  
• Push Buttons — to give digital control inputs  
• DAC Circuit — to convert digital signals into analog output  
• Communication Modules — for device interfacing  

## Signal Flow Design

The system processes signals in the following sequence:

1. Input devices provide analog and digital signals  
2. The microprocessor reads the signals  
3. Analog signals are converted into digital values  
4. Digital data is processed based on programmed logic  
5. Output is displayed or converted back to analog  

## Design Considerations

Several important factors were considered:

• Stable power distribution  
• Noise reduction in analog circuits  
• Compact PCB layout  
• Easy debugging and testing  
• Expandability for future improvements  
