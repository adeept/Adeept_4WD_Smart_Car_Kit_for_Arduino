// #include <Arduino.h>
#include "Adeept_Car_For_Arduino.h"
#include <Servo.h>

#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_NeoPixel.h>

#include <Adafruit_SSD1306.h>
#include <VK16K33.h>

/////////////////////Servo area///////////////////////////////////
#define servoPin1   A1    //Define servo interface digital interface A1
#define servoPin2   A2    //Define servo interface digital interface A2
Servo servo1;
Servo servo2;
void Servo_Setup(void)
{
  // put your setup code here, to run once:
  pinMode(servoPin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servoPin2,OUTPUT);//Set the servo interface as the output interface
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}

void Servo_Angle(int servo_id, int angle){
  if (servo_id == 1){
      // Serial.println("111"); 
    servo1.write(angle);
  }else if (servo_id == 2){
    servo2.write(angle);
  }
}

/////////////////////PCA9685 Servo area///////////////////////////////////
#define Servo_FREQ 50 // Analog servos run at ~50 Hz updates
#define PWM_Servo_Max 510 
#define PWM_Servo_Min 102 
#define Angle_Range 180.0
// called this way, PCA9685 uses the address 0x5F 
Adafruit_PWMServoDriver pwm_servo = Adafruit_PWMServoDriver(0x5F);

void PCA9685_Servo_Setup(void)
{
  // pwm_servo.begin();
  pwm_servo.setPWMFreq(50);  // Set PWM frequency to 50Hz
  pwm_servo.begin();
}

void PCA9685_Servo_Angle(int servo_id, int start, int angle){
  // servo_id: servo channel.
  // start: starting point of the pulse signal.
  // angle: servo angle.
  if (servo_id == 6 || servo_id == 7){
  int pwm_value = int(PWM_Servo_Min +((PWM_Servo_Max - PWM_Servo_Min)/Angle_Range)*angle);
  // Serial.println(pwm_value);
  pwm_servo.setPWM(servo_id, start, pwm_value);
  }
}
/////////////////////PCA9685 Motor area///////////////////////////////////
#define PIN_MOTOR_M1_IN1 15      //Define the positive pole of M1
#define PIN_MOTOR_M1_IN2 14      //Define the negative pole of M1
#define PIN_MOTOR_M2_IN1 12      //Define the positive pole of M2
#define PIN_MOTOR_M2_IN2 13      //Define the negative pole of M2
#define PIN_MOTOR_M3_IN1 11      //Define the positive pole of M3
#define PIN_MOTOR_M3_IN2 10      //Define the negative pole of M3
#define PIN_MOTOR_M4_IN1 8       //Define the positive pole of M4
#define PIN_MOTOR_M4_IN2 9       //Define the negative pole of M4
// called this way, PCA9685 uses the address 0x5F 
Adafruit_PWMServoDriver pwm_motor = Adafruit_PWMServoDriver(0x5F);

void Motor_Setup() {
  // pwm_motor.begin();
  pwm_motor.setPWMFreq(1000);  // Set PWM frequency to 50Hz.
  pwm_motor.begin();
}

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
      motorPWM(PIN_MOTOR_M1_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M1_IN2, 0);
    }
    else{
      motorPWM(PIN_MOTOR_M1_IN1, 0);
      motorPWM(PIN_MOTOR_M1_IN2, Motor_speed);
      }
  }
  else if (Motor_ID == 2){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M2_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M2_IN2, 0);
    }
    else{
      motorPWM(PIN_MOTOR_M2_IN1, 0);
      motorPWM(PIN_MOTOR_M2_IN2, Motor_speed);
      }
  }
  else if (Motor_ID == 3){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M3_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M3_IN2, 0);
    }
    else{
      motorPWM(PIN_MOTOR_M3_IN1, 0);
      motorPWM(PIN_MOTOR_M3_IN2, Motor_speed);
      
      }
  }
  else if (Motor_ID == 4){
    if (dir == 1){
      motorPWM(PIN_MOTOR_M4_IN1, Motor_speed);
      motorPWM(PIN_MOTOR_M4_IN2, 0);
    }
    else{
      motorPWM(PIN_MOTOR_M4_IN1, 0);
      motorPWM(PIN_MOTOR_M4_IN2, Motor_speed);
      }
  }
}
void AllMotorStop(){
  Motor(1, 1, 0);
  Motor(2, 1, 0);
  Motor(3, 1, 0);
  Motor(4, 1, 0);
}
//////////////////////Buzzer drive area///////////////////////////////////
const int birthdaySongNotes[] = {
  262, 262, 277, 262, 311, 294, // Happy birthday to you
  262, 262, 277, 262, 349, 330, // Happy birthday to you
  262, 262, 262, 523, 494, 466, // Happy birthday, dear [name]
  262, 262, 277, 262, 392, 349  // Happy birthday to you
};


