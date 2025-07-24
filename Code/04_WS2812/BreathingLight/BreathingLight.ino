/***********************************************************
Function:  BreathingLight LED
Website: www.adeept.com
***********************************************************/
#include <Adafruit_NeoPixel.h>

#define LED_PIN    A3         // WS2812 connect to pin A3 .
#define NUM_LEDS   7         // LED number.
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();         // Initialize the NeoPixel library.
}

void loop() {
  breatheColor(0, 0, 255);
}

void breatheColor(int R, int G, int B) {
  for (int brightness = 0; brightness <= 255; brightness++) {
    setAllPixelsColor(R, G, B, brightness);
    delay(10);
  }
  for (int brightness = 255; brightness >= 0; brightness--) {
    setAllPixelsColor(R, G, B, brightness);
    delay(10);
  }
}

void setAllPixelsColor(int R, int G, int B, int brightness) {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, scaleColor(R, brightness), scaleColor(G, brightness), scaleColor(B, brightness));
  }
  pixels.show();
}

int scaleColor(int color, int brightness) {
  return (color * brightness) / 255;
}