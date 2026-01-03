#ifndef MOVINGOBJECTS_H
#define MOVINGOBJECTS_H

#include "WorldObjects.h"

#include <string>
using namespace std;


enum Direction {SOUTH, WEST, NORTH, EAST};

class MovingObject : public WorldObjects {
    protected:
        int speed;
        Direction dir;
    public:
        MovingObject(string id, char glyph, Position p, int speed, Direction dir) : WorldObjects(move(id), glyph, p), speed(s), dir(dir) {}

        virtual void move();

        void update(int) override {move();}
};

#endif 