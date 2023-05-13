#include <Math.h>
#include <Arduino.h>

int digit[6];

double voltages[7];

int voltPins[7] = {0, 1, 2, 3, 4, 5, 6};

int digitalPins[42];

int *analogToBits(double angle) {
    angle = round(angle * 72); // WATCH
    int divisor = 180;

    for (int reg = 0; reg < 6; reg++) {
        if (angle / divisor >= 1) {
            digit[reg] = 1;
            reg++;
            divisor = divisor / 2;

        } else {
            digit[reg] = 0;
            reg++;
            divisor = divisor / 2;
        }
    }
    return digit;
}

void makeDigitalOutput(int digitalNum[6], int numPin) {
    for (int reg = 0; reg < 6; reg++) {
        if (digitalNum[reg] == 1) {
            digitalWrite(digitalPins[numPin], HIGH);
            numPin++;
        } else {
            digitalWrite(digitalPins[numPin], LOW);
            numPin++;
        }
    }
}

void setup() {
    for (int x = 0; x < 42; x++) {
        for (int n = 2; n < 14; n++) {
            digitalPins[x] = n;
            x++;
        }

        for (int n = 22; n < 53; n++) {
            digitalPins[x] = n;
            x++;
        }
    }
}

void loop() {
    for (int n = 0; n < 7; n++) { //Get voltages values from analog pins 0 through 6 and save to double array
        voltages[n] = analogRead(voltPins[n]);
    }

    voltages[0] = 3.125;

    int pinN = 0;

    for (int n = 0; n < 7; n++) {
        makeDigitalOutput(analogToBits(voltages[n]), pinN);
        pinN += 6;
    }
}