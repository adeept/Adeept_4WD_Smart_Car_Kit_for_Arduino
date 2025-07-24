/**********************************************************************
  Filename    : IR_Control.ino for Mecanum
  Auther      : www.adeept.com
**********************************************************************/

#include <IRremote.h> //Call the infrared remote control library.
#include "Adeept_Car_For_Arduino.h" 

// #include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <Adafruit_NeoPixel.h> 
#define RECV_PIN     2        // Infrared receiving pin
IRrecv irrecv(RECV_PIN);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object
unsigned long strP,strNow;


float distance;
float midDist;
float leftDist;
float rightDist;
#define avoid_Dist  40 // cm
#define minDist  20 // cm
#define lightThreshold 50
#define lightADC 512


int value;
int threshold = 80;
int value_Init;

int dir = 1;   //If the left and right wheels are installed in reverse, this parameter can be adjusted to -1.
#define motor_speed  60  // value:0-100
#define lt_speed 50  // value:0-100
int deviation = 0;
#define servo_Init  90
int servo_Angle1 = servo_Init;
int ws2812_flag= 0;
volatile int cur_status = -1;

SSD1306AsciiWire oled;
byte color_value[][3] = {{250,0,0},{250,160,0},{250,250,0},{0,250,0},{0,250,250},{0,0,250},{130,0,250}};
void setup()
{
  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  Wire.begin();
  Wire.setClock(400000L);


  Servo_Setup();               //Servo initialization
  PCA9685_Servo_Setup();   //PCA9685 Servo initialization
  Motor_Setup();               //Motor initialization
  AllMotorStop();
  Buzzer_Setup();                //Buzzer initialization
  WS2812_Setup();                //WS2812 LED initialization
  WS2812_Brightness(5);      // value 0-10
  Ultrasonic_Setup();            //Ultrasonic initialization
  Photosensitive_Setup();        //Light line initialization
  Tracking_Setup();              //Tracking Line initialization
  Matrix_Setup();
  Matrix_Clear();

  irrecv.enableIRIn();        // Start the receiver
  Serial.println("UnoCar-B IR Control Start!");
  // Serial.println(RECV_PIN);   //print the infrared receiving pin
  
  // Buzzer_Alert(1,1);
  WS2812ColorAll(255, 255,0); // Green
  Servo_Angle(1, 90);
  Servo_Angle(2, 90);
  PCA9685_Servo_Angle(6, 0, 90);
  PCA9685_Servo_Angle(7, 0, 90);
  Buzzer_Silence();

  delay(1000);
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(Adafruit5x7);
  WS2812ColorAll(0,0,0);
  oled.clear();

  oled.set2X();
  oled.setCursor(32,0); // (x,y)
  oled.print("Adeept");
  oled.println(); 
  
}

void loop()
{
  if (irrecv.decode(&results)){
    unsigned long value = results.value;
    value = switch_irr(value);
    cur_status = value;
    irrecv.resume();                      // Release the IRremote. Receive the next value
    Serial.print("value:");
    Serial.println(value);
    control(value);
  }
  else{
    if (cur_status == 12 || cur_status == 13 || cur_status == 14 || cur_status == 15 || 
      cur_status == 1 || cur_status == 3 || cur_status == 4 || cur_status == 6 || 
      cur_status == 7 || cur_status == 9){
      delay(100);
      Motor(1, 1, 0);
      Motor(2, 1, 0);
      Motor(3, 1, 0);
      Motor(4, 1, 0);
    } else {
      control(cur_status);
    }
  }
  Get_BatteryVoltage();
  //delay(100);
}


