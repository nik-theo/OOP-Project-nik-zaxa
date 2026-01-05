#ifndef SENSORS_H
#define SENSORS_H

// #include <iostream>
// #include <ctime>
#include <vector>
#include <memory>
#include "WorldObjects.h"
// #include <string>
// #include <cmath>

using namespace std;

// class Position{
//     int x;
//     int y;

//     Position(int x, int y): x(0), y(0){cout<< "created the position" <<endl;}

//     int distTo(Position obj){
//         int result = sqrt(pow(x - obj.x, 2), pow(y - obj.y, 2));
//         return result;
//     }
// };

// enum class Object_type{
//     Car, Bike, Parked, Traffic_Light, Unknown


// };


struct Sensor_Reading{
    Object_type type;
    int distance;
    Position position;
    double confidence;
    string objectId;
    //int objectId;
    int speed;
    char direction;
    string signText;
    char trafficLight;

    Sensor_Reading() : type(Object_type::Unknown), distance(-1), confidence(0), objectId(""), speed(0), direction(' '), signText(""), trafficLight(' ') {}
};


// γενικη κλαση για τα τους αισθητηρες
class Sensor{
    protected:
        Position pos;
        char direction;
        double distAccuracy;
        double typeAccuracy;
        int range;
        
    public:
        Sensor( Position p, char dir, double dAcc, double tAcc, int r) : pos(p), direction(dir), distAccuracy(dAcc), typeAccuracy(tAcc), range(r) {}

        virtual ~Sensor() {}
        
        //virtual vector<Sensor_Reading> scan(Position carpos, char cardir, const vector<WorldObject*>& objects) = 0;
        virtual vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) = 0;

        
        double calcConf(double objdist, double acc){
            if (objdist >= range){
                return 0.0;
            }
            double dist_factor = 1.0 - (objdist/range);
            double noise = ((rand() % 11) -5) / 100.0;
            double confidence = acc * dist_factor + noise;
            return max(0.0, min(1.0, confidence));

            // confidence += noise;
            // if (confidence > 1.0){
            //     confidence = 1.0;
            // } else if (confidence < 0.0){
            //     confidence = 0.0;
            // }
            // return confidence;
        }

        void updateSensorPos(Position p, char d) {
            this->pos = p;
            this->direction = d;
        }


    // public:
    //     Sensor(Position pos, char direc, double dacc, double tacc, int max): pos(pos), direction(direc), distAccuracy(dacc), typeAccuracy(tacc), range(max){}
    //     virtual ~Sensor(){}
    //     //αρχικοποιηση
    //     virtual vector<Sensor_Reading> scan(vector<WorldObject>& objects) = 0;

    //     // ενημερωνει την θεση και την κατευθυνση του αισθητηρα
    //     // αυτο το κανουμε σε καθε βημα στην main με πχ. Lidar l(carpos); l.updateSensorPos(carpos, cardir);
    //     void updateSensorPos(Position newpos, char newdirec){
    //         this->pos = newpos;
    //         this->direction = newdirec;
    //     }
};


class Lidar: public Sensor{
    public:
    Lidar(Position pos): Sensor(pos, 'N', 0.99, 0.87, 9){}

    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;
        //x, y ειναι η θεση του αμαξιου
        for (auto& obj : objects){
            int newx = abs(obj->getPos().x - this->pos.x);
            int newy = abs(obj->getPos().y - this->pos.y);

            if (newx <= 4 && newy <= 4){
                double dist = this->pos.distTo(obj->getPos());

                if (dist <= range){
                    Sensor_Reading reading;

                    reading.objectId = obj->getId();
                    reading.position = obj->getPos();
                    reading.distance = dist;
                    reading.type = obj->getType();

                    double categoryFactor = this->typeAccuracy;
                    reading.confidence = calcConf(dist, categoryFactor);

                    reads.push_back(reading);
                }
            }
        } 
        return reads;
    }
};


class Radar : public Sensor{
    public:
    Radar(Position pos, char direc) : Sensor(pos, direc, 0.95, 0.95, 12){}


    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;

        for (auto& obj : objects){
            //ελεγχει αν το αντικειμενο ειναι στατικο η οχι
            if (obj->getSpeed() <= 0){
                continue;
            }

            bool inrange = false;
            int newdist = 0;
            // αναλογα με την κατευθυνση που κοιταει κανει την αντιστοιχη αφαιρεση ωστε να βγει θετικο το νουμερο
            if (this->direction == 'N'){
                int newy = obj->getPos().y - this->pos.y;
                // αν το νουμερο ειναι αναμεσα στο 0-12 και το αντικειμενο ειναι στην ιδια γραμμη χ με το οχημα 
                if (newy > 0 && newy <= 12 && obj->getPos().x == this->pos.x){
                    inrange = true;
                    newdist = newy;     //κραταει την αποσταση
                }
            } else if (this->direction == 'E'){
                int newx = obj->getPos().x - this->pos.x;
                if (newx > 0 && newx <= 12 && obj->getPos().y == this->pos.y){
                    inrange = true;
                    newdist = newx;
                }
            } else if (this->direction == 'S'){
                int newy = this->pos.y - obj->getPos().y;
                if (newy < 0 && abs(newy) <= 12 && obj->getPos().x == this->pos.x){
                    inrange = true;
                    newdist = abs(newy);
                }
            } else if (this->direction == 'W'){
                int newx = this->pos.x - obj->getPos().x;
                if (newx < 0 && abs(newx) <= 12 && obj->getPos().y == this->pos.y){
                    inrange = true;
                    newdist = abs(newx);
                }
            }


            if (inrange){
                Sensor_Reading reading;
                //αποθηκευση τον στοιχειων
                reading.objectId = obj->getId();
                reading.type = obj->getType();
                reading.position = obj->getPos();
                reading.distance = newdist;
                reading.speed = obj->getSpeed();
                reading.direction = obj->getDirection();

                double categoryFactor = this->typeAccuracy;  //1.0;
                reading.confidence = calcConf(newdist, categoryFactor);

                reads.push_back(reading);
            }
        }
        return reads;
    }
};


class Camera : public Sensor{
    public:
    Camera(Position pos, char direc) : Sensor(pos,direc, 0.87, 0.95, 7){}


    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;

        for (auto& obj : objects){
            bool inrange = false;

            int newx = abs(obj->getPos().x - this->pos.x);
            int newy = abs(obj->getPos().y - this->pos.y);

            if (this->direction == 'N'){
                if (obj->getPos().y > this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction == 'E'){
                if(obj->getPos().x > this->pos.x && newx <= 7 && newy <=3){
                    inrange = true;
                }
            } else if (this->direction == 'S'){
                if (obj->getPos().y < this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction == 'W'){
                if (obj->getPos().x < this->pos.x && newx <= 7 && newy <=3){
                    inrange = true;
                }
            }


            if (inrange){
                Sensor_Reading reading;

                double dist = pos.distTo(obj->getPos());

                reading.objectId = obj->getId();
                reading.type = obj->getType();
                reading.position = obj->getPos();
                reading.distance = dist;
                reading.speed = obj->getSpeed();
                reading.direction = obj->getDirection();
                reading.signText = obj->getSignText();
                reading.trafficLight = obj->getLightColor();

                double categoryFactor = this->typeAccuracy;
                reading.confidence = calcConf(dist, categoryFactor);
                reads.push_back(reading);
            }
        }
        return reads;
    }
}; 

#endif