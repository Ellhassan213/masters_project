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
char previous_char_in = 10;
char data_in[20];
int itr = 0;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

/* Function Pointers */

void (*blinking_pointer)(double);
void (*align_pointer)(double);
void (*middleBox_pointer)();
void (*pixel_pointer)(double, double, double);
void (*rings_pointer)(double, double, double, double, double);

/* Functions */

void clean_up)(){

    char_in = 0;
    itr = 0;
    for(int i = 0; i < sizeof(data_in); i++){
        data_in[i] = 0;
    }
}

void rings(double count, double radius, double offset, double colour, double exposure){

    double delta = (2 * PI) / count;
    double x = 0;
    double y = 0;
    double theta = 0;
    
    int r = 0; int g = 0; int b = 0;

    if(offset == 3){
        theta = PI / 3;
    }
    else if(offset == 4){
        theta = PI / 4;
    }

    if(colour == 01){
        
        r = 7; g = 0; b = 0;
    }
    else if(colour == 02){

        r = 0; g = 7; b = 0;
    }
    else if(colour == 03){

        r = 0; g = 0; b = 7;
    }
    else{

        r = 7; g = 7; b = 7;
    }
    
    for(int i = 0; i < count; i++){

        x = radius * cos(theta) + (matrix.width() / 2);
        y = radius * sin(theta) + (matrix.height() / 2);

        matrix.drawPixel(x, y, matrix.Color333(r, g, b)); 
        digitalWrite(11, HIGH);
        delay(exposure);

        matrix.drawPixel(x, y, 0);
        digitalWrite(11, LOW);
        delay(exposure);
        
        theta += delta;    
    }
}

void middleBox(){
  
  // draw middle box
  clean_up();
  matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7));
  matrix.drawPixel(15, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7));
}

void align(double exposure){

  // Align
  clean_up();
  matrix.drawPixel(16, 13, matrix.Color333(7, 7, 7));
  delay(exposure);
  matrix.drawPixel(16, 19, matrix.Color333(7, 7, 7));
  delay(exposure);
  matrix.drawPixel(13, 16, matrix.Color333(7, 7, 7));
  delay(exposure);
  matrix.drawPixel(19, 16, matrix.Color333(7, 7, 7));
  delay(exposure);  
}

void blinking(double exposure){
    
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(exposure);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(exposure);             // wait for a second
}

void pixel(double x, double y, double colour){

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
    else{

        r = 7; g = 7; b = 7;
    }

    matrix.drawPixel(x, y, matrix.Color333(r, g, b));
}

unsigned concatenate(unsigned first, unsigned second){

    unsigned power = 10;
    while(second >= power){
        power *= 10;
    }
    return first * power + second;
}

void decoder(){
        
    while(Serial.available() > 0){

        char_in = Serial.read();
        previous_char_in = char_in;

        data_in[itr] = (char_in - '0');

        itr++;
    }

    double id = concatenate(data_in[0], data_in[1]);

    if(id == 01){

        double e1 = concatenate(data_in[2], data_in[3]);
        double e2 = concatenate(data_in[4], data_in[5]);

        double exposure = concatenate(e1, e2);
        (*blinking_pointer)(exposure);
    }
    if(id == 02){

        double e1 = concatenate(data_in[2], data_in[3]);
        double e2 = concatenate(data_in[4], data_in[5]);

        double exposure = concatenate(e1, e2);
        (*align_pointer)(exposure);
    }
    if(id == 03){

        (*middleBox_pointer)();
    }
    if(id == 04){
        
        double count = concatenate(data_in[2], data_in[3]);
        double radius = concatenate(data_in[4], data_in[5]);
        double offset = concatenate(data_in[6], data_in[7]);
        double colour = concatenate(data_in[8], data_in[9]);

        double e1 = concatenate(data_in[10], data_in[11]);
        double e2 = concatenate(data_in[12], data_in[13]);

        double exposure = concatenate(e1, e2);

        (*rings_pointer)(count, radius, offset, colour, exposure);
    }
}



/* Arduino Built-in Functions*/

void setup(){

    // initialise function pointers
    blinking_pointer = blinking;
    align_pointer = align;
    middleBox_pointer = middleBox;
    rings_pointer = rings;
    pixel_pointer = pixel;

    // initialize digital pins
    pinMode(13, OUTPUT);

    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);

    // initialise serial comms and matrix control
    Serial.begin(9600); 
    matrix.begin();
}

void loop(){

    decoder();
}