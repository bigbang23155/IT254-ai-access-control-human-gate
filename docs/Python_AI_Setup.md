# Python AI Setup Guide  
AI–Arduino Human Detection Access Control System  
Python Module for Human/Face Detection Using YOLO

This guide explains how to prepare the Python environment used to run the YOLO-based human detection engine that communicates with the Arduino via USB serial. Follow each section in order to ensure the system functions correctly.

---

# 1. Requirements

## Supported Operating Systems
- Windows 10 / 11 (recommended)
- macOS (Intel or Apple Silicon)
- Linux (Ubuntu/Debian-based)

## Supported Python Versions
- **Python 3.8**
- **Python 3.9**
- **Python 3.10**  ← best compatibility  
(Not compatible with Python 3.11+ for YOLOv11/12 GPU builds)

## Required Python Packages
````
| Package | Purpose |
|--------|---------|
| `ultralytics` | YOLOv8/11/12 model framework |
| `opencv-python` | Webcam access & image processing |
| `pyserial` | Communication with Arduino |
| `torch` | Core AI backend (CUDA optional) |
````
---

# 2. Install Python

## Windows / macOS
Download from:  
https://www.python.org/downloads/

During installation (Windows):  
✔ Check **"Add Python to PATH"**  
✔ Choose **Customize** → ensure `pip` enabled  

---

# 3. Create a Virtual Environment (Recommended)

Open a terminal (Command Prompt / PowerShell / macOS Terminal):
python -m venv ai-env

Activate it:

### Windows:
ai-env\Scripts\activate

### macOS/Linux:
source ai-env/bin/activate

If successful, your prompt will show:
(ai-env)

---

# 4. Install Required Python Packages

Run:
pip install ultralytics opencv-python pyserial

If using GPU + CUDA, install PyTorch first:

(Example for CUDA 11.8)
pip install torch --index-url https://download.pytorch.org/whl/cu118


Check PyTorch:
python -c "import torch; print(torch.cuda.is_available())"

Expected:
True (if GPU works)

---

# 5. Place YOLO Model Weights

Place your YOLO model file inside the project root directory, e.g.:
````
/AI-Access-Control/
└── yolov11s-face.pt
````


You may also store multiple custom models, such as:
- `yolov11s-face.pt`
- `yolov12n-face.pt`
- `custom-face-detector.pt`

---

# 6. Configure Python ↔ Arduino Serial Connection

The Python script uses a synchronous serial protocol.

### Arduino → Python

TRIGGER

Sent when ultrasonic sensor detects presence.

### Python → Arduino
APPROVED // human detected
DENIED // non-human / timeout

Modify the COM port in `ai_demo.py`:

```python
COM_PORT = "COM5"      # Windows
BAUD_RATE = 9600
````

macOS example:
COM_PORT = "/dev/cu.usbmodem1101"

# 7. Running the AI Detection Engine

After connecting Arduino and activating the virtual environment:

python ai_demo.py


The terminal should display:

[INFO] Connected to COM5
[SYSTEM] Waiting for TRIGGER from Arduino...


When detected:

● Webcam window will open  
● YOLO will draw bounding boxes  
● Serial messages appear automatically  

Press Q to exit manually.

# 8. File Placement Structure (Python Section)
````
/AI-Access-Control
│
├── ai_demo.py
├── yolov11s-face.pt
├── yolov12n-face.pt   (optional)
├── README.md
│
└── /docs
      ├── Python_AI_Setup.md
      ├── YOLO_Installation_Windows.md
      ├── Arduino_Setup.md
      └── index.md
````

# 9. Troubleshooting
Cannot open COM port

● Ensure Arduino Serial Monitor is closed  
● Check Device Manager (Windows) or /dev/ (macOS)  

### Webcam not found

Check available cameras:
python -c "import cv2; print(cv2.VideoCapture(0).isOpened())"

### YOLO model file not found

Ensure the filename in ai_demo.py exactly matches the actual file:
YOLO_MODEL = "yolov11s-face.pt"

### GPU not used
Ensure the correct CUDA version of PyTorch is installed.

# 10. Next Steps

Once Python AI module is working:

1. Test Arduino distance sensor triggering
2. Observe YOLO detecting a human
3. Confirm APPROVED / DENIED returned correctly
4. Perform full end-to-end unlock test with RFID or PIN
