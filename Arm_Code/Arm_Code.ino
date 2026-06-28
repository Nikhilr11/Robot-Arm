//  #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

// // Create the PWM driver object
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// // Servo channels
// const int servo1_ch = 0;
// const int servo2_ch = 1;
// const int servo3_ch = 2;
// const int servo4_ch = 3;  // New servo for accelerometer data

// // Continuous Servo PWM values
// const int STOP_SIGNAL = 307;      // Neutral (no movement)
// const int FULL_FORWARD = 409;     // Full forward speed
// const int FULL_BACKWARD = 204;    // Full backward speed

// // Neutral offsets for each motor (resting position adjustments)
// const int NEUTRAL_PWM_OFFSET1 = 10;
// const int NEUTRAL_PWM_OFFSET2 = 15;
// const int NEUTRAL_PWM_OFFSET3 = 80;  // Still reversed for actual use
// const int NEUTRAL_PWM_OFFSET4 = 5;

// const float SPEED_SCALE = 0.3; // Higher = faster response

// // Smoothed PWM values
// int smoothedPWM1 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET1;
// int smoothedPWM2 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET2;
// int smoothedPWM3 = STOP_SIGNAL - NEUTRAL_PWM_OFFSET3;  // Neutral: FORWARD only
// int smoothedPWM4 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET4;

// void setup() {
//   Serial.begin(4800);
//   Serial1.begin(4800);
//   Wire.begin();
//   pwm.begin();
//   pwm.setOscillatorFrequency(27000000);
//   pwm.setPWMFreq(50);

//   Serial.println("Mega ready. Waiting for data...");

//   // Neutral setup: servo3 forward at rest (non-reversed)
//   pwm.setPWM(servo1_ch, 0, smoothedPWM1);
//   pwm.setPWM(servo2_ch, 0, smoothedPWM2);
//   pwm.setPWM(servo3_ch, 0, smoothedPWM3);  // Neutral only: forward
//   pwm.setPWM(servo4_ch, 0, smoothedPWM4);
// }

// void loop() {
//   if (Serial1.available() > 0) {
//     char data[32] = {0};
//     int len = Serial1.readBytesUntil('\n', data, sizeof(data) - 1);

//     if (len > 0) {
//       data[len] = '\0';

//       int flex1, flex2, flex3, accel;
//       int parsed = sscanf(data, "%d,%d,%d,%d", &flex1, &flex2, &flex3, &accel);

//       if (parsed == 4) {
//         Serial.print("Received: ");
//         Serial.print(flex1); Serial.print(", ");
//         Serial.print(flex2); Serial.print(", ");
//         Serial.print(flex3); Serial.print(", ");
//         Serial.println(accel);

//         int targetPWM1 = map(flex1, 1400, 1410, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET1;
//         int targetPWM2 = map(flex2, 1400, 1402, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET2;

//         // REVERSED range for motor 3: 1410 = backward, 1400 = forward
//         int targetPWM3 = map(flex3, 1410, 1400, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET3;

//         int targetPWM4 = map(accel, 1480, 1500, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET4;

//         smoothedPWM1 += (targetPWM1 - smoothedPWM1) * SPEED_SCALE;
//         smoothedPWM2 += (targetPWM2 - smoothedPWM2) * SPEED_SCALE;
//         smoothedPWM3 += (targetPWM3 - smoothedPWM3) * SPEED_SCALE;
//         smoothedPWM4 += (targetPWM4 - smoothedPWM4) * SPEED_SCALE;

//         pwm.setPWM(servo1_ch, 0, smoothedPWM1);
//         pwm.setPWM(servo2_ch, 0, smoothedPWM2);
//         pwm.setPWM(servo3_ch, 0, smoothedPWM3);  // REVERSED for actual movement
//         pwm.setPWM(servo4_ch, 0, smoothedPWM4);

//         Serial.print("PWM Set: ");
//         Serial.print(smoothedPWM1); Serial.print(", ");
//         Serial.print(smoothedPWM2); Serial.print(", ");
//         Serial.print(smoothedPWM3); Serial.print(" (reversed for data), ");
//         Serial.println(smoothedPWM4);
//       } else {
//         Serial.println("❌ ERROR: Parsing failed!");
//       }
//     }
//   }
// }



// #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

// // Create the PWM driver object
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// // Servo channels
// const int servo1_ch = 0;
// const int servo2_ch = 1;
// const int servo3_ch = 2;
// const int servo4_ch = 3;

// // Continuous Servo PWM values
// const int STOP_SIGNAL = 307;      
// const int FULL_FORWARD = 409;     
// const int FULL_BACKWARD = 204;    

