# Driving Sim: Part Two

## Rules:
1)  Valid Traffic Control Commands: 'green', 'yellow', 'red', 'left turn green', or enter to skip
2)  Valid Semi Commands: 'forward', 'right', 'left', 'stop', or enter to skip
3)  Valid SUV Commands: 'forward', 'right', 'left', 'stop', 'run over pinto', or enter to skip
4)  If the vehicle is already moving forward, you cannot issue another forward command
5)  The vehicle cannot move forward if the light is red
6)  Only one traffic signal can be on at a given time
7)  The vehicle can only turn left if the light is left turn green
8)  After coming to a stop, the Semi can only move forward
9)  The SUV can only run over a Ford Pinto if it was already moving forward
10) After each vehicle command, the traffic light will be reset to green
11) Enter 'quit' to exit this hyper-realistic simulation!

## To Compile:
g++ main.cpp Game.cpp Vehicle.cpp Semi.cpp SUV.cpp TrafficController.cpp -o main