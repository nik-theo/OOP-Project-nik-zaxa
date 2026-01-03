#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <iostream>
#include <string>
using namespace std;

class WorldObjects {
    protected:
        string id;
        char glyph;
        int x,y;
    public:
        WorldObjects(string id, char g, int x, int y): id(id), glyph(g), x(x), y(y) {}
        
        virtual ~WorldObjects() {}

        virtual void update(int current_tick) = 0;

        char getGlyph () {
            return glyph;
        }

        string getId() {
            return id;
        }

        int getX() {
            return x;
        }

        int getY() {
            return y;
        }
};

#endif