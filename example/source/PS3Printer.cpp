#include "PS3Printer.h"
#include <bitset>


//****** Include fonts here **********//
#include "Fonts/Arial8.h" //Dont remove this include, is the default one.
#include "Fonts/SegoeScript16.h"

//************************************//

//Initialization of the static vars.
unsigned short PS3Printer::s_console::__fontHeight = 14;
unsigned short PS3Printer::s_console::__dataLength = 8;
unsigned char* PS3Printer::s_console::__fontDataTable = NULL;
unsigned int*  PS3Printer::s_console::__fontOffsetTable = NULL;
unsigned char* PS3Printer::s_console::__fontIndexTable = NULL;
unsigned char* PS3Printer::s_console::__fontWidthTable = NULL;

PS3Printer::FONTNAME PS3Printer::s_console::__fontName = ARIAL;
int PS3Printer::s_console::__fontSize = 8;
int PS3Printer::s_console::__screenWidth = 640;
int PS3Printer::s_console::__screenHeight = 480;
int PS3Printer::s_console::__curX = 0;
int PS3Printer::s_console::__curY = 0;
int PS3Printer::s_console::__fgColor = 0;
int PS3Printer::s_console::__bgColor = 0;



void PS3Printer::Init(int screenWidth, int screenHeight) {
	sconsole.__screenWidth = screenWidth;
	sconsole.__screenHeight = screenHeight;
	sconsole.__curX = 0;
	sconsole.__curY = 0;
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = FONT_COLOR_BLACK;


	//****** Set Font by default Arial 8 *******//
	sconsole.__fontName = ARIAL;
	sconsole.__fontSize = 8;
	SetDefaultValues();
	//******************************************//
}

void PS3Printer::Init(int screenWidth, int screenHeight, FONTNAME f, int fontSize) {
	sconsole.__screenWidth = screenWidth;
	sconsole.__screenHeight = screenHeight;
	sconsole.__curX = 0;
	sconsole.__curY = 0;
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = FONT_COLOR_BLACK;
	sconsole.__fontSize = fontSize;
	SetFont(f, fontSize);
}



void PS3Printer::SetFontColor(int _bgColor, int _fgColor) {
	sconsole.__bgColor = _bgColor;
	sconsole.__fgColor = _fgColor;
}
void PS3Printer::SetFontColor(int _fgColor) {
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = _fgColor;
}



void PS3Printer::SetFont(PS3Printer::FONTNAME f, int size) {
	sconsole.__fontName = f;
	sconsole.__fontSize = size;
	switch (f) {
	case ARIAL:
		switch(size) {
		case 8:
			//Arial8.h
			SetFontValues(arial8DataLength, arial8CharHeight,
			              arial8Data_table, arial8Offset_table,
			              arial8Index_table, arial8Width_table);
			break;

		default:
			//Error Value is Any text written with the wront
			//Font/size gets replaced with
			//Arial 8 with Yellow Text and red Background
			SetErrorValues();

		}
		break;
	case SEGOESCRIPT:
		switch(size) {
		case 16:
			SetFontValues(segoeScript16DataLength, segoeScript16CharHeight,
			              segoeScript16Data_table, segoeScript16Offset_table,
			              segoeScript16Index_table, segoeScript16Width_table);
			break;
		default:
			//Error Value is Any text written with the wront
			//Font/size gets replaced with
			//Arial 8 with Yellow Text and red Background
			SetErrorValues();
			
		}
	default:
		//ERROR!!
		break;
	}
}
void PS3Printer::SetFontValues(const unsigned short dataLength, const unsigned short fontHeight,
                               const unsigned char* dataTable, const unsigned int* offsetTable,
                               const unsigned char* indexTable, const unsigned char* widthTable) {

	sconsole.__dataLength = dataLength;
	sconsole.__fontHeight = fontHeight;
	sconsole.__fontDataTable = (unsigned char*) dataTable;
	sconsole.__fontOffsetTable = (unsigned int*) offsetTable;
	sconsole.__fontIndexTable = (unsigned char*) indexTable;
	sconsole.__fontWidthTable = (unsigned char*) widthTable;
}

