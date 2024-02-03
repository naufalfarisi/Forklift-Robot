// Include necessary libraries and define custom settings
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// Define pin configurations for motor 1
const int IN1 = 12;
const int IN2 = 13;
const int IN3 = A1; // Front right wheel
const int IN4 = A0; // Front right wheel
const int EN_A = 5;
const int EN_B = 6;

// Define pin configurations for motor 2
const int IN21 = 8;
const int IN22 = 9;
const int IN23 = 7; // Rear right wheel
const int IN24 = 4; // Rear right wheel
const int EN_2A = 11;
const int EN_2B = 10;

// Define pin configurations for servo
const int IN31 = A3;
const int IN32 = A4;

const int trigger = 400;
int SpeedR;
int SpeedL; // 10 difference compared to SpeedR
int SpeedR2;
int SpeedL2;
int velo;

int arah; // Direction variable
int putar; // Rotation variable

// Function to control motors based on speeds
void motor(int speedL, int speedR, int speedL2, int speedR2) {
  // Motor 1 (Front right)
  if (speedL2 < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(EN_A, -speedL2);
  } else if (speedL2 > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(EN_A, speedL2);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // Motor 2 (Rear right)
  if (speedR < 0) {
    digitalWrite(IN21, LOW);
    digitalWrite(IN22, HIGH);
    analogWrite(EN_2A, -speedR);
  } else if (speedR > 0) {
    digitalWrite(IN21, HIGH);
    digitalWrite(IN22, LOW);
    analogWrite(EN_2A, speedR);
  } else {
    digitalWrite(IN21, LOW);
    digitalWrite(IN22, LOW);
  }

  // Motor 3 (Front left)
  if (speedR2 < 0) {
    delay(2);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN_B, -speedR2);
  } else if (speedR2 > 0) {
    delay(2);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(EN_B, speedR2);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  // Motor 4 (Rear left)
  if (speedL < 0) {
    digitalWrite(IN23, LOW);
    digitalWrite(IN24, HIGH);
    analogWrite(EN_2B, -speedL);
  } else if (speedL > 0) {
    digitalWrite(IN23, HIGH);
    digitalWrite(IN24, LOW);
    analogWrite(EN_2B, speedL);
  } else {
    digitalWrite(IN23, LOW);
    digitalWrite(IN24, LOW);
  }
}

// Mecanum wheel setup
int DEGA = 135;
int DEGB = 45;
int DEGC = 225;
int DEGD = 315;
float toRADS = 0.0174533;
float MAX_SPEED = 255;

// Function to drive the robot using mecanum wheels
void drive_mecanum(float Vel, float Dir, float Turn) {
  if (Dir < 0) Dir += 360;
  else if (Dir >= 360) Dir -= 360;

  float wa = (cos((DEGA + Dir) * toRADS) * Vel) + Turn;
  float wc = (cos((DEGC + Dir) * toRADS) * Vel) + Turn;
  float wb = (cos((DEGB + Dir) * toRADS) * Vel) + Turn;
  float wd = (cos((DEGD + Dir) * toRADS) * Vel) + Turn;

  if (wa > MAX_SPEED) wa = MAX_SPEED;
  else if (wa < -MAX_SPEED) wa = -MAX_SPEED;
  if (wc > MAX_SPEED) wc = MAX_SPEED;
  else if (wc < -MAX_SPEED) wc = -MAX_SPEED;
  if (wb > MAX_SPEED) wb = MAX_SPEED;
  else if (wb < -MAX_SPEED) wb = -MAX_SPEED;
  if (wd > MAX_SPEED) wd = MAX_SPEED;
  else if (wd < -MAX_SPEED) wd = -MAX_SPEED;

  motor(-wa, wb, wc, wd);
}

// Setup function
void setup() {
  Dabble.begin(9600);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);

  pinMode(IN21, OUTPUT);
  pinMode(IN22, OUTPUT);
  pinMode(IN23, OUTPUT);
  pinMode(IN24, OUTPUT);
  pinMode(EN_2A, OUTPUT);
  pinMode(EN_2B, OUTPUT);

  pinMode(IN31, OUTPUT);
  pinMode(IN32, OUTPUT);
}

// Loop function
void loop() {
  Dabble.processInput();

  // Check the direction buttons on the gamepad
  if (GamePad.isUpPressed()) {
    arah = 0;
    velo = 230;
    motor(160, 160, -160, 160);
  } else if (GamePad.isDownPressed()) {
    arah = 180;
    velo = 230;
    motor(-160, -160, 160, -160);
  } else if (GamePad.isLeftPressed()) {
    arah = 270;
    velo = 230;
    motor(-210, 210, -230, -210);
  } else if (GamePad.isRightPressed()) {
    arah = 90;
    velo = 230;
    motor(230, -225, 235, 215);
  } else if (GamePad.isTrianglePressed()) {
    digitalWrite(IN31, LOW);
    digitalWrite(IN32, HIGH);
  } else if (GamePad.isCrossPressed()) {
    digitalWrite(IN31, HIGH);
    digitalWrite(IN32, LOW);
  } else if (GamePad.isSquarePressed()) {
    motor(180, -180, -180, -180);
  } else if (GamePad.isCirclePressed()) {
    motor(-180, 180, 180, 180);
  } else {
    // Stop the robot if no button is pressed
    digitalWrite(IN31, LOW);
    digitalWrite(IN32, LOW);
    arah = 0;
    velo = 0;
    motor(0, 0, 0, 0);
  }
}
