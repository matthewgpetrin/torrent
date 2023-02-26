int reg;
int angle;
int *digital;
double v1;
int v2;
int v3;
int v4;
int v5;
int v6;
int v7;
int v8;

int v9;
int divisor;
const int pin1=1;
#include <Math.h>
#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  v1 = analogRead(pin1);
  digital = analogtobits(v1);
  reg = 0;
  int pinnum = 0;
  while (reg <6)
    if (digital[reg] = 1){
      digitalWrite(pinnum, HIGH);
      reg++;
      pinnum++;
    } else {
      digitalWrite(reg, LOW);
      reg++;
      pinnum++;
    }

}

int *analogtobits(double num1) {
  angle = num1*72;
  angle = round(angle);
  divisor = 180;
  reg = 0;
  static int digit[6];
  while (reg<6) {
    if (angle/divisor>=1) {
      digit[reg] = 1;
      reg++;
      divisor=divisor/2;

    } else {
      digit[reg] = 0;
      reg++;
      divisor=divisor/2;
    }
  }
  return digit;
}
