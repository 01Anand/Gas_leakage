// Include necessary libraries
#include <Servo.h>
#include <SoftwareSerial.h> // If using software serial for Bluetooth

// --- Define your hardware pins ---
const int gasSensorPin = A0;
const int ledPin = 13;
const int servoPin = 9;

// --- Define Bluetooth module pins (adjust based on your module) ---
const int rxPin = 10;
const int txPin = 11;
SoftwareSerial bluetoothSerial(rxPin, txPin); // RX, TX

// --- Define thresholds and variables ---
const int gasThreshold = 500;
Servo gasValveServo;
bool gasDetected = false;
const String callCommand = "CALL_EMERGENCY"; // Generic CALL command

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600);

  pinMode(ledPin, OUTPUT);
  gasValveServo.attach(servoPin);
  gasValveServo.write(0);

  Serial.println("Gas Leakage Detector Initialized");
  bluetoothSerial.println("Gas Leakage Detector Initialized");
}

void loop() {
  int gasValue = analogRead(gasSensorPin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > gasThreshold && !gasDetected) {
    Serial.println("Gas Leakage Detected! Sending CALL command via Bluetooth...");
    bluetoothSerial.println("WARNING: Gas Leakage Detected!");
    bluetoothSerial.println(callCommand); // Send the CALL command
    digitalWrite(ledPin, HIGH);
    gasValveServo.write(90);
    gasDetected = true;
  } else if (gasValue <= gasThreshold && gasDetected) {
    Serial.println("Gas level normal.");
    bluetoothSerial.println("Gas level normal.");
    digitalWrite(ledPin, LOW);
    gasValveServo.write(0);
    gasDetected = false;
  }

  delay(1000);
}
