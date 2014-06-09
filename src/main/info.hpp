#ifndef __Main_Info
#define __Main_Info
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


std::string tostr(double);
namespace INFO {
    //this is for the central main loop, determining what needs to be initialized and rendered
	//enums start at 0, therefore MENU has an int value of 0
	enum SuperStatus{
		MENU
	};
	void cleanUp(); //this calls the functions needed to close everything and end the game
	void init();    //called on startup, don't call more than once
	sf::Vector2f angledMove(float, float, double);  //see below
	//translates and fixes object moves at an angle
	//for example, float1 is x and float2 is y, the screen is angled at double
	//so, if the angle is 90 (always to the right) and x=0 and y=-10 (up), the result
	//will be x=10 y=0, since we are angled at 90, it takes into account things you need
	//really just converting two polar vectors (the x and the y magnitude at the correct distance)
	//and adding them together, see source for more explaination
	sf::Vector2f angledMove(sf::Vector2f, double);  //see above
	sf::Vector2f angledMove(sf::Vector2i, double);  //see above
	int round(double);  //legitimately rounds double to nearest integer, rounds up at .5
	sf::Vector2f getPositionFromCenter(sf::Vector2f, double);
	//useful for getting the upper left corner (position) from the center of a window
	//the first argument is the CENTER of the screen, then it returns the POSTIION of the screen (usually a View)
	extern bool running; //true if cleanUp has not yet been called, false if it has - USE THIS instead of something like INFO::window.isOpen(), cause it causes access errors
	extern unsigned long long int renderDate;   //render's completed from startup, we shoot for 60 (equivalent to the FPS, but absolute not rate) (FPS is really a rate at which this increases)
	extern unsigned char setFPS;    //experimental, set to 60 by default but not currently used (we are using monitor-refresh-sync)
	extern sf::Font spaceFont;  //space themed looking font, main and general font to use when you need one
	extern sf::Vector2i mousePos;   //mouse position (relative and absolute values are equivalent in full screen)
	extern sf::Vector2i mouseLastClickPos;  //just what it means, where the mouse was clicked at last
	extern sf::Mouse::Button mouseLastClickButton;  //just what it means, the button that was last used
	extern sf::Vector2i screenDim;  //the screen dimensions of the current monitor that we are able to use
	extern int mouseClicked; //value 0 if no buttons pressed, 1 or higher accounting for every button pressed
	extern int M_M_return; 	//set to 0 as default, but if changed, and the main window closed, then it will return this value - RETURN VALUE FOR CLEANUP
	extern SuperStatus status, lastStatus;  //status and lastStatus, avoid using lastStatus whenever possible
	extern sf::RenderWindow window; //the main window that we will be using, includes important information of size and things
	//below are keys that you should reference for movement checks
	//dont use something like if(Keyboard::isKeyPressed(Keyboard::W)), DO NOT USE THAT
	//do something like if(Keyboard::isKeyPressed(INFO::forwardKey))
	//don't make them absolute, make the relative to these because people play in dvorak sometimes (kenan...)
	extern sf::Keyboard::Key forwardKey, backwardKey, leftturnKey, rightturnKey, rightstrafeKey, leftstrafeKey, zoominKey, zoomoutKey;
	//values in querty            w            s            a            d              e               q            x           c
	extern sf::Keyboard::Key actionKey, sp1Key, sp2Key, sp3Key, sp4Key, sp5Key, sp6Key, sp7Key, sp8Key, sp9Key, sp10Key, sp11Key, sp12Key;
	//values in querty          space      j       k       l       ;       u       i        o       p      m        ,        .        /
}
#endif
