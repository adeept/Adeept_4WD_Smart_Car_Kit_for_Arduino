/**********************************************************************
  Auther      : www.adeept.com
**********************************************************************/

#include "Adeept_Car_For_Arduino.h" 
#include <Servo.h>
#include <FlexiTimer2.h>//to set a timer to manage all server
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire oled;
float distance;
float midDist;
float leftDist;
float rightDist;
int dir = 1;   //If the left and right wheels are installed in reverse, this parameter can be adjusted to -1.
#define motor_speed  60  // value:0-100
int Track_value = -2;
bool processingID = false;
int value;
int threshold = 40;
int value_Init = 500;
#define servo_Init  90
int deviation = 0;
int IR_mark = 0;
int control_num = 0;
int servo_Angle1 = servo_Init;
int ws2812_flag= 0;
int Function_Stop_flag = 0;

void Keep_Distance();
void sendWifi(String data);
const String Move_UP = "forward";
const String Move_Down = "backward";
const String Move_UD_Stop = "DS";
const String Move_Left = "turn_left";
const String Move_LR_Stop = "TS";
const String Move_Right = "turn_right";

const String Drift_Front_Left = "drift_front_left";
const String Drift_Front_Right = "drift_front_right";
const String Drift_Left = "drift_left";
const String Drift_Right = "drift_right";
const String Drift_Rear_Left = "drift_rear_left";
const String Drift_Rear_Right = "drift_rear_right";
const String Drift_Stop = "DRS";

const String Head_Hmoe = "home";
const String Head_Down = "down";
const String Head_Left = "lookleft";
const String Head_Right = "lookright";
const String Head_Stop = "RLstop";

const String RadarScan_ON = "scan";
const String Police_ON = "police";
const String Ultrasonic_ON = "automatic";
const String Line_Tracking_ON = "trackLine";
const String Light_Tracking_ON = "lightTracking";
const String MatrixSmile_ON = "matrix";
const String KEEPDISTACE_ON = "KD";
const String BUZZER_ON = "buzzerMusic";
const String StopFunction = "StopFunction";


String comdata = "";
int judge;

void setup()
{
  Serial.begin(115200);       // set up a wifi serial communication baud rate 115200
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
  // OLED_Setup();                //OLED initialization
  Matrix_Setup();

  Serial.println("AT+CWMODE=3\r\n");//set to softAP+station mode
  delay(3000);     //delay 4s  
  Serial.println("AT+CWSAP=\"Adeept_ESP8266\",\"12345678\",8,2\r\n");   //TCP Protocol, server IP addr, port
  delay(1000);     //delay 4s
  Serial.println("AT+RST\r\n");     //reset wifi
  delay(1000);     //delay 4s
  Serial.println("AT+CIPMUX=1\r\n");//set to multi-connection mode
  delay(1000);
  Serial.println("AT+CIPSERVER=1,4000\r\n");//set as server
  delay(1000);
  Serial.println("AT+CIPSTO=7000\r\n");//keep the wifi connecting 7000 seconds
  delay(1000);

  WS2812ColorAll(255, 255,0); // Green
  Servo_Angle(1, 90);
  Servo_Angle(2, 90);
  PCA9685_Servo_Angle(6, 0, 90);
  PCA9685_Servo_Angle(7, 0, 90);
  Buzzer_Silence();
  delay(1000);
  WS2812ColorAll(0,0,0);
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
  while(Serial.available()>0)
  {  
    comdata += char(Serial.read());
    delay(1);
  }
  Get_BatteryVoltage();
  judgement();
  control(judge);
}

