#include "TrafficController.h"


void TrafficController::printState() {
    std::cout << "The light is";
    
    switch (state) {
        case Light::green :
            std::cout << " green" << std::endl;
            break;
        case Light::yellow :
            std::cout << " yellow" << std::endl;
            break;
        case Light::red :
            std::cout << " red" << std::endl;
            break;
        case Light::leftGreen :
            std::cout << " left-turn green" << std::endl;
            break;
    }
}

void TrafficController::err() { 
    std::cout << "Error: Invalid command." << std::endl;
    std::cout << "Valid commands: 'green', 'yellow', 'red', 'left turn green', enter to skip" << std::endl; 
}

bool TrafficController::setState(std::string color) {
    if (color == "green") 
        return setGreen();
    else if (color == "yellow") 
        return setYellow();
    else if (color == "red")
        return setRed();
    else if (color == "left turn green") 
        return setLeftGreen();
    else if (color == "")
        return true;
    else {
        err();
        return false;
    }
}