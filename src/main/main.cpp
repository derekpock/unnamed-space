#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
#include "info.hpp"
#include "../status/menu/main.hpp"
#include "log.hpp"
#include "images.hpp"
using namespace sf; //anything in the sf include do not need its sf:: prestatment, anything in std, however, will need its std:: prestatement (ie std::cout and std::endl)

int main() {
	OUT::init();
	debug("Initializing...");
	INFO::init();
	INFO::status = INFO::MENU;
	INFO::lastStatus = INFO::MENU;
	bool initialResizeHappened=false;   //the window throws a resize for the opening of the window, we don't want to warn the user about that
	debug("Getting video modes...");
	std::vector<VideoMode> modes = VideoMode::getFullscreenModes(); //gets all possible video modes supported
	unsigned int maxWidth=0, maxHeight=0;
	for (unsigned int i = 0; i < modes.size(); ++i){    //for all of the modes
		VideoMode mode = modes[i];
		if((mode.width * mode.height)>(maxWidth * maxHeight)) { //if it gives more resolution than the previous one
			maxWidth = mode.width;      //make it our new resolution
			maxHeight = mode.height;
		}
	}   //we basically just found the highest resolution the computer can handle
	if(modes.size() < 1 ) {
		crit("No video modes! Reverting to default 800x600!");
		maxWidth=800;
		maxHeight=600;
	}
	/*
        You may be asking "Why not use fullscreen mode?" Well it does not work! That's why...OpenGL issues break linux even after the program runs.
	*/
	INFO::screenDim.x = maxWidth;
	INFO::screenDim.y = maxHeight;
	debug("Creating window with resolution of " + tostr(maxWidth) + "x" + tostr(maxHeight) + "...");
	INFO::window.create(VideoMode(maxWidth, maxHeight), "Unnamed Space", Style::None);
	//INFO::window.setFramerateLimit(INFO::setFPS); //DONT USE BOTH THIS AND BELOW
	INFO::window.setVerticalSyncEnabled(true);  //use one or the other, this one sets FPS to the refresh rate of the screen (so as fast as would be effective, usually 60)
	IMG::loadAll();
	debug("Initializing Status_Menu...");
	S_M::init();
	debug("Starting window render loop...");
	Clock clock;
	log("Initialization Finished");
	while (INFO::window.isOpen()) {
	    INFO::renderDate++;
	    Event event;
		while (INFO::window.pollEvent(event)){  //this gives us our next event, and may sleep to preserve framerate
			switch(event.type) {
			case Event::Closed: //signal catch
				INFO::cleanUp();
				break;
			case Event::Resized:    //we NEVER want to be resized except for the first time
				if(initialResizeHappened) warn("Window resized! Container does not adjust to window resizing!");
				else initialResizeHappened=false;
				break;
			case Event::TextEntered:    //dont use...please
				//event.text.unicode
				//static_cast<char>(event.text.unicode)
				break;
			case Event::KeyPressed: //nor this, unless you really need to
				break;
			case Event::KeyReleased:    //...
				//same as above
				break;
			case Event::MouseWheelMoved:    //use this if you want to impliment anything with it; there is no getMouseWheelDistance, it's relative positioning with ticks and distance moved
				//event.mouseWheel.delta;
				break;
			case Event::MouseMoved:
				INFO::mousePos.x = event.mouseMove.x;
				INFO::mousePos.y = event.mouseMove.y;
				break;
			case Event::MouseButtonPressed:
				INFO::mouseLastClickButton = event.mouseButton.button;
				INFO::mouseLastClickPos.x = event.mouseButton.x;
				INFO::mouseLastClickPos.y = event.mouseButton.y;
				INFO::mouseClicked++;   //wait for it
				break;
			case Event::MouseButtonReleased:
				INFO::mouseClicked--;   //see what i did there?
				if(INFO::mouseClicked<0) {  //sometimes it happens, but we can't catch it if it's too high...
					warn("mouseClicked status lower than 0!");
					INFO::mouseClicked=0;
				}
				break;
            default:
                break;
			}
		}
		//starts render loop, the real game loop, but the previous is in the loop too
		INFO::window.clear();   //clear everything
		switch(INFO::status) {
			case INFO::MENU:
                S_M::render();  //we split these because it may help us if we ever impliment threading
				S_M::display();
				break;
			default:
				crit("Status void! Aborting!"); //called if we forget a case above
				INFO::cleanUp();
				break;
		}
		INFO::window.display(); //show us the result
		if(INFO::renderDate%100==0) {   //every 100 loops
            int FPS = INFO::round((1 / clock.restart().asSeconds())*100);   //get the FPS
            debug("FPS: " + tostr(FPS));   //let the debug know
		}
	}
	//at this point we are finished with the game since we are out of the render loop
	log("Done.");
	return INFO::M_M_return;
}
