#include <Wire.h>
#include <Arduino.h>

// Motor control pins on the ELEGOO Smart Car Shield
const int PWMA = 5;  // Speed control for Motor A (M1)
const int PWMB = 6;  // Speed control for Motor B (M2)
const int AIN1 = 7;  // Direction control for Motor A (M1)
const int BIN1 = 8;  // Direction control for Motor B (M2)
const int STBY = 3;  // Motor standby pin (used to enable motors)

const int speedValue = 150; // range from 0-255

void setup() {
    Serial.begin(9600);  // Use Serial for communication with ESP32
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(STBY, OUTPUT);
    stopMotors(); // Ensure motors are off on boot
}

void loop() {
    if (Serial.available()) {  // Read command from ESP32
        char command = Serial.read();
        switch (toupper(command)) {
            case 'F': moveForward(); break;
            case 'B': moveBackward(); break;
            case 'L': turnLeft(); break;
            case 'R': turnRight(); break;
            case 'S': stopMotors(); break;
        }
    }
}

// --- Motor Control Functions ---
void moveForward() {
    digitalWrite(STBY, HIGH);
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, HIGH);
}

void moveBackward() {
    digitalWrite(STBY, HIGH);
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, LOW);
}

void turnLeft() {
    digitalWrite(STBY, HIGH);
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    digitalWrite(AIN1, LOW);
    digitalWrite(BIN1, HIGH);
}

void turnRight() {
    digitalWrite(STBY, HIGH);
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    digitalWrite(AIN1, HIGH);
    digitalWrite(BIN1, LOW);
}

void stopMotors() {
    digitalWrite(STBY, LOW);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}
