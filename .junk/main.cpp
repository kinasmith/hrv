#include <Arduino.h>
#include <Bounce2.h>


Bounce sw1 = Bounce();
Bounce sw2 = Bounce();

uint8_t sw1Pin = 7;
uint8_t sw2Pin = 8;

uint8_t fan1 = 9;
uint8_t fan2 = 10;
/**
 * @brief val between 0 and 1.
 * between 0.4 and 0.6 is stopped
 * less than 0.4 is blowing out
 * greater than 0.6 is blowing in.
 */
float fSpd; 

void setup() {
  Serial.begin(9600);
  // Timer 1, pins 9 and 10
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
      | _BV(COM1B1)  // same on ch. B
      | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
  TCCR1B = _BV(WGM13)   // ditto
      | _BV(CS10);   // prescaler = 1
  ICR1   = 160;


  sw1.attach( sw1Pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  sw2.attach( sw2Pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP

  sw1.interval(10); // interval in ms
  sw2.interval(10); // interval in ms

  sw1.update();
  sw2.update();
  uint16_t sw1Val = sw1.read();
  uint16_t sw2Val = sw2.read();

  if(sw1Val == 0 && sw2Val == 0) fSpd = 0.5;
  if(sw1Val == 1 && sw2Val == 0) fSpd = 0.39;
  if(sw1Val == 0 && sw2Val == 1) fSpd = 0.2;
  if(sw1Val == 1 && sw2Val == 1) fSpd = 1;
  Serial.print(sw1Val);
  Serial.print(",");
  Serial.println(sw2Val);
}

// unsigned long now;
uint32_t last = 0;
uint32_t interval = 70 * 1000;

void loop()
{
  sw1.update();
  sw2.update();

  if (sw1.changed() || sw2.changed()) 
  {
    int sw1Val = sw1.read();
    int sw2Val = sw2.read();

    if(sw1Val == 0 && sw2Val == 0) fSpd = 0.5;
    if(sw1Val == 1 && sw2Val == 0) fSpd = 0.39;
    if(sw1Val == 0 && sw2Val == 1) fSpd = 0.2;
    if(sw1Val == 1 && sw2Val == 1) fSpd = 1;
    Serial.print(sw1Val);
    Serial.print(",");
    Serial.println(sw2Val);
  }

  unsigned long now = millis();
  if(now - last >= interval) {
    last = now;
    Serial.println(fSpd);
    fSpd = abs(fSpd-1);
    Serial.println(fSpd);
  }

  analogWrite(fan1, 160*abs(fSpd-1)); //fan1 opposite from fan 2
  analogWrite(fan2, 160*fSpd);
}
