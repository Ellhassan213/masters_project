/* Start of Arduino Instructions */


/* Include Files */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Global Variables */
char char_in = -1;
char data_in[100];
int itr = 0;

/* Function Pointers */

int (*blinking_pointer)(double);

/* Functions */

unsigned concatenate(unsigned first, unsigned second){

    unsigned power = 10;
    while(second >= power){
        power *= 10;
    }
    return first * power + second;
}

int decoder(){

    while(Serial.available() > 0){
      (*blinking_pointer)(300);
        char_in = Serial.read();
        data_in[itr] = (char_in - '0');

        Serial.write(char_in);
        itr++;
    }

    int arg_1 = concatenate(data_in[0], data_in[1]);
    
    if(char_in == '2'){

        (*blinking_pointer)(300);
    }

  return 0;
}

int blinking(double time_delay){

    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(time_delay);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(time_delay);              // wait for a second

    return 0;
}


/* Arduino Built-in Functions*/

void setup(){

    blinking_pointer = blinking;

    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);

    Serial.begin(9600); 
}

void loop(){

    decoder();
    // (*blinking_pointer)(300);
}