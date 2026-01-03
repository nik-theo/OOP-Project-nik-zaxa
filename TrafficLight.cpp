#include <iostream>
#include <string>

#include "TrafficLight.h"
#include "WorldObjects.h"
using namespace std;

TrafficLight::TrafficLight(string id, int x, int y): WorldObjects(id, 'R', x, y), state(RED), timer(0) {}

void TrafficLight::update(int current_tick) {
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