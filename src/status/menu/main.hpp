#ifndef __Status_Menu
#define __Status_Menu

//PRE-DEFINE EVERYTHING HERE! This is our index of methods, functions, and classes for StatusMenu.cpp
//Avoid implimenting things here
namespace S_M{
    extern int fade;    //fade value for exiting and such
	void init();        //called before use
	void deinit();      //called after use
	void render();      //logic and object positioning, input and output, read/write
	void display();     //chain of INFO::window.draw() functions, displays the render
	sf::View getView();  //returns the view used by this status (this view MOVES and ROATATES, relative positioning)
}

#endif
