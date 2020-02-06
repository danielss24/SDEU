#include "mbed.h"
#include "PwmOut.h"


PwmOut pin8(p8);

int main() {
    pin8.period(0.00005f);      // 20 Khz
    pin8.write(0.30f);      // 30% duty cycle, relative to period
    while(1);
}