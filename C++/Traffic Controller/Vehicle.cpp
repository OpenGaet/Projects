#include <iostream>

#include "Vehicle.h"


bool Vehicle::moveForward() { 
    if (state == Direction::forward) {
        moveErr();
        return false;
    }
    else {
        state = Direction::forward;
        return true;
    }
}

bool Vehicle::turnRight() { 
    state = Direction::right;
    return true;
}

bool Vehicle::turnLeft() {
    state = Direction::left;
    return true;
}

bool Vehicle::stop() { 
    if (state == Direction::stop) {
        stopErr();
        return false;
    }
    else {
        state = Direction::stop;
        return true;
    }
}

void Vehicle::err() { 
    std::cout << "Error: Invalid command." << std::endl;
    std::cout << "Valid commands: 'forward', 'right', 'left', 'stop', enter to skip" << std::endl; 
}

void Vehicle::printState() { 
    std::cout << "The " << type;
    
    switch (state) {
        case Direction::forward :
            std::cout << " is moving forward" << std::endl;
            break;
        case Direction::right :
            std::cout << " has turned right" << std::endl;
            break;
        case Direction::left :
            std::cout << " has turned left" << std::endl;
            break;
        case Direction::stop :
            std::cout << " is stopped" << std::endl;
            break;
    }
}

bool Vehicle::setState(std::string dir) {
    if (dir == "forward") 
        return moveForward();
    else if (dir == "right")
        return turnRight();
    else if (dir == "left")
        return turnLeft();
    else if (dir == "stop")
        return stop();
    else if (dir == "")
        return true;
    else {
        err();
        return false;
    }
}