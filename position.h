#ifndef POSITION_H
#define POSITION_H

#include <cmath>
using namespace std;

struct Position {
    int x{},y{};
    Position() = default;
    Position(int x, int y) : x(x), y(y) {}
};

#endif