// // Neutral offsets (adjust if servos don't stop at STOP_SIGNAL)
// const int NEUTRAL_PWM_OFFSET1 = 10;
// const int NEUTRAL_PWM_OFFSET2 = 15;
// const int NEUTRAL_PWM_OFFSET3 = 20;
// const int NEUTRAL_PWM_OFFSET4 = 5;

// const float SPEED_SCALE = 0.3;

// int smoothedPWM1 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET1;
// int smoothedPWM2 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET2;
// int smoothedPWM3 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET3;
// int smoothedPWM4 = STOP_SIGNAL + NEUTRAL_PWM_OFFSET4;

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   pwm.begin();
//   pwm.setOscillatorFrequency(27000000);
//   pwm.setPWMFreq(50);

//   Serial.println("Mega ready with 4 pots.");

//   // Initial servo stop
//   pwm.setPWM(servo1_ch, 0, smoothedPWM1);
//   pwm.setPWM(servo2_ch, 0, smoothedPWM2);
//   pwm.setPWM(servo3_ch, 0, smoothedPWM3);
//   pwm.setPWM(servo4_ch, 0, smoothedPWM4);
// }

// void loop() {
//   int pot1 = analogRead(A0); // Pot 1
//   int pot2 = analogRead(A1); // Pot 2
//   int pot3 = analogRead(A2); // Pot 3
//   int pot4 = analogRead(A3); // Pot 4

//   int pwm1 = map(pot1, 0, 1023, 1400, 1600);
//   int pwm2 = map(pot2, 0, 1023, 1400, 1600);
//   int pwm3 = map(pot3, 0, 1023, 1400, 1600);
//   int pwm4 = map(pot4, 0, 1023, 1400, 1600);

//   if (abs(pwm1 - 1500) < 5) pwm1 = 1500;
//   if (abs(pwm2 - 1500) < 5) pwm2 = 1500;
//   if (abs(pwm3 - 1500) < 5) pwm3 = 1500;
//   if (abs(pwm4 - 1500) < 5) pwm4 = 1500;

//   // Map to servo PWM and smooth (double the speed of change)
//   int target1 = map(pwm1, 1400, 1600, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET1;
//   int target2 = map(pwm2, 1400, 1600, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET2;
//   int target3 = map(pwm3, 1400, 1600, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET3;
//   int target4 = map(pwm4, 1400, 1600, FULL_BACKWARD, FULL_FORWARD) + NEUTRAL_PWM_OFFSET4;

//   smoothedPWM1 += 2 * (target1 - smoothedPWM1) * SPEED_SCALE;
//   smoothedPWM2 += 2 * (target2 - smoothedPWM2) * SPEED_SCALE;
//   smoothedPWM3 += 2 * (target3 - smoothedPWM3) * SPEED_SCALE;
//   smoothedPWM4 += 2 * (target4 - smoothedPWM4) * SPEED_SCALE;

//   pwm.setPWM(servo1_ch, 0, smoothedPWM1);
//   pwm.setPWM(servo2_ch, 0, smoothedPWM2);
//   pwm.setPWM(servo3_ch, 0, smoothedPWM3);
//   pwm.setPWM(servo4_ch, 0, smoothedPWM4);

//   Serial.print("PWM: ");
//   Serial.print(smoothedPWM1); Serial.print(", ");
//   Serial.print(smoothedPWM2); Serial.print(", ");
//   Serial.print(smoothedPWM3); Serial.print(", ");
//   Serial.println(smoothedPWM4);

//   delay(50);
// }




#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int servoChannels[4] = {0, 1, 2, 5}; // Skipping channels 3 and 4
const int PWM_MIN = 102;
const int PWM_MAX = 512;

void setup() {
  Serial.begin(4800);
  Serial1.begin(4800);

  Wire.begin();
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  Serial.println("Mega ready...");
  for (int i = 0; i < 4; i++) {
    int centerPulse = map(90, 0, 180, PWM_MIN, PWM_MAX);
    pwm.setPWM(servoChannels[i], 0, centerPulse);
  }
}

void loop() {
  if (Serial1.available()) {
    char data[48] = {0};
    int len = Serial1.readBytesUntil('\n', data, sizeof(data) - 1);
    if (len > 0) {
      data[len] = '\0';

      int pots[4];
      if (sscanf(data, "%d,%d,%d,%d", &pots[0], &pots[1], &pots[2], &pots[3]) == 4) {
        for (int i = 0; i < 4; i++) {
          int angle = map(pots[i], 0, 1023, 0, 180);  // Adjust to glove range
          angle = constrain(angle, 0, 180);
          int pulse = map(angle, 0, 180, PWM_MIN, PWM_MAX);
          pwm.setPWM(servoChannels[i], 0, pulse);
        }
      }
    }
  }
}
