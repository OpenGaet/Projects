#ifndef TRAFFICCONTROLLER_H
#define TRAFFICCONTROLLER_H

#include <iostream>
#include <string>


enum Light { green, yellow, red, leftGreen };

class TrafficController {
    public:
        TrafficController() :  state(Light::green) {}
        Light getLightColor() { return state; }
        void printState();
        bool setState(std::string);
    protected:
        bool setGreen() { state = Light::green; return true; }
        bool setYellow() { state = Light::yellow; return true; }
        bool setRed() { state = Light::red; return true; }
        bool setLeftGreen() { state = Light::leftGreen; return true; }
        void err();
        Light state;
};

#endif