#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Position{
    int x;
    int y;

    Position(int x, int y): x(0), y(0){cout<< "created the position" <<endl;}

    int distTo(Position obj){
        int result = sqrt(pow(x - obj.x, 2), pow(y - obj.y, 2));
        return result;
    }
};

enum class Object_type{
    Car, Bike, Parked, Traffic_Light, Unknown


};


struct Sensor_Reading{
    Object_type type;
    int distance;
    Position pos
    double confidence;
    int objectId;
    int speed;
    string direction;
    string signText;
    char trafficLight;

    Sensor_Reading(){
        type = Unknown;
        distance = -1;
        confidence = -1;
        objectId = -1;
        speed = -1;
        direction = "";
        signText = "";
        trafficLight = '';
    } 
};

// γενικη κλαση για τα τους αισθητηρες
class Sensor{
    protected:
        Position pos;
        char direction;
        double accuracy;
        int maxrange;
        

        double calcConf(double objdist){
            if (objdist > maxrange){
                return 0.0;
            }

            double distfact = 1.0 - (objdist/double(maxrange));

            double confidence = this->accuracy * distfact;

            double noise = ((rand() % 11) -5) / 100.0;

            confidence += noise;
            if (confidence > 1.0){
                confidence = 1.0;
            } else if (confidence < 0.0){
                confidence = 0.0;
            }
            return confidence;
        }


    public:
        Sensor(Position pos, char direc, double acc, int max): pos(pos), direction(direc), Accuracy(acc), maxrange(max){}
        virtual ~Sensor(){}
        //αρχικοποιηση
        virtual vector<Sensor_Reading> see(/*"θελει μαλλον βεκτορ με τα αντικειμενα που υπαρχουν στον κοσμο"*/) = 0;

        // ενημερωνει την θεση και την κατευθυνση του αισθητηρα
        // αυτο το κανουμε σε καθε βημα στην main με πχ. Lidar l(carpos); l.updateSensorPos(carpos, cardir);
        void UpdateSensorPos(Position newpos, char newdirec){
            this->pos = newpos;
            this->direction = newdirec;
        }
};


class Lidar: public Sensor{
    Lidar(Position pos): Sensor(pos,99,87){}

    vector<Sensor_Reading> see(/*πρεπει να παιρνει σαν παραμετρους τα αντικειμενα που υπαρχουν στον κοσμο*/) override{
        vector<Sensor_Reading> reads;
        //x, y ειναι η θεση του αμαξιου
        for (auto& obj : Object_type){
            int newx = abs(obj.getpos().x - this->pos.x);
            int newy = abs(obj.getpos().y - this->pos.y);

            if (newx <=4 && newy <=4){
                Sensor_Reading reading;

                reading.objectId = obj.getId();
                reading.pos = obj.getPos();
                // ελεγχει την ακριβεια στην αποσταση
                if(Accurecy(this->distanceAccuracy)){
                    reading.distance = this->position.distTo(obj.getpos());
                    reading.confidence = 1.0;
                } else {
                    reading.distance = -1;
                    reading.confidence = 0.0;
                }

                if(Accurecy(this->typeAccuracy)){
                    reading.type = obj.getType();
                } else{
                    reading.type = Unknown;
                }

                reads.push_back(reading);
            }
        }
        return reads
    }
}


class Radar : public Sensor{
    public:
    Radar(Position pos, string direc) : Sensor(pos,95,95){
        this->direction = direc;
    }



    vector<Sensor_Reading> see(/*πρεπει να παιρνει σαν παραμετρους τα αντικειμενα που υπαρχουν στον κοσμο*/) override{
        vector<Sensor_Reading> reads;

        for (auto& obj : /*Object_type*/){
            //ελεγχει αν το αντικειμενο ειναι στατικο η οχι
            if (obj.getspeed() == 0){
                continue;
            }

            bool inrange = false
            int newdist
            // αναλογα με την κατευθυνση που κοιταει κανει την αντιστοιχη αφαιρεση ωστε να βγει θετικο το νουμερο
            if (this->direction == "NORTH"){
                int newy = obj.getpos().y - this->pos.y;
                // αν το νουμερο ειναι αναμεσα στο 0-12 και το αντικειμενο ειναι στην ιδια γραμμη χ με το οχημα 
                if (newy >= 0 && newy <= 12 && obj.getpos().x == this->pos.x){
                    inrange = true;
                    newdist = newy;     //κραταει την αποσταση
                }
            } else if (this->direction == "EAST"){
                int newx = obj.getpos().x - this->pos.x;
                if (newx >= 0 && newx <= 12 && obj.getpos().y == this->pos.y){
                    inrange = true;
                    newdist = newx;
                }
            } else if (this->direction == "SOUTH"){
                int newy = this->pos.y - obj.getpos().y;
                if (newy >= 0 && newy <= 12 && obj.getpos().x == this->pos.x){
                    inrange = true;
                    newdist = newy;
                }
            } else if (this->direction == "WEST"){
                int newx = this->pos.x - obj.getpos().x;
                if (newx >= 0 && newx <= 12 && obj.getpos().y == this->pos.y){
                    inrange = true;
                    newdist = newx;
                }
            }


            if (inrange){
                Sensor_Reading reading;
                //αποθηκευση τον στοιχειων
                reading.speed = obj.getspeed();
                reading.direction = obj.getDirection();
                reading.objectId = obj.getObjId();
                reading.position = obj.getpos();
                //Για αποσταση
                if (Accurecy(this->distanceAccuracy)){
                    reading.distance = newdist;
                    reading.confidence = 0.95;
                } else {
                    reading.distance = -1;
                    reading.confidence = 0.0;
                }
                //για αναγνωριση κατηγοριας
                if(Accurecy(this->typeAccuracy)){
                    reading.type = obj.getType();
                } else{
                    reading.type = Unknown;
                }
                reads.push_back(reading);
            }
        }
        return reads;
    }
};


class Camera : public Sensor{
    public:
    Camera(Position pos, string direc) : Sensor(pos, 87, 95){
        this->direction = direc;
    }


    vector<Sensor_Reading> see() override{
        vector<Sensor_Reading> reads;

        for (auto& obj : object){
            bool inrange = false;

            int newx = abs(obj.getpos().x - this->pos.x);
            int newy = abs(obj.getpos().y - this->pos.y);

            if (this->direction == "NORTH"){
                if (obj.getpos().y > this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction == "EAST"){
                if(obj.getpos().x > this->pos.x && newx <= 7 && newy <=3){
                    inrange = true;
                }
            } else if (this->direction == "SOUTH"){
                if (obj.getpos().y < this->pos.y && newy <= 7 && newx <=3){
                    inrange = true;
                }
            } else if (this->direction =="WEST"){
                if (obj.getpos().x < this->pos.x && newx <= 7 && newx <=3){
                    inrange = true;
                }
            }


            if (inrange){
                vector<Sensor_Reading> reading;

                reading.speed = obj.getspeed();
                reading.direction = obj.getDirection();
                reading.objectId = obj.getObjId();
                reading.position = obj.getpos();

            }


        }
    }


}