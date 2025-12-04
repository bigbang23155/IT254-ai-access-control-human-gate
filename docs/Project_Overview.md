# Project Overview — AI–Arduino Human Detection Access Control System

**Author:** Equinox  
**Course:** IT 254 – Final Project  
**Year:** 2025  
**Technologies:** Arduino Mega 2560, Python, YOLOv11/YOLOv12, OpenCV, MFRC522 RFID, 4×4 Keypad, LCD1602, HC-SR04

---

## 1. Introduction

This project explores the integration of embedded hardware with localized AI image classification to build a human-aware access control system. Traditional access systems rely solely on RFID cards or PIN codes, which means *anyone holding a valid credential can enter*—even if they are not physically present or if the credential has been stolen.

This project addresses this security gap by introducing an AI-based “human presence” validation layer. Only when the system confirms that a real human is standing in front of the door can authentication (RFID or PIN) proceed.

The design emphasizes privacy, reliability, and real-time interaction through local inference with YOLO.

---

## 2. System Goals

### ✔ Add an AI validation layer before authentication  
YOLO detects whether a human or face is present. If not, the system denies access immediately.

### ✔ Combine embedded sensing with AI perception  
The ultrasonic sensor triggers the AI system only when necessary, improving efficiency.

### ✔ Provide multiple authentication methods  
The system supports:
- RFID card (MFRC522)
- 4×4 keypad PIN

Either method is sufficient for unlocking.

### ✔ Operate with clear user feedback  
LCD, buzzer, and LEDs guide the user step-by-step during authentication.

### ✔ Maintain complete offline/local operation  
No cloud services are needed.  
All inference occurs on-device to protect user privacy.

---

## 3. High-Level System Architecture

```text
 ┌────────────────────────────────────┐
 │  Presence Sensing (Arduino)        │
 │  HC-SR04 Ultrasonic Module         │
 └───────────────────────┬────────────┘
                         │  USB Serial
                         ▼
 ┌────────────────────────────────────┐
 │  AI Classification Engine (PC)     │
 │  Python + YOLOv11/YOLOv12          │
 │  OpenCV Webcam Stream              │
 │  Sends: APPROVED / DENIED         │
 └───────────────────────┬────────────┘
                         │  USB Serial
                         ▼
 ┌────────────────────────────────────┐
 │  Authentication Unit (Arduino)     │
 │  LCD1602 Display                   │
 │  RFID Reader (MFRC522)             │
 │  Keypad PIN System                 │
 │  LEDs + Buzzer                     │
 │  Lock Driver (MOSFET/S8050)        │
 └────────────────────────────────────┘
```
## 4. Core Components & Roles
  ### 4.1 Ultrasonic Sensor (HC-SR04)
  Detects presence within a configurable distance range.  
  Triggers the AI engine only when necessary.  

  ### 4.2 YOLO Human/Face Detection
  Using:  
  ● YOLOv11s-face.pt (preferred for speed + accuracy)  
  ● Or YOLOv12 if supported on GPU  

  The Python script:  
  ● Opens the webcam  
  ● Runs inference in real time  
  ● Draws labeled bounding boxes  
  ● Counts consecutive frames of human detection  
  ● Sends APPROVED or DENIED back to Arduino  

  ### 4.3 Authentication (RFID + Keypad)
  After AI approval:

  ● Scan a registered RFID card  
  or  
  ● Enter a correct PIN code  

Either method unlocks the door.

  ### 4.4 Feedback System
  ● LCD shows system status  
  ● Green/Red LEDs indicate lock state  
  ● Buzzer confirms success or failure  

  ### 4.5 Lock Driver
  MOSFET or S8050 transistor + diode protects the circuit and drives the lock/motor.

## 5. Interaction Flow

```text
[1] Object detected by ultrasonic sensor
        ↓
[2] Arduino sends TRIGGER → Python
        ↓
[3] YOLO opens webcam and detects human
        ↓
  If NO human → DENIED → cooldown
  If YES human → APPROVED
        ↓
[4] Arduino enables authentication stage
        ↓
  User presents RFID card or enters PIN
        ↓
[5] Success → Unlock door for defined time
        ↓
[6] Auto-lock → Return to idle
```
This multi-stage design increases physical security and reduces false activations.

## 6. Security Considerations
❗ Potential attack vectors
● Printed faces or photos may fool naive face detection  
● RFID cards can be cloned  
● PINs can be observed or guessed  

✔ Mitigations in this project
● Local AI validation prevents remote or automated spoofing  
● Dual-layer authentication reduces reliance on a single factor  
● Cooldown prevents repeated brute-force attempts  

✔ Future improvements
● Anti-spoofing (liveness detection)  
● Encrypted RFID (Mifare DESFire/NFC)  
● Integration with database logging  
● Mask detection or multi-person detection  

## 7. Design Strengths
● Modular — AI and hardware logic are separated cleanly  
● Expandable — New sensors or AI models can be added easily  
● Efficient — AI only runs when needed  
● Low-cost — All hardware components are inexpensive  
● Educational — Demonstrates embedded systems + computer vision integration  

## 8. Academic Contribution
This project demonstrates proficiency in:

● Arduino hardware interfacing and embedded programming  
● Python-based real-time AI inference  
● Communication protocol design (serial messaging)  
● Multi-sensor, multi-stage authentication architecture  
● Practical cybersecurity considerations in physical access control  

It fulfills IT 254 requirements by showing real-world system integration using microcontrollers and AI.

## 9. Author Statement
This documentation reflects my own analysis, design decisions, and implementation work. The project was fully created by me for the IT 254 Final Project, including:

● Hardware wiring and circuit design  
● Arduino firmware  
● Python YOLO integration  
● System architecture  
● Technical documentation  
● Testing and refinement  
