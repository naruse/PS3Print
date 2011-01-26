/*
  PS3Printer:
  Author: Juan Sebastian Munoz (naruse@gmail.com)

  Simple class to help out printing strings on the screen.

  This class is based on scognito's sconsole.
  http://scognito.wordpress.com/2010/11/07/sconsole-a-simple-function-for-printing-strings-on-ps3/

*/

#ifndef _PS3PRINTER_
#define _PS3PRINTER_


#include <string>

//#define FONT_W 8
//#define FONT_H 16

#define FONT_COLOR_NONE   -1
#define FONT_COLOR_BLACK  0x00000000
#define FONT_COLOR_WHITE  0xffffffff
#define FONT_COLOR_RED    0x00ff0000
#define FONT_COLOR_GREEN  0x0000ff00
#define FONT_COLOR_BLUE   0x000000ff
#define FONT_COLOR_YELLOW 0x00ffff00


class PS3Printer {
 private:
	struct s_console {
		static int __fontWidth;		
		static int __fontHeight;
		static int __curX;
		static int __curY;
		static int __fgColor;
		static int __bgColor;
		static int __screenWidth;
		static int __screenHeight;
	};
	static s_console sconsole;
	
	PS3Printer();//dont let anyone create an instance of this class

 public:
	static void Init(int _screenWidth, int _screenHeight, int _fontWidth, int _fontHeight);
	static void Init(int _screenWidth, int _screenHeight);

	
	static void Print(float x, float y, std::string  text, uint32_t* buffer);
	static void PrintError(float x, float y, std::string errorMsg, uint32_t* buffer);
	static void PrintWarning(float x, float y, std::string warnMsg, uint32_t* buffer);

	
	static void SetFontColor(int _bgColor, int _fgColor);
	static void SetFontColor(int _fgColor);

	static void SetFontSize(int _width, int _height);
	
};



#endif
