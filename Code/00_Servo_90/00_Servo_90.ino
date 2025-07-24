/***********************************************************
File name: 00_Servo90.ino
Description: Power on, the servo rotates 90 to straighten the robotic arm
Website: www.adeept.com
E-mail: support@adeept.com
Date:2025/04/30
***********************************************************/
#include <Servo.h>
int servopin1 = A1;    //Define servo interface digital interface 9
int servopin2 = A2;    //Define servo interface digital interface 6

Servo servo1;
Servo servo2;
int angle = 90;        //Angle of rotation of the servo

void setup() {
  // put your setup code here, to run once:
  pinMode(servopin1,OUTPUT);//Set the servo interface as the output interface
  pinMode(servopin2,OUTPUT);//Set the servo interface as the output interface
  Serial.begin(9600);
  
  servo1.attach(servopin1);
  servo2.attach(servopin2);
}

void loop()
{
  servo1.write(angle);
  servo2.write(angle);
  delay(1000);
}
