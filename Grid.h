#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "WorldObjects.h"

class SelfDrivingCar;

enum class POV{
    FULL, CENTERED, FRONT
};

class Grid {
    private:
        int w, h;
    public:
        Grid(int width, int height) : w(width), h(height) {}

        void display(const vector<WorldObject*>& objs, Position sdcPos, char direction, POV mode, int radius = -1) {

            int minX = 0; 
            int maxX = w - 1;
            int minY = 0;
            int maxY = h - 1;

            if (mode == POV::CENTERED && radius > 0){
                minX = sdcPos.x - radius;
                maxX = sdcPos.x + radius;
                minY = sdcPos.y - radius;
                maxY = sdcPos.y + radius;
            } else if (mode == POV::FRONT && radius > 0){
                if (direction == 'N'){
                    minX = sdcPos.x - radius;
                    maxX = sdcPos.x + radius;
                    minY = sdcPos.y;
                    maxY = sdcPos.y + radius;
                } else if (direction == 'E'){
                    minX = sdcPos.x;
                    maxX = sdcPos.x + radius;
                    minY = sdcPos.y - radius;
                    maxY = sdcPos.y + radius;
                } else if (direction == 'S'){
                    minX = sdcPos.x - radius;
                    maxX = sdcPos.x + radius;
                    minY = sdcPos.y - radius;
                    maxY = sdcPos.y;
                } else if (direction == 'W'){
                    minX = sdcPos.x - radius;
                    maxX = sdcPos.x;
                    minY = sdcPos.y - radius;
                    maxY = sdcPos.y + radius;
                }
            }

            minX = max(0, minX);
            minY = max(0, minY);
            maxX = min(w - 1, maxX);
            maxY = min(h - 1, maxY);

            for (int i = maxY; i >= minY; i--) {
                cout << setw(2) << i << " | ";
                for (int j = minX; j <= maxX; j++) {
                    char glyph = '.';
                    for (auto o : objs) {
                        if (o->getPos().x == j && o->getPos().y == i) {
                            glyph = o->getGlyph();
                            break;
                        }
                    }
                    cout << glyph << " ";
                }
                cout << endl;
            }
        }

        int getWidth() const {
            return w;
        }

        int getHeight() const {
            return h;
        }
};

#endif