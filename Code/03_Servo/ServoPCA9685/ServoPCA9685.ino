/***********************************************************
File name: ServoPCA9685.ino
Description: Control the servo through PCA9685.
***********************************************************/
/*
PCA9685 can set the update frequency, and the time base pulse period of 20ms is equivalent to 50HZ update frequency. PCA9685 uses a 12-bit register to control the PWM ratio. For 0.5ms, it is equivalent to a register value of 0.5/20*4096=102. And so on as follows:
0.5ms——————0 degrees: 0.5/20*4096 = 102
1.0ms————45 degrees: 1/20*4096 = 204
1.5ms———90 degrees: 1.5/20*4096 = 306
2.0ms———–135 degrees: 2/20*4096 = 408
2.5ms———–180 degrees: 2.5/20*4096 =510
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define FREQ 50 // Analog servos run at ~50 Hz updates
#define PWM_Max 510 
#define PWM_Min 102 
#define Angle_Range 180.0 

// called this way, PCA9685 uses the address 0x5F 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x5F);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(50);  // Set PWM frequency to 50Hz
}

void loop() {
  setAngle(6, 0, 90);   // CH6 servo turn to 90°, servo have angular deviation
  delay(1000);          // delay 1s.
  
  setAngle(6, 0, 0);    // turn to 0°
  delay(1000);
  
  setAngle(6, 0, 180);  // rurn to 180°
  delay(1000);
}

// Convert PWM value to angle value.
int setAngle(int num, int start, int angle){
  // num: servo channel.
  // start: starting point of the pulse signal.
  // angle: servo angle.
  int pwm_value = int(PWM_Min +((PWM_Max - PWM_Min)/Angle_Range)*angle);
  Serial.println(pwm_value);
  pwm.setPWM(num, start, pwm_value);
  }
