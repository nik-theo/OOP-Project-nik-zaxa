#include <vector>
#include "WorldObjects.h"
//#include "Grid.h"
using namespace std;

class Simulator {
    private:
        vector <WorldObjects*> objects;
        int total_ticks;
        int current_tick;
    public:
        Simulator(int ticks) : total_ticks(ticks), current_tick(0) {}

        void addObjects(WorldObjects* obj) {
            objects.push_back(obj);
        }

        void run() {
            while (current_tick < total_ticks) {
                cout << "--- Tick " << current_tick << " ---" << endl;
                for (auto obj : objects) {
                    obj->update(current_tick);
                    //Grid->display();
                }
                current_tick++;
            }
        }

        ~Simulator () {
            for (auto obj : objects) {
                delete obj;
            }
        }
};