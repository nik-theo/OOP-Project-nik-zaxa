#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "Grid.h"
#include "Navigation.h"
#include "WorldObjects.h"
using namespace std;

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

class SimulationManager {
    private:
        Grid* grid;
        SelfDrivingCar* sdc;
        vector<WorldObject*> objects;
        int maxTicks;
    public:
        SimulationManager(int x, int y, int t, double conf) : maxTicks(t) {
            grid = new Grid(x,y);
            sdc = new SelfDrivingCar("SDC_01", 0, 0, conf/100.0);
        }

        void addObj(WorldObject* o) {
            objects.push_back(o);
        }

        void setGPS(vector<Position> g) {
            sdc->setGPS(g);
            objects.push_back(sdc);
        }

        void run() {
            cout << "--- INITIAL WORLD ---\n";
            grid->display(objects, sdc->getPos());
            for (int t = 0; t< maxTicks; t++) {
                for (auto o : objects) {
                    o->update(t);
                }
                sdc->think(objects);
                sdc->executeMovement();
                cout << "\nTick" << t << " | SDC at (" << sdc->getPos().x << "," << sdc->getPos().y << ")\n";
                grid->display(objects, sdc->getPos(), 5); // 5x5
                if (sdc->getPos().x < 0 || sdc->getPos().x >= grid->getWidth() || sdc->getPos().y < 0 || sdc->getPos().y >= grid->getHeight()) {
                    break;
                }
            }
        }
};

int main(int argc, char* argv[]) {
    int seed = time(0);
    int dimX = 40, dimY = 40;
    int ticks = 100;
    double minConf = 0.4;
    vector<Position> gpsGoals;

    //γραμμη εντολων
    for (int i = 1; i <argc; i++) {
        string arg = argv[i];
        if ( arg == "--seed") {
            seed = stoi(argv[++i]);
        } else if ( arg == "--dimX") {
            dimX = stoi(argv[++i]);
        } else if ( arg == "--dimY") {
            dimY = stoi(argv[++i]);
        } else if ( arg == "--simulationTicks") {
            ticks = stoi(argv[++i]);
        } else if ( arg == "--minConfidenceThreshold") {
            minConf = stod(argv[++i])/100.0;
        } else if ( arg == "--gps") {
            while (i + 1 < argc && argv[i+1][0] != '-') {
                int x = stoi(argv[++i]);
                int y = stoi(argv[++i]);
                gpsGoals.push_back({x,y});
            }
        } else if (arg == "--help") {
            printHelp();
            return 0;
        }
    }
    if (gpsGoals.empty()) {
        cout << "Error: --gps coordinates are required!\n";
        return 1;
    }
    srand(seed);
    cout << "Simulation started with seed: " << seed << endl;

    Grid world(dimX, dimY);
    vector<WorldObject*> allObjects;

    SelfDrivingCar* sdc = new SelfDrivingCar("SDC:1", 0, 0, minConf);
    sdc->setGPS(gpsGoals);
    allObjects.push_back(sdc);

    allObjects.push_back(new TrafficLight("TL:1", 5, 5));

    for ( int t = 0; t < ticks; t++ ) {
        cout << "\n--- TICK " << t << " ---" << endl;
        for (auto obj : allObjects) {
            obj->update(t);
        }
        vector<vector<Sensor_Reading>> rawData;
        //Κινηση 
        sdc->executeMovement();
        // Οπτικοποιηση
        world.display(allObjects, sdc->getPos());
        // Ελεγχος να μην βγει εκτος οριων 
        if (sdc->getPos().x < 0 || sdc->getPos().x >= dimX || sdc->getPos().y < 0 || sdc->getPos().y >= dimY) {
            cout << "Car went out of bounds! Simulation ended.\n";
            break;
        }
    }

    for (auto obj : allObjects) {
        delete obj;
    }
    return 0;
}