const int birthdaySongDurations[] = {
  250, 250, 500, 250, 250, 500, // Happy birthday to you
  250, 250, 500, 250, 250, 500, // Happy birthday to you
  250, 250, 250, 250, 500, 500, // Happy birthday, dear [name]
  250, 250, 500, 250, 250, 500  // Happy birthday to you
};
void Buzzer_Setup(void){
  pinMode(PIN_BUZZER, OUTPUT);
  delay(10);
}

//Buzzer alarm function
void Buzzer_Alert()
{
  int numNotes = sizeof(birthdaySongNotes) / sizeof(birthdaySongNotes[0]);
  for (int i = 0; i < numNotes; i++) {
    tone(PIN_BUZZER, birthdaySongNotes[i]);
    delay(birthdaySongDurations[i]);
    noTone(PIN_BUZZER);
    delay(50); 
  }
}
void Buzzer_Silence(){
  noTone(PIN_BUZZER); //stop vocalizing
}

void Buzzer_Beep_One_Second() {
  int frequency = 2500;
  tone(PIN_BUZZER, frequency);
  delay(1000); 
  noTone(PIN_BUZZER);
}  

/////////////////////WS2812 LED area///////////////////////////////////
#define LED_PIN    A3         // WS2812 connect to pin A3 .
#define NUM_LEDS   7         // LED number.
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void WS2812_Setup() {
  pixels.begin();         // Initialize the NeoPixel library.
  pixels.setBrightness(5); // Set WS2812 LED brightness.
}
void WS2812_Brightness(int Brightness) {
  pixels.setBrightness(Brightness); // Set WS2812 LED brightness.
}

void WS2812Color(int num, int R, int G, int B){
  pixels.setPixelColor(num,pixels.Color(R,G,B));
  pixels.show();    
}

void WS2812ColorAll(int R, int G, int B){
    for(int i=0; i<=NUM_LEDS; i++){
      pixels.setPixelColor(i,pixels.Color(R,G,B));
  }
  pixels.show();  
}


/////////////////////ultrasonic area///////////////////////////////////
#define trigPin   7                  // define Trig pin for ultrasonic ranging module
#define echoPin   8                  // define Echo pin for ultrasonic ranging module
#define maxDistance   200          // define the range(cm) for ultrasonic ranging module, Maximum sensor distance is rated at 400-500cm.
#define soundVelocity   340        // Sound velocity = 340 m/s
float rangingTimeOut = 2 * maxDistance / 100.0 / soundVelocity * 1000000; // define the timeout(ms) for ultrasonic ranging module


void Ultrasonic_Setup() {
  // Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // set trigPin to output mode
  pinMode(echoPin, INPUT);  // set echoPin to input mode
}

float GetDistance() {
  unsigned long pingTime; // save the high level time returned by ultrasonic ranging module
  float distance;         // save the distance away from obstacle
  // set the trigPin output 10us high level to make the ultrasonic ranging module start to measure
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // get the high level time returned by ultrasonic ranging module
  pingTime = pulseIn(echoPin, HIGH, rangingTimeOut);
  if (pingTime != 0) {  // if the measure is not overtime
    distance = pingTime * soundVelocity / 2 / 10000;  // calculate the obstacle distance(cm) according to the time of high level returned
    return distance;    // return distance(cm)
  }
  else                  // if the measure is overtime
    return maxDistance; // returns the maximum distance(cm)
}

/////////////////////Photosensitive drive area///////////////////////////////////
#define PHOTOSENSITIVE_PIN A0 //Define the pins that Arduino reads photosensitive
int photosensitiveADC;        //Defines a variable to store ADC values

