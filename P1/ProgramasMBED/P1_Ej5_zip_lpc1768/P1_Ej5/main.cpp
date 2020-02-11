#include "mbed.h"
 
InterruptIn pin11(p11);
DigitalOut dout1(LED1);
 
void flip() {
    dout1 = !dout1;
}

int main() {
    pin11.rise(&flip);  // attach the address of the flip function to the rising edge
    pin11.fall(&flip);
    
}