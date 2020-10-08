//
// ILI9341_240x320 Display Class    			  
//

#ifndef _HYPERDISPLAY_4DLCD-320240_4WSPI_H_
#define _HYPERDISPLAY_4DLCD-320240_4WSPI_H_

#include "HyperDisplay_ILI9341.h"


#define LCD320240_WIDTH 		240
#define LCD320240_HEIGHT 		320
#define LCD320240_START_ROW	0
#define LCD320240_START_COL	0
#define LCD320240_STOP_ROW 	319
#define LCD320240_STOP_COL	239

class LCD320240_4WSPI : public ILI9341_4WSPI{
private:
protected:
public:

	

	LCD320240_4WSPI();	// Constructor

	uint8_t _bl;

	ILI9341_STAT_t begin(uint8_t dcPin, uint8_t csPin, uint8_t blPin, SPIClass &spiInterface = SPI, uint32_t spiFreq = ILI9341_SPI_DEFAULT_FREQ);
	ILI9341_STAT_t defaultConfigure( void ); // The reccomended settings from the datasheet
	void startup( void );		// The default startup for this particular display

	 void getCharInfo(uint8_t val, char_info_t  pchar);

	// Some specialized drawing frunctions
	void clearDisplay( void );

	// Special Functions
	void setWindowDefaults(wind_info_t*  pwindow); // Overrrides default implementation in hyperdisplay class
	void setBacklight(uint8_t b);
};

#endif /* _HYPERDISPLAY_4DLCD-320240_4WSPI_H_ */