/**********************************************************************
  Description : Read battery voltage.
  Auther      : www.adeept.com
**********************************************************************/
#include "Adeept_Battery_For_Arduino.h"

void setup() {
  Serial.begin(115200);                     //Set the Serial Baud rate
}

void loop() {
  Serial.print("Battery ADC : ");
  Serial.println(Get_Battery_Voltage_ADC());//Gets the battery ADC value
  Serial.print("Battery Voltage : ");
  Serial.print(Get_Battery_Voltage());      //Get the battery voltage value
  Serial.println("V");
  delay(300);
}
