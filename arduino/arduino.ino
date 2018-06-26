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
void (*align_pointer)(int);
void (*middleBox_pointer)();
void (*rings_pointer)(int, double, double, int, int, int);

/* Functions */

void rings(int count, double radius, double offset, int r, int g, int b){

  double theta = offset;
  double delta = (2 * PI) / count;
  double x = 0;
  double y = 0;
  
  for(int i = 0; i < count; i++){

    x = radius * cos(theta) + (matrix.width() / 2);
    y = radius * sin(theta) + (matrix.height() / 2);

    matrix.drawPixel(x, y, matrix.Color333(r, g, b)); 
    digitalWrite(11, HIGH);
    delay(500);

    matrix.drawPixel(x, y, 0);
    digitalWrite(11, LOW);
    delay(500);
    matrix.clear();
    
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

void align(int d){

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

    int arg_1 = concatenate(data_in[0], data_in[1]);
    
    if(arg_1 == 01){

        (*blinking_pointer)(300);
        (*align)(100);
    }

  return 0;
}

int blinking(double time_delay){
    
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(time_delay);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(time_delay);             // wait for a second

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