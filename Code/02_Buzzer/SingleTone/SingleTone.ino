#include "Adeept_Buzzer_For_Arduino.h"


const int buzzerPin = 3;
AdeeptBuzzer buzzer(buzzerPin);

void setup() {

}

void loop() {

  int frequency = 440; 
  int duration = 1000; 
  buzzer.playTone(frequency, duration);
  delay(2000); 
}