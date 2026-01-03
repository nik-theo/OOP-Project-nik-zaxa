#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>
#include "StaticObject.h"
using namespace std;

enum Light_state {RED,GREEN,YELLOW};

class TrafficLight : public StaticObject {
    private:
        Light_state state;
        int timer;
    public:
        TrafficLight(string id, Position p) : StaticObject(move(id), 'L', p) {}
        
        
        void update(int current_tick) override {
            timer++;
            if ( state == RED && timer >= 4 ) {
                state = GREEN;
                glyph = 'G';
                timer = 0;
            } else if ( state == GREEN && timer >= 8 ) {
                state = YELLOW;
                glyph = 'Y';
                timer = 0;
            } else if ( state == YELLOW && timer >= 2 ) {
                state = RED;
                glyph = 'R';
                timer = 0;
            }
        }

        Light_state getState() const {
            return state;
        }
};

#endif