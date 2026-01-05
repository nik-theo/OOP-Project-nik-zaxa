#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "WorldObjects.h"

class SelfDrivingCar;

class Grid {
    private:
        int w, h;
    public:
        Grid(int width, int height) : w(width), h(height) {}

        void display(const vector<WorldObject*>& objs, Position sdcPos, int radius = -1) {
            for (int i = h-1; i >= 0; i--) {
                if (radius > 0 && abs(i - sdcPos.y) > radius) {
                    continue;
                }
                cout << setw(2) << i << " | ";
                for (int j = 0; j < w; j++) {
                    if (radius > 0 && abs(j - sdcPos.x) > radius) {
                        continue;
                    }
                    char glyph = '.';
                    for (auto o : objs) {
                        if (o->getPos().x == j && o->getPos().y == i) {
                            glyph = o->getGlyph();
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

// class SimulationManager {
//     private:
//         Grid* grid;
//         SelfDrivingCar* sdc;
//         vector<WorldObject*> objects;
//         int maxTicks;
//     public:
//         SimulationManager(int x, int y, int t, double conf) : maxTicks(t) {
//             grid = new Grid(x,y);
//             sdc = new SelfDrivingCar(0, 0, conf/100.0);
//         }

//         void addObj(WorldObject* o) {
//             objects.push_back(o);
//         }

//         void setGPS(vector<Position> g) {
//             sdc->setGoals(g);
//             objects.push_back(sdc);
//         }

//         void run() {
//             cout << "--- INITIAL WORLD ---\n";
//             grid->display(objects, sdc->getPos());
//             for (int t = 0; t< maxTicks; t++) {
//                 for (auto o : objects) {
//                     o->update(t);
//                 }
//                 sdc->think(objects);
//                 cout << "\nTick" << t << " | SDC at (" << sdc->getPos().x << "," << sdc->getPos().y << ")\n";
//                 grid->display(objects, sdc->getPos(), 5); // 5x5
//                 if (sdc->getPos().x < 0 || sdc->getPos().x >= 40 || sdc->getPos().y < 0 || sdc->getPos().y >= 40) {
//                     break;
//                 }
//             }
//         }
// };

#endif