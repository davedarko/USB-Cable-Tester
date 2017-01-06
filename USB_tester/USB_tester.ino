/* 
​Things to safe power, according to datasheet:
 - turn off adc (not needed for LEDs)
 - turn off brown out detection (BOD)
 - turn off analog comparator
 - turn off internal voltage reference 
 - turn off watchdog timer
 - disable port pins per register
 
With the help of JChristensen/AVR Sleep
https://gist.github.com/JChristensen/5616922

*/

#include <avr/sleep.h>
#define BODS 7                     //BOD Sleep bit in MCUCR
#define BODSE 2                    //BOD Sleep enable bit in MCUCR

int leds[] = {3,0,1,2};
void setup() 
{
  for (int i=0; i<4; i++)
  {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], HIGH);
    delay(200);
    digitalWrite(leds[i], LOW);
    delay(200);
  }
  die_until_reset();
}

void loop() { }

void die_until_reset()
{
  byte  mcucr1, mcucr2;

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  MCUCR &= ~(_BV(ISC01) | _BV(ISC00));      //INT0 on low level
  ADCSRA &= ~_BV(ADEN);                     //disable ADC
  cli();                                    //stop interrupts to ensure the BOD timed sequence executes as required
  mcucr1 = MCUCR | _BV(BODS) | _BV(BODSE);  //turn off the brown-out detector
  mcucr2 = mcucr1 & ~_BV(BODSE);            //if the MCU does not have BOD disable capability,
  MCUCR = mcucr1;                           //  this code has no effect
  MCUCR = mcucr2;
  sleep_cpu();
}

