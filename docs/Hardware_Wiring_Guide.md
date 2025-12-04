# Hardware Wiring Guide

AI–Arduino Human Detection Access Control System
Version 3.8 — Hardware Integration Manual

This document provides a complete wiring reference for all electronic components used in the system, including the Arduino Mega 2560, HC-SR04 (ultrasonic sensor), MFRC522 RFID module, 4×4 matrix keypad, 1602 parallel LCD, LEDs, buzzer, and the MOSFET-controlled door lock output.

No external diagrams are used; all pin mappings are expressed as text tables for clarity and easy GitHub rendering.

##　1. Required Components
### Microcontroller
●　Arduino Mega 2560 R3 (Elegoo version recommended)  

###　Sensors & Modules
●　HC-SR04 Ultrasonic Distance Sensor  
●　MFRC522 RFID Module  
●　4×4 Membrane Keypad  
●　LCD 1602 (Parallel version, NOT I²C)  
●　Passive Buzzer or Active Buzzer  
●　LEDs (Green, Red)  
●　N-MOSFET or Relay Module (for door lock simulation)  

### Supporting Components
●　2 kΩ resistor (LCD contrast control VO pin)  
●　Multiple female-to-male and male-to-male jumper wires  
●　Breadboard (recommended)  

## 2. Arduino Pin Assignment Summary
````
| Component                | Pin Type       | Arduino Pin                 |
| ------------------------ | -------------- | --------------------------- |
| Ultrasonic TRIG          | Digital Output | D6                          |
| Ultrasonic ECHO          | Digital Input  | D7                          |
| Green LED                | Digital Output | D34                         |
| Red LED                  | Digital Output | D35                         |
| Buzzer                   | Digital Output | D36                         |
| Door Lock MOSFET Control | Digital Output | D8                          |
| RFID SS                  | SPI            | D53                         |
| RFID RST                 | Digital        | D9                          |
| Keypad Rows (R1–R4)      | Digital        | D5, D4, D3, D2              |
| Keypad Columns (C1–C4)   | Digital        | D25, D24, D23, D22          |
| LCD RS                   | Digital        | D33                         |
| LCD EN                   | Digital        | D32                         |
| LCD D4                   | Digital        | D31                         |
| LCD D5                   | Digital        | D30                         |
| LCD D6                   | Digital        | D37                         |
| LCD D7                   | Digital        | D38                         |
| LCD VDD                  | Power          | 5V                          |
| LCD VSS                  | Power          | GND                         |
| LCD VO                   | Power          | Through 2kΩ resistor to GND |
````

## 3. Wiring Details by Component

### 3.1 Ultrasonic Sensor (HC-SR04)

````
| HC-SR04 Pin | Connect To |
| ----------- | ---------- |
| VCC         | 5V         |
| GND         | GND        |
| TRIG        | D6         |
| ECHO        | D7         |
````
Notes:

  ● A measurement interval of ~250 ms is used in firmware for stable readings.  
  ● ECHO timing is extended in software to avoid false timeouts.  

### 3.2 RFID Reader (MFRC522)

````
| MFRC522 Pin | Arduino Pin    |
| ----------- | -------------- |
| VCC         | 3.3V           |
| GND         | GND            |
| RST         | D9             |
| SDA (SS)    | D53            |
| MOSI        | D51 (Mega SPI) |
| MISO        | D50            |
| SCK         | D52            |
````
Important:

  ● MFRC522 must be powered at 3.3V.  
  ● Wrong voltage may damage the module or cause unstable UID reading.  

### 3.3 4×4 Keypad Wiring

Your keypad was physically verified, and the correct working mapping is:

#### Row pins (left side, top → bottom):
````
| Row | Arduino Pin |
| --- | ----------- |
| R1  | D5          |
| R2  | D4          |
| R3  | D3          |
| R4  | D2          |
````
#### Column pins (right side, top → bottom):
````
| Column | Arduino Pin |
| ------ | ----------- |
| C1     | D25         |
| C2     | D24         |
| C3     | D23         |
| C4     | D22         |
````
#### Key Layout (for verification)
````
1  2  3  A
4  5  6  B
7  8  9  C
*  0  #  D
````
This pin mapping matches the final version validated on hardware.

### 3.4 LCD 1602 (Parallel, 4-bit Mode)

The LCD uses 4-bit parallel communication.
Below is the full wiring table:
````
| LCD Pin         | Function        | Arduino Pin              |
| --------------- | --------------- | ------------------------ |
| VSS             | Ground          | GND                      |
| VDD             | +5V Power       | 5V                       |
| VO              | Contrast        | GND through 2kΩ resistor |
| RS              | Register Select | D33                      |
| RW              | Read/Write      | GND                      |
| EN              | Enable          | D32                      |
| D4              | Data            | D31                      |
| D5              | Data            | D30                      |
| D6              | Data            | D37                      |
| D7              | Data            | D38                      |
| LED+ (optional) | Backlight       | 5V                       |
| LED− (optional) | Backlight       | GND                      |
````

Notes:

  ● A 2kΩ resistor connected between VO → GND is required for readable text.  
  ● If the screen only shows black boxes, contrast is incorrect.  

### 3.5 LEDs (Status Indicators)
````
| LED                 | Arduino Pin | Notes                     |
| ------------------- | ----------- | ------------------------- |
| Green LED Anode (+) | D34         | Indicates system approval |
| Red LED Anode (+)   | D35         | Indicates denial / error  |
| LED Cathodes (−)    | GND         | Common ground             |
````
### 3.6 Buzzer
````
| Buzzer Pin   | Arduino Pin |
| ------------ | ----------- |
| + (Positive) | D36         |
| − (Negative) | GND         |
````
● Works for both active and passive buzzers.  
● Short pulses are used for feedback (OK tone / error tone).  

### 3.7 MOSFET / Relay (Door Lock Simulation)
````
| Function     | Arduino Pin                 |
| ------------ | --------------------------- |
| Gate / IN    | D8                          |
| Drain / NO   | Connected to simulated lock |
| Source / COM | Ground                      |
````

Notes:

● In this project, the MOSFET is used to simulate unlocking (no external high-power lock).  
● A real door lock would require a flyback diode and external 12V supply.  

## 4. Power Considerations
● All components run on 5V, except:  
  ● MFRC522 must be powered at 3.3V  
● Use Arduino Mega’s onboard 5V regulator only for low-power modules.  
● For real electromagnetic locks:  
  → Use external power + MOSFET + diode.  

## 5. System Wiring Overview
This summarizes the entire wiring in a single list:
````
Ultrasonic:
  TRIG → D6
  ECHO → D7

RFID MFRC522:
  RST → D9
  SDA → D53
  MOSI → D51
  MISO → D50
  SCK → D52
  VCC → 3.3V
  GND → GND

Keypad:
  R1 → D5
  R2 → D4
  R3 → D3
  R4 → D2
  C1 → D25
  C2 → D24
  C3 → D23
  C4 → D22

LCD1602 (Parallel):
  RS → D33
  EN → D32
  D4 → D31
  D5 → D30
  D6 → D37
  D7 → D38
  VO → 2kΩ → GND
  VDD → 5V
  VSS → GND
  RW → GND

Status LEDs:
  Green → D34
  Red → D35

Buzzer:
  + → D36
  - → GND

MOSFET / Relay:
  Control → D8
````

## 6. Final Notes

● All grounds must be connected together (GND common reference).  
● Keep ultrasonic sensor wires short to avoid noisy readings.  
● Use 3.3V for RFID; never connect it to 5V.  
● LCD contrast resistor (2kΩ) is required—without it, characters will not appear.  
