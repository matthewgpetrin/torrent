int reg, angle, divisor, n;
int* digital;
double voltage[7];
int voltpins[7];
const int pin1=1;
int digitalpins[42];
#include <Math.h>
#include <Arduino.h>
int* analogToBits(double num1) {
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
void setup() {
  n=0;// put your setup code here, to run once:
  while (n<7){
    voltpins[n]=n;
    n++;
  }
  n=2;
  int x = 0;
  while (n<14){
    digitalpins[x]=n;
    x++;
    n++;
  }
  n=22;
  while (n<54){
    digitalpins[x]=n;
    n++;
    x++;
  }
}
void loop() {
  n=0;
  while (n<7){ //Get voltage values from analog pins 0 through 6 and save to double array
    voltage[n]=analogRead(voltpins[n]);
    n++;
  }
  n=0;
  voltage[0]=3.125;
  int pinnum=0;

  while (n<7){
    digital = analogtobits(voltage[n]);
 
    makedigitaloutput(digital,pinnum);
    n++;
    pinnum+=6;
  }
  
}
void makeDigitalOutput(int* digitalNum, int numPin) {
  reg = 0;
  while (reg <6) {
    if (digital[reg] = 1){
      digitalWrite(digitalpins[numPin], HIGH);
      reg++;
      numPin++;
    } else {
      digitalWrite(digitalpins[numPin], LOW);
      reg++;
      numPin++;
    }
  }
}
