/*
    Integrantes:

    Daniel Serena Sanz              e299571
    Juan Andres Escalante Rodriguez e388792
*/

#include "mbed.h"
#include <stdlib.h>    

AnalogOut Aout(p18); 
float i;
 
  
int main(){
    while(1){
		//La senal sube hasta su 100% = 3.3 en 0.5s
        for(i = 0; i <= 1; i=i+0.02)
        {
            Aout = i;
            wait(0.01);
        }

		//La senal cae a 0 en 0.5s
        for(i = 1; i >= 0; i=i-0.02)
        {
            Aout = i;
            wait(0.01);
        }  
    }
}
 
 
