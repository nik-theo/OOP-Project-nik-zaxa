#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "WorldObjects.h"

class SelfDrivingCar;
// Visualization modes for the display
enum class POV{
    FULL,  //shows the entire grid
    CENTERED, //only shows a square area around the car
    FRONT   //only shows an area infront of the car
};


class Grid {
    private:
        int w, h;       //width and height of the grid
    public:
        Grid(int width, int height) : w(width), h(height) {}

        //function that shows the map depending on the POV argument
        void display(const vector<WorldObject*>& objs, Position sdcPos, char direction, POV mode, int radius = -1) {

            //initializes the size of the full grid 
            int minX = 0; 
            int maxX = w - 1;
            int minY = 0;
            int maxY = h - 1;

            // if the POV is centered the it only displays a box around the car the size of the radius
            if (mode == POV::CENTERED && radius > 0){
                minX = sdcPos.x - radius;
                maxX = sdcPos.x + radius;
                minY = sdcPos.y - radius;
                maxY = sdcPos.y + radius;

            //if the POV is Front then it shows only the front box of the car with the radius its given
            } else if (mode == POV::FRONT && radius > 0){
                //the calculation changes based on the direction of the car
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

            //remains within the grids limits
            minX = max(0, minX);
            minY = max(0, minY);
            maxX = min(w - 1, maxX);
            maxY = min(h - 1, maxY);

            //here is where the printing starts
            //depenting on the previous calculations of the x and y 
            for (int i = maxY; i >= minY; i--) {
                //print row index
                cout << setw(2) << i << " | ";
                for (int j = minX; j <= maxX; j++) {
                    //default glyph
                    char glyph = '.';

                    //if there is an object in that position it holds the objects glyph
                    for (auto o : objs) {
                        if (o->getPos().x == j && o->getPos().y == i) {
                            glyph = o->getGlyph();
                            break;
                        }
                    }
                    //prints the glyph
                    cout << glyph << " ";
                }
                cout << endl;
            }
        }
        //returns the grid Width
        int getWidth() const {
            return w;
        }
        //returns the grid Height
        int getHeight() const {
            return h;
        }
};

#endif