void Photosensitive_Setup() {
  pinMode(PHOTOSENSITIVE_PIN, INPUT);//Configure the pins for input mode
  // Serial.begin(115200);              //Initialize the serial port and set the baud rate to 115200
}
int GetPhotosensitive(){
  return analogRead(PHOTOSENSITIVE_PIN);
}
/////////////////////Tracking Line area///////////////////////////////////
#define Tracking_Left   4 //define Tracking left port
#define Tracking_Mid    5 //define Tracking mid port
#define Tracking_Right  6 //define Tracking right port
int Val[3] = {0,0,0};

void Tracking_Setup() {
  // Serial.begin(115200); //set baud rate.
  pinMode(Tracking_Left, INPUT); //define input port
  pinMode(Tracking_Mid, INPUT); 
  pinMode(Tracking_Right, INPUT);
}

int Track_Read(void){
  int value = 0;
  Val[0] = digitalRead(Tracking_Left); //Read the value of digital interface 3 and assign it to val.
  Val[1] = digitalRead(Tracking_Mid); 
  Val[2] = digitalRead(Tracking_Right); 
  if ((Val[0]==0)&&(Val[1]==0)&&(Val[2]==0)){ // 000
    value = 0;
    }
  else if ((Val[0]==0)&&(Val[1]==1)&&(Val[2]==0)){ // 010
    value = 2;
    }
  else if ((Val[0]==1)&&(Val[1]==0)&&(Val[2]==0)){ // 100
    value = 4;
    }
  else if ((Val[0]==1)&&(Val[1]==1)&&(Val[2]==0)){ // 110
    value = 6;
    }
  else if ((Val[0]==0)&&(Val[1]==0)&&(Val[2]==1)){ // 001
    value = 1;
    }
  else if ((Val[0]==0)&&(Val[1]==1)&&(Val[2]==1)){ // 011
    value = 3;
    }
  else if ((Val[0]==1)&&(Val[1]==1)&&(Val[2]==1)){ // 111
    value = 7;
    }
  else if ((Val[0]==1)&&(Val[1]==0)&&(Val[2]==1)){ // 101
    value = 5;
    }
  return value;
}

/////////////////////Matrix area///////////////////////////////////
#define Matrix_ADDRESS 0x70
VK16K33 matrix = VK16K33();
byte x_array[][8] = {//Put the data into the left LED matrix
  0x00,0x18,0x24,0x00,0x00,0x04,0x03,0x00,
  0x00,0x10,0x28,0x44,0x00,0x00,0x00,0x00,
};

byte y_array[][8] = {//Put the data into the right LED matrix
  0x00,0x18,0x24,0x00,0x00,0x20,0xC0,0x00,
  0x00,0x08,0x14,0x22,0x00,0x00,0x00,0x00,
};
byte clear_array[][8] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

void Matrix_Setup() {
  matrix.init(Matrix_ADDRESS);
  matrix.setBrightness(15);
  matrix.clear();
  // delay(5000);
}
void Matrix_Smile() {
  int count = sizeof(x_array) / sizeof(x_array[0]);
  for (int i = 0; i < count; i++) {
    matrix.showStaticArray(x_array[i], y_array[i]);
    delay(500);
  // delay(5000);
  }
}
void Matrix_Clear() {
    matrix.showStaticArray(clear_array[0], clear_array[1]);
}

////////////////////Battery drive area/////////////////////////////////////
#define PIN_BATTERY        A3        //Set the battery detection voltage pin
float batteryVoltage = 0;       //Battery voltage variable
float batteryCoefficient = 4;   //Set the proportional coefficient
int actual_battery_voltage = 0;
//Gets the battery ADC value
int Get_Battery_Voltage_ADC(void)
{
  pinMode(PIN_BATTERY, INPUT);
  int batteryADC = 0;
  for (int i = 0; i < 5; i++)
    batteryADC += analogRead(PIN_BATTERY);
  return batteryADC / 5;
}

//Get the battery voltage value
int Get_Battery_Voltage(void)
{
  int batteryADC = Get_Battery_Voltage_ADC();
  batteryVoltage = (batteryADC / 1024.0  *5 *5.5 );
  if(batteryVoltage >= 8.4){
    batteryVoltage = 8.4;
    }
  actual_battery_voltage = int((batteryVoltage - 6) / (8.4 - 6) * 100);
  return actual_battery_voltage;
}