void judgement(){
    if (comdata.length() > 0){ 
        const String ghfghg = comdata;
        if(comdata.endsWith(Move_UP)){//forward
          Serial.println(Move_UP);
          judge=1;
        }
        else if(comdata.endsWith(Move_Down)){//backward
          judge=2;
          Serial.println(Move_Down);
        }
        else if(comdata.endsWith(Move_Left)){//left
          Serial.println(Move_Left);
          judge=3;
        }
        else if(comdata.endsWith(Move_Right)){//right.
          Serial.println(Move_Right);
          judge=4;
        }
        else if(comdata.endsWith(Move_UD_Stop)||comdata.endsWith(Move_LR_Stop)
          ||comdata.endsWith(StopFunction)  || comdata.endsWith(Drift_Stop)){//stop
          judge=5;
        }
        else if(comdata.endsWith(Head_Left)){//trun left
          judge=6;
        }
        else if(comdata.endsWith(Head_Right)){//trun right
          judge=7;
        }
        else if(comdata.endsWith(Head_Stop)){//stop servo rotation.
          judge=8;
        }

        else if(comdata.endsWith(Ultrasonic_ON)){//avoid obstacles function.
          judge=9;
          Serial.println(Ultrasonic_ON);
        }
        else if(comdata.endsWith(BUZZER_ON)){//avoid obstacles function.
          judge=10;
          Serial.println(BUZZER_ON);
        }
        else if(comdata.endsWith(Head_Hmoe)){//line tracking function.
          judge=12;
          Serial.println(Head_Hmoe);
        }
        else if(comdata.endsWith(MatrixSmile_ON)){//avoid obstacles function. bstart.
          judge=18;
          Serial.println(MatrixSmile_ON);
        }

        else if(comdata.endsWith(Line_Tracking_ON)){//line tracking function.
          judge=11;
          Serial.println(Line_Tracking_ON);
        }
        else if(comdata.endsWith(Light_Tracking_ON)){//light tracking function.
          judge=13;
          Serial.println(Light_Tracking_ON);
        }
        else if(comdata.endsWith(KEEPDISTACE_ON)){
          Serial.println(KEEPDISTACE_ON); 
          judge=15;
        }
        else if(comdata.endsWith(Police_ON)){
          judge=16;
          Function_Stop_flag = 0;

        }
        else if(comdata.endsWith(RadarScan_ON)){
          Serial.println(RadarScan_ON); 
          judge=17;
          Function_Stop_flag = 0;
        }
        
         //Drift
        else if(comdata.endsWith(Drift_Front_Left)){
          Serial.println(Drift_Front_Left); 
          judge=19;
        }
        else if(comdata.endsWith(Drift_Front_Right)){
          Serial.println(Drift_Front_Right); 
          judge=20;
        }
        else if(comdata.endsWith(Drift_Left)){
          Serial.println(Drift_Left); 
          judge=21;
        }
        else if(comdata.endsWith(Drift_Right)){
          Serial.println(Drift_Right); 
          judge=22;
        }
        else if(comdata.endsWith(Drift_Rear_Left)){
          Serial.println(Drift_Rear_Left); 
          judge=23;
        }
        else if(comdata.endsWith(Drift_Rear_Right)){
          Serial.println(Drift_Rear_Right); 
          judge=24;
        }
        comdata = "";
        delay(10);
    }
}


void control(int value){
  switch (value) {
    case 1:  // forward
      Motor(1, 1*dir, motor_speed); //Motor1 forward
      Motor(2, 1*dir, motor_speed); //Motor2 forward
      Motor(3, 1*dir, motor_speed); //Motor3 forward
      Motor(4, 1*dir, motor_speed); //Motor4 forward
      break;
     
    case 2:  // Down， 
      Motor(1, -1*dir, motor_speed); //Motor1 backward
      Motor(2, -1*dir, motor_speed); //Motor2 backward
      Motor(3, -1*dir, motor_speed); //Motor3 backward
      Motor(4, -1*dir, motor_speed); //Motor4 backward
      // control_num = 13;
      break;

    case 3:  // left 
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed);  
      break;
      
    case 4:  // right 
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed); 
      Motor(3, -1*dir, motor_speed); 
      Motor(4, -1*dir, motor_speed); 
      break;

    case 5: // stop
      Motor(1, 1, 0);
      Motor(2, 1, 0);
      Motor(3, 1, 0);
      Motor(4, 1, 0);
      Matrix_Clear();
      // control_num = -1;
      break; 

    case 6:  // trun left 
      servo_Angle1 = servo_Angle1 + 1;
      if (servo_Angle1 >180){
        servo_Angle1 = 180;
      }
      Servo_Angle(1, servo_Angle1);
      delay(10);
      break;
    case 7:  // trun right
      servo_Angle1 = servo_Angle1 - 1;
      if (servo_Angle1 < 0){
        servo_Angle1 = 0;
      }
      Servo_Angle(1, servo_Angle1);
      delay(10);
      break;

    case 8:  // stop servo rotation.
      break;
      
    case 9:
      Function_Stop_flag = 0;
      Avoid_Obstacles(); // Avoid Obstacles function
      break;
    case 10:  // stop servo rotation.
      Buzzer_Alert();
      break;
    case 11:
      Function_Stop_flag = 0;
      Line_Tracking();  // Line Tracking function
      break;
    case 12:
      Function_Stop_flag = 0;
      servo_Angle1 = 90;
      Servo_Angle(1, servo_Angle1);
      break;
    case 13:
      Function_Stop_flag = 0;
      Light_Tracking();  // Light Tracking function
      break;
    case 15:
      Function_Stop_flag = 0;
      Keep_Distance();  // Follow function
      break;

    case 16:
      Function_Stop_flag = 0;
      WS2812Blink();
      break;
    case 18:
      Function_Stop_flag = 0;
      DisplayMatrixSmile(); 
      break;
    case 17:
      Function_Stop_flag = 0;
      radarScanning();
      judge=12;
      break;
    case 19:  //drift front-left
      Motor(1,1, 0);    //drift front-left
      Motor(2,1*dir, motor_speed);
      Motor(3,1, 0);
      Motor(4,1*dir, motor_speed);
      break; 
    case 20:  //drift front-right
      Motor(1,1*dir, motor_speed); //drift front-right
      Motor(2,1, 0);
      Motor(3,1*dir, motor_speed);
      Motor(4,1, 0);
      break;
    case 21:  //drift left
      Motor(1, -1*dir, motor_speed);   //drift left
      Motor(2, 1*dir, motor_speed);
      Motor(3, -1*dir, motor_speed);
      Motor(4, 1*dir, motor_speed);
      break;
    case 22:  //drift right
      Motor(1, 1*dir, motor_speed);    //drift right
      Motor(2, -1*dir, motor_speed);
      Motor(3, 1*dir, motor_speed);
      Motor(4, -1*dir, motor_speed);
      break;
    case 23:  //drift rear-left
      Motor(1,-1*dir, motor_speed);        //drift rear-left
      Motor(2,-1*dir, 0);
      Motor(3,-1*dir, motor_speed);
      Motor(4,-1*dir, 0);
      break;
    case 24:  //drift rear-right
      Motor(1,-1, 0);               //drift rear-right
      Motor(2,-1*dir, motor_speed);
      Motor(3,-1, 0);
      Motor(4,-1*dir, motor_speed);
      break;
    default:
        Motor(1, 1, 0);
        Motor(2, 1, 0);
        Motor(3, 1, 0);
        Motor(4, 1, 0);
        break;
  }
}

