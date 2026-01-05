#ifndef POSITION_H
#define POSITION_H

#include <cmath>
using namespace std;

struct Position {
    int x,y;
    
    int distTo(Position other) const {
        return abs(x-other.x) + abs(y - other.y);
    }
};

#endif