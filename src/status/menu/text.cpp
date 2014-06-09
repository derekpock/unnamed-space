#include <SFML/Graphics.hpp>
#include "text.hpp"
#include "../../main/info.hpp"
#include "main.hpp"
#include "../../main/log.hpp"

using namespace sf;

const Text S_M::ScreenText::draw() {
    //makes sure this text is staying within the window, that means the position we give it is relative to the window view
    //this is JUST for the status menu, remember!
    //the text.getCharacterSize()/2 is the spacing in x that it needs to be readable, that/2 is just an estimate, but it looks good so far
    text.setPosition(S_M::getView().getCenter());
    text.setRotation(S_M::getView().getRotation());
    Color color = text.getColor();
    //this fades the text out, these rates may be changed to fit it most neededly, maybe have a function to change them if needed
    if(fadeSet) {
        fadeSet=false;
        return text;
    }
    if(active && currentFade<255) {
        currentFade+=fadeRate;
        if(currentFade>255) currentFade=255;
    } else if (!active && currentFade>0) {
        if(currentFade>=fadeRate) currentFade-=fadeRate;
        else currentFade=0;
    }
    signed short newFade = currentFade - S_M::fade;
    if(newFade<0) newFade=0;
    color.a = newFade;
    text.setColor(color);
    return text;
}
void S_M::ScreenText::set(const char newtext[], Vector2f newpos, Color color, Font* font, int size, double newFadeRate, bool enabled, unsigned char fade) {
    text.setString(newtext);
    text.setColor(color);
    text.setFont(*font);
    text.setCharacterSize(size);
    pos = newpos;
    fadeRate = newFadeRate;
    active = enabled;
    Color color2 = text.getColor();
    color2.a = fade;
    currentFade = fade;
    text.setColor(color2);
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    fadeSet=true;
}
const Text S_M::ScreenText::getTextItem() {
    const Text temp = text;
    return temp;
}
void S_M::ScreenText::setFade(unsigned char fade) {
    Color color = text.getColor();
    color.a = fade;
    currentFade = fade;
    text.setColor(color);
    fadeSet=true;
}
void S_M::ScreenText::setSize(int size) {
    text.setCharacterSize(size);
}
void S_M::ScreenText::setColor(Color color) {
    text.setColor(color);
}
void S_M::ScreenText::setFont(Font* font) {
    text.setFont(*font);
}
void S_M::ScreenText::setFadeRate(double newFadeRate) {
    fadeRate = newFadeRate;
}
void S_M::ScreenText::setPosition(Vector2f newpos) {
    pos = newpos;
}
void S_M::ScreenText::setText(const char newtext[]) {
    text.setString(newtext);
}
