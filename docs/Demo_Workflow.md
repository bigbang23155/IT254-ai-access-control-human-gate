# End-to-End Demo Workflow  
AI–Arduino Human Detection Access Control System  
This document describes the full demonstration sequence used to show the system working from detection → AI classification → authentication → unlocking.

---

# 1. Overview

The overall workflow includes:

1. Hardware boot-up  
2. Presence detection using the ultrasonic sensor  
3. Triggering the Python AI module  
4. YOLO-based human/face detection  
5. Authentication stage (RFID or PIN)  
6. Successful unlock and auto-relock  
7. System reset for next cycle  

This workflow is designed for classroom demo, lab evaluation, or video submission.

---

# 2. Prerequisites Before Running the Demo

Ensure the following items are prepared:

### Hardware
- Arduino Mega 2560 connected via USB  
- HC-SR04 ultrasonic sensor mounted facing forward  
- LCD1602 functional with readable contrast  
- MFRC522 RFID module wired correctly (3.3V only)  
- 4×4 keypad verified working  
- LEDs and buzzer connected  
- MOSFET / motor output connected to simulate door lock  

### Software
- Arduino firmware (`Arduino_Project_Demo.ino`) uploaded  
- Python environment activated  
- YOLO model file placed in project root  
- Python script (`ai_demo.py`) configured with correct COM port  

### Serial Monitor
- Must be **closed** before running Python  

---

# 3. Demo Sequence

Below is the official demonstration flow.

---

## Step 1 — Power on the System

When Arduino resets, the LCD should show:

AI Access System
Booting...


After initialization:



System Ready
Waiting Echo...


This indicates the system is actively monitoring presence using the ultrasonic sensor.

---

## Step 2 — Approach the Sensor (Presence Detection)

Walk into the detection zone (5–80 cm).

The LCD displays:



Object Detected
Scan RFID/PIN


Arduino simultaneously sends:



TRIGGER


to Python.

---

## Step 3 — Python AI Module Activates (YOLO)

In the terminal, running:



python ai_demo.py


You should see:



[ACTION] TRIGGER received → starting detection window


Then the webcam window opens.

YOLO begins analyzing frames in real time.

---

## Step 4 — AI Human Detection Outcome

### Case A — Human Detected (Expected)
If YOLO detects a person/face for several consecutive frames:

Terminal shows:



[RESULT] Human confirmed → APPROVED


Python sends:



APPROVED


Arduino updates LCD:



AI: Human OK
Scan RFID/PIN


### Case B — No Human Detected
YOLO times out or sees an object (chair, pet, shadow, etc.):



[TIMEOUT] No human detected → DENIED


Arduino LCD shows:



Access Denied
Not Human / Fail


System returns to idle automatically.

---

# Step 5 — Authentication Stage (MFA Option)

If **APPROVED**, the user may authenticate with **either** method:

## Option A: RFID Card  
Tap a whitelisted UID card.  
LCD shows:



RFID Scanned
<UID>


If authorized:



Access Granted
Door Unlocked


## Option B: PIN Code  
Enter digits on the keypad.  
Press **#** to submit, **\*** to clear.

If correct:



PIN Correct
Door Unlocked


Either method is sufficient to unlock.

---

# Step 6 — Door Unlocking & Indicators

On successful authentication:

- **Green LED turns ON**  
- **Red LED turns OFF**  
- **Buzzer plays success tone**  
- **MOSFET output activates** the simulated lock  
- LCD shows:



Access Granted
Door Unlocked


Unlock remains active for the configured duration (e.g., 3 seconds).

---

# Step 7 — Auto Relock and System Reset

After timeout:



Door Locked
Waiting Echo...


Indicators reset:

- Green LED OFF  
- Red LED ON  
- Buzzer OFF  
- MOSFET output LOW  

System returns to idle and waits for the next detection event.

---

# 8. Recommended Demo Script (For Presentation)

Below is a recommended script for classroom / professor demonstration:

1. “System powered on, initialized, and waiting for ultrasonic trigger.”  
2. “User approaches — object detected.”  
3. “Arduino notifies Python; AI module activates.”  
4. “YOLO verifies whether a human is present.”  
5. If approved → “Authentication stage begins.”  
6. Demonstrate RFID card authentication.  
7. Demonstrate PIN authentication.  
8. Door unlocks briefly, then relocks.  
9. System resets to idle state.

This script matches the IT 254 final project evaluation format.

---

# 9. Expected Output Summary

### Arduino → Python


TRIGGER


### Python → Arduino


APPROVED
DENIED


### LCD Messages


System Ready
Waiting Echo...
Object Detected
AI: Human OK
Scan RFID/PIN
Access Granted
Door Unlocked
Door Locked
Access Denied


---

# 10. End of Document
