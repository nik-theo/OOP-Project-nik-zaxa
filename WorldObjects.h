#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <iostream>
#include <string>
#include "position.h"
using namespace std;

class WorldObjects {
    protected:
        string id;
        char glyph;
        Position pos;
    public:
        WorldObjects(string id, char g, Position p): id(move(id)), glyph(g), pos(p) {}
        
        virtual ~WorldObjects() = default;

        virtual void update(int current_tick) = 0;

        char getGlyph() const{
            return glyph;
        }

        string getId() const{
            return id;
        }

        Position getPosition() const{
            return pos;
        }
};

#endif