#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define PIN_MOTOR_M1_IN1 15      //Define the positive pole of M1
#define PIN_MOTOR_M1_IN2 14      //Define the negative pole of M1
#define PIN_MOTOR_M2_IN1 12      //Define the positive pole of M2
#define PIN_MOTOR_M2_IN2 13      //Define the negative pole of M2
#define PIN_MOTOR_M3_IN1 11      //Define the positive pole of M3
#define PIN_MOTOR_M3_IN2 10      //Define the negative pole of M3
#define PIN_MOTOR_M4_IN1 8       //Define the positive pole of M4
#define PIN_MOTOR_M4_IN2 9       //Define the negative pole of M4
int speed=80;

// called this way, PCA9685 uses the address 0x5F.
Adafruit_PWMServoDriver pwm_motor = Adafruit_PWMServoDriver(0x5F);

void setup() {
  pwm_motor.begin();
  pwm_motor.setPWMFreq(2000);  
}

void loop() {
  //Motor(motor_ID, direction, speed)
  // motor_ID: Motor number, 1-4(M1~M4)
  // direction: Motor rotation direction. 1 or -1.
  // speed: Motor speed. 0-100.
  Motor(1, 1, speed); // M1, forward rotation, fast rotation.
  Motor(2, 1, speed);  // M2, forward rotation,low rotation.
  Motor(3, 1, speed); // M3, forward rotation, fast rotation.
  Motor(4, 1, speed); // M4, forward rotation, fast rotation.
  delay(2000);      

  Motor(1,-1, speed);  //backward
  Motor(2,-1, speed);
  Motor(3,-1, speed);
  Motor(4,-1, speed);
  delay(2000);

  Motor(1,-1, speed);  //turn left
  Motor(2,-1, speed);
  Motor(3,1, speed);
  Motor(4,1, speed);
  delay(2000);

  Motor(1,1, speed);  //turn right
  Motor(2,1, speed);
  Motor(3,-1, speed);
  Motor(4,-1, speed);
  delay(2000);
   
  Motor(1, -1, speed);   //drift left
  Motor(2, 1, speed);
  Motor(3, -1, speed);
  Motor(4, 1, speed);
  delay(2000);

  Motor(1, 1, speed);    //drift right
  Motor(2, -1, speed);
  Motor(3, 1, speed);
  Motor(4, -1, speed);
  delay(2000);

  Motor(1,1, 0);    //drift front-left
  Motor(2,1, speed);
  Motor(3,1, 0);
  Motor(4,1, speed);
  delay(2000);

  Motor(1,-1, 0);   //drift rear-right
  Motor(2,-1, speed);
  Motor(3,-1, 0);
  Motor(4,-1, speed);
  delay(2000);

  Motor(1,1, speed);        //drift front-right
  Motor(2,1, 0);
  Motor(3,1, speed);
  Motor(4,1, 0);
  delay(2000);

  Motor(1,-1, speed);        //drift rear-left
  Motor(2,-1, 0);
  Motor(3,-1, speed);
  Motor(4,-1, 0);
  delay(2000);

}

// Convert motor speed to PWM value.
void motorPWM(int channel, int motor_speed){
  motor_speed = constrain(motor_speed, 0, 100);
  int motor_pwm = map(motor_speed, 0, 100, 0, 4095);
  if (motor_pwm == 4095){
    pwm_motor.setPWM(channel, 4096, 0);
  }
  else if (motor_pwm == 0){
    pwm_motor.setPWM(channel, 0, 4096);
  }
  else{
    pwm_motor.setPWM(channel, 0, motor_pwm);
    // pwm_motor.setPWM(channel, 0, 4095 - motor_pwm);
  }
}

// Control motor rotation.
void Motor(int Motor_ID, int dir, int Motor_speed){
  if(dir > 0){dir = 1;}
  else {dir = -1;} 

  if (Motor_ID == 1){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M1_IN1, 0);
      motorPWM(PIN_MOTOR_M1_IN2, Motor_speed);
    }
    else{
      motorPWM(PIN_MOTOR_M1_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M1_IN2, 0);
      }
  }
  else if (Motor_ID == 2){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M2_IN1, 0);
      motorPWM(PIN_MOTOR_M2_IN2, Motor_speed);
    }
    else{
      motorPWM(PIN_MOTOR_M2_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M2_IN2, 0);
      }
  }
  else if (Motor_ID == 3){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M3_IN1, 0);
      motorPWM(PIN_MOTOR_M3_IN2, Motor_speed);
    }
    else{
      motorPWM(PIN_MOTOR_M3_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M3_IN2, 0);
      }
  }
  else if (Motor_ID == 4){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M4_IN1, 0);
      motorPWM(PIN_MOTOR_M4_IN2, Motor_speed);
    }
    else{
      motorPWM(PIN_MOTOR_M4_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M4_IN2, 0);
      }
  }
}
