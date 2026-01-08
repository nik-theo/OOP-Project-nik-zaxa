#ifndef SENSORS_H
#define SENSORS_H


#include <vector>
#include <memory>
#include "WorldObjects.h"


using namespace std;


// holds all the parameters detected by the sensor for one object
struct Sensor_Reading{
    Object_type type;
    int distance;
    Position position;
    double confidence;
    string objectId;
    int speed;
    char direction;
    string signText;
    char trafficLight;

    //initialize them with some default values 
    Sensor_Reading() : type(Object_type::Unknown), distance(-1), confidence(0), objectId(""), speed(0), direction(' '), signText(""), trafficLight(' ') {}
};


// general class for the sensors
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
        
        //virtual function for all the sensors that will do the main job of the sensors
        virtual vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) = 0;

        //function that calculates the confidence 
        double calcConf(double objdist, double acc){
            // Objects outside sensor range have zero confidence
            if (objdist >= range){
                return 0.0;
            }
            // Distance-based confidence factor: farther objects reduce confidence
            double dist_factor = 1.0 - (objdist/range);
            //random noise from [0.05, -0.05] 
            double noise = ((rand() % 11) -5) / 100.0; 
            double confidence = acc * dist_factor + noise;
            //makes sure that the confidence is between [0.0, 1.0]
            return max(0.0, min(1.0, confidence));
        }
        //function that updates the position of the sensors while the car is moving
        void updateSensorPos(Position p, char d) {
            this->pos = p;
            this->direction = d;
        }
};

//subclass of sensor
class Lidar: public Sensor{
    public:
    // initializes position, direction, distance accuracy, confidence accurecy, range
    Lidar(Position pos): Sensor(pos, 'N', 0.99, 0.87, 9){}
    ~Lidar(){}

    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;
        
        for (auto& obj : objects){
            //gets the absolute distance between the object and the car
            int newx = abs(obj->getPos().x - this->pos.x);
            int newy = abs(obj->getPos().y - this->pos.y);
            //if the distances for both x and y is <= 4 it means that the object is inside the 9*9 square 
            //that the sensor can see
            if (newx <= 4 && newy <= 4){
                //gets the accual distance of the object
                double dist = this->pos.distTo(obj->getPos());

                if (dist <= range){
                    //Store this object's detected
                    Sensor_Reading reading;

                    //passes the counts that the Lidar can see and stores them to the variable 
                    reading.objectId = obj->getId();
                    reading.position = obj->getPos();
                    reading.distance = dist;
                    reading.type = obj->getType();

                    double categoryFactor = this->typeAccuracy;
                    //calls the function that calculates the confidence
                    reading.confidence = calcConf(dist, categoryFactor);

                    reads.push_back(reading);
                }
            }
        }
        //returns all the data that was collected from the sensor to a vector
        return reads;
    }
};

//subclass of sensor
class Radar : public Sensor{
    public:
    // initializes position, direction, distance accurecy, confidence accurecy, range
    Radar(Position pos, char direc) : Sensor(pos, direc, 0.95, 0.95, 12){}


    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;

        for (auto& obj : objects){
            //checks if the object is static or moving
            if (obj->getSpeed() <= 0){
                continue;
            }

            bool inrange = false;   
            int newdist = 0;
            // depending the direction of the car it does the equivalent subtraction of the positions
            if (this->direction == 'N'){
                int newy = obj->getPos().y - this->pos.y;
                //if the number is between 0-12 and the object is in the same x line with the car then
                if (newy > 0 && newy <= 12 && obj->getPos().x == this->pos.x){
                    inrange = true;     //the object is in range
                    newdist = newy;     //keeps the distance
                }
            } else if (this->direction == 'E'){
                //subtractes the x position and the y position stays the same
                int newx = obj->getPos().x - this->pos.x;
                if (newx > 0 && newx <= 12 && obj->getPos().y == this->pos.y){
                    inrange = true;
                    newdist = newx;
                }
            } else if (this->direction == 'S'){
                //looking at the opposite direction and the positions are negative so we use absolute
                int newy = this->pos.y - obj->getPos().y;
                if (newy < 0 && abs(newy) <= 12 && obj->getPos().x == this->pos.x){
                    inrange = true;
                    newdist = abs(newy);    //to get a positive distance value
                }
            } else if (this->direction == 'W'){
                //same here but for the x position
                int newx = this->pos.x - obj->getPos().x;
                if (newx < 0 && abs(newx) <= 12 && obj->getPos().y == this->pos.y){
                    inrange = true;
                    newdist = abs(newx);
                }
            }


            if (inrange){
                Sensor_Reading reading;
                //Store this object's detected
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

//subclass of sensor
class Camera : public Sensor{
    public:
    // initializes position, direction, distance accurecy, confidence accurecy, range
    Camera(Position pos, char direc) : Sensor(pos,direc, 0.87, 0.95, 7){}


    vector<Sensor_Reading> scan(const vector<WorldObject*>& objects) override{
        vector<Sensor_Reading> reads;

        for (auto& obj : objects){
            //again it keeps a veriable to see if the object is in range
            bool inrange = false;
            //gets the difference of distance between the objects and cars position
            int newx = abs(obj->getPos().x - this->pos.x);
            int newy = abs(obj->getPos().y - this->pos.y);

            //according to the direction
            if (this->direction == 'N'){
                //object must be in fornt of the car and inside a 7*7 square
                //y needs to be <= 7 because it can see 7 squares in front of it
                //x is in the midle so 3 from the right and 3 from the left
                if (obj->getPos().y > this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction == 'E'){
                //looking right now the x position of the object needs to be in front of the car
                //we change accordingly the x and y as well
                if(obj->getPos().x > this->pos.x && newx <= 7 && newy <=3){
                    inrange = true;
                }
            } else if (this->direction == 'S'){
                //same here
                if (obj->getPos().y < this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction == 'W'){
                //same here
                if (obj->getPos().x < this->pos.x && newx <= 7 && newy <=3){
                    inrange = true;
                }
            }

            //if one of the above is true it means that there is an object in the view of the sensor
            if (inrange){
                //Store this object's detected
                Sensor_Reading reading;

                double dist = pos.distTo(obj->getPos());
                // it saves all the values that the sensor can see
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