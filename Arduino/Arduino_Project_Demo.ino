/*
  IT254 Final Project — AI-Arduino Human Detection Access Control System (v3.9.2 Fast Response)
  Author: Equinox (2025)
  Board: Arduino Mega2560 R3 (Elegoo)

Improvements:

- Instant ultrasonic sensing activation upon power-on

- Increased measurement frequency (100ms)

- Trigger on first detection, no delay

- Fast camera wake-up (TRIGGER instant transmission)

- Stable, non-repeating buzzer and LCD beeping

- Fully synchronized with Python v4.0 (YOLOv12)
*/

#include <LiquidCrystal.h> // Library for 16x2 LCD
#include <SPI.h> // SPI bus for RFID reader
#include <MFRC522.h> // RFID module (MFRC522)
#include <Keypad.h> // Matrix keypad input

// ---------------- LCD ----------------
LiquidCrystal lcd(33, 32, 31, 30, 37, 38);
// RS, E, D4, D5, D6, D7 pin mapping to Arduino digital pins

// ---------------- RFID ----------------
#define SS_PIN   53 // RFID module SDA/SS pin
#define RST_PIN   9 // RFID module reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create RFID reader instance

// ---------------- Keypad ----------------
const byte ROWS = 4, COLS = 4; // 4x4 keypad
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5,4,3,2};//From left to right // Row pins (top to bottom)
byte colPins[COLS] = {25,24,23,22};//Continue to the right from the steps above. // Column pins (left to right)
Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------------- Sensing and Control ----------------
#define TRIG_PIN 6 // Ultrasonic trigger pin
#define ECHO_PIN 7 // Ultrasonic echo pin
#define GREEN_LED 34 // Indicator LED for success
#define RED_LED   35 // Indicator LED for failure
#define BUZZER    36 // Buzzer for alerts
#define MOSFET_PIN 8 // MOSFET controlling door lock power

// ---------------- Timing & Distance Constants ----------------
#define UNLOCK_MS 3000 // Door unlock duration (ms)
#define DIST_MIN 10   // Minimum detection distance (cm)
#define DIST_MAX 70 // Maximum detection distance (cm)
#define MEASURE_INTERVAL 100     // Ultrasonic sampling interval (ms)
#define COOLDOWN_MS 5000         // Delay after re-locking before next trigger

// ---------------- Verification materials ----------------
String authorizedUIDs[] = {"67 62 CD 73"}; //Authorized RFID UID
String correctPIN = "1234"; //Authorized keypad PIN
String inputPIN = ""; // Buffer for user PIN entry

// ---------------- State variables ----------------
bool doorUnlocked = false;  // Tracks if door is currently unlocked
bool verifyEnabled = false; // True once YOLO approves and user can verify
bool personDetected = false; // True when ultrasonic detects a nearby object
bool verifyLocked = false; // Prevents retriggering during current cycle
bool beepedThisCycle = false; // Ensures single beep per detection event

unsigned long unlockStart = 0;  // Timestamp when door was unlocked
unsigned long lastMeasure = 0;  // Timestamp of last ultrasonic reading
String lastDisplay = "";        // Stores last LCD message to prevent flicker

// ---------------- Utility: Buzzer Patterns ----------------
void buzzOK(uint16_t ms=100) {
  // Short confirmation beep
  digitalWrite(BUZZER, HIGH);
  delay(ms);
  digitalWrite(BUZZER, LOW);
}

void buzzErr(uint16_t ms=250) {
  // Double error beep pattern
  digitalWrite(BUZZER, HIGH); delay(ms);
  digitalWrite(BUZZER, LOW);  delay(80);
  digitalWrite(BUZZER, HIGH); delay(80);
  digitalWrite(BUZZER, LOW);
}

// ---------------- Utility: LCD Message Display ----------------
void displayStatus(const String &l1, const String &l2="") {
  // Displays message only if it changed (reduces flicker)
  String combo = l1 + "|" + l2;
  if (combo != lastDisplay) {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print(l1);
    lcd.setCursor(0,1); lcd.print(l2);
    lastDisplay = combo;
  }
}

// ---------------- Ultrasonic Distance Measurement ----------------
long measureDistance() {
  // Sends a short ultrasonic pulse and measures return echo
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measures echo time (max wait 30ms = ~5 meters)
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  // Convert time to distance in cm (speed of sound = 0.034 cm/µs)
  long distance = duration * 0.034 / 2;
  return distance;
}

// ---------------- Initialization ----------------
void setup() {
  Serial.begin(9600);            // Serial link to Python
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOSFET_PIN, OUTPUT);

  // Initialize default states
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, LOW);
  digitalWrite(MOSFET_PIN, LOW);

  lcd.begin(16, 2);              // Initialize LCD 16x2
  displayStatus("AI Access System","Starting...");
  delay(800);

  SPI.begin();                   // Initialize SPI for RFID
  mfrc522.PCD_Init();            // Start MFRC522 module

  // Discard first few ultrasonic readings (stabilization)
  for (int i=0; i<3; ++i) {
    measureDistance();
    delay(60);
  }

  displayStatus("System Ready","Echo Active!");
}

