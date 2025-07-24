/*************************************
File name: SnowOLED.ino
Description: OLED displays snowflake and star effect on Arduino.
Website: www.Adeept.com
*************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>
#include <time.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

class Snowflake {
public:
    int x;
    int y;
    int speed;

    Snowflake(int xPos, int yPos, int s) {
        x = xPos;
        y = yPos;
        speed = s;
    }

    void fall() {
        y += speed;
        if (y > 64) {
            y = 0;
            x = random(0, 128);
        }
    }

    void draw() {
        display.drawPixel(x, y, WHITE);
    }
};

class Star {
public:
    int x;
    int y;
    bool isBright;

    Star(int xPos, int yPos) {
        x = xPos;
        y = yPos;
        isBright = random(0, 2);
    }

    void twinkle() {
        if (random(0, 10) == 0) {
            isBright =!isBright;
        }
    }

    void draw() {
        display.drawPixel(x, y, isBright? WHITE : BLACK);
    }
};

Snowflake* snowflakes[20];
Star* stars[10];

void setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    randomSeed(millis());
    for (int i = 0; i < 20; i++) {
        snowflakes[i] = new Snowflake(random(0, 128), random(0, 64), random(1, 3));
    }

    for (int i = 0; i < 10; i++) {
        stars[i] = new Star(random(0, 128), random(0, 64));
    }
}

void loop() {
    display.clearDisplay();

    for (int i = 0; i < 20; i++) {
        snowflakes[i]->fall();
        snowflakes[i]->draw();
    }

    for (int i = 0; i < 10; i++) {
        stars[i]->twinkle();
        stars[i]->draw();
    }

    display.display();
    delay(100);
}    