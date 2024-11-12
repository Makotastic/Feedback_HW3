#include <Arduino.h>
#include "IMU6050_v2.h"
#include "TiltController.h"
#include "MotorControl.h"

int sda_pin = 9; // GPIO16 as I2C SDA
int scl_pin = 8; // GPIO17 as I2C SCL

unsigned int count = 0;
unsigned long prevTimeCount = micros();
unsigned int countTiltPID = 0;
unsigned long prevLoop = micros();
float targetAngle = -30;
String inputString = "";

int pwm;

void checkSerial();

void setup() {
  Serial.begin(115200);
  Wire.setPins(sda_pin, scl_pin);
  IMU6050setup();
  setUpPWM();
  Serial.println("setup over");
}

void loop()
{
  checkSerial();
  if (IMU6050loop()) {
  pwm = CalcMotorPower(targetAngle,pitchV2,pitchRateV2);
    setPWM(pwm);
  }
  
  if (micros() - prevTimeCount > 1000000) 
  {
    Serial.print("Hz: ");
    Serial.println(count);
    Serial.print("Pitch: ");
    Serial.println(pitchV2);
    Serial.print("Rate: ");
    Serial.println(pitchRateV2);
    Serial.print("PWM: ");
    Serial.println(pwm);
    count = 0;
    prevTimeCount = micros();
  }
  count++;
}

void checkSerial() {
  while (Serial.available() > 0) {
    inputString = Serial.readStringUntil('\n');  // Read until newline
    if (inputString.length() < 3) {
      resetI();
      continue;
    }
    
    // Find the positions of spaces
    int firstSpace = inputString.indexOf(' ');
    int secondSpace = inputString.indexOf(' ', firstSpace + 1);
    
    // Extract substrings and convert to float
    String kpString = inputString.substring(0, firstSpace);
    String kiString = inputString.substring(firstSpace + 1, secondSpace);
    String kdString = inputString.substring(secondSpace + 1);
    
    // Convert to float
    SetGains(kpString.toFloat(), kiString.toFloat(), kdString.toFloat());
  }
}
