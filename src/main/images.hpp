#ifndef __Images
#define __Images
#include <SFML/Graphics.hpp>
namespace IMG {
	extern sf::Texture spaceBackground,  //space stars background - ready for use on startup
                        yourShip1;        //your ship's texture, use it wisely :D
	void loadAll(); //loads all textures, avoid calling more than once (don't call it, since it's called in main_main)
}
#endif
