#ifndef __ScreenText
#define __ScreenText
#include <SFML/Graphics.hpp>

namespace S_M {
    class ScreenText {
        public:
            const sf::Text draw();      //returns the text to draw, only use inside a draw function of a window or renderedtexture
            void set(const char[], sf::Vector2f, sf::Color, sf::Font*, int, double, bool, unsigned char);    //set multiple things at once
            //            text         position      color  font pointer  size faderate active startShown
            void setText(const char[]); //set text to display
            void setPosition(sf::Vector2f); //set position RELATIVE to the menuView in status_menu
            void setColor(sf::Color);
            void setFont(sf::Font*);    //this is a POINTER to a font, usually do setFont(&INFO::spaceFont)
            void setSize(int);      //set size in PIXELS of font
            void setFadeRate(double);   //set faderate in VALUES (255) PER RENDER
            void setFade(unsigned char);  //set fade in color values, 255 is opaque, 0 is transparent
            const sf::Text getTextItem();     //returns an unchangable copy of the text, not yet drawn preped
            bool active;            //determines if the font should fade or display (using fadeRate)
        private:
            sf::Text text;
            sf::Vector2f pos;
            double fadeRate;
            bool fadeSet;
            signed short currentFade;
    };
}

#endif
