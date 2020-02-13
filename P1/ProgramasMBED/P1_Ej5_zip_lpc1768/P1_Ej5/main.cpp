#include "mbed.h"
 
InterruptIn pin11(p11);
DigitalOut dout1(LED1);

// Con cada excepcion se cambia el estado del led
void flip() {
    dout1 = !dout1;
}

int main() {
	
	//Damos de alta la excepcion cuando hay un aumento o decremento
    pin11.rise(&flip); 
    pin11.fall(&flip);
    
}
