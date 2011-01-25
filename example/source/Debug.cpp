#include "Debug.h"
#include "console_fonts.h"

//Initialization of the static vars.

int Debug::s_console::__screenWidth = 640;
int Debug::s_console::__screenHeight = 480;
int Debug::s_console::__curX = 0;
int Debug::s_console::__curY = 0;
int Debug::s_console::__fgColor = 0;
int Debug::s_console::__bgColor = 0;


void Debug::Init(int screenWidth, int screenHeight) {
	sconsole.__screenWidth = screenWidth;
	sconsole.__screenHeight = screenHeight;
	sconsole.__curX = 0;
	sconsole.__curY = 0;
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = FONT_COLOR_BLACK;
}

void Debug::FontColor(int _bgColor, int _fgColor) {
	sconsole.__bgColor = _bgColor;
	sconsole.__fgColor = _fgColor;
}
void Debug::FontColor(int _fgColor) {
	sconsole.__bgColor = FONT_COLOR_NONE;
	sconsole.__fgColor = _fgColor;
}


void Debug::LogError(float x, float y, std::string errorMsg, uint32_t* buffer) {
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	FontColor(FONT_COLOR_RED, FONT_COLOR_YELLOW);//red background, yellow letters
	Log(x, y, errorMsg, buffer);
	FontColor(oldbgColor, oldfgColor);
}

void Debug::LogWarning(float x, float y, std::string warnMsg, uint32_t* buffer) {
	int oldbgColor = sconsole.__bgColor;
	int oldfgColor = sconsole.__fgColor;
	FontColor(FONT_COLOR_WHITE, FONT_COLOR_YELLOW);//white background, yellow letters
	Log(x, y, warnMsg, buffer);
	FontColor(oldbgColor, oldfgColor);
}
/*
  
  _x goes from 0 to 1; where 0 is left and 1 is the right side of your screen.
  _y goes from 0 to 1; where 0 represents the top of the screen and 1 represents the bottom of the screen
  txt is the message you want to display
  buffer represents the console framebuffer

*/
void Debug::Log(float _x, float _y, std::string txt, uint32_t* buffer) {

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
			sconsole.__curY += FONT_H;
		} else {
			if(txt[i] < 32 || txt[i] > 132)
				txt[i] = 180;
			
			for(int j = 0; j < FONT_W * FONT_H; j++) {
				if(consoleFont[-32+txt[i]][j] == 0) {
					if(sconsole.__bgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + tempy) * sconsole.__screenWidth + sconsole.__curX + tempx] = sconsole.__bgColor;
				} else{
					if(sconsole.__fgColor != FONT_COLOR_NONE)
						buffer[(sconsole.__curY + tempy) * sconsole.__screenWidth + sconsole.__curX + tempx] = sconsole.__fgColor;
				}
				tempx++;
				if(tempx == FONT_W) {
					tempx = 0;
					tempy++;
				}
			}
			tempy = 0;
			sconsole.__curX += FONT_W;
		}
	}
	sconsole.__curY += FONT_H;
	sconsole.__curX = oldConsoleX;
}

