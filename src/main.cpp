#include <Arduino.h>
//#include "TimerInterrupt.h"
//#include <avr/eeprom.h>

#include <AceButton.h>

#define buzz PB3
#define ledPin PB2  
#define buttInner_MISO PB1
#define buttOuter_MOSI PB0  
#define swCoil PB4  
//uint8_t ledPin = butt1;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzz, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(swCoil, OUTPUT);
  pinMode(buttInner_MISO, INPUT_PULLUP);
  pinMode(buttOuter_MOSI, INPUT_PULLUP);
 }

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(buzz, HIGH);   // turn the LED on (HIGH is the voltage level)
 delay(500);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(buzz, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}