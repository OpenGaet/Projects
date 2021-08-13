#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>


enum Direction { forward, right, left, stop, invalid };

class Vehicle {
    public:
        std::string getType() { return type; }
        Direction getState() { return state; }
        virtual bool setState(std::string);
        void printState();
        void moveErr() { std::cout << "Error: The " << type << " cannot move forward right now." << std::endl; }
        void turnErr() { std::cout << "Error: The " << type << " cannot turn right now." << std::endl; }
        void stopErr() { std::cout << "Error: The " << type << " is already stopped." << std::endl; }
    protected:
        Direction state;
        std::string type;
        bool moveForward();
        bool stop();
        virtual bool turnRight();
        virtual bool turnLeft();
        virtual void err();
};

#endif