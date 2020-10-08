#include "HyperDisplay_4DLCD-320240_4WSPI.h"

////////////////////////////////////////////////////////////
//					ILI9341_240x320 Implementation			  //
////////////////////////////////////////////////////////////
char_info_t LCD320240_Default_CharInfo;
wind_info_t LCD320240_Default_Window;

LCD320240_4WSPI::LCD320240_4WSPI() : hyperdisplay(LCD320240_WIDTH, LCD320240_HEIGHT), ILI9341_4WSPI(LCD320240_WIDTH, LCD320240_HEIGHT)
{

}

ILI9341_STAT_t LCD320240_4WSPI::begin(uint8_t dcPin, uint8_t csPin, uint8_t blPin, SPIClass &spiInterface, uint32_t spiFreq)
{
	// Call the functions to setup the super classes
// Associate 
	_dc = dcPin;
	// _rst = rstPin;
	_rst = 0xFF; // Not using reset pin
	_cs = csPin;
	_bl = blPin;
	// _rd = rdPin;
	_spi = &spiInterface;

	if(spiFreq != ILI9341_SPI_DEFAULT_FREQ)
	{
		setSPIFreq( spiFreq );
	}

	_spi->begin();

	// Set pinmodes
	pinMode(_cs, OUTPUT);
	// pinMode(_rst, OUTPUT);
	pinMode(_dc, OUTPUT);
	pinMode(_bl, OUTPUT);

	// Set pins to default positions
	digitalWrite(_cs, HIGH);
	// digitalWrite(_rst, HIGH);
	digitalWrite(_dc, HIGH);
	digitalWrite(_bl, LOW);

	// Setup the default window
	setWindowDefaults(pCurrentWindow);

	// Power up the device

	// try starting SPI with a simple byte transmisssion to 'set' the SPI peripherals
	uint8_t temp_buff[1];
	_spi->beginTransaction(_spisettings);
	_spi->transfer(temp_buff, 1);
	_spi->endTransaction();

	startup();	
	defaultConfigure();

	return ILI9341_STAT_Nominal;
}

ILI9341_STAT_t LCD320240_4WSPI::defaultConfigure( void )
{
	ILI9341_STAT_t retval = ILI9341_STAT_Nominal;

	retval = sleepOut(); // WriteComm(0x11);
	if(retval != ILI9341_STAT_Nominal){ return retval; }

  	delay(20);

  	retval = selectGammaCurve( 0x01 );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setNormalFramerate( 0x00, 0x1B );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setPowerControl1( 0x21, 0x00 );
 	if(retval != ILI9341_STAT_Nominal){ return retval; }

  	retval = setPowerControl2( 0x10 );
  	if(retval != ILI9341_STAT_Nominal){ return retval; }

//  	retval = setPowerControl3( 0x02 );
//  	if(retval != ILI9341_STAT_Nominal){ return retval; }

  	retval = setVCOMControl1( 0x3E, 0x28 );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setVCOMOffsetControl( true, 0x86 );
	if(retval != ILI9341_STAT_Nominal){ return retval; }
	
	retval = setInversion( true );
	if(retval != ILI9341_STAT_Nominal){ return retval; }
	
	retval = setInterfacePixelFormat( 0x05 );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setColumnAddress( LCD320240_START_COL, LCD320240_STOP_COL );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setRowAddress( LCD320240_START_ROW, LCD320240_STOP_ROW );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	retval = setMemoryAccessControl( false, true, false, false, true, false );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

//	retval = setSrcDriverDir( false );
//	if(retval != ILI9341_STAT_Nominal){ return retval; }

//	retval = setGamRSel( true );
//	if(retval != ILI9341_STAT_Nominal){ return retval; }

	uint8_t pgam[16] =  { 0x0F, 0x16, 0x14, 0x0A, 0x0D, 0x06, 0x43, 0x75, 0x33, 0x06, 0x0E, 0x00, 0x0C, 0x09, 0x08 };
	retval = setPositiveGamCorr( pgam );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

	uint8_t ngam[16] = { 0x08, 0x2B, 0x2D, 0x04, 0x10, 0x04, 0x3E, 0x24, 0x4E, 0x04, 0x0F, 0x0E, 0x35, 0x38, 0x0F };
	setNegativeGamCorr( ngam );
	if(retval != ILI9341_STAT_Nominal){ return retval; }

  	delay(20);

  	setPower( true );

  // WriteComm(0x2C);
  	return ILI9341_STAT_Nominal;
}

