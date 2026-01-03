#ifndef SELFDRIVINGCAR_H
#define SELFDRIVINGCAR_H

#include "MovingObjects.h"
#include <vector>
using namespace std;

enum SpeedState {
    STOPPED = 0, HALF_SPEED = 1, FULL_SPEED = 2
};


class SelfDrivingCar : public MovingObject {
    private: 
        SpeedState current_speed;
        NavigationSystem navigation_system;
        //vector <Sensor*> sensors;
    public:
        SelfDrivingCar(string id, int x, int y, vector <Position> targets);
        ~SelfDrivingCar();

        void update(int current_tick) override;
        void collectSensorData();
        void syncNavigationSystem();
        void executeMovement();
};


#endif