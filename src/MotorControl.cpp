#include <Arduino.h>
int IN1 = 1;  // Input pin 1 (controls direction)

const int frequency = 1000;  // PWM frequency (1kHz)
const int pwm_channelA = 0;   // PWM channel (using channel 0)
const int resolution = 8;    // 8-bit resolution (0-255 for speed control)

void setUpPWM() {
    pinMode(IN1, OUTPUT);

    // Setup PWM on the IN1 pin
    ledcSetup(pwm_channelA, frequency, resolution);
    ledcAttachPin(IN1, pwm_channelA);
    ledcWrite(pwm_channelA, 0);
}

void setPWM(int pwm) {
    ledcWrite(pwm_channelA, abs(pwm));
}
