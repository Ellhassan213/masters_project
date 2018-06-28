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
char data_in[50];
int itr = 0;

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

/* Function Pointers */

void (*blinking_pointer)(double);
void (*align_pointer)(double);
void (*middleBox_pointer)();
void (*pixel_pointer)(double, double, double);
void (*rings_pointer)(double, double, double, double, double);
void (*rectangle_pointer)(double, double, double, double, double);
void (*rectangle_fill_pointer)(double, double, double, double, double, double);

/* Functions */

void clean_up(){
  
  char_in = 0;
  itr = 0;
  for(int i = 0; i < sizeof(data_in); i++){
  data_in[i] = 0;
  }
  matrix.clear();
}

void rectangle_fill(double x, double y, double w, double h, double colour, double exposure){

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
  
  for(double i = x; i < w; i++) {
    for(double j = y; j < h; j++){

      matrix.drawPixel(i, j, matrix.Color333(r, g, b));

      digitalWrite(11, HIGH);
      delay(exposure);

      matrix.drawPixel(i, j, 0);
      digitalWrite(11, LOW);
      delay(exposure);
    }
  }
}

void rectangle(double x, double y, double w, double h, double colour){

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

  matrix.fillRect(x, y, w, h, matrix.Color333(r, g, b));
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
  matrix.drawPixel(15, 15, matrix.Color333(7, 7, 7));
  matrix.drawPixel(15, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 16, matrix.Color333(7, 7, 7));
  matrix.drawPixel(16, 15, matrix.Color333(7, 7, 7));
}