String RadarScan() {
  String result = "";
  int pwm0_min = 0;
  int pwm0_max = 180;
  int scan_speed = 2;
  for (int pwm0_pos = pwm0_max; pwm0_pos > pwm0_min; pwm0_pos -= scan_speed) {
    int servo_angle = pwm0_pos; 
    Servo_Angle(1, servo_angle);
    delay(20);
   
    float distance = GetDistance();
    if (distance <= 200) { 
      int theta = pwm0_pos; 
      result += String(distance) + "," + String(theta) + ";";
    }
  }
  Servo_Angle(1, 90);
  sendWifi(result);
  return result;
}


void Get_BatteryVoltage(){
  pinMode(A3, INPUT);
  oled.set1X();
  oled.setCursor(0, 40); // (x,y)
  int batteryVoltage = Get_Battery_Voltage();
  if(batteryVoltage < 10)
  {
    oled.print("Battery level:  ");
  }
  else if(batteryVoltage < 100){
    oled.print("Battery level: ");
  }
  else{
    oled.print("Battery level:");
  }
  
  oled.print(batteryVoltage);
  oled.print("%");
  if(batteryVoltage <= 10)
  {
    Buzzer_Beep_One_Second();
  }
}

void DisplayMatrixSmile() {
  Matrix_Smile();
  delay(500);
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

void Avoid_Obstacles(){
    distance = GetDistance();
    if (distance > 30){
        Servo_Angle(1, 90);
        Motor(1,1*dir,motor_speed); //forward
        Motor(2,1*dir,motor_speed);
        Motor(3,1*dir,motor_speed); //forward
        Motor(4,1*dir,motor_speed);
    }
    else if (distance >= 10 and distance <=30){
        Motor(1,1,0); //Stop the car
        Motor(2,1,0);
        Motor(3,1,0); //Stop the car
        Motor(4,1,0);
        Servo_Angle(1, 45);
        if (distance > 20){
          Motor(1,-1*dir,motor_speed); //Stop the car
          Motor(2,-1*dir,motor_speed);
          Motor(3,1*dir,motor_speed); //Stop the car
          Motor(4,1*dir,motor_speed);
        }
        else{
            Servo_Angle(1, 135);
            Motor(1,1*dir,motor_speed); //Stop the car
            Motor(2,1*dir,motor_speed);
            Motor(3,-1*dir,motor_speed); //Stop the car
            Motor(4,-1*dir,motor_speed);
        }
    }
      else {
        Motor(1,-1*dir,motor_speed); //Stop the car
        Motor(2,-1*dir,motor_speed);
        Motor(3,-1*dir,motor_speed); //Stop the car
        Motor(4,-1*dir,motor_speed);
      }
  

}


void Light_Tracking(){
    value = GetPhotosensitive();
    if (value < (value_Init - threshold)){ //left
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed);
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed);
      Serial.print(value_Init);
      Serial.print(":");
      Serial.println(value);
      
    }
    else if (value > (value_Init + threshold)){ //right
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed);
      Motor(3, -1*dir, motor_speed); 
      Motor(4, -1*dir, motor_speed);

      Serial.print(value_Init);
      Serial.print(":");
      Serial.println(value);
    }
    else{
      Motor(1, 1*dir, motor_speed);
      Motor(2, 1*dir, motor_speed);
      Motor(3, 1*dir, motor_speed);
      Motor(4, 1*dir, motor_speed);
      Serial.print(value_Init);
      Serial.print(":");
      Serial.println(value);
    }
}


