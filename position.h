#ifndef POSITION_H
#define POSITION_H
#include <cmath>

#include <vector>
using std::vector;
using namespace std;


struct Position {
    int x,y;
    
    //manhattan distance
    int distTo(Position other) const {
        return abs(x-other.x) + abs(y - other.y);
    }
};

#endif