#include <Ps3Controller.h>

// Mac address PS3 Controller
const char* Ps3Mac = "11:22:33:44:55:66";

// Pin L298N
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENA 32
#define ENB 33

const int freq = 1000;
const int res = 8;

int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
int yAxis = 0, xAxis = 0;

const int deadzone = 50; // The smaller the value the more responsive the joystick
float corr_factor = 0.7; // The larger the value the sharper the turning

// Set rotation speed for turning in place (left & right)
const int rotate_speed = 210; // The larger the value the faster the inplace rotation

String currentStatus = "Stop";
int player = 0;
int battery = 0;


// Kontrol Motor
void driveMotor(int rightSpeed, int leftSpeed) {
  digitalWrite(IN1, rightSpeed > 0);
  digitalWrite(IN2, rightSpeed < 0);
  digitalWrite(IN3, leftSpeed > 0);
  digitalWrite(IN4, leftSpeed < 0);
  ledcWrite(ENA, abs(rightSpeed));
  ledcWrite(ENB, abs(leftSpeed));
}

// PS3 Notify
void notify() {
  yAxis = Ps3.data.analog.stick.ly;
  xAxis = Ps3.data.analog.stick.rx;

  // Digital cross/square/triangle/circle button events 
  if( Ps3.event.button_down.cross )
      Serial.println("Started pressing the cross button");
  if( Ps3.event.button_up.cross )
      Serial.println("Released the cross button");

  if( Ps3.event.button_down.square )
      Serial.println("Started pressing the square button");
  if( Ps3.event.button_up.square )
      Serial.println("Released the square button");

  if( Ps3.event.button_down.triangle )
      Serial.println("Started pressing the triangle button");
  if( Ps3.event.button_up.triangle )
      Serial.println("Released the triangle button");

  if( Ps3.event.button_down.circle )
      Serial.println("Started pressing the circle button");
  if( Ps3.event.button_up.circle )
      Serial.println("Released the circle button");

  // Digital Dpad button events
  if( Ps3.event.button_down.up )
      Serial.println("Started pressing the up button");
  if( Ps3.event.button_up.up )
      Serial.println("Released the up button");

  if( Ps3.event.button_down.right )
      Serial.println("Started pressing the right button");
  if( Ps3.event.button_up.right )
      Serial.println("Released the right button");

  if( Ps3.event.button_down.down )
      Serial.println("Started pressing the down button");
  if( Ps3.event.button_up.down )
      Serial.println("Released the down button");

  if( Ps3.event.button_down.left )
      Serial.println("Started pressing the left button");
  if( Ps3.event.button_up.left )
      Serial.println("Released the left button");

  // Digital shoulder button events 
  if( Ps3.event.button_down.l1 )
      Serial.println("Started pressing the left shoulder button");
  if( Ps3.event.button_up.l1 )
      Serial.println("Released the left shoulder button");

  if( Ps3.event.button_down.r1 )
      Serial.println("Started pressing the right shoulder button");
  if( Ps3.event.button_up.r1 )
      Serial.println("Released the right shoulder button");

  // Digital trigger button events 
  if( Ps3.event.button_down.l2 )
      Serial.println("Started pressing the left trigger button");
  if( Ps3.event.button_up.l2 )
      Serial.println("Released the left trigger button");

  if( Ps3.event.button_down.r2 )
      Serial.println("Started pressing the right trigger button");
  if( Ps3.event.button_up.r2 )
      Serial.println("Released the right trigger button");

  // Digital stick button events 
  if( Ps3.event.button_down.l3 )
      Serial.println("Started pressing the left stick button");
  if( Ps3.event.button_up.l3 )
      Serial.println("Released the left stick button");

  if( Ps3.event.button_down.r3 )
      Serial.println("Started pressing the right stick button");
  if( Ps3.event.button_up.r3 )
      Serial.println("Released the right stick button");

  // Digital select/start/ps button events 
  if( Ps3.event.button_down.select )
      Serial.println("Started pressing the select button");
  if( Ps3.event.button_up.select )
      Serial.println("Released the select button");

  if( Ps3.event.button_down.start )
      Serial.println("Started pressing the start button");
  if( Ps3.event.button_up.start )
      Serial.println("Released the start button");

  if( Ps3.event.button_down.ps )
      Serial.println("Started pressing the Playstation button");
  if( Ps3.event.button_up.ps )
      Serial.println("Released the Playstation button");

  // Analog stick value events 
  if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
      Serial.print("Moved the left stick:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
      Serial.println();
  }

  if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
      Serial.print("Moved the right stick:");
      Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
      Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
      Serial.println();
  }

  // Analog Dpad button events 
  if( abs(Ps3.event.analog_changed.button.up) ){
      Serial.print("Pressing the up button: ");
      Serial.println(Ps3.data.analog.button.up, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.right) ){
      Serial.print("Pressing the right button: ");
      Serial.println(Ps3.data.analog.button.right, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.down) ){
      Serial.print("Pressing the down button: ");
      Serial.println(Ps3.data.analog.button.down, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.left) ){
      Serial.print("Pressing the left button: ");
      Serial.println(Ps3.data.analog.button.left, DEC);
  }

  // Analog shoulder/trigger button events 
  if( abs(Ps3.event.analog_changed.button.l1)){
      Serial.print("Pressing the left shoulder button: ");
      Serial.println(Ps3.data.analog.button.l1, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.r1) ){
      Serial.print("Pressing the right shoulder button: ");
      Serial.println(Ps3.data.analog.button.r1, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.l2) ){
      Serial.print("Pressing the left trigger button: ");
      Serial.println(Ps3.data.analog.button.l2, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.r2) ){
      Serial.print("Pressing the right trigger button: ");
      Serial.println(Ps3.data.analog.button.r2, DEC);
  }

  // Analog cross/square/triangle/circle button events 
  if( abs(Ps3.event.analog_changed.button.triangle)){
      Serial.print("Pressing the triangle button: ");
      Serial.println(Ps3.data.analog.button.triangle, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.circle) ){
      Serial.print("Pressing the circle button: ");
      Serial.println(Ps3.data.analog.button.circle, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.cross) ){
      Serial.print("Pressing the cross button: ");
      Serial.println(Ps3.data.analog.button.cross, DEC);
  }

  if( abs(Ps3.event.analog_changed.button.square) ){
      Serial.print("Pressing the square button: ");
      Serial.println(Ps3.data.analog.button.square, DEC);
  }

  // Battery events 
  if( battery != Ps3.data.status.battery ){
      battery = Ps3.data.status.battery;
      Serial.print("The controller battery is ");
      if( battery == ps3_status_battery_charging )      Serial.println("charging");
      else if( battery == ps3_status_battery_full )     Serial.println("FULL");
      else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
      else if( battery == ps3_status_battery_low)       Serial.println("LOW");
      else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
      else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
      else Serial.println("UNDEFINED");
  }
}

 
// PS3 Connect/Disconnect
void onConnect() {
  Serial.println("PS3 Connected!");
}

