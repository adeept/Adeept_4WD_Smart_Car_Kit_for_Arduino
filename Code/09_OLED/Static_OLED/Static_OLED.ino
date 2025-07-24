 /*************************************
File name:OLED
Description:OLED displays text.
Website:www.addept.com
*************************************/
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h> 
SSD1306AsciiWire display;

void setup() {
  
  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x64, 0x3C);
  display.setFont(Adafruit5x7);
    display.clear();
  //Set the font size
  display.set2X();
  display.setCursor(0,40); // (x,y)
  //String displayed
  display.println("\n4WD Smart\nCar");
  display.println("\n");

  display.set1X();
  //Set the display location
  display.setCursor(80,55);
  display.print("Adeept");
  //Began to show
}

void loop() {
  
}
