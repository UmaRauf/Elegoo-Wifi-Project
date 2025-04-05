#include <Wire.h>
#include <Arduino.h>

// Motor control pins on the ELEGOO Smart Car Shield (updated based on your pin mapping)
const int PWMA = 5;  // Speed control for Motor A (M1)
const int PWMB = 6;  // Speed control for Motor B (M2)
const int AIN1 = 7;  // Direction control for Motor A (M1)
const int BIN1 = 8;  // Direction control for Motor B (M2)
const int STBY = 3;  // Motor standby pin (used to enable motors)

const int speedValue = 75; // Adjust speed (0-255)

void setup() {
    Serial.begin(9600);
    Serial.println("Setup complete. Waiting for input...");

    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(STBY, OUTPUT);

    stopMotors(); // Ensure motors are stopped at startup
}

void loop() {
    if (Serial.available()) {
        char command = Serial.read();
        Serial.print("Received: ");
        Serial.println(command);

        switch (command) {
            case 'W': case 'w': 
                Serial.println("Moving Forward");
                moveForward(); 
                break;
            case 'S': case 's': 
                Serial.println("Moving Backward");
                moveBackward(); 
                break;
            case 'A': case 'a': 
                Serial.println("Turning Left");
                turnLeft(); 
                break;
            case 'D': case 'd': 
                Serial.println("Turning Right");
                turnRight(); 
                break;
            case 'X': case 'x': 
                Serial.println("Stopping Motors");
                stopMotors(); 
                break;
            default:
                Serial.println("Invalid command");
                break;
        }
    }
}

void moveForward() {
    digitalWrite(STBY, HIGH); // Enable motors
    
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    
    digitalWrite(AIN1, HIGH); // Forward direction for Motor A
    digitalWrite(BIN1, HIGH); // Forward direction for Motor B
}

void moveBackward() {
    digitalWrite(STBY, HIGH); // Enable motors
    
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    
    digitalWrite(AIN1, LOW);  // Reverse direction for Motor A
    digitalWrite(BIN1, LOW);  // Reverse direction for Motor B
}

void turnLeft() {
    digitalWrite(STBY, HIGH); // Enable motors
    
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    
    digitalWrite(AIN1, LOW);  // Reverse direction for Motor A
    digitalWrite(BIN1, HIGH); // Forward direction for Motor B
}

void turnRight() {
    digitalWrite(STBY, HIGH); // Enable motors
    
    analogWrite(PWMA, speedValue);
    analogWrite(PWMB, speedValue);
    
    digitalWrite(AIN1, HIGH); // Forward direction for Motor A
    digitalWrite(BIN1, LOW);  // Reverse direction for Motor B
}

void stopMotors() {
    Serial.println("Motors Stopped");
    digitalWrite(STBY, LOW); // Disable motors
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}
