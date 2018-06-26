/* Start of Arduino Instructions */


/* Include Files */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Pinout for 32x32 Matrix - Arduino uno
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define clear() fillScreen(0)

/*Global Variables */
char char_in = -1;
char data_in[100];
int itr = 0;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

/* Function Pointers */

int (*blinking_pointer)(double);
void (*align_pointer)(double);
void (*middleBox_pointer)();
void (*rings_pointer)(double, double, double, double);

/* Functions */

void rings(double count, double radius, double offset, double colour){

  double theta = offset;
  double delta = (2 * PI) / count;
  double x = 0;
  double y = 0;
  
  int r = 0; int g = 0; int b = 0;

  if(colour == 01){
      
      r = 7; g = 0; b = 0;
  }
  else if(colour == 02){

      r = 0; g = 7; b = 0;
  }
  else if(colour == 03){

      r = 0; g = 0; b = 7;
  }
  
  for(int i = 0; i < count; i++){

    x = radius * cos(theta) + (matrix.width() / 2);
    y = radius * sin(theta) + (matrix.height() / 2);

    matrix.drawPixel(x, y, matrix.Color333(r, g, b)); 
    digitalWrite(11, HIGH);
    delay(500);

    matrix.drawPixel(x, y, 0);
    digitalWrite(11, LOW);
    delay(500);
    
    theta += delta;    
  }
}

void middleBox(){
  
  // draw middle box
  matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7));
  matrix.drawPixel(15, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7));

  delay(500);  
}

void align(double d){

  // Align
  matrix.drawPixel(16, 13, matrix.Color333(7, 7, 7));
  delay(d);
  matrix.drawPixel(16, 19, matrix.Color333(7, 7, 7));
  delay(d);
  matrix.drawPixel(13, 16, matrix.Color333(7, 7, 7));
  delay(d);
  matrix.drawPixel(19, 16, matrix.Color333(7, 7, 7));
  delay(d);  
}

unsigned concatenate(unsigned first, unsigned second){

    unsigned power = 10;
    while(second >= power){
        power *= 10;
    }
    return first * power + second;
}

int decoder(){

    while(Serial.available() > 0){

        char_in = Serial.read();
        data_in[itr] = (char_in - '0');

        Serial.write(char_in);
        itr++;
    }

    double id = concatenate(data_in[0], data_in[1]);
    double arg_1 = concatenate(data_in[2], data_in[3]);
    double arg_2 = concatenate(data_in[4], data_in[5]);
    double arg_3 = concatenate(data_in[6], data_in[7]);
    double arg_4 = concatenate(data_in[8], data_in[9]);
    
    if(id == 01){

        (*blinking_pointer)(arg_1);
    }
    else if(id == 02){

        (*align_pointer)(arg_1);
    }
    else if(id == 03){

        (*middleBox_pointer)(arg_1);
    }
    else if(id == 04){

        (*rings_pointer)(arg_1, arg_2, arg_3, arg_4);
    }

  return 0;
}

int blinking(double d){
    
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(d);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(d);             // wait for a second

    return 0;
}


/* Arduino Built-in Functions*/

void setup(){

    // initialise serial comms and matrix control
    Serial.begin(9600); 
    matrix.begin();

    // initialise function pointers
    blinking_pointer = blinking;
    align_pointer = align;
    middleBox_pointer = middleBox;
    rings_pointer = rings;

    // initialize digital pins
    pinMode(13, OUTPUT);

    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);
}

void loop(){

    decoder();
}