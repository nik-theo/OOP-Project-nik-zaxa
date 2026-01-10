#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <string>
#include "position.h"
using namespace std;

//type of objects that can exist in the world
enum class Object_type{Car, Bike, PARKED_CAR, TRAFFIC_LIGHT, STOP_SIGN, SELF_DRIVING_CAR, Unknown};

//Base class for the objects
class WorldObject {
    protected: 
        string id;
        char glyph;     //characters used for visualization
        Position pos;
        int speed;
        char direction;
    public:
        WorldObject(string id, char g, int x, int y, int s = 0, char dir = ' ') : id(id), glyph(g), pos({x,y}), speed(s), direction(dir) {}

        virtual ~WorldObject() {}

        //updates the objects state for the given tick  
        virtual void update(int current_tick) = 0;

        //Getters
        Position getPos() const {
            return pos;
        }

        char getGlyph() const {
            return glyph;
        }

        string getId() const {
            return id;
        }

        int getSpeed() const {
            return speed;
        }

        char getDirection() const {
            return direction;
        }

        virtual Object_type getType() const = 0;

        virtual string getSignText() const {
            return "";
        }

        virtual char getLightColor() const {
            return ' ';
        }
};



//traffic light object that cycles between the colours based on the time
class TrafficLight : public WorldObject {
    public:
        TrafficLight(string id, int x, int y) : WorldObject(id, 'R', x, y) {}

        //updates for every tick the colour of the light
        /*RED: 4 ticks
          YELLOW: 2 ticks
          GREEN: 8 ticks */
        void update(int tick) override {
            int cycle = tick % 14;
            if (cycle < 4) {
                glyph = 'R';
            } else if (cycle < 12) {
                glyph = 'G';
            } else {
                glyph = 'Y';
            }
        }

        Object_type getType() const override {
            return Object_type::TRAFFIC_LIGHT;
        }

        char getLightColor() const override {
            return glyph;
        }
};


// subclass for all the moving objects
class MovingObject : public WorldObject {
    private:
        Object_type type;
    public:
        MovingObject(string id, Object_type t, char g, int x, int y, int s, char d) : WorldObject(id, g, x, y, s, d), type(t) {}

        virtual ~MovingObject() {}

        //updates the position of the car bases on the speed and direction its going
        void update(int tick) override {
            if (direction == 'N') {
                pos.y += speed;
            } else if (direction == 'S') {
                pos.y-= speed;
            } else if (direction == 'E') {
                pos.x+= speed;
            } else if (direction == 'W') {
                pos.x-=speed;
            }
        } 

        Object_type getType() const override {
            return type;
        }
};


//static stop sign
class StopSign : public WorldObject {
    public:
        StopSign(string id, int x, int y) : WorldObject(id, 'S', x, y) {}

        void update(int tick) override {}

        Object_type getType() const override {
            return Object_type::STOP_SIGN;
        }

        // returns the text on the sign
        string getSignText() const override {
            return "STOP";
        }
};

#endif