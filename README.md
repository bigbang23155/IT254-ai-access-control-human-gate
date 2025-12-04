# AI–Arduino Human Detection Access Control System

**Course:** IT 254 – Final Project  
**Author:** Equinox (2025)  
**Board:** Arduino Mega 2560 (Elegoo)  
**AI Engine:** Python + YOLOv11/YOLOv12 (Local Inference)

---

## 1️⃣ 🤖 Project Overview

This project implements a smart access control system that uses both hardware sensors and AI-based human detection. Before authentication is allowed, the system must first confirm that an actual human is standing in front of the door.

**Workflow Summary:**

1. An ultrasonic distance sensor detects that an object is near.
2. Arduino sends a `TRIGGER` message to a Python AI engine.
3. Python opens the webcam and executes a YOLO model (human/face detection).
4. If a human is detected consistently, the AI sends `APPROVED` to Arduino.
5. Arduino activates the authentication stage:
   ●  RFID card reader (MFRC522) **or**
   ●  4×4 Keypad PIN
6. If either authentication succeeds → the system unlocks the door (MOSFET driver).
7. Door relocks automatically after a defined timeout.

All AI computation is performed **locally** on the host device for privacy and reliability.

---

## 2️⃣ 🔧 System Architecture

```text
 ┌──────────────────────────────┐
 │      Presence Sensing        │
 │   HC-SR04 Ultrasonic Sensor  │
 └──────────────┬───────────────┘
                │  Serial (USB)
                ▼
 ┌──────────────────────────────┐
 │   AI Classification Engine   │
 │   Python + YOLOv11/YOLOv12   │
 │   - Webcam via OpenCV        │
 │   - Human / Face detection   │
 │   - Sends APPROVED/DENIED    │
 └──────────────┬───────────────┘
                │  Serial (USB)
                ▼
 ┌──────────────────────────────┐
 │      Arduino Access Unit     │
 │  - LCD1602 Display           │
 │  - RFID Authentication       │
 │  - Keypad PIN Authentication │
 │  - Buzzer + LEDs             │
 │  - MOSFET Lock Control       │
 └──────────────────────────────┘
```
## 3️⃣ 🧩 Key Features

● Human Detection Gatekeeping
AI determines whether the approaching object is truly human before authentication is permitted.

● Event-Driven AI Activation
YOLO runs only when triggered by the ultrasonic sensor.

● Dual Authentication Methods
Either RFID or Keypad PIN can unlock the door.

● Clear User Feedback
LCD messages, buzzer tones, and LED indicators provide intuitive system status.

● Auto Relocking & Cooldown Handling
Prevents repeated triggering and ensures stable operation.

● Modular Architecture
Hardware and AI logic are separated and communicate over a simple serial protocol.

## 4️⃣ 🛠️ Hardware Components & Wiring Summary
Core Components

● Arduino Mega 2560 R3  
● HC-SR04 Ultrasonic Sensor  
● MFRC522 RFID Module  
● 4×4 Matrix Keypad  
● LCD1602 (parallel version)  
● Green + Red LEDs  
● Active Buzzer  
● MOSFET / S8050 Transistor Lock Driver + 1N4007  
● 5V regulated power supply  

## Pin Map Overview
```
| **Module**           | **Pin on Arduino Mega 2560**                                 |
|----------------------|---------------------------------------------------------------|
| **Ultrasonic HC-SR04** | TRIG → **D6**, ECHO → **D7**                                 |
| **RFID MFRC522**       | SDA → **D53**, SCK → **D52**, MOSI → **D51**, MISO → **D50**, RST → **D9** |
| **LCD1602 (Parallel)** | RS → **D33**, RW → **D32**, E → **D31**, D4 → **D30**, D5 → **D37**, D6 → **D38** |
| **Keypad 4×4**         | Rows (R1–R4) → **D5, D4, D3, D2**<br>Cols (C1–C4) → **D25, D24, D23, D22** |
| **LED Indicators**     | Green LED → **D34**, Red LED → **D35**                       |
| **Buzzer**             | **D36**                                                      |
| **Lock Driver (MOSFET / S8050)** | Control Pin → **D8**                               |
```

Full details are available in:
docs/Hardware_Wiring_Guide.md

## 5️⃣ ⚙️ Software Requirements
Arduino Side

● Arduino IDE  
● Board: Arduino Mega or Mega 2560  
● Upload: Arduino_Project_Demo.ino  

Python AI Side

● Python 3.8–3.10  
● Required packages:  

pip install ultralytics opencv-python pyserial


● Recommended YOLO model:  

  ● yolov11s-face.pt  
  ● or YOLOv12 (development version)  

Python AI script:
ai_demo.py

## 5️⃣ ⚙️ Repository Structure

```
AI-Access-Control/
├── Arduino_Project_Demo.ino
├── ai_demo.py
├── yolov11s-face.pt          # (Optional in repo)
├── /docs
│   ├── Project_Overview.md
│   ├── Hardware_Wiring_Guide.md
│   └── YOLOv12_Installation.md
└── /runs (optional YOLO outputs)
```

## 7️⃣ 📡 Running the System

1. Connect Arduino via USB.
2. Run Python AI engine:
python ai_demo.py

3. Approach the HC-SR04 sensor.
4. AI activates and detects human presence.
5. If human confirmed → authentication enabled.
6. Authenticate using:
  ● Whitelisted RFID card, or
  ● Correct keypad PIN
7. Door unlocks → relocks automatically.

## 8️⃣ 🔐 Limitations & Future Enhancements

● Current face/human detection is not resistant to spoofing.  
● Environmental lighting can affect detection accuracy.  
● Future features may include:  
  ● NFC support  
  ● Anti-spoof face detection  
  ● Web dashboard & logging  
  ● Multi-factor voice authentication  

## 9️⃣ 🚀 Author Statement

This project was fully designed and implemented by Equinox as part of the IT 254 course final project.
The README, documentation, Arduino firmware, and Python code reflect my own work and understanding of embedded systems, AI, and sensor integration.
