
#include <io/pad.h>
#include "VideoInit.h"
#include "PS3Printer.h"



#include <string>
#include "utilities.h"
using namespace::std;





s32 main(int argc, const char* argv[]) {
	
	PadInfo padinfo;
	PadData paddata;
	int i;
	VideoInit vid = VideoInit();//Initializes the screen

	ioPadInit(7);

	string str = "";

	long frame = 0; // To keep track of how many frames we have rendered.

	int incr = 0;
	
	
	PS3Printer::Init(vid.GetCurrentBuffer()->width, vid.GetCurrentBuffer()->height);
	
	
	// Ok, everything is setup. Now for the main loop.
	while(1){
		// Check the pads.
		ioPadGetInfo(&padinfo);
		for(i=0; i<MAX_PADS; i++){
			if(padinfo.status[i]){
				ioPadGetData(i, &paddata);
				
				if(paddata.BTN_CROSS){
					incr ++;
					str = "X: " + IntToString(incr);
				}
			}
		}
		
	
		        
		vid.WaitFlip();
		vid.DrawBackground(0xffffffff);//White background (format AARRGGBB)
		frame++;
		PS3Printer::SetFont(PS3Printer::ARIAL, 8);
		PS3Printer::SetFontColor(FONT_COLOR_BLACK);
		PS3Printer::Print(0.025, 0.025, "This is a normal string!", vid.GetCurrentBuffer()->ptr);

		PS3Printer::SetFontColor(FONT_COLOR_GREEN);
		PS3Printer::SetFont(PS3Printer::SEGOESCRIPT, 16);
		PS3Printer::Print(0.25, 0.25, "This is another green string!" + str, vid.GetCurrentBuffer()->ptr);

		
		PS3Printer::SetFontColor(0x00ff00ff); //purple
		PS3Printer::SetFont(PS3Printer::SEGOESCRIPT, 16);
		PS3Printer::Print(0.5, 0.5, "Frame: " + LongToString(frame), vid.GetCurrentBuffer()->ptr);

		PS3Printer::SetFontColor(FONT_COLOR_BLACK);
		PS3Printer::SetFont(PS3Printer::ARIAL, 8);
		PS3Printer::Print(0.5, 0.6, "This is an Arial 8 test!", vid.GetCurrentBuffer()->ptr);
		                    
		
		PS3Printer::PrintError(0.75, 0.75, "This is an error message!!!", vid.GetCurrentBuffer()->ptr);
		PS3Printer::PrintWarning(0.8, 0.95, "This is a warning message :O", vid.GetCurrentBuffer()->ptr);
		vid.Flip();

	}
	
	return 0;
}