void LCD320240_4WSPI::startup( void )
{
	// Assume that VDD and VCC are stable when this function is called
	if(_rst != 0xFF){ digitalWrite( _rst , HIGH); }
  	delay(10);
  	if(_rst != 0xFF){ digitalWrite( _rst , LOW); }
  	delay(10);
  	if(_rst != 0xFF){ digitalWrite( _rst , HIGH); }
  	delay(120);
	// Now you can do initialization
}

// void UG6464TDDBG01::getCharInfo(uint8_t val, char_info_t * pchar)
// {
// 	char_info_t * pcharinfo = &UG6464TDDBG01_Default_CharInfo;
// 	// Do stuff to fill out the default char info structure, then return a pointer to it
// 	pcharinfo->data = NULL;						// Color information for each pixel to be drawn
// 	pcharinfo->xLoc = NULL;						// X location wrt upper-left corner of char location(cursor) for each pixel
// 	pcharinfo->yLoc = NULL;						// Y location wrt upper-left corner of char location(cursor) for each pixel
//     pcharinfo->numPixels = 0;					// Number of pixels contained in the data, xLoc, and yLoc arrays
// 	pcharinfo->show = false;					// Whether or not to actually show the character
// 	pcharinfo->causedNewline = false;			// Whether or not the character triggered/triggers a new line
// }


void LCD320240_4WSPI::clearDisplay( void )
{
	// Store the old window pointer: 
	wind_info_t * ptempWind = pCurrentWindow;

	// Make a new default window
	wind_info_t window;
	pCurrentWindow = &window;

	// Ensure the window is set up so that you can clear the whole screen
	setWindowDefaults(&window);

	// Make a local 'black' color 
	ILI9341_color_18_t black;
	black.r = 0;
	black.g = 0;
	black.b = 0;

	// Fill the temporary window with black
	fillWindow((color_t)&black);												// Pass the address of the buffer b/c we know it will be safe no matter what SSD1357 color mode is used

	// Restore the previous window
	pCurrentWindow = ptempWind;
}

void LCD320240_4WSPI::setWindowDefaults(wind_info_t * pwindow)
{
	// Fills out the default window structure with more or less reasonable defaults
	pwindow->xMin = LCD320240_START_COL;
	pwindow->yMin = LCD320240_START_ROW;
	pwindow->xMax = LCD320240_STOP_COL;
	pwindow->yMax = LCD320240_STOP_ROW;
	pwindow->cursorX = 0;							// cursor values are in window coordinates
	pwindow->cursorY = 0;
	pwindow->xReset = 0;
	pwindow->yReset = 0;
	
	pwindow->lastCharacter.data = NULL;
	pwindow->lastCharacter.xLoc = NULL;
	pwindow->lastCharacter.yLoc = NULL;
	pwindow->lastCharacter.xDim = 0;
	pwindow->lastCharacter.yDim = 0;
	pwindow->lastCharacter.numPixels = 0;
	pwindow->lastCharacter.show = false;
	pwindow->lastCharacter.causesNewline = false;
	
	pwindow->bufferMode = false;			// Start out in direct mode
	pwindow->data = NULL;				// No window data yet
	pwindow->numPixels = 0;
	pwindow->dynamic = false;
	setWindowColorSequence(pwindow, NULL, 1, 0);	// Setup the default color (Which is NULL, so that you know it is not set yet)
}

void LCD320240_4WSPI::setBacklight(uint8_t b){
#if defined(ARDUINO_ARCH_ESP32)
	ledcAttachPin(_bl,15);
	ledcSetup(15,12000,8);
	ledcWrite(15, 255-b);
#else
	analogWrite(_bl, 255-b);
#endif
}
