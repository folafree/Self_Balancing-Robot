#include <Wire.h>
#include <Servo.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

Servo servoPitch;  // top servo (front/back)
Servo servoRoll;   // bottom servo (left/right)

// Pins
const int SERVO_PITCH_PIN = 9;
const int SERVO_ROLL_PIN  = 10;

// Centers – tweak later so tray is level
int pitchCenter = 90;
int rollCenter  = 85;

// Servo limits
const int SERVO_MIN = 40;
const int SERVO_MAX = 140;

// Target (level)
float setPitch = 0.0;
float setRoll  = 0.0;

// Filtered angles
float pitchFilt = 0.0;
float rollFilt  = 0.0;

unsigned long lastMs = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin();
  byte status = mpu.begin();
  while (status != 0) {
    Serial.println("MPU6050 not found, check wiring!");
    delay(1000);
  }

  Serial.println("Calibrating... keep platform still.");
  delay(1500);
  mpu.calcOffsets(true, true);

  servoPitch.attach(SERVO_PITCH_PIN);
  servoRoll.attach(SERVO_ROLL_PIN);
  servoPitch.write(pitchCenter);
  servoRoll.write(rollCenter);

  lastMs = millis();
}

void loop() {
  mpu.update();

  // Raw angles from IMU
  float pitchRaw = mpu.getAngleX();
  float rollRaw  = mpu.getAngleY();

  // Low-pass filter (smooth)
  const float alpha = 0.1;  // 0.1–0.2 = smooth
  pitchFilt = pitchFilt + alpha * (pitchRaw - pitchFilt);
  rollFilt  = rollFilt  + alpha * (rollRaw  - rollFilt);

  unsigned long now = millis();
  float dt = (now - lastMs) / 1000.0;
  if (dt <= 0.0) dt = 0.001;
  lastMs = now;

  // ===== VERY SIMPLE P-ONLY CONTROL =====
  float Kp = 1.5;          // gentle
  const float DEADBAND = 2.0;   // ignore small errors
  const float MAX_OUTPUT = 20;  // max +/- servo steps

  // ---------- PITCH ----------
  float eP = setPitch - pitchFilt;
  if (fabs(eP) < DEADBAND) eP = 0;

  float uP = Kp * eP;

  if (uP >  MAX_OUTPUT) uP =  MAX_OUTPUT;
  if (uP < -MAX_OUTPUT) uP = -MAX_OUTPUT;


  int pitchCmd = pitchCenter + (int)uP;
  pitchCmd = constrain(pitchCmd, SERVO_MIN, SERVO_MAX);

  // ---------- ROLL ----------
  float eR = setRoll - rollFilt;
  if (fabs(eR) < DEADBAND) eR = 0;

  float uR = Kp * eR;

  if (uR >  MAX_OUTPUT) uR =  MAX_OUTPUT;
  if (uR < -MAX_OUTPUT) uR = -MAX_OUTPUT;


  int rollCmd = rollCenter + (int)uR;
  rollCmd = constrain(rollCmd, SERVO_MIN, SERVO_MAX);

  // ---------- SEND TO SERVOS ----------
  servoPitch.write(pitchCmd);
  servoRoll.write(rollCmd);

  delay(12);
}
