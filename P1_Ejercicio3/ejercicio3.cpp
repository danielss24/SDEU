#include "mbed.h"

// Initialize a pins to perform analog and digital output fucntions
AnalogOut  aout(P18);

int main(void)
{
    while (1) {
        aout = 3.3f;            
        wait(1.0f);
        aout = 0.0f;            
        wait(1.0f);
    }
}