// Default Font Arial 8.
void PS3Printer::SetDefaultValues() {
	SetFontValues(arial8DataLength, arial8CharHeight,
	              arial8Data_table, arial8Offset_table,
	              arial8Index_table, arial8Width_table);
}


// Private function used when someone sets a Font/Size that is not found
// in the Fonts/ folder.
// It draws the text that was supposed to be written in Arial 8 in a red
// background with a yellow font
void PS3Printer::SetErrorValues() {
	SetDefaultValues();	
	// No font was found so use Arial 8 and use
	// yellow text with red background
	sconsole.__bgColor = 0xffffff00;
	sconsole.__fgColor = 0xffff0000;
}

void PS3Printer::SetFontSize(int size) {
	sconsole.__fontSize = size;
	SetFont(sconsole.__fontName, size);
}




/*
  
  _x goes from 0 to 1; where 0 is left and 1 is the right side of your screen.
  _y goes from 0 to 1; where 0 represents the top of the screen and 1 represents the bottom of the screen
  txt is the message you want to display
  buffer represents the console framebuffer

*/
void PS3Printer::Print(float _x, float _y, std::string txt, uint32_t* buffer) {
	int x = (int) (_x * (float)sconsole.__screenWidth);
	int y = (int) (_y * (float)sconsole.__screenHeight);
	sconsole.__curX = x;
	sconsole.__curY = y;
	int oldConsoleX = sconsole.__curX;
	int offsetX = 0;//scans the font bitmap
	int offsetY = 0;//scans the font bitmap
	

	for(unsigned int indexTxt = 0; indexTxt < txt.length(); ++indexTxt) {
		if(txt[indexTxt] == '\n') {
			sconsole.__curX = x;
			sconsole.__curY += sconsole.__fontHeight;
		} else {
			if(txt[indexTxt] < 32 || txt[indexTxt] > 132)
				txt[indexTxt] = 180;

			// Get some char info.
			const unsigned char index = sconsole.__fontIndexTable[(unsigned int) txt[indexTxt]]; //index_table[(unsigned int) txt[indexTxt]];
			const unsigned int offset = sconsole.__fontOffsetTable[index]; //offset_table[index];
			//array of hex codes that represents the bitmap of the letter we are drawing
			const unsigned char *charData = sconsole.__fontDataTable + sizeof(unsigned char) * offset;			
			const unsigned short charWidth = sconsole.__fontWidthTable[index]; //width_table[index];
			
			short bitIndex = sconsole.__dataLength-1;
			for (unsigned short j = 0; j < sconsole.__fontHeight * charWidth; j++) {		     
				std::bitset<sizeof(char) * 8> bitMap(*charData);
				if(bitMap.test(bitIndex)) { //means is part of the fgColor
					if(sconsole.__fgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + offsetY)*sconsole.__screenWidth + sconsole.__curX + offsetX] = sconsole.__fgColor;
				} else {
					if(sconsole.__bgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + offsetY)*sconsole.__screenWidth + sconsole.__curX + offsetX] = sconsole.__bgColor;
				}
				if(bitIndex == 0) {
					bitIndex = 7;
					++charData;
				} else {
					bitIndex--;
				}

				offsetX++;
				if(offsetX == charWidth) {//sconsole.__width
					offsetX = 0;
					offsetY++;
				}

			}
			offsetY = 0;
			sconsole.__curX += charWidth;
		}
	}
	sconsole.__curY += sconsole.__fontHeight;
	sconsole.__curX = oldConsoleX;
}

void PS3Printer::PrintError(float x, float y, std::string errorMsg, uint32_t* buffer) {
	SetDefaultValues();	
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	SetFontColor(FONT_COLOR_RED, FONT_COLOR_YELLOW);//red background, yellow letters
	Print(x, y, errorMsg, buffer);
	SetFontColor(oldbgColor, oldfgColor);
}

void PS3Printer::PrintWarning(float x, float y, std::string warnMsg, uint32_t* buffer) {
	SetDefaultValues();
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	SetFontColor(FONT_COLOR_WHITE, FONT_COLOR_YELLOW);//white background, yellow letters
	Print(x, y, warnMsg, buffer);
	SetFontColor(oldbgColor, oldfgColor);
}
