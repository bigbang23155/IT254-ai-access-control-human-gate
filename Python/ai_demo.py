"""
AI Access Control v4.0
Author: Equinox (2025)

System Flow:
  1. Arduino detects motion using the ultrasonic sensor and sends "TRIGGER".
  2. Python activates the webcam and performs YOLOv11/12-based human detection.
  3. If a human is detected → send "APPROVED" to Arduino for verification (RFID/PIN).
  4. Once Arduino confirms unlock, Python closes the camera and starts cooldown.
"""

# ---------------- Library Imports ----------------
from ultralytics import YOLO        # For object detection (YOLOv11/12)
import cv2                          # OpenCV for camera input and display
import serial                       # For Arduino serial communication
import time                         # Timing and delays

# ---------------- Configuration Section ----------------
YOLO_MODEL = r"C:\Arduino_Project_Demo\py\yolov11s-face.pt"  # Path to YOLO model file (.pt)
COM_PORT = "COM3"                   # Arduino serial port (adjust as needed)
BAUD_RATE = 9600                    # Must match Arduino Serial.begin()
CONF_THRESHOLD = 0.5                # Minimum confidence for detection
FRAME_W, FRAME_H = 640, 480         # Camera resolution
HUMAN_LABELS = {"person", "human", "face"}  # Object names considered as "human"

# Timing parameters
DETECTION_FRAMES_REQUIRED = 3       # Number of consecutive detections to confirm human
AUTH_TIMEOUT = 15                   # Seconds before timeout if no human detected
COOLDOWN_DELAY = 6                  # Seconds to wait after unlock before re-enabling detection

# ---------------- Serial Connection Initialization ----------------
try:
    # Attempt to open serial port connection with Arduino
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    print(f"[INFO] Connected to {COM_PORT}")
    time.sleep(2)  # Allow Arduino time to reset
except Exception as e:
    # If connection fails, print error and continue without serial
    print(f"[ERROR] Unable to connect to serial port: {e}")
    ser = None

# ---------------- YOLO Model Initialization ----------------
print("[INFO] Loading YOLO model...")
model = YOLO(YOLO_MODEL)  # Load pretrained model
print("[READY] YOLO model loaded and ready.")
print("[SYSTEM] Waiting for TRIGGER from Arduino...")

# ---------------- Helper Functions ----------------
def open_camera():
    """Initialize and return the webcam stream."""
    cam = cv2.VideoCapture(0)
    cam.set(cv2.CAP_PROP_FRAME_WIDTH, FRAME_W)
    cam.set(cv2.CAP_PROP_FRAME_HEIGHT, FRAME_H)
    if not cam.isOpened():
        raise RuntimeError("Unable to turn on the camera")
    return cam

def close_camera(cam):
    """Safely release camera resources and close OpenCV window."""
    if cam:
        cam.release()
    cv2.destroyAllWindows()

def send(msg):
    """Send a message to Arduino via serial port."""
    if ser:
        ser.write((msg + "\n").encode())  # Send message followed by newline
        print(f"[SEND] {msg}")

# ---------------- Runtime Variables ----------------
triggered = False         # True when ultrasonic trigger received
camera_active = False     # True when camera is turned on
cap = None                # OpenCV camera object

# ---------------- Main Program Loop ----------------
while True:
    try:
        # Listen continuously for serial messages from Arduino
        if ser and ser.in_waiting:
            data = ser.readline().decode(errors="ignore").strip()
            if data:
                print(f"[ARDUINO] {data}")

            # Step 1: Arduino triggers Python when motion detected
            if data == "TRIGGER" and not triggered:
                print("[ACTION] Ultrasonic Trigger → Turn on the camera")
                triggered = True

                # Attempt to initialize the camera
                try:
                    cap = open_camera()
                    camera_active = True
                except Exception as e:
                    print(f"[ERROR] {e}")
                    send("DENIED")     # Notify Arduino of failure
                    triggered = False
                    continue

                # Reset detection timers and counters
                start_time = time.time()   # Start of detection session
                last_seen = None           # Last time a human was seen
                consecutive = 0            # Count of consecutive valid frames
                human_detected = False     # Flag once human detection confirmed

                # Step 2: Begin continuous YOLO detection loop
                while True:
                    ret, frame = cap.read()  # Capture current video frame
                    if not ret:
                        print("[ERROR] Unable to read image frame")
                        break

                    # Perform YOLO object detection on the frame
                    results = model.predict(frame, conf=CONF_THRESHOLD, verbose=False)
                    detected_human = False  # Reset per-frame detection flag

                    # Process each detected object
                    for r in results:
                        boxes = r.boxes
                        names = model.names
                        for b in boxes:
                            x1, y1, x2, y2 = map(int, b.xyxy[0].tolist())   # Bounding box
                            conf = float(b.conf[0])                          # Confidence
                            cls_id = int(b.cls[0])                           # Class ID
                            cls_name = names.get(cls_id, str(cls_id))        # Class name

                            # Color green for human, red otherwise
                            color = (0,255,0) if cls_name.lower() in HUMAN_LABELS else (0,0,255)
                            label = f"{cls_name} {conf*100:.1f}%"

                            # Draw bounding box and label on the frame
                            cv2.rectangle(frame, (x1,y1), (x2,y2), color, 2)
                            cv2.putText(frame, label, (x1, max(20,y1-10)), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

                            # Mark as human if match and confidence high enough
                            if cls_name.lower() in HUMAN_LABELS and conf >= CONF_THRESHOLD:
                                detected_human = True

                    # Display the annotated video in a window
                    cv2.imshow("AI Human Detection", frame)

                    # Step 3: Handle human detection logic
                    if detected_human:
                        consecutive += 1
                        last_seen = time.time()

                        # Confirm human after N consecutive valid frames
                        if consecutive >= DETECTION_FRAMES_REQUIRED and not human_detected:
                            print("[RESULT] Human confirmed → APPROVED")
                            send("APPROVED")      # Notify Arduino
                            human_detected = True # Prevent duplicates
                    else:
                        consecutive = 0  # Reset if no human found

                    # Step 4: Listen for Arduino response (e.g., "UNLOCKED")
                    if ser and ser.in_waiting:
                        msg = ser.readline().decode(errors="ignore").strip()
                        if msg == "UNLOCKED":
                            print("[INFO] Arduino Verification complete → Turn off the camera")
                            close_camera(cap)
                            camera_active = False
                            triggered = False
                            time.sleep(COOLDOWN_DELAY)
                            print("[STATE] Cooling complete → Waiting for the next ultrasonic trigger")
                            break

                    # Step 5: Manual user exit via 'q' key
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        print("[QUIT] Manual exit")
                        break

                    # Step 6: Timeout protection if no human detected in given time
                    if time.time() - start_time > AUTH_TIMEOUT and not human_detected:
                        print("[TIMEOUT] Unmanned detection → DENIED")
                        send("DENIED")
                        close_camera(cap)
                        camera_active = False
                        triggered = False
                        time.sleep(COOLDOWN_DELAY)
                        break

        # Short delay between serial reads to prevent CPU overuse
        time.sleep(0.05)

    except KeyboardInterrupt:
        # Allow manual program termination via Ctrl+C
        print("\n[EXIT] Manual stop")
        break
    except Exception as e:
        # Catch unexpected runtime errors and continue
        print(f"[ERROR] {e}")
        time.sleep(0.5)

# ---------------- Clean Shutdown ----------------
if cap:
    close_camera(cap)   # Ensure camera is released
if ser:
    ser.close()         # Close serial connection
print("🔴 The system has been shut down.")
