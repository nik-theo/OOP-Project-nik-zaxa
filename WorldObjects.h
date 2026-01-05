#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H

#include <string>
#include "position.h"
using namespace std;

enum class Object_type{CAR, BIKE, PARKED_CAR, TRAFFIC_LIGHT, STOP_SIGN, SELF_DRIVING_CAR, UNKNOWN};

class WorldObject {
    protected: 
        string id;
        char glyph;
        Position pos;
        int speed;
        char direction;
    public:
        WorldObject(string id, char g. int x, int y, int s = 0, char dir = ' ') : id(id), glyph(g), pos({x,y}), speed(s), direction(dir) {}

        virtual ~WorldObject() {}

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

class TrafficLight : public WorldObject {
    public:
        TrafficLight(string id, int x, int y) : WorldObject(id, 'R', x, y) {}

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


class MovingObject : public WorldObject {
    private:
        Object_type type;
    public:
        MovingObject(string id, Object_type t, char g, int x, int y, int s, char d) : WorldObject(id, g, x, y, s, d), type(t), {}

        virual ~MovingObject() {}

        void update(int tick) override {
            if (direction == 'N') {
                pos.y++;
            } else if (direction == 'S') {
                pos.y--;
            } else if (direction == 'E') {
                pos.x++;
            } else if (direction == 'W') {
                pos.x--;
            }
        } 

        Object_type getType() const override {
            return type;
        }
};

class StopSign : public WorldObject {
    public:
        StopSign(string id, int x, int y) : WorldObject(id, 'S', x, y) {}

        void update(int tick) override {}

        Object_type getType() const override {
            return Object_type::STOP_SIGN;
        }

        string getSignText() const override {
            return "STOP";
        }
};

#endif