// ---------------- Main Loop ----------------
void loop() {
  // 1️⃣ Ultrasonic Trigger: Detect approaching object
  if (!verifyLocked && !verifyEnabled && !doorUnlocked &&
      millis() - lastMeasure >= MEASURE_INTERVAL) {

    lastMeasure = millis();
    long distance = measureDistance();

    // If object detected within threshold range
    if (distance > DIST_MIN && distance < DIST_MAX) {
      personDetected = true;
      verifyLocked = true; // Prevent re-triggering
      displayStatus("Object Detected","Waiting AI...");
      if (!beepedThisCycle) { buzzOK(80); beepedThisCycle = true; }
      Serial.println("TRIGGER");   // Notify Python to start camera check
    }
  }

  // 2️⃣ Receive AI Results from Python (Serial communication)
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    if (msg == "APPROVED" && !doorUnlocked) {
      // YOLO verified human presence
      verifyEnabled = true;
      displayStatus("AI: Human OK","Scan RFID/PIN");
      buzzOK(100);
    }
    else if (msg == "DENIED") {
      // YOLO rejected (not a human)
      deniedAccess("AI Denied");
    }
  }

  // 3️⃣ Verification Phase: RFID or Keypad
  if (verifyEnabled && !doorUnlocked) {
    if (checkRFID() || checkKeypad()) {
      unlockDoor();
      Serial.println("UNLOCKED"); // Notify Python system
    }
  }

  // 4️⃣ Automatic Door Locking after timeout
  if (doorUnlocked && millis() - unlockStart >= UNLOCK_MS) {
    lockDoor();
    delay(COOLDOWN_MS); // Short cooldown before next detection
  }
}

// ---------------- RFID Verification ----------------
bool checkRFID() {
  // Check for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    return false;

  // Convert UID bytes to string format
  String uid="";
  for (byte i=0;i<mfrc522.uid.size;i++){
    uid+=String(mfrc522.uid.uidByte[i],HEX);
    if(i<mfrc522.uid.size-1) uid+=" ";
  }
  uid.toUpperCase();

  displayStatus("RFID Scanned", uid);

  // Compare against authorized UIDs
  for(String a:authorizedUIDs){
    if(uid==a){
      buzzOK();
      mfrc522.PICC_HaltA(); // Stop reading this card
      return true;
    }
  }

  // Unauthorized card
  buzzErr();
  mfrc522.PICC_HaltA();
  displayStatus("RFID Denied");
  delay(600);
  displayStatus("Scan RFID/PIN");
  return false;
}

// ---------------- Keypad Verification ----------------
bool checkKeypad() {
  char k = keypad.getKey();   // Read pressed key
  if (!k) return false;       // No input

  if (k == '#') {
    // User submitted PIN
    if (inputPIN == correctPIN) {
      displayStatus("PIN Correct");
      inputPIN="";
      buzzOK();
      return true;
    } else {
      displayStatus("Wrong PIN");
      inputPIN="";
      buzzErr();
      delay(500);
      displayStatus("Enter PIN:");
    }
  } else if (k == '*') {
    // Clear entered digits
    inputPIN="";
    displayStatus("Enter PIN:");
  } else if (k >= '0' && k <= '9') {
    // Append numeric key
    inputPIN += k;
    lcd.setCursor(0,0); lcd.print("Enter PIN:");
    lcd.setCursor(0,1);
    // Display asterisks for entered digits
    for(size_t i=0;i<inputPIN.length()&&i<16;i++) lcd.print('*');
  }
  return false;
}

// ---------------- Door Unlock ----------------
void unlockDoor() {
  digitalWrite(MOSFET_PIN,HIGH);  // Activate MOSFET (unlock relay)
  digitalWrite(GREEN_LED,HIGH);   // Green LED ON
  digitalWrite(RED_LED,LOW);      // Red LED OFF
  displayStatus("Access Granted","Door Unlocked");
  doorUnlocked = true;
  unlockStart = millis();         // Record unlock start time
  verifyEnabled = false;
  personDetected = false;
  verifyLocked = false;
  beepedThisCycle = false;
  buzzOK(120);
  Serial.println("[Arduino] Door Unlocked");
}

// ---------------- Door Lock ----------------
void lockDoor() {
  digitalWrite(MOSFET_PIN,LOW);   // Deactivate MOSFET (lock relay)
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,HIGH);
  displayStatus("Door Locked","Echo Ready");
  doorUnlocked = false;
  inputPIN = "";
  verifyEnabled = false;
  personDetected = false;
  verifyLocked = false;
  beepedThisCycle = false;
  Serial.println("[Arduino] Door Locked");
}

// ---------------- Access Denied ----------------
void deniedAccess(const char* reason) {
  digitalWrite(RED_LED,HIGH);
  buzzErr();
  displayStatus("Access Denied", reason);
  delay(1000);
  displayStatus("Echo Ready");
  verifyEnabled = false;
  verifyLocked = false;
  personDetected = false;
  beepedThisCycle = false;
  inputPIN = "";
}
