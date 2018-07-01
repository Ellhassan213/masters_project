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
const int array_size = 70;
char data_in[array_size] = {};
int itr = 0;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);


/* Functions */

void clean_up(){
  
  char_in = 0;
  itr = 0;
  for(unsigned int i = 0; i < sizeof(data_in); i++){
      
      data_in[i] = 0;
  }
  matrix.clear();
}

void line(int16_t start, int16_t finish, int16_t axis, int16_t colour){

    int16_t r, g, b;

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

    for(int16_t i = start; i <= finish; i++){

        matrix.drawPixel(i, axis, matrix.Color333(r, g, b));
    }
}

void rectangle_fill_incr(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour, int16_t exposure){

  int16_t r, g, b;

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
  
  for(int16_t i = x; i < w; i++) {
    for(int16_t j = y; j < h; j++){

      matrix.drawPixel(i, j, matrix.Color333(r, g, b));

      digitalWrite(11, HIGH);
      delay(exposure + 1);

      matrix.clear();
      digitalWrite(11, LOW);
      delay(exposure + 1);
    }
  }
}

void rectangle_fill(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour){

    int16_t r, g, b;

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
    
    for(int16_t i = x; i < w; i++) {
        for(int16_t j = y; j < h; j++){

        matrix.drawPixel(i, j, matrix.Color333(r, g, b));
        }
    }
}

void rectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour){

    int16_t r, g, b;

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

    matrix.drawRect(x, y, w, h, matrix.Color333(r, g, b));    
}

void rings(int count, int radius, int offset, uint16_t  colour, int16_t  exposure){

    float delta = ( (2 * PI) / count );
    float theta = 0;
    double x, y;
    
    int16_t r, g, b;

    if(offset == 3){
        theta = PI / 3;
    }
    else if(offset == 4){
        theta = PI / 4;
    }
    else{
        theta = 0;
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

        x = radius * cos(theta) + (16);
        y = radius * sin(theta) + (16);

        matrix.drawPixel((int16_t)x, (int16_t)y, matrix.Color333(r, g, b)); 
        digitalWrite(11, HIGH);
        delay(exposure + 1);

        matrix.clear();

        digitalWrite(11, LOW);
        delay(exposure + 1);
        
        theta += delta;    
    }
}

void middleBox(int16_t exposure){
  
  // draw middle box
  matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(15, 16, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(16, 16, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
}

void align(int16_t exposure){

  // Align
  matrix.drawPixel(16, 13, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(16, 19, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(13, 16, matrix.Color333(7, 7, 7));
  delay(exposure + 1);
  matrix.drawPixel(19, 16, matrix.Color333(7, 7, 7));
  delay(exposure + 1);  
}

void blinking(int16_t exposure){
    
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(exposure + 1);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(exposure + 1);             // wait for a second
}

void pixel(int16_t x, int16_t y, uint16_t colour){

    int16_t r, g, b;

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
        
        data_in[itr] = (char_in - '0');

        delay(10);
        itr++;
    }

    int16_t id = concatenate(data_in[0], data_in[1]);

    if(id == 01){

        int16_t e1 = concatenate(data_in[2], data_in[3]);
        int16_t e2 = concatenate(data_in[4], data_in[5]);

        int16_t exposure = concatenate(e1, e2);
        
        clean_up();
        blinking(exposure);
    }
    else if(id == 02){

        int16_t e1 = concatenate(data_in[2], data_in[3]);
        int16_t e2 = concatenate(data_in[4], data_in[5]);

        int16_t exposure = concatenate(e1, e2);

        clean_up();
        align(exposure);
    }
    else if(id == 03){

        int16_t e1 = concatenate(data_in[2], data_in[3]);
        int16_t e2 = concatenate(data_in[4], data_in[5]);

        int16_t exposure = concatenate(e1, e2);

        clean_up();
        middleBox(exposure);
    }
    else if(id == 04){
        
        // int count = concatenate(data_in[2], data_in[3]);
        // int radius = concatenate(data_in[4], data_in[5]);
        // int offset = concatenate(data_in[6], data_in[7]);
        // uint16_t colour = concatenate(data_in[8], data_in[9]);

        // int16_t e1 = concatenate(data_in[10], data_in[11]);
        // int16_t e2 = concatenate(data_in[12], data_in[13]);

        // int16_t exposure = concatenate(e1, e2);

        // clean_up();
        // rings(count, radius, offset, colour, exposure);

        int16_t x = concatenate(data_in[2], data_in[3]);
        int16_t y = concatenate(data_in[4], data_in[5]);
        int16_t width = concatenate(data_in[6], data_in[7]);
        int16_t height = concatenate(data_in[8], data_in[9]);
        uint16_t colour = concatenate(data_in[10], data_in[11]);

        clean_up();
        line(x, width, y, colour);
        line(x, width, height, colour);
        line(height, y, x, colour);
        line(height, y, width, colour);
    }
    else if(id == 05){ // execute single pixel function

        int16_t x = concatenate(data_in[2], data_in[3]);
        int16_t y = concatenate(data_in[4], data_in[5]);
        uint16_t colour = concatenate(data_in[6], data_in[7]);

        clean_up();
        pixel(x, y, colour);
    }
    else if(id == 06){ // execute rectangle function

        int16_t x = concatenate(data_in[2], data_in[3]);
        int16_t y = concatenate(data_in[4], data_in[5]);
        int16_t width = concatenate(data_in[6], data_in[7]);
        int16_t height = concatenate(data_in[8], data_in[9]);
        uint16_t colour = concatenate(data_in[10], data_in[11]);

        clean_up();
        rectangle_fill(x, y, width, height, colour);
    }
    else if(id == 07){ // execute rectangle increamental fill function

        int16_t x = concatenate(data_in[2], data_in[3]);
        int16_t y = concatenate(data_in[4], data_in[5]);
        int16_t width = concatenate(data_in[6], data_in[7]);
        int16_t height = concatenate(data_in[8], data_in[9]);
        uint16_t colour = concatenate(data_in[10], data_in[11]);
        
        int16_t e1 = concatenate(data_in[12], data_in[13]);
        int16_t e2 = concatenate(data_in[14], data_in[15]);

        int16_t exposure = concatenate(e1, e2);

        clean_up();
        rectangle_fill_incr(x, y, width, height, colour, exposure);
    }
    else if(id == 99){

        int16_t count = concatenate(data_in[2], data_in[3]);

        int16_t e1 = concatenate(data_in[4], data_in[5]);
        int16_t e2 = concatenate(data_in[6], data_in[7]);
        int16_t exposure = concatenate(e1, e2);

        uint16_t colour = concatenate(data_in[8], data_in[9]);

        int16_t r, g, b;

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

        matrix.clear();
        for(int16_t i = 10; i <= (count * 4) + 6; i += 4){

            matrix.drawPixel(concatenate(data_in[i], data_in[i+1]), concatenate(data_in[i+2], data_in[i+3]), matrix.Color333(r, g, b));
            digitalWrite(11, HIGH);
            delay(exposure + 1);

            matrix.clear();

            digitalWrite(11, LOW);
            delay(exposure + 1);
        }
        clean_up();
    }
}




/* Arduino Built-in Functions*/

void setup(){

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