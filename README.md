# IT254-ai-access-control-human-gate
IT 254 Final Project — AI-Arduino Human Detection Access Control System

## 🔐 AI Access Control System
Human vs. Non-Human Detection for Smart Entry (Arduino + Computer Vision)

## 📘Overview
This project integrates Arduino hardware with AI-based computer vision to create a smart, privacy-aware access control system. The door controller remains idle until an infrared (IR) proximity sensor detects presence. Upon IR trigger, the connected computer activates a webcam, analyzes the captured frames locally, and classifies the scene as “Human” or “Non-Human” (e.g., pets, vehicles, shadows, inanimate objects).
No confidence threshold is used to grant access; instead, a strict categorical decision (Human / Non-Human) determines whether the Arduino should unlock or remain locked (or alert).

## 🧩 System Architecture
            ┌──────────────────────────────┐
            │      Presence Sensing        │
            │   Arduino (IR)               │
            │   - IR proximity sensor      │
            │   - Event trigger (IR)       │
            └──────────────┬───────────────┘
                           │  Serial
                           ▼
            ┌──────────────────────────────┐
            │     AI Classification        │
            │   AI Host (Python)           │
            │   - Webcam via OpenCV        │
            │   - Human vs Non-Human model │
            └──────────────┬───────────────┘
                           │  Serial
                           ▼
            ┌──────────────────────────────┐
            │        Door Control          │
            │   Arduino (Act.)             │
            │   - Servo / Relay control    │
            │   - Unlock / lock / alarm    │
            └──────────────────────────────┘
                           ▲
                           │
                 (Idle until IR trigger)


## Decision policy:

Human detected → Send an approval signal to the Arduino to unlock (timed relock optional).
Non-Human detected → Keep locked and optionally trigger an alert (buzzer/LED/log).

## 💡 Key Features  

- ⚡ **Energy-aware workflow:** Camera only activates when IR sensor is triggered.  
- 🧠 **Local, categorical decision:** Human vs. Non-Human (no confidence threshold tuning).  
- 🧩 **Modular integration:** Hardware (Arduino IR + actuator) and software (OpenCV + local model).  
- 🍀 **Extensible:** Add voice/RFID for multi-factor verification; add local logging or dashboards.  


## 🧰Requirements

Hardware
Arduino (Uno/Mega or equivalent)
IR proximity sensor module
Webcam (USB)
Servo motor or relay module for lock control
Optional: buzzer/LED for alerts

Software

Python 3.x
Arduino IDE (for board firmware upload)
Computer vision stack (OpenCV + a lightweight classification/detection model)
Python Dependencies (install via pip)
pip install opencv-python pyserial torch ultralytics

Accounts / Ecosystem (as applicable)
GitHub (version control and public repository for review)
Optional: Hugging Face (to obtain a lightweight model)
Optional: Google Teachable Machine (to produce a simple Human/Non-Human classifier)

## 🔄Operating Flow

IR Sensor Trigger: Arduino detects presence and notifies the AI host.
Camera Activation: The AI host opens the webcam and captures a frame/short burst.
Local Classification: The AI model categorizes the scene as Human or Non-Human.
Action: The AI host sends the categorical result back to Arduino.
Human → unlock (servo/relay), optional auto-relock timer
Non-Human → remain locked, optional alert
(Optional) Logging: Record timestamp, decision, and optional thumbnail locally.

## 📂 Project Structure  

```
/ (repo root)
├─ README.md
├─ /docs → diagrams, design notes, dataset notes
├─ /hardware → wiring notes, IR module datasheet, actuator specs
├─ /models → model card/notes; pointers to source (no binaries if large)
└─ /scripts → runtime and utility scripts (documentation only)
```

## 🔒Safety & Privacy

On-device inference (no cloud upload by default).
Camera runs only after IR trigger to minimize continuous recording.
Clear, auditable Human/Non-Human policy reduces misconfiguration risks.

## 👤Author Statement
This repository and README were written individually by me and reflect my own understanding and system design. The repository is public for academic evaluation.

## 📜License
Released under the MIT License (or your preferred license).
