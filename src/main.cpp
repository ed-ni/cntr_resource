#include <Arduino.h>
//#include "TimerInterrupt.h"
//#include <avr/eeprom.h>

#include <AceButton.h>

#define buzz 2
#define butt1 PB2  
uint8_t ledPin = butt1;

void setup() {
  // put your setup code here, to run once:
  pinMode(PB2, OUTPUT);
  }

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}