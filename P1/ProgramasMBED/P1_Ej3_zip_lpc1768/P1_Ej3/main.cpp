#include "mbed.h"
#include <stdlib.h>    

AnalogOut Aout(p18); 
float i;
 
  
int main(){
    while(1){
        for(i = 0; i <= 1; i=i+0.02)
        {
            Aout = i;
            wait(0.01);
        }

        for(i = 1; i >= 0; i=i-0.02)
        {
            Aout = i;
            wait(0.01);
        }  
    }
}
 
 