void align(double exposure){

  // Align
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
        
        data_in[itr] = (char_in - '0');

        delay(10);
        itr++;
    }

    double id = concatenate(data_in[0], data_in[1]);

    if(id == 01){

        double e1 = concatenate(data_in[2], data_in[3]);
        double e2 = concatenate(data_in[4], data_in[5]);

        double exposure = concatenate(e1, e2);
        
        clean_up();
        (*blinking_pointer)(exposure);
    }
    else if(id == 02){

        double e1 = concatenate(data_in[2], data_in[3]);
        double e2 = concatenate(data_in[4], data_in[5]);

        double exposure = concatenate(e1, e2);

        clean_up();
        (*align_pointer)(exposure);
    }
    else if(id == 03){

        clean_up();
        // (*middleBox_pointer)();
        (*rings_pointer)(12, 10, 00, 01, 0500);
    }
    else if(id == 04){
        
        double count = concatenate(data_in[2], data_in[3]);
        double radius = concatenate(data_in[4], data_in[5]);
        double offset = concatenate(data_in[6], data_in[7]);
        double colour = concatenate(data_in[8], data_in[9]);

        double e1 = concatenate(data_in[10], data_in[11]);
        double e2 = concatenate(data_in[12], data_in[13]);

        double exposure = concatenate(e1, e2);

        clean_up();
        (*rings_pointer)(count, radius, offset, colour, exposure);
    }
    else if(id == 05){ // execute single pixel function

        double x = concatenate(data_in[2], data_in[3]);
        double y = concatenate(data_in[4], data_in[5]);
        double colour = concatenate(data_in[6], data_in[7]);

        clean_up();
        (*pixel_pointer)(x, y, colour);
    }
    else if(id == 06){ // execute rectangle function

        double x = concatenate(data_in[2], data_in[3]);
        double y = concatenate(data_in[4], data_in[5]);
        double width = concatenate(data_in[6], data_in[7]);
        double height = concatenate(data_in[8], data_in[9]);
        double colour = concatenate(data_in[10], data_in[11]);

        clean_up();
        (*rectangle_pointer)(x, y, width, height, colour);
    }
    else if(id == 07){ // execute rectangle increamental fill function

        double x = concatenate(data_in[2], data_in[3]);
        double y = concatenate(data_in[4], data_in[5]);
        double width = concatenate(data_in[6], data_in[7]);
        double height = concatenate(data_in[8], data_in[9]);
        double colour = concatenate(data_in[10], data_in[11]);
        
        double e1 = concatenate(data_in[12], data_in[13]);
        double e2 = concatenate(data_in[14], data_in[15]);

        double exposure = concatenate(e1, e2);

        clean_up();
        (*rectangle_fill_pointer)(x, y, width, height, colour, exposure);
    }
    else if(id == 10){  // execute multiple concentric rings

        double n_o_r = concatenate(data_in[2], data_in[3]);

        if(n_o_r == 01){    // 1 ring requested

            double count = concatenate(data_in[4], data_in[5]);
            double radius = concatenate(data_in[6], data_in[7]);
            double offset = concatenate(data_in[8], data_in[9]);
            double colour = concatenate(data_in[10], data_in[11]);

            double e1 = concatenate(data_in[12], data_in[13]);
            double e2 = concatenate(data_in[14], data_in[15]);

            double exposure = concatenate(e1, e2);

            clean_up();
            (*rings_pointer)(count, radius, offset, colour, exposure);
        }
        else if(n_o_r == 02){   // 2 rings requested

            double count_1 = concatenate(data_in[4], data_in[5]);
            double radius_1 = concatenate(data_in[6], data_in[7]);
            double offset_1 = concatenate(data_in[8], data_in[9]);
            double colour_1 = concatenate(data_in[10], data_in[11]);

            double e1_1 = concatenate(data_in[12], data_in[13]);
            double e2_1 = concatenate(data_in[14], data_in[15]);

            double exposure_1 = concatenate(e1_1, e2_1);



            double count_2 = concatenate(data_in[16], data_in[17]);
            double radius_2 = concatenate(data_in[18], data_in[19]);
            double offset_2 = concatenate(data_in[20], data_in[21]);
            double colour_2 = concatenate(data_in[22], data_in[23]);

            double e1_2 = concatenate(data_in[24], data_in[25]);
            double e2_2 = concatenate(data_in[26], data_in[27]);

            double exposure_2 = concatenate(e1_2, e2_2);

            clean_up();
            (*rings_pointer)(count_1, radius_1, offset_1, colour_1, exposure_1);
            (*rings_pointer)(count_2, radius_2, offset_2, colour_2, exposure_2);
        }
        else if(n_o_r == 03){   // 3 rings requested

            double count_1 = concatenate(data_in[4], data_in[5]);
            double radius_1 = concatenate(data_in[6], data_in[7]);
            double offset_1 = concatenate(data_in[8], data_in[9]);
            double colour_1 = concatenate(data_in[10], data_in[11]);

            double e1_1 = concatenate(data_in[12], data_in[13]);
            double e2_1 = concatenate(data_in[14], data_in[15]);

            double exposure_1 = concatenate(e1_1, e2_1);



            double count_2 = concatenate(data_in[16], data_in[17]);
            double radius_2 = concatenate(data_in[18], data_in[19]);
            double offset_2 = concatenate(data_in[20], data_in[21]);
            double colour_2 = concatenate(data_in[22], data_in[23]);

            double e1_2 = concatenate(data_in[24], data_in[25]);
            double e2_2 = concatenate(data_in[26], data_in[27]);

            double exposure_2 = concatenate(e1_2, e2_2);



            double count_3 = concatenate(data_in[28], data_in[29]);
            double radius_3 = concatenate(data_in[30], data_in[31]);
            double offset_3 = concatenate(data_in[32], data_in[33]);
            double colour_3 = concatenate(data_in[34], data_in[35]);

            double e1_3 = concatenate(data_in[36], data_in[37]);
            double e2_3 = concatenate(data_in[38], data_in[39]);

            double exposure_3 = concatenate(e1_3, e2_3);

            clean_up();
            (*rings_pointer)(count_1, radius_1, offset_1, colour_1, exposure_1);
            (*rings_pointer)(count_2, radius_2, offset_2, colour_2, exposure_2);
            (*rings_pointer)(count_3, radius_3, offset_3, colour_3, exposure_3);  
        }
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
    rectangle_pointer = rectangle;
    rectangle_fill_pointer = rectangle_fill;

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
//  rectangle_fill(8, 8, 24, 24, 2, 10);
//  rectangle_fill(8, 8, 24, 24, 2, 10);

}