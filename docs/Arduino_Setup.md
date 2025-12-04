# Arduino Setup Guide  
AI–Arduino Human Detection Access Control System  
Arduino Firmware Version 3.8

This document provides all steps required to prepare, configure, and upload the Arduino firmware for the access-control system. It also explains how the Arduino communicates with the Python AI module and how each hardware device is initialized.

---

# 1. Required Hardware

The following components must be available before uploading the firmware:

- Arduino Mega 2560 R3 (recommended for high pin count)
- HC-SR04 ultrasonic distance sensor
- MFRC522 RFID module
- 4×4 matrix keypad
- LCD1602 (parallel mode, not I²C)
- MOSFET/S8050 transistor + 1N4007 diode (lock driver)
- Active or passive buzzer
- Red and green LEDs
- USB Type-B cable
- Breadboard and jumper wires

---

# 2. Install Arduino IDE

Download the Arduino IDE:  
https://www.arduino.cc/en/software

After installation:

### Select the board  
Tools → Board → Arduino Mega or Mega 2560


### Select the correct COM port  
Tools → Port → COMx (Windows)
Tools → Port → /dev/cu.usbmodemXXX (macOS)


---

# 3. Required Arduino Libraries

Install the following libraries using the Arduino Library Manager:
````
| Library       | Purpose                        |
|---------------|--------------------------------|
| MFRC522       | Communication with RFID module |
| Keypad        | 4×4 matrix keypad scanning     |
| LiquidCrystal | LCD1602 parallel display       |
| SPI           | Required by MFRC522            |
| Wire          | I²C support (if needed)        |
````
### Installing via Library Manager
Tools → Manage Libraries

Search for each library name and install.

---

# 4. Uploading the Firmware

1. Open the file:  
   `Arduino_Project_Demo.ino`
2. Connect Arduino Mega via USB.
3. Set board and COM port.
4. Click:
   - **Verify**  
   - **Upload**

If upload is successful, the Serial Monitor will show:

System Ready
Waiting Echo...


---

# 5. Finalized Pin Mapping (Firmware v3.8)

Below is the complete pin mapping used by the system.

## LCD1602 (Parallel Mode)
````
| LCD Pin | Function       | Arduino Pin        |
|---------|----------------|--------------------|
| RS      | Command select | 33                 |
| EN      | Enable         | 32                 |
| D4      | Data           | 31                 |
| D5      | Data           | 30                 |
| D6      | Data           | 37                 |
| D7      | Data           | 38                 |
| VSS     | Ground         | GND                |
| VDD     | Power          | 5V                 |
| VO      | Contrast       | 2kΩ resistor → GND |
| RW      | Read/Write     | GND                |
````
---

## HC-SR04 Ultrasonic Sensor
````
| Pin  | Arduino  |
|------|----------|
| VCC  | 5V       |
| GND  | GND      |
| TRIG | 6        | 
| ECHO | 7        |
````
---

## RFID MFRC522
````
| Pin  | Arduino Mega |
|------|--------------|
| SDA  | 53           |
| SCK  | 52           |
| MOSI | 51           |
| MISO | 50           |
| RST  | 9            |
| VCC  | 3.3V         |
| GND  | GND          |
````
---

## 4×4 Keypad

### Row pins (left side)
````
| Row | Arduino Pin |
|-----|-------------|
| R1  | 5           |
| R2  | 4           |
| R3  | 3           |
| R4  | 2           |
````
### Column pins (right side)
````
| Column | Arduino Pin |
|--------|-------------|
| C1     |    25       |
| C2     |    24       |
| C3     |    23       |
| C4     |    22       |
````
---

## Status LEDs / Buzzer / MOSFET
````
| Component            | Pin |
|----------------------|-----|
| Green LED            | 34  |
| Red LED              | 35  |
| Buzzer               | 36  |
| MOSFET / Lock Output | 8   |
````
---

# 6. Serial Communication with Python AI Module

The Arduino and Python script communicate through USB serial.

### Messages sent from Arduino → Python
TRIGGER // ultrasonic detected presence


### Messages sent from Python → Arduino
APPROVED // YOLO detected a human
DENIED // YOLO rejected or timed out


### Baud Rate
Both sides must use:
9600 baud


### Important
The Arduino Serial Monitor **must be closed** when running the Python script,  
otherwise Python will not be able to open the COM port.

---

# 7. Troubleshooting

### LCD displays only black boxes
- VO contrast pin must use a ~2kΩ resistor to GND.
- Check wiring of RS, EN, and D4–D7.

### Keypad returns incorrect characters
- Ensure row/column pins follow the exact mapping in this document.
- If wiring is reversed, keypad scanning will fail.

### RFID module not responding
- MFRC522 **must** be powered at **3.3V**.
- Reinsert wires firmly; loose SPI wiring causes failures.

### Python cannot access COM port
- Close Arduino Serial Monitor.
- Confirm the COM port matches Python settings.

---

# 8. Project Directory Structure (Arduino Section)
````
/docs
├── Arduino_Setup.md
├── Hardware_Wiring_Guide.md
├── YOLO_Installation_Windows.md
├── YOLO_Installation_macOS.md
├── Python_AI_Setup.md
└── index.md

/firmware
└── Arduino_Project_Demo.ino

/python
└── ai_demo.py

README.md
````


---

# 9. Next Steps

After completing Arduino setup:

- Configure Python environment  
- Install YOLO (CPU or GPU version)  
- Run the AI detection engine with:  
python ai_demo.py

- Verify that TRIGGER → APPROVED/DENIED serial logic works

