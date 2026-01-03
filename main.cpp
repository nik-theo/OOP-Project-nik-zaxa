#include <iostream>
#include <string>
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
                if (i<10) {
                    cout << i << "  | ";
                } else {
                    cout << i << " | ";
                }
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

void printHelp () {
cout << "Self-Driving Car Simulation" << endl;
cout << "Usage: " << endl;
cout << " --seed <n>                        Random seed ( default : current time)" << endl;
cout << " --dimX <n>                        World width ( default : 40)" << endl;
cout << " --dimY <n>                        World height ( default : 40)" << endl;
cout << " --numMovingCars <n>               Number of moving cars ( default : 3)" << endl;
cout << " --numMovingBikes <n>              Number of moving bikes ( default : 4)" << endl;
cout << " --numParkedCars <n>               Number of parked cars ( default : 5)" << endl;
cout << " --numStopSigns <n>                Number of stop signs (default : 2)" << endl;
cout << " --numTrafficLights <n>            Number of traffic lights ( default : 2)" << endl;
cout << " --simulationTicks <n>             Maximum simulation ticks ( default : 100)" << endl;
cout << " --minConfidenceThreshold <n>      Minimum confidence cutoff ( default : 40)" << endl;
cout << " --gps <x1> <y1> [x2 y2 . . .]     GPS target coordinates (required)" << endl;
cout << " --help                            Show this help message" << endl << endl;
cout << "Example usage: " << endl;
cout << " ./ oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
}

int main(int argc, char* argv[]) {
    if ( argc < 2 ) {
        cout << "No arguments given." << endl;
        printHelp();
        return 1;
    }

    for ( int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--help") {
            printHelp();
            return 0;
        }
    }
    Grid mygrid(40,40);
    WorldObjects car1("CAR:1", 'C', 2, 3);
    WorldObjects tree1("STOP:1", 'S', 5, 5);
    WorldObjects ParkedCar("PC:1", 'P', 10, 5);
    WorldObjects tree2("STOP:2", 'S', 1, 7);
    WorldObjects car2("CAR:2", 'C', 4, 6);


    mygrid.Object_placement(2,3,&car1);
    mygrid.Object_placement(5,5,&tree1);
    mygrid.Object_placement(10,5,&ParkedCar);
    mygrid.Object_placement(1,7,&tree2);
    mygrid.Object_placement(4,6,&car2);
    cout << "Start" << endl;
    mygrid.display();
    return 0;
}