/*
SparkFun TFT LCD 2.4" 320x240 using HyperDisplay
Example3 Advanced HyperDisplay

This example goes into more detail about the powerful features of HyperDisplay

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

ILI9341_color_18_t defaultColor;

void setup() {
  SERIAL_PORT.begin(115200);

  //Uncomment this if you want to wait to println until the serial monitor is open.
  //while (!SERIAL_PORT); //Wait for Serial Monitor to Open

  SERIAL_PORT.println("Example3 Advanced HyperDisplay : SparkFun TFT LCD 2.4in");

  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);  // This is a non-hyperdisplay function, but it is required to make the display work
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_18);
  myTFT.clearDisplay();                                       // clearDisplay is also not pat of hyperdisplay, but we will use it here for simplicity

  defaultColor = myTFT.rgbTo18b( 255, 255, 255 );
  myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);

  // In Example2 we talked about basic usage of the gyperdisplay drawing functions, but they really have a lot more to offer!
  // Here are the function prototypes from the library -- we can break them down bit by bit
  /*
        void     pixel(hd_extent_t x0, hd_extent_t y0, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);
        void    xline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goLeft = false);
        void    yline(hd_extent_t x0, hd_extent_t y0, hd_extent_t len, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool goUp = false);
        void    rectangle(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false, bool gradientVertical = false);
        void    fillFromArray(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, color_t data = NULL, hd_pixels_t numPixels = 0, bool Vh = false);
        void    fillWindow(color_t color, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0);
        void    line(hd_extent_t x0, hd_extent_t y0, hd_extent_t x1, hd_extent_t y1, uint16_t width = 1, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false);
        void    polygon(hd_extent_t x[], hd_extent_t y[], uint8_t numSides, uint16_t width = 1, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false);
        void    circle(hd_extent_t x0, hd_extent_t y0, hd_extent_t radius, bool filled = false, color_t data = NULL, hd_colors_t colorCycleLength = 1, hd_colors_t startColorOffset = 0, bool reverseGradient = false);
  */
  // In every case the first arguments have to do with the geometry of the function and they are required
  // After that come arguments having to do with the color - or more appropriately the color cycle
  //    - color_t data : this is a pointer to the color. The data that exists at this pointer will be assumed to be of the color type
  //      that the display uses. This defaults to NULL in which case it will try to use a default color
  //    - hd_colors_t colorCycleLength : this number tells how long the color cycle is - in other words it is a promise that this many
  //      color objects exist consecutively after the pointer 'data.' If not specified this defaults to 1
  //    - hd_colors_t startColorOffset : this number allows the color cycle to start part-way through the color cycle. If this number is greater
  //      than the color cycle length it is wrapped back around with a modulo operation. This value defaults to 0
  // Then finally there may be some additional options that are specific to each function. To change these you will need to explicitly state all other arguments

  // Now let's practice making and using a color cycle!
  const uint8_t CCLength = 10;
  ILI9341_color_18_t colorArray[CCLength];         // Declare an array of color structures
  for(uint8_t indi = 0; indi < CCLength; indi++){   // Loop through the array
    colorArray[indi].r = 0x00;
    colorArray[indi].g = (127*sin(((double)indi/CCLength)*2*3.1415)) + 127;   // In this case the color cycle will be a sin wave in green
    colorArray[indi].b = 0x00;
  }

  myTFT.line(2,2, 238,2, 4, (color_t)colorArray, CCLength, 0);

  // You can use the startColorOffset parameter to easily make an 'animated' line
  for(uint8_t indi = 0; indi < 254; indi++){
    myTFT.line(2,12, 238,12, 4, (color_t)colorArray, CCLength, indi); // Notice that it is okay for startColorOffset to exceed colorCycleLength
    delay(20);
  }
  myTFT.clearDisplay();


  // Color cycles work in two ways for rectangles:
  // For a non-filled rectangle the cycle goes around the perimeter, and you can reverse the direction. It always starts from the upper-left corner
  // For a filled rectangle the color cycle is used to create either a horizontal or vertical gradient, both of which can be reversed
  const uint8_t rectCCLength = 60;
  ILI9341_color_18_t rectColorArray[rectCCLength];         // Declare an array of color structures
  for(uint8_t indi = 0; indi < rectCCLength; indi++){       // Loop through the array
    rectColorArray[indi].r = 0x00;
    rectColorArray[indi].g = 255 * ((double)indi/rectCCLength);  // A ramp color cycle to help illustrate the reversed and vertical options of rectangle
    rectColorArray[indi].b = 0x00;
  }
  // Color cycles and options for rectangles:
  myTFT.rectangle(3, 3, 236, 316, false, (color_t)rectColorArray, rectCCLength, 0);               // Makes a non-filled rectablge around the whole screen
  myTFT.rectangle(0, 0, 239, 319, false, (color_t)rectColorArray, rectCCLength, 0, true);         // Makes a non-filled rectablge around the whole screen with the cycle going in the other direction
  myTFT.rectangle(6, 6, 60, 76, true, (color_t)rectColorArray, rectCCLength, 0);                  // Makes a filled rectangle that defaults to a non-reversed horizontal gradient (the color cycle is the gradient)
  myTFT.rectangle(68, 6, 233, 76, true, (color_t)rectColorArray, rectCCLength, 0, true);          // Makes a filled rectangle with a horizontal gradient that is reversed
  myTFT.rectangle(6, 84, 60, 313, true, (color_t)rectColorArray, rectCCLength, 0, false, true);   // Makes a filled rectangle with a non-reversed vertical gradient
  myTFT.rectangle(68, 84, 233, 313, true, (color_t)rectColorArray, rectCCLength, 0, true, true);  // Makes a filled rectangle with a vertical gradient that is reversed
  delay(5000);
  myTFT.clearDisplay();



  // Another HyperDisplay feature not discussed in Example2 is the "polygon" function(s).
  // Polygon will automatically draw several connected lines when given the coordinates in a list.
  // The main advantages are simplicity and the fact that a color cycle can run uninterrupted through the whole polygon.
  hd_extent_t xCoords[] = {120, 239,  0, 239,   0};
  hd_extent_t yCoords[] = {24, 280, 120,  120, 280};
  if(sizeof(xCoords) != sizeof(yCoords)){
    SERIAL_PORT.println("Coordinate array dimensions mismatch. Please make xCoords and yCoords have the same number of elements. Freezing");
    while(1){};
  }
  myTFT.polygon(xCoords, yCoords, sizeof(xCoords)/sizeof(hd_extent_t), 1, (color_t)rectColorArray, rectCCLength, 0, false); // You can choose the width (3 here) and whether or not to reverse the gradient direction too
  delay(5000);
  myTFT.clearDisplay();


  // Now we will discuss the window feature of hyperdisplay.
  // Windows allow you to easily reposition drawing objects, restrict drawing ranges, and position text.
  // Here's how a window structure looks:
  /*
  typedef struct window_info{
    hd_hw_extent_t  xMin;                      // FYI window min/max use the hardware frame of reference
    hd_hw_extent_t  xMax;           //
    hd_hw_extent_t  yMin;           //
    hd_hw_extent_t  yMax;           //
    hd_extent_t     cursorX;                  // Where the cursor is currently in window-coordinates
    hd_extent_t     cursorY;                  // Where the cursor is currently in window-coordinates
    hd_extent_t     xReset;                   // Where the cursor goes on a reset location (window coordinates)
    hd_extent_t     yReset;                   // Where the cursor goes on a reset location (window coordinates)
    char_info_t     lastCharacter;            // Information about the last character written.
    color_t         currentSequenceData;      // The data that is used as the default color sequence
    hd_colors_t     currentColorCycleLength;  // The default color sequence number of pixels
    hd_colors_t     currentColorOffset;       // The current offset
    bool            buffer;                   // Indicates either buffer or direct mode (direct is default)
    color_t         data;                     // A pointer to pixel data that is specific to the window. Can be left as NULL
    hd_pixels_t    numPixels;                 // The number of pixel types that data points to
    bool            dynamic;                  // Indicator if the current buffer memory was dynamically allocated - so that it can be disposed of automatically
  }wind_info_t;                        // Window infomation structure for placing objects on the display
  */
  // You'll notice that the window structure handles min/max extent values, current cursor values, reset cursor values, character info, and color cycle info

  // In previous examples we never mentioned windows and that's OK because the default window covers the entire screen.
  // Now we will do a few demos with windows...

  wind_info_t wind1, wind2, wind3;  // Create several window objects
  ILI9341_color_18_t color1, color2, color3;

  // Initialize the windows to defualt settings (this is a pretty important step unless you are extra careful to manually initialize each and every paramter)
  myTFT.setWindowDefaults(&wind1);
  myTFT.setWindowDefaults(&wind2);
  myTFT.setWindowDefaults(&wind3);

  color1.r = 0xFF;    // Set the colors to red, green, and blue respectively
  color1.g = 0x00;
  color1.b = 0x00;

  color2.r = 0x00;
  color2.g = 0xFF;
  color2.b = 0x00;

  color3.r = 0x00;
  color3.g = 0x00;
  color3.b = 0xFF;

  // Now we will set up the boundaries of the windows, the cursor locations, and their default colors
  wind1.xMin = 0;
  wind1.yMin = 0;
  wind1.xMax = 118;
  wind1.yMax = 118;
  wind1.cursorX = 1;
  wind1.cursorY = 1;
  wind1.xReset = 1;
  wind1.yReset = 1;
  wind1.currentSequenceData = (color_t)&color1;
  wind1.currentColorCycleLength = 1;
  wind1.currentColorOffset = 0;

  wind2.xMin = 121;
  wind2.yMin = 0;
  wind2.xMax = 239;
  wind2.yMax = 118;
  wind2.cursorX = 15;
  wind2.cursorY = 15;
  wind2.xReset = 15;
  wind2.yReset = 15;
  wind2.currentSequenceData = (color_t)&color2;
  wind2.currentColorCycleLength = 1;
  wind2.currentColorOffset = 0;

  wind3.xMin = 0;
  wind3.yMin = 121;
  wind3.xMax = 239;
  wind3.yMax = 319;
  wind3.cursorX = 1;
  wind3.cursorY = 1;
  wind3.xReset = 10;                              // This extra line will give a hanging indent effect for window 3
  wind3.yReset = 1;
  wind3.currentSequenceData = (color_t)&color3;
  wind3.currentColorCycleLength = 1;
  wind3.currentColorOffset = 0;

  // All hyperdisplay drawing functions are applied to the current window, and the coordinates are with respect to the window.
  // To demonstrate this we will use the same exact drawing function to draw in each of the three windows, each with to unique effect
  wind_info_t* windowPointers[] = {&wind1, &wind2, &wind3};
  for(uint8_t indi = 0; indi < 3; indi++){
    myTFT.pCurrentWindow = windowPointers[indi];  // Set the current window
    myTFT.line(1,1, 238,78);                      // Use the window's default color to draw a line on the window - notice that it is truncated beyond the window area
  }
  delay(5000);

  // The 'fillWindow' function can be used with or without a specified color
  for(uint8_t indi = 0; indi < 3; indi++){
    myTFT.pCurrentWindow = windowPointers[indi];  // Set the current window
    myTFT.fillWindow();                           // Fill the window with the window's color
  }
  delay(5000);

  // And lastly, the printing functions are impacted by windows.
  for(uint8_t indi = 0; indi < 3; indi++){
    myTFT.pCurrentWindow = windowPointers[indi];                  // Set the current window
    myTFT.setCurrentWindowColorSequence((color_t)&defaultColor);  // Change each window's color to the white value we defined at the beginning
    myTFT.println("You see, each window handles printed text on its own!");

    myTFT.print("Done!");
  }

  SERIAL_PORT.println("Done!");
  myTFT.println("\n\nDone!");
}

void loop() {

}