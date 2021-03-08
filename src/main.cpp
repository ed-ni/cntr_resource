#include "main.h"

using namespace ace_button;

AceButton button;
bool show = false;
int cycle = 0;

void handleEvent(AceButton *, uint8_t, uint8_t);
uint32_t haos(uint32_t start, uint8_t pause, uint8_t pin);

#define buzz PB3
#define ledPin PB2
#define buttInner_MISO PB1
#define buttOuter_MOSI PB0
#define swCoil PB4

volatile uint32_t count; // счетчик прерываний
unsigned long timeout, mcrs;
volatile uint32_t sec = 0;

ISR(TIM0_COMPA_vect)
{
  count++; // добавляем
}

void setup()
{
  //  INIT PINs
  DDRB |= _BV(ledPin) | _BV(buzz) | _BV(swCoil);
  PORTB = ~_BV(ledPin) & ~_BV(buzz) | _BV(swCoil);

  TCCR0B |= _BV(CS02) | _BV(CS00); // 1200000 дел. 1024 = 853 мкс
  TCCR0A |= _BV(WGM01);            //set  CTC mode
  TIMSK0 |= _BV(OCIE0A);           // разрешение прерываний по совпадению т.рег.А
  OCR0A = 12;                      // 853*117 =~ 0.1 сек.

  timeout = 0U;
  //timer
  sec = eeprom_read_dword((uint32_t *)46);

  // We use the AceButton::init() method here instead of using the constructor
  // to show an alternative. Using init() allows the configuration of the
  // hardware pin and the button to be placed closer to each other.
  button.init(buttInner_MISO, HIGH);

  // Configure the ButtonConfig with the event handler, and enable the LongPress
  // and RepeatPress events which are turned off by default.
  ButtonConfig *buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  //buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
}

int main(void)
{
  setup();
  uint32_t start[5] = {0};
  uint8_t pause[5] = {1, 10, 20, 30, 40};
  sei();

  while (1)
  {
    start[0] = haos(start[0], pause[1], ledPin);
  }
}

///////////////////test func
uint32_t haos(uint32_t start, uint8_t pause, uint8_t pin)
{

  if ((count - start) >= pause)
  {
    start = count;
    PORTB ^= (1 << pin); /* code */
  }
  return start;
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
    // if (cycle == 0)
    // {
    //   cycle = 1;
    //   timeout = micros() + 1000000;
    //   break;
    // }

    // if (timeout > micros())
    // {
    //   /* code */
    //   timeout = micros() + 1000000;
    //   cycle++;
    // }
    // if (cycle >= 3)
    // {
    //   timeout = 0;
    //   cycle = 0;
    //   sec = 0;
    // }
    break;
  }
}