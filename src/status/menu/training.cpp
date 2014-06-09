#include "training.hpp"
#include "text.hpp"
#include "../../main/info.hpp"
#include <SFML/System.hpp>
#include "../../main/log.hpp"
#include "main.hpp"

using namespace sf;

namespace S_M_T {
    const Time DISAPPEAR_DELAY=seconds(5), REAPPEAR_DELAY=seconds(2);
    bool thisRunning, changing;
    Clock clock;
    long long int date;
    unsigned char round;
    S_M::ScreenText movementMessage, escapeMessage, spaceBarMessage;
    void init() {
        thisRunning=true;
        round=0;
        changing=false;
        date=0;
        if(INFO::backwardKey == Keyboard::O) {
            movementMessage.set("Use ',.aoe to Control Thrust", Vector2f(0,0), Color::White, &INFO::spaceFont, 24, 10, true, 0);
        } else {
            movementMessage.set("Use qweasd to Control Thrust", Vector2f(0,0), Color::White, &INFO::spaceFont, 24, 10, true, 0);
        }
        escapeMessage.set("Hold Escape to Enter Sortie Vortex", Vector2f(0, 0), Color::White, &INFO::spaceFont, 24, 10, false, 0);
        spaceBarMessage.set("Hold Spacebar to Stabilize", Vector2f(0,0), Color::White, &INFO::spaceFont, 24, 10, false, 0);   //see the text class for explaination
    }
    void render(bool ending) {
        if(!thisRunning) return;
        switch(round) {
            case 0:
                if(ending) {
                    movementMessage.setFade(0);
                } else if(changing) {
                    if(clock.getElapsedTime()>DISAPPEAR_DELAY && clock.getElapsedTime()<(DISAPPEAR_DELAY+REAPPEAR_DELAY)) {
                        movementMessage.active=false;
                    } else if (clock.getElapsedTime()>(DISAPPEAR_DELAY+REAPPEAR_DELAY)) {
                        changing=false;
                        spaceBarMessage.active=true;
                        round++;
                    }
                } else {
                    if(Keyboard::isKeyPressed(INFO::leftstrafeKey)) {
                        changing=true;
                        clock.restart();
                    }
                }
                break;
            case 1:
                if(ending) {
                    spaceBarMessage.setFade(0);
                } else if(changing) {
                    if(clock.getElapsedTime()>DISAPPEAR_DELAY && clock.getElapsedTime()<(DISAPPEAR_DELAY+REAPPEAR_DELAY)) {
                        spaceBarMessage.active=false;
                    } else if (clock.getElapsedTime()>(DISAPPEAR_DELAY+REAPPEAR_DELAY)) {
                        changing=false;
                        escapeMessage.active=true;
                        round++;
                    }
                } else {
                    if(Keyboard::isKeyPressed(INFO::actionKey)) {
                        changing=true;
                        clock.restart();
                    }
                }
                break;
            case 2:
                if(ending) {
                    escapeMessage.setFade(0);
                    escapeMessage.active=false;
                    round++;
                }
                break;
            case 3:
                thisRunning=false;
                break;
        }
    }
    void display(bool ending) {
        if(!thisRunning || ending) return;
        switch(round) {
            case 0:
                INFO::window.draw(movementMessage.draw());
                break;
            case 1:
                INFO::window.draw(spaceBarMessage.draw());
                break;
            case 2:
                INFO::window.draw(escapeMessage.draw());
                break;
            case 3:
                thisRunning=false;
                break;
        }
    }
}
