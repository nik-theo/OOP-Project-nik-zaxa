#ifndef MOVINGOBJECTS_H
#define MOVINGOBJECTS_H

#include "WorldObjects.h"

#include <string>
using namespace std;


enum Direction {
    SOUTH, WEST, NORTH, EAST
};

class MovingObject : public WorldObjects {
    protected:
        int speed;
        Direction dir;
    public:
        MovingObject(string id, int x, int y, int s, Direction d) : WorldObjects(id, g, x, y), x(x), y(y), speed(s), dir(d) {}

        void update(int current_tick) override {
            if ( dir == NORTH ) {
                y += speed;
            } else if ( dir == WEST ) {
                x -= speed;
            } else if ( dir == EAST ) {
                x += speed;
            } else if ( dir == SOUTH ) {
                y -= speed;
            }
        }
};

#endif 