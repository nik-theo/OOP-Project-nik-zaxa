#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Sensors.h"
using namespace std;

class SensorFusionEngine {
    private:
        //variable that has the lowest allowed confidence
        double minConfidenceThreshold;
    public:
        SensorFusionEngine(double min) : minConfidenceThreshold(min) {}


        //function that combines all the data from all the sensors into one result
        vector<Sensor_Reading> fuseSensorData(const vector<vector<Sensor_Reading>>& allData) {
            vector<Sensor_Reading> totalResult;
            //mapping the group
            map<string, vector<Sensor_Reading>> group;
            // grouping based on the objectId
            for (const auto& readings : allData) {
                for (const auto& read : readings) {
                    group[read.objectId].push_back(read);
                }
            }
            //for every objectId in the group
            for (auto& [id, data] : group) {
                Sensor_Reading result;
                //initializes the values that it needs for the calculations
                double totalDistWeight = 0.0, totalSpeedWeight = 0.0, totalConf = 0.0;
                double maxConfFound = -1.0;
                bool isBike = false;
                bool isStopsign = false;
                bool isTrafficLight = false;

                result.objectId = id;
                //for every object in the group
                for (const auto& r : data) {
                    //if at least one sensor detects a bike
                    if (r.type == Object_type::Bike) {
                        isBike = true;
                    }
                    if (r.type == Object_type::STOP_SIGN){
                        isStopsign = true;
                    }
                    if (r.type == Object_type::TRAFFIC_LIGHT){
                        isTrafficLight = true;
                    }
                    // adds the distance and the speed from every sensor with the weights of the confidence
                    if (r.distance != -1) {
                        totalDistWeight += r.distance * r.confidence;
                    }
                    if (r.speed != -1) {
                        totalSpeedWeight += r.speed * r.confidence;
                    } 
                    totalConf += r.confidence;
                    // we save the best results bases on the confidence
                    if (r.confidence > maxConfFound) {
                        maxConfFound = r.confidence;
                        result.type = r.type;
                        //result.signText = r.signText;
                        //result.trafficLight = r.trafficLight;
                        result.position = r.position;
                        result.direction = r.direction;
                    }

                    if (r.trafficLight != ' ' && r.trafficLight != '\0') {
                        result.trafficLight = r.trafficLight;
                    }
                    // Το ίδιο και για το κείμενο πινακίδας
                    if (!r.signText.empty()) {
                        result.signText = r.signText;
                    }
                }
                double averageConf = totalConf / data.size();
                //if the average confidence is less than the acceptable and it is not a bike we dont keep the values
                if (averageConf < minConfidenceThreshold && !isBike && !isStopsign && !isTrafficLight) {
                    continue;
                }
                //else we keep the average of the values
                if (totalConf > 0) {
                    result.distance = totalDistWeight / totalConf;
                    result.speed = totalSpeedWeight / totalConf;
                    result.confidence = averageConf;
                }
                totalResult.push_back(result);
            }
            return totalResult;
        }
};

class SelfDrivingCar : public WorldObject {
    private:
        vector<Sensor*> sensors;
        SensorFusionEngine* fusionEngine;
        vector<Position> gpsTargets;
        size_t currentTargetIdx;
        int stoptickcount = 0;


        enum SpeedState {STOPPED, HALF_SPEED, FULL_SPEED};
        SpeedState speedState;
    public:
        //returns if the car has reached its destination
        bool isFinished() const { 
             return currentTargetIdx >= gpsTargets.size() && !gpsTargets.empty(); 
        }


        SelfDrivingCar(string id, int x, int y, double minConf) : WorldObject(id, '@', x, y), currentTargetIdx(0), speedState(STOPPED) {
            fusionEngine = new SensorFusionEngine(minConf);
            //sensor fusion
            sensors.push_back(new Lidar(this->pos));
            sensors.push_back(new Radar(this->pos, 'N'));
            sensors.push_back(new Camera(this->pos, 'N'));
        }

        void setGPS(vector<Position> targets) {
            gpsTargets = targets;
        }

        void update(int current_tick) override {
            vector<vector<Sensor_Reading>> allSensorData;
        }
        

