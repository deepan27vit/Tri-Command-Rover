#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // TX=10, RX=11 (to Receiver)

MPU6050 mpu;
bool dmpReady = false;
uint8_t fifoBuffer[64];
Quaternion q; VectorFloat gravity; float ypr[3];
float pitch, roll;
volatile bool mpuInterrupt = false;
uint8_t packetSize;
void dmpDataReady() { mpuInterrupt = true; }

void setup() {
  Wire.begin(); BTSerial.begin(9600); Serial.begin(38400);
  mpu.initialize();
  mpu.setXGyroOffset(126); mpu.setYGyroOffset(57); mpu.setZGyroOffset(-69); mpu.setZAccelOffset(1869);
  mpu.setDMPEnabled(true);
  attachInterrupt(0, dmpDataReady, RISING);
  packetSize = mpu.dmpGetFIFOPacketSize();
}

void loop() {
  if (!dmpReady) return;

  if (mpuInterrupt) {
    mpuInterrupt = false;
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      roll = ypr[2] * 180/M_PI;
      pitch = ypr[1] * 180/M_PI;

      // Translate tilt to direction
      if (pitch > 30) BTSerial.write('F');
      else if (pitch < -30) BTSerial.write('B');
      else if (roll > 30) BTSerial.write('R');
      else if (roll < -30) BTSerial.write('L');
      else BTSerial.write('S');
    }
  }

  // Voice command via app sends 'F', 'B', 'L', 'R', 'S', 'gesture', 'app'
  if (Serial.available()) {
    char voiceCommand = Serial.read();
    BTSerial.write(voiceCommand);
  }
}
