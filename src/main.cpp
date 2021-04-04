#include "main.h"

void coil(void);                //coil on/off
void ledpin(void);              //led on/off
void button();                  //button short/long pression
void buzzer(long int interval); //функция звук (инт-л)
//void LED4(byte pin, long int interval);             //функция считывания кнопки

#define buzzPin PB3
#define ledPin PB2
#define buttInnerMISO PB1
#define buttOuterMOSI PB0
#define coilPin PB4
#define INTERV_DEBOUNCE 9 //0.01s x 10 = 0.1 s debounce BUZZ_DURATION
#define INTERV_LONG 29    //0.01s x INTERV_DEBOUNCE x 30 = 3 s
#define BUZZ_DURATION 4   //buzzer sound duration 0.05 s
#define COIL_DURATION 199   //2 s

volatile bool press100ms = 0; //turn off coil on 6 s
volatile bool press3s = 1; //turn off coil
bool press;                  //press button event

volatile bool timerLED1On, timerLED2On, timerLED3On, timerLED4On; //переменная вкл/выкл таймера
volatile unsigned int buttCntr;                                   //debounced/short durations counter
bool LED1On, LED2On, LED3On, LED4On;                              //переменная для хранения состояния
volatile unsigned long int timerLED1, timerLED1Loop;              //переменные подсчета мс и
volatile unsigned long int timerLED2, timerLED2Loop;              //переменные подсчета мс и
volatile unsigned long int timerLED3, timerLED3Loop;              //переменные подсчета мс и
volatile unsigned long int timerLED4, timerLED4Loop;              //переменные подсчета мс и
//volatile unsigned long int timerLED4, timerLED4Loop; //переменные подсчета мс и

ISR(TIM0_COMPA_vect)
{
  if (timerLED1On) //если включен миллисекудный таймер для coil
    timerLED1++;   //инкремент переменной таймера (+1)

  if (timerLED2On) //если включен миллисекудный таймер для button
    timerLED2++;   //инкремент переменной таймера (+1)

  if (timerLED3On) //если включен миллисекудный таймер для buzzer
    timerLED3++;   //инкремент переменной таймера (+1)

  if (timerLED4On) //если включен миллисекудный таймер для LED2
    timerLED4++;   //инкремент переменной таймера (+1)
}

void setup()
{
  //  INIT PINs
  DDRB |= _BV(ledPin) | _BV(buzzPin) | _BV(coilPin);
  PORTB = ~_BV(ledPin) & ~_BV(buzzPin) | _BV(coilPin) | _BV(buttInnerMISO) | _BV(buttOuterMOSI);

  TCCR0B |= _BV(CS02) | _BV(CS00); // 1200000 дел. 1024 = 853 мкс
  TCCR0A |= _BV(WGM01);            //set  CTC mode
  TIMSK0 |= _BV(OCIE0A);           // разрешение прерываний по совпадению т.рег.А
  OCR0A = 11;                      // 853us*12 =~ 0.01 сек.
  sei();
}

int main(void)
{
  setup();

  while (1)
  {
    coil();
    button();
    buzzer(600);
    ledpin();
  }
}

/////////////////// func
void coil(void) //coil function, timer 1
{
  if (!timerLED1On && !press3s && press100ms) //если таймер не был запущен и разрешена coil and butt pressed
  {
    timerLED1On = 1; //запустить таймер
  }
  if (timerLED1On && press3s) //если таймер был запущен и запрещена coil
  {
    cli();           //остановить прерывания
    timerLED1On = 0; //запретить пополнение переменной таймера
    timerLED1 = 0;   //обнулить переменную таймера
    sei();           //разрешить прерывания
    LED1On = 0;      //установить флаг выключения
  }

  if (!press3s) //если разрешена coil
  {
    cli();                              //остановить прерывания
    timerLED1Loop = timerLED1;          //сохранить значение переменной таймера
    sei();                              //разрешить прерывания
    if (timerLED1Loop >= COIL_DURATION) //сравнить значение таймера с заданным интервалом
    {                                   //если значение превысило интервал
      cli();
      timerLED1 = 0; //обнулить таймер
      sei();
      if (LED1On == 1) //if flag was set
      {
        PORTB &= ~_BV(coilPin); //coil turn off
      }
      LED1On = 0; // flag reset
      timerLED1On = 0; //запретить пополнение переменной таймера
    }
    else //идет счет
    {
      if (LED1On == 0) //если светодиод был выключен
      {
        PORTB |= _BV(coilPin); //coil turn on
      }
      LED1On = 1; //flag set
    }
  }
}

void button(void) //press long/sort func, timer 2
{
  if (timerLED2On == 0)
  {
    timerLED2On = 1;
  }

  cli();
  timerLED2Loop = timerLED2;
  sei();

  if (timerLED2Loop >= INTERV_DEBOUNCE)
  {
    cli();
    timerLED2 = 0; //timer counter reset
    sei();
    if (~PINB & _BV(buttInnerMISO)) //is button pressed?
    {
      press100ms = 1;
      if (buttCntr >= INTERV_LONG) //long press is reached max
      {
        buttCntr = 0;           //reset debounced counter
        press3s = !press3s; //flip long press flag
      }
      else
      {
        buttCntr++;
      }
    }
    else
    {
      buttCntr = 0; //reset debounced counter
      press100ms = 0;//reset short press flag
    }
  }
}

void ledpin(void) //led control, temer4
{
  if (timerLED4On == 0)
  {
    timerLED4On = 1;
  }

  cli();
  timerLED4Loop = timerLED4;
  sei();
  if (timerLED4Loop >= INTERV_DEBOUNCE) //end time period reach
  {
    cli();
    timerLED4 = 0; //timer conter reset
    sei();
    //  LED4On = 0; //flag reset
    if (!press3s && press100ms) //long press
    {
      PORTB &= ~_BV(ledPin); //port set
    }
    else //
    {
      PORTB |= _BV(ledPin); //port reset
    }
  }
}

void buzzer(long int interval) //buzz control
{
  if (timerLED3On == 0)
  {
    timerLED3On = 1;
  }

  cli();
  timerLED3Loop = timerLED3;
  sei();
  if (timerLED3Loop >= interval) //end time period reach
  {
    cli();
    timerLED3 = 0; //timer conter reset
    sei();
    //  LED3On = 0; //flag reset
  }
  else if (timerLED3Loop > (interval - BUZZ_DURATION)) //BUZZ_DURATION time slot
  {
    if (LED3On == 0)
    {
      PORTB |= _BV(buzzPin); //port set
      LED3On = 1;            //flag set
    }
  }
  else //rest time slot
  {
    if (LED3On == 1)
    {
      PORTB &= ~_BV(buzzPin); //port reset
      LED3On = 0;             //flag reset
    }
  }
}
