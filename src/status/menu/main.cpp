#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "main.hpp"
#include "../../main/images.hpp"
#include "../../main/info.hpp"
#include "../../main/log.hpp"
#include "text.hpp"
#include "training.hpp"

using namespace sf; //anything in the sf include do not need its sf:: prestatment, anything in std, however, will need its std:: prestatement (ie std::cout and std::endl)

namespace S_M{
    int endingRound, fade;
	Sprite spaceBackground, playerShip;
	RectangleShape whiteBackground;
	View menuView;
	Vector2f backgroundMomentum;
	float backgroundSpinMomentum;
	View getView() {
        return menuView;
	}
	void init() {   //initialize everything here, called before we use it
		playerShip.setTexture(IMG::yourShip1, false);
		playerShip.setScale(0.1,0.1);
		playerShip.setOrigin(playerShip.getLocalBounds().width/2, playerShip.getLocalBounds().height/2);
		spaceBackground.setTexture(IMG::spaceBackground, true);
		spaceBackground.setTextureRect(sf::IntRect(0, 0, INFO::screenDim.x*5, INFO::screenDim.y*5));    //background is actually 5x5 times larger than the actual screen (incase the correction is slow)
        menuView.reset(FloatRect(0, 0, INFO::screenDim.x, INFO::screenDim.y));  //real init of menuView
        INFO::window.setView(menuView);
        backgroundMomentum.x = 0;
        backgroundMomentum.y = 0;
        backgroundSpinMomentum = 450;   //we start spinning
        endingRound=60; //we start coming out of the game
        whiteBackground.setSize(Vector2f(INFO::screenDim.x*5, INFO::screenDim.y*5));
        whiteBackground.setFillColor(Color(0, 0, 0));
        S_M_T::init();
    }
	void deinit() { //cleanup everything here, called after we are done with it
        menuView.reset(FloatRect(0, 0, INFO::screenDim.x, INFO::screenDim.y));
        backgroundMomentum.x = 0;
        backgroundMomentum.y = 0;
        backgroundSpinMomentum = 0;
        endingRound=59;
	}
	void render(){  //the logic of the gameplay
	    if(Keyboard::isKeyPressed(Keyboard::Escape)) {
	        endingRound++;
            backgroundMomentum.x=10 * endingRound;
            backgroundMomentum.y=10 * endingRound;  //increase the speed that we are leaving
            backgroundSpinMomentum=5 * endingRound;
            if(endingRound>50) {
                INFO::cleanUp();
            }
	    } else if(endingRound>=10) {
	        endingRound-=1;
            backgroundMomentum.x=10 * endingRound;  //if we are above 10 but not increasing, slow down
            backgroundMomentum.y=10 * endingRound;
        } else {    //the control reading
            if(Keyboard::isKeyPressed(INFO::forwardKey)) backgroundMomentum.y--;
            if(Keyboard::isKeyPressed(INFO::backwardKey)) backgroundMomentum.y++;
            if(Keyboard::isKeyPressed(INFO::leftstrafeKey)) backgroundMomentum.x--;
            if(Keyboard::isKeyPressed(INFO::rightstrafeKey)) backgroundMomentum.x++;
            if(Keyboard::isKeyPressed(INFO::rightturnKey)) backgroundSpinMomentum+=0.2;
            if(Keyboard::isKeyPressed(INFO::leftturnKey)) backgroundSpinMomentum-=0.2;
	    }
	    if(endingRound<10 && !Keyboard::isKeyPressed(Keyboard::Escape) && endingRound!=-1) {    //if we are below 10 and not increasing and we are not finished with the ending
            if((endingRound==0 && (menuView.getRotation()>=359 || menuView.getRotation()<=1)) || Keyboard::isKeyPressed(INFO::leftturnKey) || Keyboard::isKeyPressed(INFO::rightturnKey)) endingRound-=1;
            //if we are almost finished (but not yet, -1 is finished) and the menuview is within 1 degree of rotation at 0 (still) (so that we still move a bit, but not too much)
            //OR if the user is trying to spin the ship, end the endingRounds lock
	        else if (endingRound>0) endingRound-=1; //otherwise, if we are not ALMOST finished, lower the endingRound
            backgroundSpinMomentum/=1.2;
            //the next code just slows the rotation rate of the view (from the ending round)
            if(menuView.getRotation()>180) menuView.rotate((360-menuView.getRotation()) - ((360-menuView.getRotation())/1.2));
            else menuView.setRotation(menuView.getRotation()/1.2);
            if(endingRound>=2) {
                backgroundMomentum.x=10 * (endingRound-2);
                backgroundMomentum.y=10 * (endingRound-2);
            }
	    }
	    /*
	    while(backgroundMomentum.x>(int)(IMG::spaceBackground.getSize().x)) backgroundMomentum.x=(int)(IMG::spaceBackground.getSize().x);
        while(backgroundMomentum.x<-(int)(IMG::spaceBackground.getSize().x)) backgroundMomentum.x=-(int)(IMG::spaceBackground.getSize().x);
        while(backgroundMomentum.y>(int)(IMG::spaceBackground.getSize().y)) backgroundMomentum.y=(int)(IMG::spaceBackground.getSize().y);
        while(backgroundMomentum.y<-(int)(IMG::spaceBackground.getSize().y)) backgroundMomentum.y=-(int)(IMG::spaceBackground.getSize().y);
        //*/
        int x, y;
        //get the current magnetude of the x and y momentum
        if(backgroundMomentum.x>0) x=backgroundMomentum.x; else x=-backgroundMomentum.x;
        if(backgroundMomentum.y>0) y=backgroundMomentum.y; else y=-backgroundMomentum.y;
        if(Keyboard::isKeyPressed(INFO::actionKey)) {   //if the user breaks
            backgroundMomentum.x/=1.2;  //make the speed 5/6 of what is was
            backgroundMomentum.y/=1.2;
            backgroundSpinMomentum/=1.2;    //and the spin
            //this finals the spin in the correct direction (note the if on which side of the 180 we are at...)
            if(menuView.getRotation()>180) menuView.rotate((360-menuView.getRotation()) - ((360-menuView.getRotation())/1.2));
            else menuView.setRotation(menuView.getRotation()/1.2);
        }
        //get the larger one, whichever one is faster, that determines the fade
        (x>y) ? fade=x-(int)(IMG::spaceBackground.getSize().x-800) : fade=y-(int)(IMG::spaceBackground.getSize().y-800);
        if(fade<0) fade=0;  //always positive
        if(fade>255) fade=255;  //never greater than 255
        spaceBackground.setColor(Color(255, 255, 255, 255-fade));   //...beacuse of this

        S_M_T::render(endingRound!=-1);

        //the actual rotation and movement of the view
        menuView.rotate(backgroundSpinMomentum);
        menuView.move(INFO::angledMove(backgroundMomentum, menuView.getRotation()));
        //this moves the screen to the EXACT duplicate location closer to the middle of the screen, so we don't need a huge background, just one 4x as big
	    //this will not be needed on the games, since we have a map, its not infinite (well...maybe...)
        while(menuView.getCenter().x<INFO::screenDim.x*1.5) menuView.move(((float)IMG::spaceBackground.getSize().x), 0);
        while(menuView.getCenter().x>(INFO::screenDim.x*2.5)) menuView.move((-1 * (float)IMG::spaceBackground.getSize().x), 0);
	    while(menuView.getCenter().y<INFO::screenDim.y*1.5) menuView.move(0, ((float)IMG::spaceBackground.getSize().y));
	    while(menuView.getCenter().y>(INFO::screenDim.y*2.5)) menuView.move(0, (-1 * (float)IMG::spaceBackground.getSize().y));
	    //render everything else after here, as to get the correct menuView information
        playerShip.setRotation(menuView.getRotation()-90);
        playerShip.setPosition(menuView.getCenter());
	}
	void display(){
        INFO::window.draw(whiteBackground); //this is behind...
        INFO::window.draw(spaceBackground); //...this, so when this is invisible, that shows
        INFO::window.draw(playerShip);
        INFO::window.setView(menuView);
        S_M_T::display(endingRound!=-1);
    }
}