        //checks if the car is in the same row or colom with the object
        bool isAhead(const Sensor_Reading& r) const {
            // same colom
            if (direction == 'N' && r.position.x == pos.x && r.position.y > pos.y)
                return true;
            if (direction == 'S' && r.position.x == pos.x && r.position.y < pos.y)
                return true;

            // same row
            if (direction == 'E' && r.position.y == pos.y && r.position.x > pos.x)
                return true;
            if (direction == 'W' && r.position.y == pos.y && r.position.x < pos.x)
                return true;

            return false;
        }



        //function that tells the car where to go next
        void makeDecision(const vector<Sensor_Reading>& fusedData) {
            //flages
            bool veryClose = false;
            bool causion = false;
            //if it has found a stop previous wait
            if (stoptickcount > 0){
                speedState = STOPPED;
                stoptickcount--;
                return;
            }


            //if the car has reached the destination stop
            if (isFinished()) {
                speedState = STOPPED;
                return;
            }

            for(const auto& r : fusedData) {


                //Traffic lights
                if (r.type == Object_type::TRAFFIC_LIGHT && isAhead(r)){
                    bool isRedOrYellow = (r.trafficLight == 'R' || r.trafficLight == 'Y');

                    if (isRedOrYellow && (r.distance == 1 || (speedState == STOPPED && r.distance<= 3))){
                        speedState = STOPPED;
                        return;
                    }

                    if (isRedOrYellow && r.distance <= 3){
                        causion = true;
                    }
                }

                //moving object within 2 position
                if (r.speed > 0 && r.distance <= 2 && r.distance != -1){
                    veryClose = true;
                }

                //for stop signs
                if (r.type == Object_type::STOP_SIGN && r.signText == "STOP" && isAhead(r)){
                    //if it is not stopped and the stop count is zero and the stop sign is right in front of the car it stops
                    if (r.distance !=-1 && r.distance == 1 && stoptickcount == 0 && speedState != STOPPED){
                        speedState = STOPPED;
                        stoptickcount = 1;
                        return;
                    }
                    //if it is less than 5 squares away it slows down
                    if (r.distance <= 4){
                        causion = true;
                    }
                }
            }
            //target within 5 positions
            Position target = gpsTargets[currentTargetIdx];
            if (abs(pos.x - target.x) + abs(pos.y - target.y) <= 5){
                causion = true;
            }
            //desides the speed
            if (veryClose){
                speedState = STOPPED;
            } else if (causion){
                speedState = HALF_SPEED;
            } else {
                if (speedState == STOPPED){
                    speedState = HALF_SPEED;
                } else {
                    speedState = FULL_SPEED;
                }
            }
        }
        

        //the brain of the car
        void think(const vector<WorldObject*>& objects) {
            vector<vector<Sensor_Reading>> allData;
            // loop for every sensor reading
            for (auto s : sensors) {
                //updates the position and direction of the sensors
                s->updateSensorPos(this->pos, this->direction);
                //scans the enviroment and adds the data to allData
                allData.push_back(s->scan(objects));
            }
            //calls the function that fuses all the readings
            vector<Sensor_Reading> fused = fusionEngine->fuseSensorData(allData);
            makeDecision(fused);
        }


        //function that moves the car towards the current gps target
        void executeMovement() {
            // prevents unwanted movement
            if (speedState == STOPPED || isFinished()) {
                return; 
            }
            //gets the current gps target
            Position target = gpsTargets[currentTargetIdx];
            int step = (speedState == FULL_SPEED) ? 2 : 1;


            //moving based on the Manhattan distance
            //moves first on the x position and only when it reached the goal moves on the y position
            if (this->pos.x < target.x) {
                //moves east without going over the target
                this->pos.x += min(step, target.x - this->pos.x);
                //needed to update the sensors
                direction = 'E';
            } else if (this->pos.x > target.x) {
                //moves west without going over the target
                this->pos.x -= min(step, this->pos.x - target.x);
                direction = 'W';
            } else if (this->pos.y < target.y) {
                //moves north without going over the target
                this->pos.y += min(step, target.y - this->pos.y);
                direction = 'N';
            } else if (this->pos.y > target.y) {
                //moves south without going over the target
                this->pos.y -= min(step, this->pos.y - target.y);
                direction = 'S';
            }

            //when a checkpoint is reached moves to the next target
            if (this->pos.x == target.x && this->pos.y == target.y) {
                currentTargetIdx++;
            }
        }

        Object_type getType() const override {
            return Object_type::Car;
        }
};

#endif