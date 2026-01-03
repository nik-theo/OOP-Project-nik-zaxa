#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
// #include "WorldObjects.h"
using namespace std;

struct Position {
    int x;
    int y;
};


class WorldObjects {
    protected:
        string id;
        char glyph;
        Position pos;
    public:
        WorldObjects(string id, char g, int x, int y): id(id), glyph(g), pos({x,y}) {}
        virtual ~WorldObjects() {}
        char getGlyph () {
            return glyph;
        }
};


class Grid {
    private:
        // Πλατος grid
        int width;
        // Υψος grid
        int height;
        // Πλεγμα απο pointers που ειτε δειχνει στο κενο 
        // ειτε σε ενα αντικειμενο μεσα στο grid
        vector<vector<WorldObjects*>> grid;
    public:
        Grid(int w, int h): width(w), height(h) {
            // Αρχικοποιω το vector με κενο δεικτη δεν δειχνει πουθενα ακομα 
            grid.resize(height, vector <WorldObjects*>(width,nullptr));
        }
        // Βαζω αντικειμενο στο grid 
        bool Object_placement(int x, int y, WorldObjects* object) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                grid[y][x] = object;
                return true;
            }
            return false;
        }
        // Απεικονιση του κοσμου 
        void display() {
            // Εκτυπωση απο το height-1 για να ειναι το (0,0) κατω αριστερα
            for (int i = height - 1; i >= 0; i-- ) {
                // Αριθμος γραμμης 
                cout << i << " | ";
                for ( int j = 0; j < width; j++ ) {
                    if ( grid[i][j] == nullptr) {
                        cout << ". ";
                    } else {
                        cout << grid[i][j]->getGlyph() << " ";
                    }
                }
                cout << endl;
            }
            // Αξονας χ 
            cout << "   ";
            for ( int j = 0; j < width; j++ ) {
                cout << "--";
            }
            cout << endl << "   ";
            for ( int j = 0; j < width; j++ ) {
                cout << j % 10 << " ";
            }
            cout << endl;
        }
};

int main() {
    Grid mygrid(40,40);
    WorldObjects car("CAR:1", 'C', 2, 3);
    WorldObjects tree("STOP:1", 'S', 5, 5);

    mygrid.Object_placement(2,3,&car);
    mygrid.Object_placement(5,5,&tree);
    cout << "Έναρξη προσομοίωσης" << endl;
    mygrid.display();
    return 0;
}

#endif