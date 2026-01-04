#include <iostream>
#include <vector>
#include <map>
#include <string>
// + header files from Sensor
using namespace std;


class SensorFusion{
    private:
        double minConfidenceThreshold;

    public:
        SensorFusion(double min): minConfidenceThreshold(min){}

        vector<Sensor_Reading> fuseSensorData(vector<vector<Sensor_Reading>>& allData){

        }

};




















//ΓΙΑ ΤΗΝ MAIN

// // 1. Κάθε αισθητήρας κάνει τη δουλειά του και επιστρέφει ένα απλό vector
// vector<SensorReading> lidarData = myLidar.see(worldObjects);   
// vector<SensorReading> radarData = myRadar.see(worldObjects);   
// vector<SensorReading> cameraData = myCamera.see(worldObjects); 

// // 2. Τώρα πρέπει να τα στείλεις όλα ΜΑΖΙ στη μηχανή συγχώνευσης (Fusion)
// // Φτιάχνουμε το "διπλό" vector
// vector<vector<SensorReading>> allData;

// // Βάζουμε μέσα τις λίστες
// allData.push_back(lidarData);  
// allData.push_back(radarData);   
// allData.push_back(cameraData); 

// // 3. Καλούμε τη συνάρτηση
// fusionEngine.fuseSensorData(allData);