void control(unsigned long value){
//  Serial.println(value);

  switch (value) {
    // Move Control
    case 12:  // Up
        Motor(1, 1*dir, motor_speed); //Motor1 forward
        Motor(2, 1*dir, motor_speed); //Motor2 forward
        Motor(3, 1*dir, motor_speed); //Motor3 forward
        Motor(4, 1*dir, motor_speed); //Motor4 forward
      break;
     
    case 13:  // Down
      Motor(1, -1*dir, motor_speed); //Motor1 backward
      Motor(2, -1*dir, motor_speed); //Motor2 backward
      Motor(3, -1*dir, motor_speed); //Motor3 backward
      Motor(4, -1*dir, motor_speed); //Motor4 backward
      break;

    case 14:  // left
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed); 
      break;
      
    case 15:  // right 
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed); 
      Motor(3, -1*dir, motor_speed); 
      Motor(4, -1*dir, motor_speed); 
      break;
        
    case 1:
      Motor(1,1, 0);    //drift front-left
      Motor(2,1*dir, motor_speed);
      Motor(3,1, 0);
      Motor(4,1*dir, motor_speed);
        break;
    case 3:
      Motor(1,1*dir, motor_speed); //drift front-right
      Motor(2,1, 0);
      Motor(3,1*dir, motor_speed);
      Motor(4,1, 0);
        break;
    case 4:
      Motor(1, -1*dir, motor_speed);   //drift left
      Motor(2, 1*dir, motor_speed);
      Motor(3, -1*dir, motor_speed);
      Motor(4, 1*dir, motor_speed);
        break;
    case 6:
      Motor(1, 1*dir, motor_speed);    //drift right
      Motor(2, -1*dir, motor_speed);
      Motor(3, 1*dir, motor_speed);
      Motor(4, -1*dir, motor_speed);
        break;
    case 7:
      Motor(1,-1*dir, motor_speed);        //drift rear-left
      Motor(2,-1*dir, 0);
      Motor(3,-1*dir, motor_speed);
      Motor(4,-1*dir, 0);
        break; 
    case 9:
      Motor(1,-1, 0);               //drift rear-right
      Motor(2,-1*dir, motor_speed);
      Motor(3,-1, 0);
      Motor(4,-1*dir, motor_speed);
        break; 
    case 2:
        WS2812Blink();
        break;
    case 5:
        DisplayMatrixSmile();
        break;
    case 8:
        Line_Tracking(); // Line Tracking function
        break;

    case 10:
        Avoid_Obstacles(); // Avoid Obstacles function
        break;
      
    case 11:
        Light_Tracking();  // Light Tracking function
        break;
      
    case 16:  // OK. Stop function.
        Servo_Angle(1, servo_Init + deviation);
        Motor(1, 1, 0);
        Motor(2, 1, 0);
        Motor(3, 1, 0);
        Motor(4, 1, 0);
        break; 
    case 0:
        Keep_Distance();
        break;
    default:
        break;
  }
}
  

void Get_BatteryVoltage(){
  pinMode(A3, INPUT);
  oled.set1X();
  oled.setCursor(0, 40); // (x,y)
  int batteryVoltage = Get_Battery_Voltage();
  oled.print("Battery level:");
  oled.print(batteryVoltage);
  oled.print("%");
//  Serial.print(batteryVoltage);
//  Serial.println("%");
  if(batteryVoltage <= 10)
  {
    Buzzer_Beep_One_Second();
  }
}

void WS2812Blink() {
  pinMode(A3, OUTPUT);
  WS2812ColorAll(255, 0, 0);
  delay(200);
  WS2812ColorAll(0, 0, 0);
  delay(200);
  WS2812ColorAll(0, 0, 255);
  delay(200);
  WS2812ColorAll(0, 0, 0);
  delay(200);
}

void DisplayMatrixSmile() {
  Matrix_Smile();
  delay(500);
}
void Keep_Distance(){
    Servo_Angle(1, servo_Angle1);
    distance = GetDistance();
    if (distance <= 20){
        Servo_Angle(1, servo_Init + deviation);    
        Motor(1,-1*dir,motor_speed); 
        Motor(2,-1*dir,motor_speed);
        Motor(3,-1*dir,motor_speed); 
        Motor(4,-1*dir,motor_speed);
    }
    else if (distance > 30){
        Servo_Angle(1, servo_Init + deviation);   
        Motor(1,1*dir,motor_speed); 
        Motor(2,1*dir,motor_speed);
        Motor(3,1*dir,motor_speed); 
        Motor(4,1*dir,motor_speed);
    }
    else {
        Motor(1,1,0);  // stop
        Motor(2,1,0);
        Motor(3,1,0);  
        Motor(4,1,0);
    }
}

void Avoid_Obstacles(){
  distance = GetDistance();
  Serial.println(distance);
  if (distance > avoid_Dist){
      Servo_Angle(1, 90);
      Motor(1,1*dir,motor_speed); 
      Motor(2,1*dir,motor_speed);
      Motor(3,1*dir,motor_speed); 
      Motor(4,1*dir,motor_speed);
  }
  else if (distance >= minDist and distance <=avoid_Dist){
      Motor(1,1,0); 
      Motor(2,1,0);
      Motor(3,1,0); 
      Motor(4,1,0);
      Servo_Angle(1, 135);
      delay(200);
      leftDist = GetDistance();
      Servo_Angle(1, 45);
      delay(200);
      rightDist = GetDistance();
      if (leftDist >= rightDist){
        Motor(1,-1*dir,motor_speed); 
        Motor(2,-1*dir,motor_speed);
        Motor(3,1*dir,motor_speed); 
        Motor(4,1*dir,motor_speed);
      }
      else{
          Motor(1,1*dir,motor_speed); 
          Motor(2,1*dir,motor_speed);
          Motor(3,-1*dir,motor_speed); 
          Motor(4,-1*dir,motor_speed);
      }
      delay(200);
  }
  else {
    Motor(1,-1*dir,motor_speed); 
    Motor(2,-1*dir,motor_speed);
    Motor(3,-1*dir,motor_speed); 
    Motor(4,-1*dir,motor_speed);
    delay(200);
  }

}


