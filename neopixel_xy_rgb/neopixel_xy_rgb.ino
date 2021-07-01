// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 25 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 0 // Time (in milliseconds) to pause between pixels

void dot(int x,int y,bool on);

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

int j = 0,k=0,l=0;

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(500); // Pause before next pass through loop

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
   // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
   // Here we're using a moderately bright green color:
//    dot(0,0); dot(0,1);
//    dot(1,0); dot(1,1);  
//    pixels.show();   // Send the updated pixel colors to the hardware.
//    delay(5000); // Pause before next pass through loop

    for(l=1;l<6;l++){
      for(j=0;j<l;j++){
        for(k=0;k<l;k++){
//         dot(k,j,true,150,150,150);
         dot(k,j,true,150,0,0);
        }
      }
      
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(500); // Pause before next pass through loop
    }

    for(l=1;l<6;l++){
      for(j=0;j<l;j++){
        for(k=0;k<l;k++){
//         dot(k,j,true,150,150,150);
         dot(k,j,true,0,150,0);
        }
      }
      
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(500); // Pause before next pass through loop
    }

    for(l=1;l<6;l++){
      for(j=0;j<l;j++){
        for(k=0;k<l;k++){
//         dot(k,j,true,150,150,150);
         dot(k,j,true,0,150,150);
        }
      }
      
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(500); // Pause before next pass through loop
    }
//    while(1){}
    for(l=1;l<6;l++){
      for(j=0;j<l;j++){
        for(k=0;k<l;k++){
//         dot(k,j,true,150,150,150);
         dot(k,j,true,150,150,150);
        }
      }
      
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(500); // Pause before next pass through loop
    }

    for(l=1;l<6;l++){
      for(j=0;j<l;j++){
        for(k=0;k<l;k++){
         dot(k,j,false,0,0,0);
        }
      }
      
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(500); // Pause before next pass through loop
    }
/*
    int level =255;
    dot(0,0,true,level,0,0);
    dot(4,0,true,0,level,0);
    dot(2,4,true,0,0,level);
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(2000); // Pause before next pass through loop

    dot(0,0,true,0,0,150);
    dot(4,0,true,150,0,0);
    dot(2,4,true,0,150,0);
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(2000); // Pause before next pass through loop

    dot(0,0,true,0,150,0);
    dot(4,0,true,0,0,150);
    dot(2,4,true,150,0,0);
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(2000); // Pause before next pass through loop
*/
}

void dot(int x,int y,bool on,int r,int g,int b){
//    int x,y;
  int value_r,value_g ,value_b;
  if(on==true){
      value_r = r;
      value_g = g;
      value_b = b;
    }else{
      value_r = 0;
      value_g = 0;
      value_b = 0;
    }
    
    if(y%2==0){
      pixels.setPixelColor(x+(5*y), pixels.Color(value_r, value_g, value_b));
    }else{
      pixels.setPixelColor((4-x)+(5*y), pixels.Color(value_r, value_g, value_b));
    }
}
