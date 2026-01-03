#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>
#include "WorldObjects.h"
using namespace std;

enum Light_state {
    RED,GREEN,YELLOW
};

class TrafficLight : public WorldObjects {
    private:
        Light_state state;
        int timer;
    public:
        TrafficLight(string id, int x, int y);
        void update(int current_tick) override;

        Light_state getState() const {
            return state;
        }
};

#endif