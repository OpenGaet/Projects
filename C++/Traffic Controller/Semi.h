#ifndef SEMI_H
#define SEMI_H

#include <string>

#include "Vehicle.h"


class Semi: public Vehicle {
    public:
        Semi() { type = "Semi Truck"; state = Direction::stop; }
        bool setState(std::string dir) { return Vehicle::setState(dir); }
    protected:
        bool turnRight();
        bool turnLeft();
        void err() { Vehicle::err(); }
};

#endif