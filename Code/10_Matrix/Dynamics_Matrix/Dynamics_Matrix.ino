/**********************************************************************
  Product     : 16*8 Matrix LED 
  Auther      : www.adeept.com
**********************************************************************/
#include <VK16K33.h>

#define ADDRESS 0x70
#define SDA     13
#define SCL     14

VK16K33 matrix = VK16K33();

byte x_array[][8] = {//Put the data into the left LED matrix
  0x00,0x18,0x24,0x00,0x00,0x04,0x03,0x00,
  0x00,0x10,0x28,0x44,0x00,0x00,0x00,0x00,
};

byte y_array[][8] = {//Put the data into the right LED matrix
  0x00,0x18,0x24,0x00,0x00,0x20,0xC0,0x00,
  0x00,0x08,0x14,0x22,0x00,0x00,0x00,0x00,
};

void setup()
{
  matrix.init(ADDRESS);
  matrix.setBrightness(15);
  
}

void loop()
{ 
  int count = sizeof(x_array) / sizeof(x_array[0]);
  for (int i = 0; i < count; i++) {
    matrix.showStaticArray(x_array[i], y_array[i]);
    delay(1000);
  }
}
