
#include <io/pad.h>
#include "VideoInit.h"
#include "Debug.h"



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
	
	
	Debug::Init(vid.GetCurrentBuffer()->width, vid.GetCurrentBuffer()->height);
	
	
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
		Debug::FontColor(FONT_COLOR_BLACK);
		Debug::Log(0.025, 0.025, "This is a normal string!", vid.GetCurrentBuffer()->ptr);

		Debug::FontColor(FONT_COLOR_GREEN);
		Debug::Log(0.25, 0.25, "This is another green string!" + str, vid.GetCurrentBuffer()->ptr);

		Debug::FontColor(0x00ff00ff); //purple
		Debug::Log(0.5, 0.5, "Frame: " + LongToString(frame), vid.GetCurrentBuffer()->ptr);
		
		Debug::LogError(0.75, 0.75, "This is an error message!!!", vid.GetCurrentBuffer()->ptr);
		Debug::LogWarning(0.8, 0.95, "This is a warning message :O", vid.GetCurrentBuffer()->ptr);
		vid.Flip();

	}
	
	return 0;
}
