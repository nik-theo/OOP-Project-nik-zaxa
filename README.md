<p align="center"  style="font-size: 55px;">
     SELF-DRIVING CAR PROJECT 🚘
</p>

<div align="center" style="font-size: 45px;">
 ΖΑΧΑΡΑΤΟΣ ΔΗΜΗΤΡΗΣ 1115202400048

 ΘΕΟΔΟΣΟΠΟΥΛΟΣ ΝΙΚΗΦΟΡΟΣ 1115202400054
</div>

## Build 
This program can be compiled with the following command:
```shell 
g++ -std=c++17 -O3 -Wall main.cpp -o sdc
```
💡 ``` -std=c++17 ``` is used to determine the language standard to be used.

💡 ``` -O3 ``` is the flag for optimization.

💡 ``` -Wall ``` is used to detect all possible errors in the code.

💡 ``` main.cpp ``` is the input file containing the code I want to compile.

💡 ``` -o sdc ``` is the final executable file.

## Usage 
This program can be executed in this manner:

❗ For the --help parameter which will print instructions for using the program you must execute the following command:
```shell
./sdc --help
```

❗ To run the program you must execute the following command:
```shell
./sdc
```
Followed by any of the following parameters based on the simulation you want to create and execute:
```shell 
--seed <n>                        Random seed ( default : current time)" 
--dimX <n>                        World width ( default : 40)" 
--dimY <n>                        World height ( default : 40)" 
--numMovingCars <n>               Number of moving cars ( default : 3)" 
--numMovingBikes <n>              Number of moving bikes ( default : 4)" 
--numParkedCars <n>               Number of parked cars ( default : 5)" 
--numStopSigns <n>                Number of stop signs (default : 2)" 
--numTrafficLights <n>            Number of traffic lights ( default : 2)" 
--simulationTicks <n>             Maximum simulation ticks ( default : 100)" 
--minConfidenceThreshold <n>      Minimum confidence cutoff ( default : 0.4)" 
--gps <x1> <y1> [x2 y2 . . .]     GPS target coordinates (required)" 
--pov <n>                         POV for visualization (default : front)" 
```
Usage example:
```shell
./sdc --seed 12 --dimX 40 --dimY 40 --numTrafficLights 60 --numStopSigns 30 --numMovingBikes 30 --minConfidenceThreshold 0.2 --pov center --gps 20 15 22 1
```

## Coding methods
The code has been divided into modules to hide information and to make the code easier to edit and read.
#### 🛠️ Data Representation & Structures
● Grid: Represented as a two-dimensional Cartesian space. Εach position is represented as two points in the Cartesian space $(x,y)$ and the distances are calculated using the ManhattanDistance method. All the objects of the simulation are displayed in the grid. 

The symbols of each object:
    
    Self-Driving Car -> @  

    Empty Cell -> .

    Outside Bounds -> X

    RED Light -> R

    YELLOW Light -> Y

    Stop Sign -> S

    Moving Bike -> B

    Moving Car -> C

    GREEN Light -> G

    Parked Car -> P

    Unknown/Other Object -> ?

● Inheritance: There is a base class named WorldObject. It is an abstract class that defines the properties such as ID, position, glyph.

There are also subclasses. Objects are divided into static such as TrafficLight or StopSign and moving such as MovingObject(Moving Cars, Bikes) . This allows the SimulationManager to manage them polymorphically using a ``` std::vector<WorldObject*> ```

● Sensor System: Each sensor (Lidar, Radar, Camera) inherits from a common base, but implements its own scan() method. Each sensor returns a SensorReading structure.

#### 🛠️ Design Assumptions
● Update Cycle: At each time step, ie at each tick, all objects in the world are first updated. Updates include traffic lights changing color, moving vehicles changing position etc. Then the autonomous vehicle makes decisions. This order ensures that the self-driving car's decisions are based on the latest state of the environment.

● Display: In case two objects are at the same cell, at the same time, the self-driving car(@) always has priority on display.  

The user also has the choice to select the POV of the simulation by giving the corresponding argument. The available POVs are ```front``` and ```center``` and there is a full grid display at the beginning of the simulation and after the self-driving car reaches its target.

● Movement: The movement is done in discrete steps. Each step is either 1 or 2 cells and there are different speed states(```STOPPED, HALF_SPEED, FULL_SPEED```).

#### 🛠️ Navigation Logic
● Sensor Fusion: The fusion methodology uses a weighted average based on confidence. 

A very important assumption in sensor fusion is that if a sensor finds a traffic light, this information can't be falsified by sensors that do not see the color. The assumptions are also true for the stop signs and for the passing bikes.

● Vehicle Behavior: The vehicle operates based on different states. The transition from one speed to another depends on "flags" that caute you for danger.

● Bounds Checking: The vehicle constantly checks whether its next move is within the boundaries of the ```Grid```. If not so, the simulation is terminated immediately.

#### 🛠️ Other Features of the Code

● srand(): Usage of the ``` srand() ``` function with a seed (``` --seed ```) from the user to create reproducible experiments.

● Memory: Use of pointers for polymorphic management of objects, with careful memory release to avoid leaks. 

## Partial Description of Methodology
1️⃣ ***Creating the environment(Grid)***

The first step was to create a coordinate system with two dimensions $(x,y)$ and then define the following:

✅ Representation: All objects are inherited from the base class ```WorldObject```.

✅ Assumption: SDC has priority in display regardless of the fact that each grid cell can accommodate an object.

✅ Movement: The vehicle moves first in the X axis and then in the Y axis to reach its destination, using Manhattan distance.  

2️⃣ ***Sensors***

There are three different types of sensors: 

📡 Lidar & Radar: They focus on the accuracy of the distance and speed of obstacles.

📡 Camera: The only sensor that can recognize sign text and traffic light colors.

3️⃣ ***Sensor Fusion***

The ```SensorFusionEngine``` processes the multiple measurements. Measurements might be conflicting in some cases.

📡 Weighted Average: The final distance is estimated by averaging the measurements, weighted by the confidence level of each sensor. 

📡 Color Information: The implementation ensures that if camera detects color, this information is not erased by the other sensors that do not see color.

4️⃣ ***Decision Making***

The vehicle operates based on different states. The ```makeDecision``` function analyzes the merged data and chooses the speed of the self-driving car.

⏲ If a red light or obstacle is detected three cells away, the speed drops to ``` HALF_SPEED ```.

🛑 If the red light or obstacle is detected one cell away, the vehicle stops ``` (STOPPED) ```.

⏲ The vehicle constantly checks the data and once the traffic light turns green, the vehicle starts moving again.

5️⃣ ***Execute Movement***

🚗💨 Depending on the ``` speedState ```, the vehicle moves 1 or 2 cells per tick.

## Files 

📂 ```Grid.h```: Contains the management of the visualization of the world in a grid format, supporting different points of view (POV) for displaying the simulation.

📂 ```main.cpp```: It is the input file containing the code the user wants to compile and takes charge of reading the parameters, initializing the world, and executing the main loop of the simulation.

📂 ```Navigation.h```: Contains the data fusion engine (```SensorFusionEngine```) and the ```SelfDrivingCar``` class, where movement and speed decisions are made.

📂 ```position.h```: Contains the definition of the basic Position coordinate structure and the method for calculating the Manhattan distance between two points on the grid.

📂 ```Sensors.h```: Contains the implementation of the sensor classes (```Lidar, Radar, Camera```), which simulate object detection with different ranges and accuracy.

📂 ```WorldObjects.h```: Contains the hierarchy of classes for all objects in the world (static and moving), defining their properties and behavior at each time step.