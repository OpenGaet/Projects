#ifndef SUV_H
#define SUV_H

#include <string>

#include "Vehicle.h"


class SUV: public Vehicle {
    public:
        SUV() { type = "SUV"; state = Direction::stop; }
        bool runOverPinto();
        bool setState(std::string);
    protected:
        bool turnRight();
        bool turnLeft();
        void pintoErr() { std::cout << "Error: The " << type << " cannot run over a Ford Pinto right now." << std::endl; }
        void err();
};

#endif