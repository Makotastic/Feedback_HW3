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
float targetAngle = -33;
String inputString = "";

int pwm;

// Data collection variables
#define MAX_SAMPLES 1000
float pitchV2_values[MAX_SAMPLES];
int pwm_values[MAX_SAMPLES];
int dataIndex = 0;
bool isCollecting = false;


void checkSerial();
void printData();

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

      // Collect data if active and there's space
    if (isCollecting && dataIndex < MAX_SAMPLES) {
      pitchV2_values[dataIndex] = pitchV2;
      pwm_values[dataIndex] = pwm;
      dataIndex++;
      
      // Auto-stop if buffer is full
      if (dataIndex >= MAX_SAMPLES) {
        isCollecting = false;
        Serial.println("Data collection auto-stopped - buffer full");
        printData();
      }
    }
  }
  
  if (micros() - prevTimeCount > 10000) 
  {
    // Serial.print("Hz: ");
    // Serial.println(count);
    // Serial.print("Pitch: ");
    // Serial.println(pitchV2);
    // Serial.print("Rate: ");
    // Serial.println(pitchRateV2);
    // Serial.print("PWM: ");
    // Serial.println(pwm);
    count = 0;
    prevTimeCount = micros();
  }
  count++;
}

void checkSerial() {
  while (Serial.available() > 0) {
    inputString = Serial.readStringUntil('\n');  // Read until newline
    inputString.trim();
    if (inputString.length() <= 2) {
      if (inputString == "s") {
        isCollecting = true;
        dataIndex = 0;
        Serial.println("Data collection started");
      } else if (inputString == "e") {
        isCollecting = false;
        Serial.println("Data collection ended");
        printData();
      } else if (inputString == "i") {
        resetI();
        Serial.printf("Pitch: %.2f Rate: %.2f PWM: %d\n", pitchV2, pitchRateV2, pwm);
        // Serial.print("print test");
        
      }
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

void printData() {
  Serial.println("Data Collection Results:");
  Serial.println("Index,Pitch,PWM");
  for (int i = 0; i < dataIndex; i++) {
    Serial.printf("%d,%.2f,%d\n", i, pitchV2_values[i], pwm_values[i]);
  }
  Serial.println("End of data");
}