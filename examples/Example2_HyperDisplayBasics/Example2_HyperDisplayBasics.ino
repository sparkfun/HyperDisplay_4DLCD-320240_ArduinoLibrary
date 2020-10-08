/*
SparkFun TFT LCD 2.4" 320x240 using HyperDisplay
Example2 HyperDisplay Basics

This example is designed to introduce the base functions that are available in any HyperDisplay powered display.

Author: Alex Wende
Modified: 9/22/20
This software is open source. Use it how you like, just don't hurt people.

*/
#include "HyperDisplay_4DLCD-320240_4WSPI.h" // Click here to get the library: https://github.com/sparkfun/HyperDisplay_4DLCD-320240_ArduinoLibrary
                                           // Click here to get the mid-level library: http://librarymanager/All#SparkFun_HyperDisplay_ILI9341
                                           // Click here to get HyperDisplay (top level): http://librarymanager/All#SparkFun_HyperDisplay

/*
What is hyperdisplay?
HD is an abstract C++ library for controlling 2D output devices by taking advantage of a common interface.

Despite many individual display technologies needing varying electrical interfaces to operate they nearly
all can be controlled (conceptually) as long as you have the ability to set a particular pixel to an
arbitrary color. Hyperdisplay "assumes" that this capability exists and then uses it to implement drawing
functions, text, and many other capabilities in a standard, hardware-independent manner.
*/

#define SERIAL_PORT Serial    // Allows users to easily change target serial port (e.g. SAMD21's SerialUSB)

#define PWM_PIN PWM0             // Pin definitions
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000    // Requests host uC to use the fastest possible SPI speed up to 32 MHz

LCD320240_4WSPI myTFT;        // The LCD320240_4WSPI class is used for this breakout, and we will call our object myTFT
                              // The class is derived from the hyperdisplay class, and so it has all the functionality of
                              // hyperdisplay plus a little more. In this example we will only focus on the functions that
                              // come from hyperdisplay

ILI9341_color_18_t defaultColor; // To use a default color it is best to make the variable global

void setup() {
  SERIAL_PORT.begin(115200);

  //Uncomment this if you want to wait to println until the serial monitor is open.
  //while (!SERIAL_PORT); //Wait for Serial Monitor to Open

  SERIAL_PORT.println("Example2 HyperDisplay Basics : SparkFun TFT LCD 2.4in");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);  // This is a non-hyperdisplay function, but it is required to make the display work
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_18);
  myTFT.clearDisplay();                                       // clearDisplay is also not pat of hyperdisplay, but we will use it here for simplicity

  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );                     // Colors are specific to display types, so this is not part of hyperdisplay...
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);        // But this is!

  // Exploration of HyperDisplay abilities:
  /*
      The base layer of hyperdisplay only handles drawing functions:
        pixel
        xline
        yline
        line
        rectangle
        circle
        polygon
        fillFromArray
        fillWindow
      Each of these functions roughly follows a pattern for the arguments: (locations, options, color info)
      Also the location information of these functions is relative to a "window" - though the defualt window
      the entire display, so for basic applications there's no need to worry about it.

      Because HD can handle everything from monochrome OLEDs to full color TFTs it is up to the user to specify
      color information. HD uses the notion of color cycles - that is patterns of colors that can be repeated.
      Every drawing function uses parameters called colorCycleLength and colorCycleOffset, which default to 1 and 0
  */

  // Let's do a simple example using pixel...
  myTFT.setTextCursor(0,0);
  myTFT.print("Pixel Example...");
  delay(1000);

  // Check out the definition of the 18-bit color type that is used with this breakout:
  /*
    typedef struct ILI9141C_color_18{
      uint8_t r;
      uint8_t g;
      uint8_t b;
    }ILI9341_color_18_t;
  */
  // By default myTFT assumes that colors are in this form - so let's make one!
  ILI9341_color_18_t pixelColor; // Name this color "pixel color"
  pixelColor.r = 0xFF;            // Set the r, g, and b values how you like
  pixelColor.g = 0;
  pixelColor.b = 0;

  myTFT.clearDisplay();                     // To be flexible the 'pixel' function needs to take a 'color_t' type
  myTFT.pixel(0,0, (color_t)&pixelColor);   // so we cast the reference of our pixelColor to the color_t type
  delay(5000);

  // Now let's do examples of the other HyperDipslay drawing functions using our color:
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.print("(X/Y)Line Example...");
  delay(1000);
  myTFT.clearDisplay();
  myTFT.xline(2, 1, 10, (color_t)&pixelColor);        // the first three arguments re x0, y0, and length - so this draws from coordinate (2,1) to (12,1)
  myTFT.yline(1, 2, 10, (color_t)&pixelColor);        // Now using yline the length goes down, so from (1,2) to (1,12)
  myTFT.line(8, 8, 20, 60, 4, (color_t)&pixelColor);  // the 'line' function will connect (x0, y0) and (x1, y1) with a line, and you can specify a width (4 in this case)
  delay(5000);

  ILI9341_color_18_t innerRectColor = {    // Using an initializer list we can make a new color
    0x00, // red
    0xA0, // green
    0x80, // blue
  };

  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.print("Rectangle Example...");
  delay(1000);
  myTFT.clearDisplay();
  myTFT.rectangle(5, 5, 30, 45, false, (color_t)&pixelColor); // The arguments are x0, y0, x1, y1, then a boolean for filled or not, then the color
  myTFT.rectangle(7, 7, 28, 43, true, (color_t)&innerRectColor);
  delay(5000);


  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.print("Circle Example...");
  delay(1000);
  myTFT.clearDisplay();
  myTFT.circle(64, 64, 32, false, (color_t)&pixelColor); // The arguments are x0, y0, x1, y1, then a boolean for filled or not, then the color
  myTFT.circle(64, 64, 24, true, (color_t)&innerRectColor);
  myTFT.circle(64, 64, 12, false, (color_t)&pixelColor);
  delay(5000);



  // Now let's talk about the option to use a default color!
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.print("Default Color Example...");  // Default color is used to print to the screen
  delay(1000);
  myTFT.clearDisplay();
  myTFT.pixel(3, 40, (color_t)&pixelColor);
  myTFT.pixel(5, 40);                                                   // Default color also can be used in place of a color in any drawing function
  myTFT.setTextCursor(0,0);
  myTFT.setCurrentWindowColorSequence((color_t)&innerRectColor);        // You can change the current default color like this
  myTFT.print("Changing the default color changes the color of text and drawing functions!");
  myTFT.pixel(7, 40);                                                   // And as shown here the new default color is used for the next call to a drawing function
  delay(20000);



  ILI9341_color_18_t doneColor;
  doneColor.r = 0x48;
  doneColor.g = 0xB0;
  doneColor.b = 0x08;

  myTFT.setCurrentWindowColorSequence((color_t)&doneColor);
  myTFT.clearDisplay();
  myTFT.setTextCursor(0,0);
  myTFT.print("Done!\n\n Move on to Example3 for advanced hyperdisplay use...");
  SERIAL_PORT.println("Done!\n\n Move on to Example3 for advanced hyperdisplay use...");
}

void loop() {

}
