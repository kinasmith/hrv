#include <Arduino.h>

volatile float R_T,F_T,period;
float pulseWidth;

int inFan = 5;
int inFanFlow = 0;
int outFan = 6;
int outFanFlow = 0;

int therm_pin = A3;
int R1 = 9920;
float singlePointCal = 2.8;

uint32_t long now;
uint32_t fanLast = 0;
uint32_t fanInterval = 30*1000;
uint32_t tempLast = 0;
uint32_t tempInterval = 1000; 
float fSpd = .1; //.5 is off. 0 is full speed

void rising();
void falling();
float readThermistor(int,int);

void setup() {
  analogWrite(6, 255*fSpd);
  Serial.begin(9600);
  attachInterrupt(0, rising, RISING); //Interrupt 0 is pin D2
}
 

void loop() { 
  pulseWidth = (F_T-R_T)/period * 100.;
  if(pulseWidth > 0) { //filter out bad data
    // Serial.print(pulseWidth); Serial.print(", ");
    inFanFlow = map(pulseWidth, 50, 100, 0, 255);
    outFanFlow = map(pulseWidth, 50, 0, 0, 255);
    // Serial.print(inFanFlow); Serial.print(", ");
    // Serial.print(outFanFlow); Serial.println(", ");
    if(inFanFlow > 0) {
      analogWrite(inFan, inFanFlow); 
    } else analogWrite(inFan, 0);
    if(outFanFlow > 0) {
      analogWrite(outFan, outFanFlow);
    } else analogWrite(outFan, 0);
  }
}

void rising() {
  attachInterrupt(0, falling, FALLING); //Interrupt 0 is pin D2
  period = micros() - R_T;
  R_T = micros();
}
void falling() {
  attachInterrupt(0, rising, RISING); //Interrupt 0 is pin D2
  F_T = micros();
}