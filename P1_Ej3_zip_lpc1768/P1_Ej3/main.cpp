#include "mbed.h"
#include <stdlib.h>     /* abs */

AnalogOut Aout(p18); // Declare pin 18 as an analog out
float i; // Declare a float to range between 0 and 1
 
  
int main(){
    while(1){
        // Make 'Aout' go from 0 to 1, in 100 steps of 0.01 
        for(i = 0; i <= 1; i=i+0.01)
        {
            Aout = i;
            wait(0.005);
        }
        // Make 'aout' go from 1 to 0, in 100 steps of 0.01
        for(i = 1; i >= 0; i=i-0.01)
        {
            Aout = i;
            wait(0.005);
        }  
    }


}
 
 