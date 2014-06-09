#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include "info.hpp"
#include "log.hpp"
#include <fstream>
#include <cmath>

using namespace sf;

std::string tostr(double num) {
    std::ostringstream os;
    os << num;
    return os.str();
}

namespace INFO{
	void loadQwerty(); void loadDvorak();
	unsigned long long int renderDate;
	unsigned char setFPS;
    bool running;
    Font spaceFont;
	int mouseClicked=0;
	int M_M_return=0;
	Vector2i mousePos;
	Vector2i mouseLastClickPos;
	Mouse::Button mouseLastClickButton;
	Vector2i screenDim;
	SuperStatus status, lastStatus;
	RenderWindow window;
	Keyboard::Key forwardKey, backwardKey, leftturnKey, rightturnKey, rightstrafeKey, leftstrafeKey, zoominKey, zoomoutKey;
	//                w            s            a            d              e               q            x           c
	Keyboard::Key actionKey, sp1Key, sp2Key, sp3Key, sp4Key, sp5Key, sp6Key, sp7Key, sp8Key, sp9Key, sp10Key, sp11Key, sp12Key;
	//              space      j       k       l       ;       u       i        o       p      m        ,        .        /
	//   need more keys? really? fine...ask Derek first

	void cleanUp() {
		log("Cleaning up...");
		running=false;
		OUT::deinit();

		//ENTER NEW STUFF ABOVE HERE

		window.close();	//final end, because of above, when window is closed, the main will move on and return as 0
    }
	void init() {
	    debug("Initializing Main_Info...");
	    running=true;
	    renderDate=0;
	    setFPS=60;  //dont change unless you know what you are doing!
	    bool keyboardFound=false;   //for the options file, to see if we have set it or not (defaults to querty)
        std::ifstream confFile;
        confFile.open("options");   //always from the working directory
        std::string line="";
        while(confFile.good()) {    //read until we can't
            std::getline(confFile, line);   //getline from confFile to line
            if(line.compare("qwerty")==0) { //no \n needed (it knows)
                keyboardFound = true;
                debug("Found querty option.");
                loadQwerty(); //load the querty keyboard options
            } else if(line.compare("dvorak")==0) {
                keyboardFound = true;
                debug("Found dvorak option.");
                loadDvorak();   //load the dvorak keyboar options
            } else {
                if(confFile.good()) warn("Blank line in options file. Ignoring.");  //the if-we-are-still-good is important for checking if its just the last line in the file or not (sometimes happens)
            }
        }
        confFile.close();
        if(!keyboardFound) {
            warn("No keyboard option specified. Assuming qwerty."); //yes, we assume most people use qwerty
            loadQwerty();
        }

        if(!spaceFont.loadFromFile("resources/moonhouse.ttf")) {
            crit("Unable to find moonhouse.ttf font! Aborting!");
            cleanUp();
        }
	}
	void loadQwerty() {
        forwardKey = Keyboard::W;
        backwardKey = Keyboard::S;
        leftturnKey = Keyboard::A;
        rightturnKey = Keyboard::D;
        rightstrafeKey = Keyboard::E;
        leftstrafeKey = Keyboard::Q;
        zoominKey = Keyboard::X;
        zoomoutKey = Keyboard::C;
        actionKey = Keyboard::Space;
        sp1Key = Keyboard::J;
        sp2Key = Keyboard::K;
        sp3Key = Keyboard::L;
        sp4Key = Keyboard::SemiColon;
        sp5Key = Keyboard::U;
        sp6Key = Keyboard::I;
        sp7Key = Keyboard::O;
        sp8Key = Keyboard::P;
        sp9Key = Keyboard::M;
        sp10Key = Keyboard::Comma;
        sp11Key = Keyboard::Period;
        sp12Key = Keyboard::Slash;// /
	}
	void loadDvorak() {
        forwardKey = Keyboard::Comma;
        backwardKey = Keyboard::O;
        leftturnKey = Keyboard::A;
        rightturnKey = Keyboard::E;
        rightstrafeKey = Keyboard::Period;
        leftstrafeKey = Keyboard::Quote;
        zoominKey = Keyboard::Q;
        zoomoutKey = Keyboard::J;
        actionKey = Keyboard::Space;
        sp1Key = Keyboard::H;
        sp2Key = Keyboard::T;
        sp3Key = Keyboard::N;
        sp4Key = Keyboard::S;
        sp5Key = Keyboard::G;
        sp6Key = Keyboard::C;
        sp7Key = Keyboard::R;
        sp8Key = Keyboard::L;
        sp9Key = Keyboard::M;
        sp10Key = Keyboard::W;
        sp11Key = Keyboard::V;
        sp12Key = Keyboard::Z;
	}
	int round(double x) {
		int floorx = (int) x;
		if((x-floorx)>=0.5) return floorx+1;
		else return floorx;
	}
	Vector2f getPositionFromCenter(Vector2f center, double angleDeg) {
        return (center - angledMove(screenDim, angleDeg));
	}
	Vector2f angledMove(Vector2f vector, double angleDeg) { return angledMove(vector.x, vector.y, angleDeg); }
	Vector2f angledMove(Vector2i vector, double angleDeg) { return angledMove(vector.x, vector.y, angleDeg); }
	Vector2f angledMove(float xDistMove, float yDistMove, double angleDeg) {
	    /*
            This took a bit of math for me to figure out, please don't waste my time by undoing anything.
            This adds two vectors that need rotated a certain angle, this is most commonly used when
            the user turns the ship, the view then changes, and the ship needs to head the direction their
            facing, not the absolute N or whatever direction, for example, if I am heading up north (negative y)
            at 10 speed, then I turn to the right, I need to logically go 10 speed east (postive x) instead of
            still going north (or left, as the user would see it). It's helpful.
            It deals with creating polar coords for x's magnitude at the specified angle, and y's magnitude at the
            specified angle. Positive and negative values (and y's orientation) taken into account (note the
            +180, +90, and +270 at certain places).

            Objections? ask me for explaination before you explode this area...
            BTW...
                                        N               -y
                                    W       E       -x      +x
                                        S               +y
	    */
	    float xx, xy, yx, yy;
        if(xDistMove > 0){  //involves x's x and x's y from the angle
            xx = (xDistMove * std::cos((angleDeg) * 3.1415926535 / 180));
            xy = (xDistMove * std::sin((angleDeg) * 3.1415926535 / 180));
        } else {
            xx = ((-xDistMove) * std::cos((angleDeg + 180) * 3.1415926535 / 180));
            xy = ((-xDistMove) * std::sin((angleDeg + 180) * 3.1415926535 / 180));
        }
        if(yDistMove > 0) { //involves y's x and y's y from the angle
            yy = (yDistMove * std::sin((angleDeg + 90) * 3.1415926535 / 180));
            yx = (yDistMove * std::cos((angleDeg + 90) * 3.1415926535 / 180));
        } else {
            yy = ((-yDistMove) * std::sin((angleDeg + 270) * 3.1415926535 / 180));
            yx = ((-yDistMove) * std::cos((angleDeg + 270) * 3.1415926535 / 180));
        }
        return Vector2f(xx + yx, yy + xy);  //combine them and return
	}
}

