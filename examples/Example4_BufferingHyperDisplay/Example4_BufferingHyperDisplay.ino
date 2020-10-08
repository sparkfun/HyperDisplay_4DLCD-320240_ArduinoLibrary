/*
SparkFun TFT LCD 2.4" 320x240 using HyperDisplay
Example4 Buffering HyperDisplay

In this example we will look at buffering in HyperDisplay.
Buffering, in this context, means storing graphics data to be sent to the display later, all at once.
The default mode of HyperDisplay is to send data to the display as soon as a function like 'line()' is
called. This is good for saving memory on small microcontrollers, however when the data transfer is slow
you might be able to see the time between one function call and another. If you desire to flip right
from one full image to another it may be useful to compose the second one without modifying the data in
the display and then unleash all that data at one moment later. This does, however, require more memory
usage in the microcontroller.

This example will walk through the basics of buffering operations in HyperDisplay.

*Note* Because of the memory requirements of this example it cannot be run on an Arduino Uno or other
AtMega 328 based board. Consider using a SAMD21 or Teensy board instead.



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
#define SPI_SPEED 32000000    // Requests host uC to use the fastest possible SPI speed up to 32 MHz

LCD320240_4WSPI myTFT;

ILI9341_color_16_t defaultColor;

void setup() {
  SERIAL_PORT.begin(115200);

  //Uncomment this if you want to wait to println until the serial monitor is open.
  //while (!SERIAL_PORT); //Wait for Serial Monitor to Open

  SERIAL_PORT.println("Example4 HyperDisplay : SparkFun TFT LCD 2.4in");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);  // This is a non-hyperdisplay function, but it is required to make the display work
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  myTFT.clearDisplay();                                       // clearDisplay is also not part of hyperdisplay, but we will use it here for simplicity

  defaultColor = myTFT.rgbTo16b( 255, 255, 255 );
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);


  // In Example3 we demonstrated the full usage of HyperDisplay drawing functions. Now we will show how to accumulate drawing objects in a window buffer
  // so that they can all be shown at once in a process similar to double-buffering in computer graphics.

  // By default HyperDisplay runs in "direct" mode where data is sent directly to the display memory
  // This is to reduce using memory usage but to accumulate drawing operations we will need to provide some memory.
  ILI9341_color_16_t wholeScreenMem[240*320];  // This is enough memory for the whole screen
  ILI9341_color_16_t smallWindowMem[32*64];    // And this is a smaller portion of memory - good for 32*64 pixels
#ifndef ESP_PLATFORM
  ILI9341_color_16_t randomWindowMem[64*64];   // This is a portion of memory that we will use later.
#endif

  wind_info_t smallWindow;                // Here we are creating another window to use in this example - you'll see why shortly
  myTFT.setWindowDefaults(&smallWindow);  // This function helps to initialize the window so we can do less work and still not worry
  smallWindow.xMin = 16;
  smallWindow.yMin = 72;
  smallWindow.xMax = 16 + 31; // See, 16+0 would be one pixel, so 16+31 is actually 32 pixels wide
  smallWindow.yMax = 72 + 63; // and this means 64 pixels tall.. so now our small window can be filled with 32*64 pixels
  myTFT.setWindowColorSequence(&smallWindow, (color_t)&defaultColor);

  // Good, but HyperDisplay still needs to know about the memory
#ifndef ESP_PLATFORM
  myTFT.setCurrentWindowMemory((color_t)wholeScreenMem, 240*320);       // setCurrentWindowMemory applies the memory to the current window. By default the current window is the whole screen
#endif
  myTFT.setWindowMemory(&smallWindow, (color_t)smallWindowMem, 32*64);  // It is really importatnt to tell the right number of pixels to this function to avoid buffer over/underflow problems
                                                                        // P.S. this function allows you to set memory to a window that is not the current window

  // Alright, so we are ready to begin drawing.
  // At first we will compare direct and buffering modes by drawing the same set of items to both windows (default and small)
  // at the same time, with some delays. We will use the small window in buffering mode and the large window in direct mode.
  // Since functions in HyperDisplay work on the current window we will have to do a fair amount of switching back and forth, but thats ok.

  myTFT.pCurrentWindow = &smallWindow;                                    // Switch to small window
  myTFT.buffer();                                                         // Set the current window to buffer mode (only needs to happen once, this is stored in the window info structure)
  ILI9341_color_16_t smallBGcolor = myTFT.rgbTo16b( 0, 127, 200 );       // Make a background color for small window
  myTFT.fillWindow((color_t)&smallBGcolor);                                   // If the small window was not set to buffer the large window fill would completely cover up this call...
  myTFT.pCurrentWindow = &hyperdisplayDefaultWindow;                      // Switch back to default window
  ILI9341_color_16_t defaultBGcolor = myTFT.rgbTo16b( 220, 0, 64 );     // Make a background color for small window
  myTFT.fillWindow((color_t)&defaultBGcolor);
  delay(1000);                              // Watch as it all unfolds before your very own eyes!


  myTFT.pCurrentWindow = &smallWindow;  // Switch back to small window
  myTFT.line(0,0, 55,45);               // Draw a line using the default color sequence
  myTFT.pCurrentWindow = &hyperdisplayDefaultWindow; // Switch back to the default window
  myTFT.line(0,0, 239,130);               // Draw with the same exact function
  delay(1000);                          // Pause for dramatic effect

  myTFT.pCurrentWindow = &smallWindow;  // Switch back to small window
  myTFT.rectangle(0,0, 31, 63);         // Draw a border around the window
  myTFT.pCurrentWindow = &hyperdisplayDefaultWindow;  // Switch back to default window
  myTFT.rectangle(0,0, 239, 319);         // Draw a border around the window
  delay(1000);                          // Pretend you're Leo getting an Oscar

  // To output the changes made to the window onto the screen call the 'show()' method
  myTFT.pCurrentWindow = &smallWindow;  // Switch back to small window
  myTFT.show();

  // Now that that little experiment is over we can switch the small window back into direct mode
  // I'll also let you in on a little secret... you can specify the window to change modes and show
  myTFT.direct(&smallWindow); // Also valid are myTFT.buffer(&smallWindow) and myTFT.show(&smallWindow);





#ifndef ESP_PLATFORM

  // As a side note (because it is cool) let's see what happens if we show uninitialized memory...
  wind_info_t randomWindow;                // Here we are creating another window to use in this example - you'll see why shortly
  myTFT.setWindowDefaults(&randomWindow);  // This function helps to initialize the window so we can do less work and still not worry
  randomWindow.xMin = 62;
  randomWindow.yMin = 94;
  randomWindow.xMax = 125;
  randomWindow.yMax = 157;
  myTFT.setWindowMemory(&randomWindow, (color_t)randomWindowMem, 64*64);
  myTFT.show(&randomWindow);               // What will show up in this window?
#endif
  /* The answer is "whatever data was in memory when you created the 'randomWindowMem' array," however there are
     some interesting follow-ups to that question. Chances are good that when you try this you will see something
     that looks like static on a CRT TV. This leads us to believe that the memory contents were random when the
     microcontroller powered up (Power On Reset). Why is that? Is it intentional (i.e. for security) or is it a
     fact of how the RAM works?

     Can you ever get a non-randon pattern to appear there -- without drawing on it on purpose? When I was writing
     this example I found a large section of pure white pixels in mine, but it disappeared when I power cycled the
     Teensy. Perhaps before I added the random memory array that location in RAM was used internally by HyperDisplay.
     Some data was stored there at one point but moved somewhere else when I declared the array. When I power cycled
     all of RAM was randomized and so we lost it.

      Well if it's not repeatabe what good is it? Try this:
      1. Comment out the last block of code (from 'wind_info_t randomWindow;' to ' myTFT.show(&randomWindow);') and upload - let it run to the end
      2. Un-comment the same block and re-upload. Let the example run through again.

      I was able to repeat the results but it is basically just luck that that exact memory location was used - in the
      future we might find that changes to the compiler or other build settings might remove this little easter egg.

      As a final thought: If you are interested try to find the part of HyperDisplay that might make such an artifact appear.
      Happy Hunting!
  */
}

void loop() {

}