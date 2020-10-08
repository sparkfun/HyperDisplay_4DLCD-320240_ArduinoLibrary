/*
SparkFun TFT LCD 2.4" 320x240 using HyperDisplay
Example1 Display Test

This example can be used as a visual indication that the display is working as expected.

Author: Alex Wende
Modified: 9/22/20
This software is open source. Use it how you like, just don't hurt people.

*/
#include "HyperDisplay_4DLCD-320240_4WSPI.h" // Click here to get the library: https://github.com/sparkfun/HyperDisplay_4DLCD-320240_ArduinoLibrary
                                           // Click here to get the mid-level library: http://librarymanager/All#SparkFun_HyperDisplay_ILI9341
                                           // Click here to get HyperDisplay (top level): http://librarymanager/All#SparkFun_HyperDisplay

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN PWM0             // Pin definitions
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000        // Requests host uC to use the fastest possible SPI speed up to 32 MHz

LCD320240_4WSPI myTFT;           // The LCD320240_4WSPI class is used for this breakout, and we will call our object myTFT

ILI9341_color_16_t defaultColor; // Global objects are used for default colors so that they are always in-scope

void setup() {
  SERIAL_PORT.begin(115200);

  //Uncomment this if you want to wait to println until the serial monitor is open.
  //while (!SERIAL_PORT); //Wait for Serial Monitor to Open

  SERIAL_PORT.println("Example1 Display Test : SparkFun TFT LCD 2.4in");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();

  myTFT.setTextCursor(0,0);            // Sets the cursor relative to the current window, however the current (default) window is equivalent to the whole display. (0,0) is the upper left corner and (myTFT.xExt-1, myTFT.yExt-1) is the lower right
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);

  uint16_t hue = HSV_HUE_MIN;
  while(hue <= HSV_HUE_MAX){
    myTFT.setTextCursor(0,0);
    defaultColor = myTFT.hsvTo16b( hue+=30, 255, 255 );
    myTFT.println("Hello world!");   // Prints using the default font at the cursor location
  }
}

void loop() {
    lineTest();
    delay(500);

    rectTest();
    delay(500);

    circleTest();
    for(uint8_t indi = 250; indi > 1; indi--){
      myTFT.setBacklight(indi); // Set the brightness of the backlight using PWM output
      delay(10);
    }
    for(uint8_t indi = 0; indi < 250; indi++){
      myTFT.setBacklight(indi); // Set the brightness of the backlight using PWM output
      delay(10);
    }
    delay(500);
}

void lineTest( void )
{
  ILI9341_color_16_t color;

  myTFT.clearDisplay();

  color = myTFT.rgbTo16b( 255, 255, 255 );
  for(hd_hw_extent_t indi = 0; indi < myTFT.xExt; indi++)
  {
    myTFT.line(0,0,indi,myTFT.yExt-1,1,(color_t)&color);
  }

  color = myTFT.rgbTo16b( 255, 0, 0 );
  for(hd_hw_extent_t indi = 0; indi < myTFT.yExt; indi++)
  {
    myTFT.line(0,myTFT.yExt-1,myTFT.xExt-1,myTFT.yExt-indi-1,1,(color_t)&color);
  }

  color = myTFT.rgbTo16b( 0, 255, 0 );
  for(hd_hw_extent_t indi = 0; indi < myTFT.xExt; indi++)
  {
    myTFT.line(myTFT.xExt-1,myTFT.yExt-1,myTFT.xExt-indi-1,0,1,(color_t)&color);
  }

  color = myTFT.rgbTo16b( 0, 0, 255 );
  for(hd_hw_extent_t indi = 0; indi < myTFT.yExt; indi++)
  {
    myTFT.line(myTFT.xExt-1,0,0,indi,1,(color_t)&color);
  }
}

void rectTest( void )
{
  myTFT.clearDisplay();
  ILI9341_color_16_t color;

  color = myTFT.rgbTo16b( 255, 255, 255 );
  for(uint8_t indi = 0; indi < myTFT.xExt/2; indi+=1)
  {
    myTFT.rectangle(myTFT.xExt/2-1-indi, myTFT.yExt/2-1-indi, myTFT.xExt/2+1+indi, myTFT.yExt/2+1+indi, false, (color_t)&color);
    delay(50);
  }

  color = myTFT.rgbTo16b( 0, 0, 255 );
//  for(uint8_t indi = 0; indi < myTFT.xExt/2; indi+=1)
//  {
//    myTFT.rectangle(myTFT.xExt/2-1-indi, myTFT.yExt/2-1-indi, myTFT.xExt/2+1+indi, myTFT.yExt/2+1+indi, false, (color_t)&color);
//    delay(50);
//  }

  myTFT.rectangle(0, 0, myTFT.xExt, myTFT.yExt, true, (color_t)&color);


  color = myTFT.rgbTo16b( 0, 0, 0 );
  for(uint8_t indi = 0; indi < myTFT.xExt/2; indi+=1)
  {
    myTFT.rectangle(myTFT.xExt/2-1-indi, myTFT.yExt/2-1-indi, myTFT.xExt/2+1+indi, myTFT.yExt/2+1+indi, false, (color_t)&color);
    delay(50);
  }
}

void circleTest( void )
{
  myTFT.clearDisplay();
  ILI9341_color_16_t color;

  for(uint8_t indi = 0; indi < (myTFT.xExt/2 - 1); indi++)
  {
    color = myTFT.hsvTo16b( (uint16_t)((double)(((double)HSV_HUE_MAX*indi)/((double)myTFT.xExt/2 - 1))), 255, 255 );
    myTFT.circle((myTFT.xExt/2 - 1),(myTFT.yExt/2 - 1),(myTFT.xExt/2 - 1)-indi, true, (color_t)&color);
    delay(100);
  }
}
