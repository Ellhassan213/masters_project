#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Pinout
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define clear() fillScreen(0)

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

void setup(){

  matrix.begin();

  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

//  rings(12, 10, 0, 0, 7, 0);
//  rings(8, 8, PI/4, 0, 7, 0);
  

//  middleBox();

//  align(0);
}

void loop(){

rings(8, 4, PI/3, 0, 7, 0);

//  pinMode(13, OUTPUT);
//  digitalWrite(13, HIGH);
//  delay(500);
//  digitalWrite(13, LOW);
//  delay(500);
//    rings(8, 4, PI/3, 0, 7, 0);
//  rings(8, 8, PI/4, 7, 0, 0);
//  rings(12, 10, 0, 0, 7, 0);
}

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
//    matrix.clear();
    
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
