# YOLO Installation Guide (macOS)
AI–Arduino Human Detection Access Control System  
This guide explains how to install YOLO (v8/v11/v12) on macOS systems, including Intel and Apple Silicon (M1/M2/M3). macOS uses CPU acceleration only, since CUDA is not supported.

---

# 1. Supported macOS Versions

- macOS Monterey (12.x)
- macOS Ventura (13.x)
- macOS Sonoma (14.x)

Both **Intel** and **Apple Silicon (M1/M2/M3)** chips are supported.

*Note:* YOLO on macOS runs in CPU mode. This is normal and expected.

---

# 2. Install Python

Download Python for macOS:
https://www.python.org/downloads/macos/

### Recommended Versions:
- Python **3.8**
- Python **3.9**
- Python **3.10**

### Verify Installation
Open Terminal:

python3 --version
pip3 --version


Expected:



Python 3.10.x
pip 23.x.x


---

# 3. Create a Virtual Environment (Recommended)

In Terminal:



python3 -m venv ai-env


Activate the environment:

### macOS:


source ai-env/bin/activate


Your prompt should now display:



(ai-env)


---

# 4. Install Required Python Packages

YOLO installation on macOS is simple because no CUDA or GPU-specific builds are needed.

Run:



pip install ultralytics opencv-python pyserial


This installs:
- YOLO framework
- OpenCV for webcam access
- Serial communication tools

### Optional: Update pip


pip install --upgrade pip


---

# 5. Install PyTorch (CPU Version for macOS)

Since CUDA is not available on macOS, install the CPU-only version:



pip install torch torchvision torchaudio


Verify PyTorch installs correctly:



python - <<EOF
import torch
print("PyTorch OK:", torch.version)
print("CUDA available:", torch.cuda.is_available())
EOF


Expected output:



PyTorch OK: 2.x.x
CUDA available: False


This is correct on macOS.

---

# 6. Verify YOLO Installation

To test YOLO loads properly:



python - <<EOF
from ultralytics import YOLO
model = YOLO("yolov8n.pt")
print("YOLO OK")
EOF


Expected output:



YOLO OK


---

# 7. Test Webcam

On macOS, the webcam may request permission the first time.

Run:



python - <<EOF
import cv2
cap = cv2.VideoCapture(0)
print("Camera opened:", cap.isOpened())
EOF


If macOS requests camera permission, go to:



System Settings → Privacy & Security → Camera


Enable access for **Terminal** or **VS Code**.

---

# 8. YOLO Webcam Test

To run a quick YOLO detection preview:

```python
from ultralytics import YOLO
import cv2

model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    res = model(frame)
    cv2.imshow("YOLO macOS Test", res[0].plot())
    if cv2.waitKey(1) == ord('q'):
        break
````

Press Q to quit.

# 9. Model File Placement

Place your YOLO model (e.g., face or person detector) in the project root:
````
AI-Access-Control/
 ├── ai_demo.py
 ├── yolov11s-face.pt
 ├── yolov12-face.pt (optional)
 └── docs/
````

Set the model path in script:

YOLO_MODEL = "yolov11s-face.pt"

# 10. Troubleshooting (macOS-specific)
### YOLO cannot access webcam

Check permissions:  
System Settings → Privacy & Security → Camera  

Permit access for:  

● Terminal  
● VS Code  
● Python  

### "Operation not permitted" errors

Grant full disk access if needed:  

System Settings → Privacy → Full Disk Access  

### ImportError: cannot import name 'A2C2f'

Cause: wrong PyTorch / YOLO version  
Fix:  

pip install --upgrade ultralytics  
pip install --upgrade torch torchvision torchaudio  

### Camera window is black

Try camera index 1:  
cap = cv2.VideoCapture(1)  

Or close other apps using the webcam:  

● FaceTime  
● Zoom  
● Chrome camera tab  
● OBS  

# 11. Completion Checklist
````
| Task                        | Status |
| --------------------------- | ------ |
| Python installed            | ✔      |
| Virtual environment created | ✔      |
| YOLO installed              | ✔      |
| PyTorch CPU installed       | ✔      |
| OpenCV installed            | ✔      |
| Webcam permissions granted  | ✔      |
| Model file placed correctly | ✔      |
| YOLO test completed         | ✔      |
````
