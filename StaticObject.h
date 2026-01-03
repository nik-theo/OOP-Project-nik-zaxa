#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "WorldObjects.h"
using namespace std;

class StaticObject : public WorldObjects {
    public:
        StaticObject(string id, char glyph, Position p) : WorldObjects(move(id), glyph, p) {}

        void update(int) override {}
};

#endif