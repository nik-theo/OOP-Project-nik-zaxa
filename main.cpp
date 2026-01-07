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
            sdc = new SelfDrivingCar("SDC_01", 0, 0, conf);
        }

        void addObj(WorldObject* o) {
            objects.push_back(o);
        }

        void setGPS(vector<Position>& g) {
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
                if (sdc->isFinished()) {
                    cout << "\n=================================================" << endl;
                    cout << ">>> MISSION COMPLETE: Destination Reached! <<<" << endl;
                    cout << ">>> Stopping simulation at Tick " << t << " <<<" << endl;
                    cout << "=================================================" << endl;
                    break; // Σπάει το loop και τερματίζει
                }
                if (sdc->getPos().x < 0 || sdc->getPos().x >= grid->getWidth() || sdc->getPos().y < 0 || sdc->getPos().y >= grid->getHeight()) {
                    break;
                }
            }
        }
    };

    Position getRandomPos(int maxX, int maxY) {
    Position p;
    p.x = rand() % maxX;
    p.y = rand() % maxY;
    return p;
}

    
    
    int main(int argc, char* argv[]) {
        
        int seed = time(0);
        int dimX = 40, dimY = 40, ticks = 100;
        int nMCars = 3, nMBikes = 4, nPCars = 5, nStop = 2, nLights = 2;
    double minConf = 40.0;
    vector<Position> gpsGoals;

    // 1. Parsing Command Line Arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--seed") seed = stoi(argv[++i]);
        else if (arg == "--dimX") dimX = stoi(argv[++i]);
        else if (arg == "--dimY") dimY = stoi(argv[++i]);
        else if (arg == "--numMovingCars") nMCars = stoi(argv[++i]);
        else if (arg == "--numMovingBikes") nMBikes = stoi(argv[++i]);
        else if (arg == "--numParkedCars") nPCars = stoi(argv[++i]);
        else if (arg == "--numStopSigns") nStop = stoi(argv[++i]);
        else if (arg == "--numTrafficLights") nLights = stoi(argv[++i]);
        else if (arg == "--simulationTicks") ticks = stoi(argv[++i]);
        else if (arg == "--minConfidenceThreshold") minConf = stod(argv[++i]);
        else if (arg == "--gps") {
            while (i + 2 < argc && argv[i+1][0] != '-') {
                int gx = stoi(argv[++i]);
                int gy = stoi(argv[++i]);
                gpsGoals.push_back({gx, gy});
            }
        } else if (arg == "--help") { printHelp(); return 0; }
    }

    if (gpsGoals.empty()) {
        cout << "Error: --gps target coordinates are required!\n";
        return 1;
    }

    srand(seed);
    
    // 2. Δημιουργία του SimulationManager
    SimulationManager sim(dimX, dimY, ticks, minConf);

    // 3. Τοποθέτηση GPS στόχων στο όχημα
    sim.setGPS(gpsGoals);

    // 4. Δημιουργία και τοποθέτηση όλων των αντικειμένων (WorldObjects)
    
    // Moving Cars (C)
    for(int i=0; i<nMCars; ++i) {
        Position p = getRandomPos(dimX, dimY);
        sim.addObj(new MovingObject("CAR:"+to_string(i), Object_type::Car, 'C', p.x, p.y, 1, 'E'));
    }
    
    // Moving Bikes (B)
    for(int i=0; i<nMBikes; ++i) {
        Position p = getRandomPos(dimX, dimY);
        sim.addObj(new MovingObject("BIKE:"+to_string(i), Object_type::Bike, 'B', p.x, p.y, 1, 'N'));
    }

    // Parked Cars (P)
    for(int i=0; i<nPCars; ++i) {
        Position p = getRandomPos(dimX, dimY);
        sim.addObj(new MovingObject("PARKED:"+to_string(i), Object_type::PARKED_CAR, 'P', p.x, p.y, 0, ' '));
    }

    // Stop Signs (S)
    for(int i=0; i<nStop; ++i) {
        Position p = getRandomPos(dimX, dimY);
        sim.addObj(new StopSign("STOP:"+to_string(i), p.x, p.y));
    }

    // Traffic Lights (R/G/Y)
    for(int i=0; i<nLights; ++i) {
        Position p = getRandomPos(dimX, dimY);
        sim.addObj(new TrafficLight("LIGHT:"+to_string(i), p.x, p.y));
    }

    // 5. Εκτέλεση της Προσομοίωσης
    sim.run();

    return 0;
}
//     int seed = time(0);
//     int dimX = 40, dimY = 40;
//     int ticks = 100;
//     double minConf = 0.4;
//     vector<Position> gpsGoals;

//     //γραμμη εντολων
//     for (int i = 1; i <argc; i++) {
//         string arg = argv[i];
//         if ( arg == "--seed") {
//             seed = stoi(argv[++i]);
//         } else if ( arg == "--dimX") {
//             dimX = stoi(argv[++i]);
//         } else if ( arg == "--dimY") {
//             dimY = stoi(argv[++i]);
//         } else if ( arg == "--simulationTicks") {
//             ticks = stoi(argv[++i]);
//         } else if ( arg == "--minConfidenceThreshold") {
//             minConf = stod(argv[++i])/100.0;
//         } else if ( arg == "--gps") {
//             while (i + 2 < argc && argv[i+1][0] != '-' && argv[i+2][0] != '-') {
//                 int x = stoi(argv[++i]);
//                 int y = stoi(argv[++i]);
//                 gpsGoals.push_back({x,y});
//             }
//         } else if (arg == "--help") {
//             printHelp();
//             return 0;
//         }
//     }
//     if (gpsGoals.empty()) {
//         cout << "Error: --gps coordinates are required!\n";
//         return 1;
//     }
//     srand(seed);
//     cout << "Simulation started with seed: " << seed << endl;

//     Grid world(dimX, dimY);
//     vector<WorldObject*> allObjects;

//     SelfDrivingCar* sdc = new SelfDrivingCar("SDC:1", 0, 0, minConf);
//     sdc->setGPS(gpsGoals);
//     allObjects.push_back(sdc);

//     allObjects.push_back(new TrafficLight("TL:1", 5, 5));

//     for ( int t = 0; t < ticks; t++ ) {
//         cout << "\n--- TICK " << t << " ---" << endl;
//         for (auto obj : allObjects) {
//             obj->update(t);
//         }
//         vector<vector<Sensor_Reading>> rawData;
//         //Κινηση 
//         sdc->executeMovement();
//         // Οπτικοποιηση
//         world.display(allObjects, sdc->getPos());
//         // Ελεγχος να μην βγει εκτος οριων 
//         if (sdc->getPos().x < 0 || sdc->getPos().x >= dimX || sdc->getPos().y < 0 || sdc->getPos().y >= dimY) {
//             cout << "Car went out of bounds! Simulation ended.\n";
//             break;
//         }
//     }

//     for (auto obj : allObjects) {
//         delete obj;
//     }
//     allObjects.clear();
//     return 0;
// }






// compile g++ -std=c++17 -O3 -Wall main.cpp -o