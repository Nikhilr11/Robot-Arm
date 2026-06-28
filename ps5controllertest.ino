#include <Bluepad32.h>
#include <Arduino.h>

// -------- Bluepad32 controller setup --------
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Servo pins
const int SHOULDER_PIN = 25;
const int ELBOW_PIN    = 26;
const int WRIST_PIN    = 27;

// LEDC setup
const int SERVO_FREQ = 50;
const int SERVO_RESOLUTION = 16;

// Servo limits
const int SERVO_MIN = 0;
const int SERVO_MAX = 180;

// Arm segment lengths (mm)
const float L1 = 108.0;  // Shoulder → elbow
const float L2 = 93.5;   // Elbow → wrist tip

// Target position
float targetX = L1 + L2; // start fully extended forward
float targetY = 0;

// Current angles
float shoulderAngle = 90;
float elbowAngle = 90;
float wristAngle = 90;

// Speed control
const float STEP = 2.0; // degrees per loop for smooth motion

void setup() {
  Serial.begin(115200);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();

  // Setup LEDC for servos
  ledcSetup(0, SERVO_FREQ, SERVO_RESOLUTION);
  ledcAttachPin(SHOULDER_PIN, 0);
  ledcSetup(1, SERVO_FREQ, SERVO_RESOLUTION);
  ledcAttachPin(ELBOW_PIN, 1);
  ledcSetup(2, SERVO_FREQ, SERVO_RESOLUTION);
  ledcAttachPin(WRIST_PIN, 2);

  // Initialize servos
  writeServo(0, shoulderAngle);
  writeServo(1, elbowAngle);
  writeServo(2, wristAngle);

  Serial.println("Ready. Pair PS5 controller.");
}

void loop() {
  BP32.update();

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr ctl = myControllers[i];
    if (ctl && ctl->isConnected()) {

      int16_t joyX = ctl->axisX(); // left stick horizontal
      int16_t joyY = ctl->axisY(); // left stick vertical

      // Map joystick (-512 → 512) to target motion scale
      float deltaX = map(joyX, -512, 512, -2, 2); // forward/back mm
      float deltaY = map(joyY, -512, 512, -2, 2); // up/down mm

      targetX += deltaX;
      targetY += deltaY;

      // Clamp reachable target
      float dist = sqrt(targetX * targetX + targetY * targetY);
      float maxReach = L1 + L2;
      if (dist > maxReach) {
        float scale = maxReach / dist;
        targetX *= scale;
        targetY *= scale;
      }
      if (targetY < 0) targetY = 0; // cannot go below base

      // Inverse Kinematics
      float cosElbow = (targetX*targetX + targetY*targetY - L1*L1 - L2*L2) / (2*L1*L2);
      if (cosElbow > 1) cosElbow = 1;
      if (cosElbow < -1) cosElbow = -1;
      float elbowRad = acos(cosElbow);

      float k1 = L1 + L2 * cos(elbowRad);
      float k2 = L2 * sin(elbowRad);
      float shoulderRad = atan2(targetY, targetX) - atan2(k2, k1);

      // Convert to degrees
      float desiredShoulder = shoulderRad * 180.0 / PI;
      float desiredElbow   = elbowRad * 180.0 / PI;

      // Smooth motion
      shoulderAngle += constrain(desiredShoulder - shoulderAngle, -STEP, STEP);
      elbowAngle    += constrain(desiredElbow - elbowAngle, -STEP, STEP);

      // Wrist simply keeps tip horizontal
      wristAngle = 180 - (shoulderAngle + elbowAngle);

      // Write to servos
      writeServo(0, shoulderAngle);
      writeServo(1, elbowAngle);
      writeServo(2, wristAngle);

      // Debug
      Serial.print("Shoulder: "); Serial.print(shoulderAngle);
      Serial.print(" | Elbow: "); Serial.print(elbowAngle);
      Serial.print(" | Wrist: "); Serial.println(wristAngle);
    }
  }

  delay(15);
}

// -------- Servo write function --------
void writeServo(int channel, float angle) {
  angle = constrain(angle, SERVO_MIN, SERVO_MAX);
  int pulse = map(angle, 0, 180, 500, 2400); // us
  uint32_t duty = (uint32_t)((pulse / 20000.0) * 65535);
  ledcWrite(channel, duty);
}

// -------- Controller callbacks --------
void onConnectedController(ControllerPtr ctl) {
  Serial.println("Controller connected!");
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      myControllers[i] = ctl;
      return;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  Serial.println("Controller disconnected!");
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      myControllers[i] = nullptr;
      return;
    }
  }
}









