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
            vector<Sensor_Reading> totalResult; // για να αποθηκευει τις τελικες μετρησεις
            map<int, vector<Sensor_Reading>> group;  
            // ομαδοποιουμε ολα τα objId με τις μετρησεις τους σε ενα map
            for (auto& readings : allData){
                for (auto& read : readings){
                    group[read.objId].push_back(read);
                }
            }
            // για καθε αντικειμενο 
            for (auto& [id, data] : group){

                Sensor_Reading result;
                //αρχικοποιουμε τους μετρητες
                double totaldist = 0.0;
                double totalspeed = 0.0;
                double totalconf = 0.0;
                double maxconf = -1.0;
                result.objectId = id;
                
                bool seesbike = false;
                //για ολα τα στοιχεια καθε αντικειμενου 
                for (auto& r : data){
                    //τσεκαρουμε αν ειναι ποδηλατο
                    if (r.type == "BIKE") {
                        seesbike = true;
                    }
                    // βρισκουμε την ολικη αποσταση με βαροι
                    if (r.distance != -1){
                        totaldist += r.distance * r.confidence; //λογο του weighted average
                    }
                    //βρισκουμε την ολικη ταχυτητα με βαροι
                    if (r.speed != -1){
                        totalspeed += r.speed *r.confidence;
                    }
                    totalconf += r.confidence;
                    // για πραγματα που δεν μπορουμε να παρουμε μεσο ορο παιρνουμε απλα αυτο που ειχε την υψηλοτερη βεβαιοτητα
                    if (r.confidence > maxconf){
                        maxconf = r.confidence;
                        result.type = r.type;
                        result.signText = r.signText;
                        result.trafficLight = r.trafficLight;
                        result.position = r.position;
                    }

                }

                double averageconf = totalconf/ data.size();
                // αμα δεν παιρναει το minConfidenceThreshold δεν το αποθηκευουμε εκτος αν ειναι τυπος ποδηλατο
                if (averageconf < minConfidenceThreshold && !seesbike){
                    continue;
                }
                // ελεγχος για διαιρεση με 0
                if (totalconf > 0){
                    result.distance = totaldist / totalconf;
                    result.speed = totalspeed / totalconf;
                } else {
                    result.distance = -1;
                    result.speed = 0;
                }
                totalResult.push_back(result);
            }
            return totalResult;
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