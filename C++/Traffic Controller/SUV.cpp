#include "SUV.h"


bool SUV::turnRight() {
    state = Direction::right;
    return true;
}

bool SUV::turnLeft() {
    state = Direction::left;
    return true;
}

void SUV::err() { 
    std::cout << "Error: Invalid command." << std::endl;
    std::cout << "Valid commands: 'forward', 'right', 'left', 'stop', 'run over pinto', or enter to skip" << std::endl; 
}

bool SUV::runOverPinto() {
    if (state == forward) {
        std::cout << std::endl << "GASP! You just ran over a Ford Pinto and didn't bat an eye!" << std::endl;
        return true;
    }
    else {
        pintoErr();
        return false;
    }
}

bool SUV::setState(std::string dir) {
    if (dir == "run over pinto")
        return runOverPinto();
    else
        return Vehicle::setState(dir);
}