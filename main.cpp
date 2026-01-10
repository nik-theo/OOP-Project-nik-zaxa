#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Grid.h"
#include "Navigation.h"
#include "WorldObjects.h"
using namespace std;

//helper function that shows how to run the simulation
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
cout << " --minConfidenceThreshold <n>      Minimum confidence cutoff ( default : 0.4)" << endl;
cout << " --gps <x1> <y1> [x2 y2 . . .]     GPS target coordinates (required)" << endl;
cout << " --pov <n>                         POV for visualization (default : front)" << endl;
cout << " --help                            Show this help message" << endl << endl;
cout << "Example usage: " << endl;
cout << " ./ oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
}



class SimulationManager {
    private:
        POV povmode;  //Visualization mode
        int maxTicks;   // maximum runtime of the simulation
        Grid* grid;     //world grid
        SelfDrivingCar* sdc;    //self driving car
        vector<WorldObject*> objects;   //all objects
    public:
        SimulationManager(int x, int y, int t, double conf, POV mode) : povmode(mode), maxTicks(t){
            //creates the grid and initializes the car
            grid = new Grid(x,y);
            sdc = new SelfDrivingCar("SDC_01", 0, 0, conf);
        }

        ~SimulationManager(){
            for (auto& o : objects){
                delete o;
            }
        }

        //adds objects to the simulation
        void addObj(WorldObject* o) {
            objects.push_back(o);
        }

        //sets the goal of the car
        void setGPS(vector<Position>& g) {
            sdc->setGPS(g);
            objects.push_back(sdc);
        }

        
        //runs the main simulation loop
        void run() {
            cout << "--- INITIAL WORLD ---\n";
            //calls the display for the first time and prints the full grid
            grid->display(objects, sdc->getPos(), sdc->getDirection(), POV::FULL);

            //for every clock tick
            for (int t = 0; t< maxTicks; t++) {

                //updates all world objects for this tick
                for (auto o : objects) {
                    o->update(t);
                }

                //let the car analyze the environment and diside the next move
                sdc->think(objects);
                sdc->executeMovement();

                // prints the tick we are at and the position of the car
                cout << "\nTick" << t << " | SDC at (" << sdc->getPos().x << "," << sdc->getPos().y << ")\n";

                // prints the grid using the selected POV
                grid->display(objects, sdc->getPos(), sdc->getDirection(), povmode, 5); // 5x5

                //checks if it has reached the final destination
                if (sdc->isFinished()) {
                    cout << "\n=================================================" << endl;
                    cout << ">>> MISSION COMPLETE: Destination Reached! <<<" << endl;
                    cout << ">>> Stopping simulation at Tick " << t << " <<<" << endl;
                    cout << "=================================================" << endl;
                    break; // breaks the loop and ends the simulation
                }

                //ends the simulation if the car goes out of bounds
                if (sdc->getPos().x < 0 || sdc->getPos().x >= grid->getWidth() || sdc->getPos().y < 0 || sdc->getPos().y >= grid->getHeight()) {
                    break;
                }
            }
            //prints the final grid
            cout << " \n -----final world-----\n";
            grid->display(objects, sdc->getPos(), sdc->getDirection(), POV::FULL);
        }
    };



//returns a random position within the grid
Position getRandomPos(int maxX, int maxY) {
    Position p;
    p.x = rand() % maxX;
    p.y = rand() % maxY;
    return p;
}

 

//the main function    
int main(int argc, char* argv[]) {
    // initializes the parameters with default values
    int seed = time(0);
    int dimX = 40, dimY = 40, ticks = 100;
    int nMCars = 3, nMBikes = 4, nPCars = 5, nStop = 2, nLights = 2;
    double minConf = 0.4;
    POV povmode = POV::FRONT;
    vector<Position> gpsGoals;

    //checks all the command arguments and changes the ones that are given
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
        else if (arg == "--pov") {
            string mode =argv[++i];
            if (mode == "front"){
                povmode = POV::FRONT;
            } else if (mode == "center"){
                povmode = POV::CENTERED;
            } else if (mode == "full"){
                povmode = POV::FULL;
            }
        }
        else if (arg == "--gps") {
            //reads all the gps coordinate pairs
            while (i + 2 < argc && argv[i+1][0] != '-') {
                int gx = stoi(argv[++i]);
                int gy = stoi(argv[++i]);
                gpsGoals.push_back({gx, gy});
            } 
        } else if (arg == "--help") { printHelp(); return 0; }
    }

    //if there are no goals given then prints an error messege and stops
    if (gpsGoals.empty()) {
        cout << "Error: --gps target coordinates are required!\n";
        return 1;
    }

    //seed the random number generater once
    srand(seed);
    
    // creates the SimulationManager
    SimulationManager sim(dimX, dimY, ticks, minConf, povmode);

    // gives the goals to the car
    sim.setGPS(gpsGoals);

    // creates and places all the objects in the world (WorldObjects)
    
    // Moving Cars (C)
    for(int i=0; i<nMCars; ++i) {
        //gets a random position to put the object
        Position p = getRandomPos(dimX, dimY);
        //
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

    // runs the simulation
    sim.run();

    return 0;
}
