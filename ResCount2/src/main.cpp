#include <Arduino.h>
#include "timer-api.h"
#include <avr/eeprom.h>

#include <AceButton.h>
using namespace ace_button;

// The pin number attached to the button.
const int BUTTON_PIN = 11;
// Automatically uses the default ButtonConfig. We will configure this later
// using AceButton::init() method in setup() below.
AceButton button;
bool show = false;
int cycle = 0;

void handleEvent(AceButton *, uint8_t, uint8_t);

#define a 2
#define b 5
#define c 7
#define d 3
#define e 4
#define f 1
#define g 6

#define buzz 10
#define butt1 11

void DigitalOn(int t);
void pickNum(int x, int exp);
void zero();
void one();
void two();
void three();
void four();
void five();
void six();
void seven();
void eight();
void nine();

const byte digit_pins[4]{PIN_A5, PIN_A3, PIN_A1, 9}; // From com1 - com4
const byte segment_pins[7]{2, 1, 7, 3, 4, 5, 6};     // From a - g (abc...g)
const byte dp_pin = 0;
unsigned long timeout;
volatile uint32_t sec = 0;
void zero_set(void);

void setup()
{
  timeout = 0U;
  //timer
  sec = eeprom_read_dword((uint32_t *)46);
  if (sec == 0xFFFFFFFF)
  {
    /* code */
    zero_set();
  }

  timer_init_ISR_2Hz(TIMER_DEFAULT);
  for (int i = 0; i <= A5; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(segment_pins[i], HIGH);
  }
  for (int i = 0; i < 4; i++)
  {
    /* code */
    digitalWrite(digit_pins[i] - 1, HIGH);
    digitalWrite(digit_pins[i], HIGH);
  }
  //button
  // Button uses an internal resistor.
  pinMode(butt1, INPUT_PULLUP);

  // We use the AceButton::init() method here instead of using the constructor
  // to show an alternative. Using init() allows the configuration of the
  // hardware pin and the button to be placed closer to each other.
  button.init(BUTTON_PIN, HIGH);

  // Configure the ButtonConfig with the event handler, and enable the LongPress
  // and RepeatPress events which are turned off by default.
  ButtonConfig *buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  //buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
}

int n;

void loop()
{
  button.check();
  if (sec > 28800000U)
    digitalWrite(buzz, HIGH);

  if (show)
  {
    // put your main code here, to run repeatedly:
    uint16_t curr;
    curr = sec / 3200;
    pickNum(curr, n);
    DigitalOn(n);
    n = (n + 1) & 0b11;
    delay(1);
  }
  else
  {
    /* code */
    for (int i = 0; i < 4; i++)
    {
      /* code */
      digitalWrite(digit_pins[i] - 1, HIGH);
      digitalWrite(digit_pins[i], HIGH);
    }
  }
}
#define MAX_HOUR_IN_WORK 32000000U /* 3200*10000 */
void timer_handle_interrupts(int timer)
{
  sec++;
  if (timeout < micros())
  {
    timeout = 0;
    cycle = 0;
  }

  if (sec >= MAX_HOUR_IN_WORK)
  {
    /* code */
    sec = 0;
  }
  if ((sec % 100) == 0)
  {
    /* code */
    eeprom_write_dword((uint32_t *)46, sec);
  }
}

void DigitalOn(int t)
{
  static int previos_dig = 0;
  //turn off
  digitalWrite(digit_pins[previos_dig] - 1, HIGH);
  digitalWrite(digit_pins[previos_dig], HIGH);
  //fire
  digitalWrite(digit_pins[t] - 1, LOW);
  digitalWrite(digit_pins[t], LOW);
  previos_dig = t;
}

// определение символа (цифры)
void pickNum(int x, int exp)
{
  switch (exp)
  {
  case 0 /* constant-expression */:
    /* code */
    x = x / 1000;
    break;
  case 1 /* constant-expression */:
    /* code */
    x = (x % 1000) / 100;
    break;
  case 2 /* constant-expression */:
    /* code */
    x = (x % 100) / 10;
    break;
  case 3 /* constant-expression */:
    /* code */
    x = x % 10;
    break;

  default:
    return;
  }
  switch (x)
  {
  default:
    zero();
    break;
  case 1:
    one();
    break;
  case 2:
    two();
    break;
  case 3:
    three();
    break;
  case 4:
    four();
    break;
  case 5:
    five();
    break;
  case 6:
    six();
    break;
  case 7:
    seven();
    break;
  case 8:
    eight();
    break;
  case 9:
    nine();
    break;
  }
}

// очистка индикатора
void zero()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void three()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void zero_set()
{
  eeprom_write_dword((uint32_t *)46, 0);
  sec = 0;
}

// The event handler for the button.
void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t buttonState)
{

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off.
  switch (eventType)
  {
  case AceButton::kEventReleased:
    show = false;
    break;
  case AceButton::kEventLongPressed:
    show = true;
    break;
  case AceButton::kEventClicked:
    if (cycle == 0)
    {
      cycle = 1;
      timeout = micros() + 1000000;
      break;
    }

    if (timeout > micros())
    {
      /* code */
      timeout = micros() + 1000000;
      cycle++;
    }
    if (cycle >= 3)
    {
      timeout = 0;
      cycle = 0;
      sec = 0;
    }
    break;
  }
}