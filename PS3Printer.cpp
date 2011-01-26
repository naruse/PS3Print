#include "PS3Printer.h"
#include "console_fonts.h"

//Initialization of the static vars.

int PS3Printer::s_console::__screenWidth = 640;
int PS3Printer::s_console::__screenHeight = 480;
int PS3Printer::s_console::__curX = 0;
int PS3Printer::s_console::__curY = 0;
int PS3Printer::s_console::__fgColor = 0;
int PS3Printer::s_console::__bgColor = 0;
int PS3Printer::s_console::__fontWidth = 8;
int PS3Printer::s_console::__fontHeight = 16;



void PS3Printer::Init(int screenWidth, int screenHeight) {
	sconsole.__screenWidth = screenWidth;
	sconsole.__screenHeight = screenHeight;
	sconsole.__curX = 0;
	sconsole.__curY = 0;
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = FONT_COLOR_BLACK;
	sconsole.__fontWidth = 8;
	sconsole.__fontHeight = 16;
}

void PS3Printer::Init(int screenWidth, int screenHeight, int fontWidth, int fontHeight) {
	sconsole.__screenWidth = screenWidth;
	sconsole.__screenHeight = screenHeight;
	sconsole.__curX = 0;
	sconsole.__curY = 0;
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = FONT_COLOR_BLACK;
	sconsole.__fontWidth = fontWidth;
	sconsole.__fontHeight = fontHeight;
}



void PS3Printer::SetFontColor(int _bgColor, int _fgColor) {
	sconsole.__bgColor = _bgColor;
	sconsole.__fgColor = _fgColor;
}
void PS3Printer::SetFontColor(int _fgColor) {
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = _fgColor;
}




  void PS3Printer::SetFontSize(int width, int height) {
	  /*
	    In progress	...
	    sconsole.__fontWidth = width;
	    sconsole.__fontH	eight = height;
	  */
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
	
	int tempx = 0;//scans the font bitmap
	int tempy = 0;
	int oldConsoleX = sconsole.__curX;
	sconsole.__curX = x;
	sconsole.__curY = y;

	for(int i = 0; i < (int)txt.length(); ++i) {
		if(txt[i] == '\n') {
			sconsole.__curX = x;
			sconsole.__curY += sconsole.__fontHeight;
		} else {
			if(txt[i] < 32 || txt[i] > 132)
				txt[i] = 180;
			
			for(int j = 0; j < sconsole.__fontWidth * sconsole.__fontHeight; j++) {
				if(consoleFont[-32+txt[i]][j] == 0) {
					if(sconsole.__bgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + tempy) * sconsole.__screenWidth + sconsole.__curX + tempx] = sconsole.__bgColor;
				} else{
					if(sconsole.__fgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + tempy) * sconsole.__screenWidth + sconsole.__curX + tempx] = sconsole.__fgColor;
				}
				tempx++;
				if(tempx == sconsole.__fontWidth) {
					tempx = 0;
					tempy++;
				}
			}
			tempy = 0;
			sconsole.__curX += sconsole.__fontWidth;
		}
	}
	sconsole.__curY += sconsole.__fontHeight;
	sconsole.__curX = oldConsoleX;
}

void PS3Printer::PrintError(float x, float y, std::string errorMsg, uint32_t* buffer) {
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	SetFontColor(FONT_COLOR_RED, FONT_COLOR_YELLOW);//red background, yellow letters
	Print(x, y, errorMsg, buffer);
	SetFontColor(oldbgColor, oldfgColor);
}

void PS3Printer::PrintWarning(float x, float y, std::string warnMsg, uint32_t* buffer) {
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	SetFontColor(FONT_COLOR_WHITE, FONT_COLOR_YELLOW);//white background, yellow letters
	Print(x, y, warnMsg, buffer);
	SetFontColor(oldbgColor, oldfgColor);
}