void Light_Tracking(){
  value = GetPhotosensitive();
  if (value < (lightADC - lightThreshold)){
    Motor(1, -1*dir, motor_speed); 
    Motor(2, -1*dir, motor_speed);
    Motor(3, 1*dir, motor_speed); 
    Motor(4, 1*dir, motor_speed);
    Serial.print("LIGHT ADC:");
    Serial.println(value);
  }
  else if (value > (lightADC + lightThreshold)){
    Motor(1, 1*dir, motor_speed); 
    Motor(2, 1*dir, motor_speed);
    Motor(3, -1*dir, motor_speed); 
    Motor(4, -1*dir, motor_speed);
    Serial.print("LIGHT ADC:");
    Serial.println(value);
  }
  else{
    Motor(1, 1*dir, motor_speed);
    Motor(2, 1*dir, motor_speed);
    Motor(3, 1*dir, motor_speed);
    Motor(4, 1*dir, motor_speed);
    Serial.print("LIGHT ADC:");
    Serial.println(value);
  }

}

void Line_Tracking(){
  value = Track_Read();  //Read the value of the tracking module.

  switch (value)  
  {
    case 0:   //000 left
      Motor(1, -1*dir, lt_speed); 
      Motor(2, -1*dir, lt_speed); 
      Motor(3, 1*dir, lt_speed); 
      Motor(4, 1*dir, lt_speed); 
      break;

    case 2:   //010 forward
      Motor(1, 1*dir, lt_speed); //Motor1 forward
      Motor(2, 1*dir, lt_speed); //Motor2 forward
      Motor(3, 1*dir, lt_speed); //Motor3 forward
      Motor(4, 1*dir, lt_speed); //Motor4 forward
      break;
    
    case 4:   //100 left
      Motor(1, -1*dir, lt_speed); 
      Motor(2, -1*dir, lt_speed); 
      Motor(3, 1*dir, lt_speed); 
      Motor(4, 1*dir, lt_speed); 
      break;
      
    case 6:   //110 left
      Motor(1, -1*dir, lt_speed); 
      Motor(2, -1*dir, lt_speed); 
      Motor(3, 1*dir, lt_speed); 
      Motor(4, 1*dir, lt_speed); 
      break;
      
    case 1:   //001 right
      Motor(1, 1*dir, lt_speed); 
      Motor(2, 1*dir, lt_speed); 
      Motor(3, -1*dir, lt_speed); 
      Motor(4, -1*dir, lt_speed); 
      break;
      
    case 3:   //011 right
      Motor(1, 1*dir, lt_speed); 
      Motor(2, 1*dir, lt_speed); 
      Motor(3, -1*dir, lt_speed); 
      Motor(4, -1*dir, lt_speed); 
      break;

    case 7:   //111 forward
      Motor(1, 1*dir, lt_speed); 
      Motor(2, 1*dir, lt_speed); 
      Motor(3, 1*dir, lt_speed); 
      Motor(4, 1*dir, lt_speed); 
      break;
      
    case 5:   //101 forward
      Motor(1, 1*dir, lt_speed); //Motor1 forward
      Motor(2, 1*dir, lt_speed); //Motor2 forward
      Motor(3, 1*dir, lt_speed); //Motor3 forward
      Motor(4, 1*dir, lt_speed); //Motor4 forward
      break;
    default:
      break;
  }
}


int switch_irr(int irr_data)
{
  switch(irr_data)
  {
  case 16750695: return 0;
  case 16753245: return 1;
  case 16736925: return 2;
  case 16769565: return 3;
  case 16720605: return 4;
  case 16712445: return 5;
  case 16761405: return 6;
  case 16769055: return 7;
  case 16754775: return 8;
  case 16748655: return 9;
  case 16738455: return 10; // *
  case 16756815: return 11; // #
  case 16718055: return 12; // up
  case 16730805: return 13; // down
  case 16716015: return 14; // left
  case 16734885: return 15; // right
  case 16726215: return 16; // ok
  }
}
