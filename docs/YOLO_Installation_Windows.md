# YOLO Installation Guide (Windows 10/11)
AI–Arduino Human Detection Access Control System  
This guide explains how to install YOLO (v8/v11/v12), PyTorch, and required dependencies on Windows 10/11 for both CPU-only and GPU-accelerated setups.

---

# 1. System Requirements

## Operating System
- Windows 10 (21H2 or newer)
- Windows 11 (recommended)

## Python Version
- Python **3.8 – 3.10**
(Do NOT use 3.11+; Ultralytics and Torch compatibility issues may occur)

## Hardware
- **CPU-only option works on all systems**
- GPU acceleration requires:
  - NVIDIA GPU (GTX / RTX)
  - Latest GPU driver
  - CUDA Toolkit + cuDNN (for PyTorch GPU builds)

---

# 2. Install Python (Required for All Installations)

Download Python:  
https://www.python.org/downloads/windows/

During installation:  
✔ **Check "Add Python to PATH"**  
✔ Ensure **pip** is included  

Verify installation:

python --version
pip --version

Expected:
Python 3.10.x
pip 23.x.x

---

# 3. Create a Virtual Environment (Recommended)
python -m venv ai-env
ai-env\Scripts\activate


Your terminal prompt should change to:



(ai-env)


You will install all packages inside this environment.

---

# 4. CPU-Only YOLO Installation (Simplest & Most Compatible)

If you **do not** have an NVIDIA GPU, or simply prefer a stable install:



pip install ultralytics opencv-python pyserial


This installs:
- YOLOv8 / YOLOv11 / YOLOv12 framework
- OpenCV webcam support
- Serial communication tools

Verify:



yolo --version


And:



python - <<EOF
from ultralytics import YOLO
model = YOLO("yolov8n.pt")
print("YOLO loaded successfully.")
EOF


If no errors appear → installation succeeded.

---

# 5. GPU Installation (CUDA + cuDNN + PyTorch)

This section is only for users with an NVIDIA GPU.

---

# 5.1 Step 1 — Install NVIDIA GPU Driver
Download latest Game Ready / Studio driver:

https://www.nvidia.com/Download/index.aspx

After installation, reboot your PC.

---

# 5.2 Step 2 — Install CUDA Toolkit (Version Must Match PyTorch)

Recommended version:
- **CUDA 11.8** (most stable for YOLO)
- **CUDA 12.1** (latest, but requires matching PyTorch builds)

Download:
https://developer.nvidia.com/cuda-toolkit-archive

After installing CUDA, verify:



nvcc --version


---

# 5.3 Step 3 — Install cuDNN (Required for CUDA deep learning)

1. Go to NVIDIA cuDNN archive:  
   https://developer.nvidia.com/cudnn-downloads

2. Download cuDNN for your CUDA version.
3. Extract the ZIP.
4. Copy the following folders **into your CUDA installation directory**:



cudnn\bin → C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA<version>\bin
cudnn\include → CUDA\include
cudnn\lib → CUDA\lib


---

# 5.4 Step 4 — Install PyTorch with CUDA Support

### CUDA 11.8 build:


pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118


### CUDA 12.1 build:

pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

Verify CUDA is active:

python - <<EOF  
import torch  
print("CUDA available:", torch.cuda.is_available())  
print("GPU:", torch.cuda.get_device_name(0) if torch.cuda.is_available() else "None")  
EOF  

Expected output:

CUDA available: True  
GPU: NVIDIA GeForce RTX ...  

---

# 5.5 Step 5 — Install YOLO (Same as CPU)

pip install ultralytics

YOLO will automatically detect whether PyTorch is running with CUDA.

---

# 6. Installing OpenCV and Serial Tools

pip install opencv-python pyserial

Verify OpenCV:

python - <<EOF  
import cv2  
print("OpenCV OK:", cv2.version)  
EOF  

---

# 7. Test YOLO with Webcam

Run the following:

yolo predict model=yolov8n.pt source=0

OR using Python:

```python
from ultralytics import YOLO
import cv2

model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    res = model(frame)
    cv2.imshow("YOLO Test", res[0].plot())
    if cv2.waitKey(1) == ord('q'):
        break
````

If a webcam window opens and boxes appear → YOLO installation succeeded.

# 8. Model File Placement

Place all model weights in the project root:
````
AI-Access-Control/
 ├── ai_demo.py
 ├── yolov11s-face.pt
 ├── yolov12n-face.pt (optional)
 └── docs/
````
Set the filename in Python:

YOLO_MODEL = "yolov11s-face.pt"

# 9. Common Issues & Solutions
### YOLO error: “A2C2f missing”  

Solution:  
Use Python 3.10 and YOLO >= 8.3.16.  

### Webcam not opening  

Another application may be using the webcam.  
Close:  
● Zoom  
● Discord  
● Windows Camera app  

### “CUDA not available: False”

Check:
● PyTorch installed with CUDA  
● GPU driver installed  
● Matching CUDA toolkit version  
● Use Python 3.10 instead of 3.11  

### “No module named ultralytics”
Install again:

pip install ultralytics --upgrade

# 10. Completion Checklist
````
| Task                                   | Status |
| -------------------------------------- | ------ |
| Python 3.8–3.10 installed              | ✔      |
| Virtual environment created            | ✔      |
| YOLO installed                         | ✔      |
| PyTorch (CPU or CUDA) installed        | ✔      |
| OpenCV + pyserial installed            | ✔      |
| Webcam tested                          | ✔      |
| AI model file placed in correct folder | ✔      |
````