void Line_Tracking(){
  int value;
  value = Track_Read();  //Read the value of the tracking module.
  // Serial.println(value);
  switch (value)  
  {
    case 0:   //000 left
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed); 
      Track_value = 0; 
      break;

    case 2:   //010 forward
      Motor(1, 1*dir, motor_speed); //Motor1 forward
      Motor(2, 1*dir, motor_speed); //Motor2 forward
      Motor(3, 1*dir, motor_speed); //Motor3 forward
      Motor(4, 1*dir, motor_speed); //Motor4 forward
      Track_value = 2; 
      break;
    
    case 4:   //100 Left
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed); 
      Track_value = 4; 
      break;
      
    case 6:   //110 Left
      Motor(1, -1*dir, motor_speed); 
      Motor(2, -1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed); 
      Track_value = 6; 
      break;
      
    case 1:   //001 right
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed); 
      Motor(3, -1*dir, motor_speed); 
      Motor(4, -1*dir, motor_speed); 
      break;
      
    case 3:   //011 right
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed); 
      Motor(3, -1*dir, motor_speed); 
      Motor(4, -1*dir, motor_speed); 
      break;

    case 7:   //111 forward
      Motor(1, 1*dir, motor_speed); 
      Motor(2, 1*dir, motor_speed); 
      Motor(3, 1*dir, motor_speed); 
      Motor(4, 1*dir, motor_speed);  
      break;
    case 5:   //101 forward
      Motor(1, 1*dir, motor_speed); //Motor1 forward
      Motor(2, 1*dir, motor_speed); //Motor2 forward
      Motor(3, 1*dir, motor_speed); //Motor3 forward
      Motor(4, 1*dir, motor_speed); //Motor4 forward
      break;
    default:
      break;
  }
  
}


void Keep_Distance(){
    Serial.println("value");
    Servo_Angle(1, servo_Angle1);
    distance = GetDistance();
    if (distance < 30){
        Servo_Angle(1, servo_Init + deviation);    
        Motor(1,-1*dir,motor_speed); 
        Motor(2,-1*dir,motor_speed);
        Motor(3,-1*dir,motor_speed); 
        Motor(4,-1*dir,motor_speed);
    }
    else if (distance > 40){
        Servo_Angle(1, servo_Init + deviation);    
        Motor(1,1*dir,motor_speed); 
        Motor(2,1*dir,motor_speed);
        Motor(3,1*dir,motor_speed); 
        Motor(4,1*dir,motor_speed);
    }
    else {
        Motor(1,1,0);  // stop
        Motor(2,1,0);
    }
    delay(100);
}



void radarScanning(){
    judge = 5;
    for (int angle = 0; angle <= 180; angle += 4) {
    Servo_Angle(1, angle);
    delay(5);
    if (angle % 4 == 0) {  
      sendWifi(angle);
    }
  }
}

void sendWifi(int iAngle){
  distance = GetDistance();
  if(processingID==true){
    Serial.println("AT+CIPSEND=1,8\r\n");
  }else{
    Serial.println("AT+CIPSEND=0,8\r\n");
  }
    delay(10);
    if(iAngle<10){
      Serial.print(0);
    }
    if(iAngle<100){
      Serial.print(0);
    }
    Serial.print(iAngle); // Sends the current degree into the Serial Port
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    if(distance < 10){
      Serial.print(0);
    }
    if(distance < 100){
      Serial.print(0);
    }
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.println("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    delay(100);
}
