#include <PS2X_lib.h>

// Motor control pins
#define ENA 3      // PWM Left Motor
#define IN1 4      // Direction A1
#define IN2 5      // Direction A2
#define IN3 7      // Direction B1
#define IN4 8      // Direction B2
#define ENB 6      // PWM Right Motor

PS2X ps2x; // PS2 Controller instance

// Controller variables
int error = 0;
byte type = 0;
const int DEADZONE = 20; // Analog stick deadzone threshold

void setup() {
  // Configure motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize motors stopped
  stopMotors();
  
  Serial.begin(57600);
  
  // PS2 controller initialization
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
  
  if(error == 0) {
    Serial.println("PS2 Controller Connected");
    type = ps2x.readType();
    if(type == 1) Serial.println("DualShock Controller Ready");
  } else {
    Serial.println("Controller Error - Check Wiring");
  }
}

void loop() {
  if(error != 0) return; // Skip if controller error
  
  ps2x.read_gamepad(false, 0); // Read controller (vibration off)
  
  // Analog stick control (Left Stick)
  int LY = ps2x.Analog(PSS_LY);  // 0-255, 127=center
  int LX = ps2x.Analog(PSS_LX);  // 0-255, 127=center
  
  // Apply deadzone and convert to motor control range (-255 to 255)
  LY = (abs(LY - 127) > DEADZONE) ? map(LY, 0, 255, 255, -255) : 0;
  LX = (abs(LX - 127) > DEADZONE) ? map(LX, 0, 255, -255, 255) : 0;

  // Mix steering inputs (Tank drive algorithm)
  int leftSpeed = constrain(LY + LX, -255, 255);
  int rightSpeed = constrain(LY - LX, -255, 255);
  
  // Set motor speeds
  setMotorSpeed(leftSpeed, rightSpeed);
  
  // Fallback D-pad control
  if(ps2x.Button(PSB_PAD_UP)) moveForward(200);
  else if(ps2x.Button(PSB_PAD_DOWN)) moveBackward(200);
  else if(ps2x.Button(PSB_PAD_LEFT)) pivotLeft(200);
  else if(ps2x.Button(PSB_PAD_RIGHT)) pivotRight(200);
  else if(LY == 0 && LX == 0) stopMotors(); // Stop if no input
  
  delay(20);
}

// ======== MOTOR CONTROL FUNCTIONS ========
void setMotorSpeed(int left, int right) {
  // Left motor control
  digitalWrite(IN1, (left > 0) ? LOW : HIGH);
  digitalWrite(IN2, (left > 0) ? HIGH : LOW);
  analogWrite(ENA, abs(left));
  
  // Right motor control
  digitalWrite(IN3, (right > 0) ? HIGH : LOW);
  digitalWrite(IN4, (right > 0) ? LOW : HIGH);
  analogWrite(ENB, abs(right));
}

void moveForward(int speed) {
  setMotorSpeed(speed, speed);
}

void moveBackward(int speed) {
  setMotorSpeed(-speed, -speed);
}

void pivotLeft(int speed) {
  setMotorSpeed(-speed, speed);
}

void pivotRight(int speed) {
  setMotorSpeed(speed, -speed);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
