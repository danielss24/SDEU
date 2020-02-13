#include "mbed.h"
#include "PwmOut.h"


PwmOut pin8(p26);

int main() {
    pin8.period(0.00005f);  // 20 kHz
    pin8.write(0.30f);      //ciclo de trabajo
    while(1);
}