void onDisconnect() {
  driveMotor(0, 0);
  Serial.println("PS3 Disconnected!");
}

// Setup Pin & PWM
void setupPins() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);

  // Setup PWM for ESP32 Arduino Core 3.x
  ledcAttach(ENA, freq, res);
  ledcAttach(ENB, freq, res);
}

// Setup
void setup() {
  setupPins();
  Serial.begin(115200);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisconnect);
  Ps3.begin(Ps3Mac);

  Serial.println("PS3 Control Ready");
}

// Main loop
void loop() {
  if (!Ps3.isConnected()) {
    driveMotor(0, 0);
    return;
  }

  // Player LEDs 
  Serial.print("Setting LEDs to player "); Serial.println(player, DEC);
  Ps3.setPlayer(player);

  player += 1;
  if(player > 10) player = 0;

  // Digital cross/square/triangle/circle buttons 
  if( Ps3.data.button.cross && Ps3.data.button.down )
      Serial.println("Pressing both the down and cross buttons");
  if( Ps3.data.button.square && Ps3.data.button.left )
      Serial.println("Pressing both the square and left buttons");
  if( Ps3.data.button.triangle && Ps3.data.button.up )
      Serial.println("Pressing both the triangle and up buttons");
  if( Ps3.data.button.circle && Ps3.data.button.right )
      Serial.println("Pressing both the circle and right buttons");

  if( Ps3.data.button.l1 && Ps3.data.button.r1 )
      Serial.println("Pressing both the left and right bumper buttons");
  if( Ps3.data.button.l2 && Ps3.data.button.r2 )
      Serial.println("Pressing both the left and right trigger buttons");
  if( Ps3.data.button.l3 && Ps3.data.button.r3 )
      Serial.println("Pressing both the left and right stick buttons");
  if( Ps3.data.button.select && Ps3.data.button.start )
      Serial.println("Pressing both the select and start buttons");

  // Set forward & backward speed *(255)
  int throttle = map(yAxis, 127, 128, 250, 250);   // Bottom: 127, Center: 0, Top: 128
  int steering = map(xAxis, 128, 127, 250, 250);   // Left: 128, Center: 0, Right: 127

  if (abs(throttle) < deadzone && abs(steering) < deadzone) {
    currentStatus = "Stop";
    rightMotorSpeed = 0;
    leftMotorSpeed  = 0;
  }
  else if (throttle > deadzone) {
    //  Move Forward 
    rightMotorSpeed = throttle + steering * corr_factor;
    leftMotorSpeed  = throttle  steering * corr_factor;

    if (steering > deadzone) {
      currentStatus = "Forward Left";
    }
    else if (steering < deadzone) {
      currentStatus = "Forward Right";
    }
    else {
      currentStatus = "Forward";
    }
  }
  else if (throttle < deadzone) {
    //  Move Backward 
    rightMotorSpeed = throttle  steering * corr_factor;
    leftMotorSpeed  = throttle + steering * corr_factor;

    if (steering > deadzone) {
      currentStatus = "Backward Left";
    }
    else if (steering < deadzone) {
      currentStatus = "Backward Right";
    }
    else {
      currentStatus = "Backward";
    }
  }
  else if (steering > deadzone) {
    //  Rotate in place to the left 
    currentStatus = "Rotate Left";
    rightMotorSpeed = rotate_speed;  // right motor forward
    leftMotorSpeed = rotate_speed;  // left motor backward
  }
  else if (steering < deadzone) {
    //  Rotate in place to the right 
    currentStatus = "Rotate Right";
    rightMotorSpeed = rotate_speed; // right motor backward
    leftMotorSpeed = rotate_speed;   // left motor forward
  }
  else {
    //  Default Stop 
    currentStatus = "Stop";
    rightMotorSpeed = 0;
    leftMotorSpeed  = 0;
  }

  // Limit speed so it does not exceed PWM maximum 
  rightMotorSpeed = constrain(rightMotorSpeed, 255, 255);
  leftMotorSpeed  = constrain(leftMotorSpeed, 255, 255);

  // Run the motor
  driveMotor(rightMotorSpeed, leftMotorSpeed);

  // Print status
  Serial.print("Status: ");
  Serial.println(currentStatus);

}
