/*
    Integrantes:

    Daniel Serena Sanz              e299571
    Juan Andres Escalante Rodriguez e388792
*/


#include "mbed.h"

// Pins utilizados
AnalogIn   ain(p15);
DigitalOut dout1(LED1);
DigitalOut dout2(LED2);
DigitalOut dout3(LED3);
DigitalOut dout4(LED4);



int main(void) {

    float maxi = 0.0f;
    float value = 0.0f;

    while (1) {

        value = ain;

		//Actualiza el mayor valor detectado
        if (value > maxi){
            maxi = value;
        }

		//Prende los diferentes leds tomando en cuenta sus rangos

        if(value > maxi * 0.2f) {
            dout1 = 1;
        } else {
            dout1 = 0;
        }

        if(value > maxi * 0.4f) {
            dout2 = 1;
        } else {
            dout2 = 0;
        }

        if(value > maxi * 0.6f) {
            dout3 = 1;
        } else {
            dout3 = 0;
        }

        if(value > maxi * 0.8f) {
            dout4 = 1;
        } else {
            dout4 = 0;
        }      
    }
}
