#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <vector>
#include "position.h"
using namespace std;

class Navigation {
    private:
        vector <Position> gps_target;
        size_t current_target = 0;
    public:
        void setTarget(vector <Position> t) {
            gps_target = move(t);
        }

        bool hasTarget() const {
            return current_target < gps_target.size();
        }

        const Position& getCurrentTarget() const {
            return gps_target[current_target];
        }

        void advance_target() {
            if (hasTarget()) {
                current_target++;
            }
        }
}

#endif