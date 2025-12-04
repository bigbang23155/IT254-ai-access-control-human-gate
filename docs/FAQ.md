# Frequently Asked Questions (FAQ)
This document lists the most common issues encountered while building and running the AI–Arduino Human Detection Access Control System, along with their recommended solutions.

---

# 1. General System Issues

### **Q1. The system does not react when I approach the ultrasonic sensor.**
Possible causes:
- Incorrect TRIG/ECHO wiring  
- ECHO pin not using a valid digital pin  
- Distance outside configured detection range  
- Loose ground connection  

Verify:
- TRIG → D6  
- ECHO → D7  
- Power (5V) and GND are solid  
- Arduino Serial Monitor prints correct distance trigger events

---

# 2. LCD1602 Display Issues

### **Q2. LCD shows only black boxes but no text.**
This is the most common LCD problem.

Solution:
- Install a **2kΩ resistor** between VO → GND  
- Ensure RS, EN, D4–D7 pins match the firmware  
- Confirm RW is tied to GND  
- Power LCD with 5V, not 3.3V  

---

### **Q3. LCD backlight works but characters are invisible.**
Cause: contrast too low or too high.

Fix:
- Adjust VO resistor between 1k–5k  
- Try testing with direct potentiometer if needed  

---

# 3. Keypad Issues

### **Q4. Keypad returns incorrect numbers or wrong characters.**
Your keypad was verified to require reversed row mapping.

Correct mapping:

| Row | Arduino Pin |
|------|-------------|
| R1 | D5 |
| R2 | D4 |
| R3 | D3 |
| R4 | D2 |

| Column | Arduino Pin |
|---------|-------------|
| C1 | D25 |
| C2 | D24 |
| C3 | D23 |
| C4 | D22 |

If characters are still wrong:
- Ensure keypad ribbon pins are not flipped  
- Ensure software uses the correct key layout  

---

### **Q5. Keypad does not respond at all.**
Check:
- Pin direction: rows output, columns input  
- No shorts in the keypad membrane  
- Pressing multiple buttons simultaneously may fail  
- Use Serial print debugging to verify scan results  

---

# 4. RFID Issues

### **Q6. RFID reader (MFRC522) does not detect cards.**
RFID is sensitive to wiring and voltage.

Checklist:
- VCC must be **3.3V ONLY**  
- SDA → D53  
- SCK → D52  
- MOSI → D51  
- MISO → D50  
- RST → D9  
- Antenna must face card directly  

---

### **Q7. The card UID appears but does not match the whitelist.**
Check formatting:
- Use uppercase hex format: `67 62 CD 73`  
- Include spaces between bytes  
- Compare exactly (case-sensitive, space-sensitive)

---

# 5. AI / YOLO Detection Issues

### **Q8. Python prints an error: “A2C2f not found”.**
Cause:
- YOLO model incompatible with current Ultralytics version  
- Python version too new  

Fix:
- Use Python **3.10**  
- Install YOLO ≥ `8.3.16`  

---

### **Q9. Webcam window does not appear when YOLO runs.**
Possible causes:
- Another app is using the webcam  
- macOS permission denied  
- OpenCV not installed correctly  

Fix:
1. Close Zoom, Discord, Camera App, OBS  
2. Reinstall OpenCV:  
pip install --upgrade opencv-python
3. macOS users: grant camera access in  
**System Settings → Privacy & Security → Camera**

---

### **Q10. YOLO detects objects but not humans.**
Check:
- Using correct model (`yolov11s-face.pt`, `face.pt`, or `person.pt`)  
- Lighting conditions  
- Camera resolution set properly  
- CONF threshold not too high  

---

### **Q11. YOLO detection is very slow.**
This is normal on:
- CPU-only systems  
- macOS without GPU acceleration

Possible improvements:
- Use `n` (nano) version models  
- Reduce frame size  
- Lower confidence threshold  

---

# 6. Python Serial Communication Issues

### **Q12. Python error: “Cannot open COM port”.**
Fix:
- Close the Arduino Serial Monitor  
- Check COM port using Device Manager  
- Ensure only one process uses the port  

---

### **Q13. Arduino does not receive APPROVED/DENIED from Python.**
Check:
- Baud rate must be **9600** on both sides  
- Python must send newline `\n`  
- USB cable must be data-capable, not charge-only  

---

### **Q14. Python script stuck at “Waiting for TRIGGER…”**
Meaning:
- Arduino is not sending `"TRIGGER"`  
- Ultrasonic sensor may not detect presence  

Verify:
- Sensor wiring  
- Distance threshold in code  
- Arduino Serial prints debug messages  

---

# 7. Arduino Firmware Issues

### **Q15. The buzzer beeps nonstop.**
Cause:
- System entered authentication mode repeatedly  
- PIR/sensor noise  

Fix:
- Ensure `verifyEnabled = false` resets correctly  
- Increase debounce or timeout  

---

### **Q16. LEDs do not behave correctly.**
Check:
- Green LED → D34  
- Red LED → D35  
- Both require current-limiting resistor  

---

# 8. Door Lock / MOSFET Issues

### **Q17. Motor or lock does not activate.**
Check:
- MOSFET gate connected to D8  
- Diode across motor terminals  
- External power (if using real lock)  
- Drain → motor (+), Source → GND  

---

### **Q18. Lock activates only sometimes.**
Fix:
- Ensure common ground between Arduino and motor power  
- Verify MOSFET orientation  
- Avoid breadboard high-current limitations  

---

# 9. Model File Issues

### **Q19. YOLO model file not found.**
Ensure:
- File is placed in project root  
- Python path matches filename  

YOLO_MODEL = "yolov11s-face.pt"


---

### **Q20. Using custom model shows “RuntimeError: size mismatch”.**
Cause:
- Model trained for classification but used for detection  
Fix:
- Use YOLO **detection models** only (YOLOv8/11/12 *detect* versions)

---

# 10. System Integration Issues

### **Q21. Arduino triggers before AI finishes processing.**
This is expected.  
The firmware waits for AI result before enabling authentication again.

---

### **Q22. User fails authentication because AI timed out.**
AI timeout can be tuned in `ai_demo.py`:
AUTH_TIMEOUT = 15
INACTIVITY_TIMEOUT = 5


---

# 11. Quick Diagnostic Checklist
````
| Component | Working? |
|-----------|----------|
| LCD shows text | ✔ |
| Keypad prints correct characters | ✔ |
| RFID scans cards | ✔ |
| Ultrasonic detects presence | ✔ |
| Python receives TRIGGER | ✔ |
| YOLO opens webcam | ✔ |
| APPROVED/DENIED returns to Arduino | ✔ |
| Lock activates and relocks | ✔ |
````
---

# 12. When to Reset the System

Reset Arduino if:

- Serial communication desynchronizes  
- LCD stops updating  
- Keypad freezes  
- YOLO camera fails to open  

Soft reset by pressing the Arduino **RESET** button.

---

# 13. Contact & Support

If this project is being graded:
- Refer to the project README  
- Review `Hardware_Wiring_Guide.md`  
- Confirm Python virtual environment is activated  

For further debugging, enable verbose serial prints in both Arduino and Python.

---

# End of Document
