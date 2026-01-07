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
        double minConfidenceThreshold;
    public:
        SensorFusionEngine(double min) : minConfidenceThreshold(min) {}
        vector<Sensor_Reading> fuseSensorData(const vector<vector<Sensor_Reading>>& allData) {
            vector<Sensor_Reading> totalResult;
            map<string, vector<Sensor_Reading>> group;
            // Ομαδοποιηση με βαση το objectId
            for (const auto& readings : allData) {
                for (const auto& read : readings) {
                    group[read.objectId].push_back(read);
                }
            }
            for (auto& [id, data] : group) {
                Sensor_Reading result;
                double totalDistWeight = 0.0, totalSpeedWeight = 0.0, totalConf = 0.0;
                double maxConfFound = -1.0;
                bool isBike = false;

                result.objectId = id;
                for (const auto& r : data) {
                    if (r.type == Object_type::Bike) {
                        isBike = true;
                    }
                    if (r.distance != -1) {
                        totalDistWeight += r.distance * r.confidence;
                    }
                    if (r.speed != -1) {
                        totalSpeedWeight += r.speed * r.confidence;
                    } 
                    totalConf += r.confidence;

                    if (r.confidence > maxConfFound) {
                        maxConfFound = r.confidence;
                        result.type = r.type;
                        result.signText = r.signText;
                        result.trafficLight = r.trafficLight;
                        result.position = r.position;
                        result.direction = r.direction;
                    }
                }
                double averageConf = totalConf / data.size();
                if (averageConf < minConfidenceThreshold && !isBike) {
                    continue;
                }
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

        enum SpeedState {STOPPED, HALF_SPEED, FULL_SPEED};
        SpeedState speedState;
    public:

        bool isFinished() const { 
             return currentTargetIdx >= gpsTargets.size() && !gpsTargets.empty(); 
        }
        // bool hasArrived() const {
        //     return navSystem->isFinished();
        // }

        SelfDrivingCar(string id, int x, int y, double minConf) : WorldObject(id, '@', x, y), currentTargetIdx(0), speedState(STOPPED) {
            fusionEngine = new SensorFusionEngine(minConf);
            //Συνθεση αισθητηρων
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
        
        
        void makeDecision(const vector<Sensor_Reading>& fusedData) {
            speedState = FULL_SPEED; //Default

            if (isFinished()) {
                speedState = STOPPED;
                return;
            }
            //bool destinationReached = false;
            for(const auto& r : fusedData) {
                // if (currentTargetIdx >= gpsTargets.size()) {
                //     destinationReached = true; // <--- ΕΔΩ ΑΛΛΑΖΕΙ
                //     return; // Stop
                // }
                if (r.type == Object_type::TRAFFIC_LIGHT && (r.trafficLight == 'R' || r.trafficLight == 'Y') && r.distance <= 3) {
                    speedState = STOPPED;
                }
                if (r.speed > 0 && r.distance <= 2) {
                    speedState = STOPPED;
                }
            }
        }
        
        void think(const vector<WorldObject*>& objects) {
            vector<vector<Sensor_Reading>> allData;
            for (auto s : sensors) {
                s->updateSensorPos(this->pos, this->direction);
                allData.push_back(s->scan(objects));
            }
            vector<Sensor_Reading> fused = fusionEngine->fuseSensorData(allData);
            makeDecision(fused);
        }

        void executeMovement() {
            if (speedState == STOPPED || isFinished()) {
                return; 
            }
            Position target = gpsTargets[currentTargetIdx];
            int step = (speedState == FULL_SPEED) ? 2 : 1;
            //Κινηση με βαση την αποσταση Manhattan
            if (this->pos.x < target.x) {
                this->pos.x += min(step, target.x - this->pos.x);
                direction = 'E';
            } else if (this->pos.x > target.x) {
                this->pos.x -= min(step, this->pos.x - target.x);
                direction = 'W';
            } else if (this->pos.y < target.y) {
                this->pos.y += min(step, target.y - this->pos.y);
                direction = 'N';
            } else if (this->pos.y > target.y) {
                this->pos.y -= min(step, this->pos.y - target.y);
                direction = 'S';
            }
            
            if (this->pos.x == target.x && this->pos.y == target.y) {
                currentTargetIdx++;
            }
        }

        Object_type getType() const override {
            return Object_type::Car;
        }
};

#endif