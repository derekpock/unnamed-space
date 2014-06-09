#include "images.hpp"
#include <SFML/Graphics.hpp>
#include "log.hpp"
#include "info.hpp"

/*
    Load ALL Images Here! loadAll is called on startup.
    We do this because somewhere else you may use the same image, and we don't want to create new textures, it's taxing.
    Be sure to make them all Textures, not Images, we will rarely use raw images
*/

using namespace sf;
namespace IMG {
	Texture spaceBackground;
	Texture yourShip1;
	//call if you error ever
	void panic() {
		crit("Cannot load image from file! Aborting!");
		INFO::cleanUp();    //the global panic button
	}
	void loadAll() {    //called upon startup
		debug("Loading images...");
		if(!spaceBackground.loadFromFile("resources/backgrounds/background3.jpg")) panic();   //# can be 1, 2, 3, 4
		spaceBackground.setRepeated(true);  //this "tiles" the texture, only do this if you want it to repeat until it fills it's sprite's size
        if(!yourShip1.loadFromFile("resources/ships/topdownfighter.png")) panic();
	}
}
