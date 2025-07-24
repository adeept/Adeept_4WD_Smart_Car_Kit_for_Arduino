#ifndef _Arduino_h
#define _Arduino_h

// #include <Arduino.h>
// #include <PCA9685.h>

/////////////////////Servo area///////////////////////////////////
void Servo_Setup(void);
void Servo_Angle(int servo_id, int angle);

/////////////////////PCA9685 Servo area///////////////////////////////////
void PCA9685_Servo_Setup(void);
void PCA9685_Servo_Angle(int servo_id, int start, int angle);

/////////////////////PCA9685 Motor area///////////////////////////////////
void Motor_Setup();
void motorPWM(int channel, int motor_speed);
void Motor(int Motor_ID, int dir, int Motor_speed);
void AllMotorStop();

//////////////////////Buzzer drive area///////////////////////////////////
//Buzzer pin definition             
#define PIN_BUZZER 3                    //Define the pins for the Arduino control buzzer
#define BUZZER_CHN 0                    //Define the PWM channel for Arduino
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 
void Buzzer_Setup(void);                //Buzzer initialization
void Buzzer_Alert();//Buzzer alarm function
void Buzzer_Silence();
void newtone(int tonePin, int frequency);
void Buzzer_Beep_One_Second();
/////////////////////WS2812 LED area///////////////////////////////////
void WS2812_Setup();
void WS2812_Brightness(int Brightness);
void WS2812Color(int num, int R, int G, int B);
void WS2812ColorAll(int R, int G, int B);

/////////////////////ultrasonic area///////////////////////////////////
void Ultrasonic_Setup();
float GetDistance();

/////////////////////Photosensitive drive area///////////////////////////////////
void Photosensitive_Setup();
int GetPhotosensitive();

/////////////////////Tracking Line area///////////////////////////////////
// int Val[3];
void Tracking_Setup();
int Track_Read(void);

/////////////////////OLED area///////////////////////////////////
// void OLED_Setup();
// void OLED_clear();
// void OLED(int font, int x, int y, char text[]);

/////////////////////Matrix area///////////////////////////////////
void Matrix_Setup();
void Matrix_Smile();
void Matrix_Clear();

////////////////////Battery drive area/////////////////////////////////////
// extern float batteryCoefficient;    //Set the proportional coefficient

int Get_Battery_Voltage_ADC(void);   //Gets the battery ADC value
int Get_Battery_Voltage(void);     //Get the battery voltage value 

#endif
