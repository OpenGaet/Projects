#ifndef GAME_H
#define GAME_H

#include <string>

#include "Semi.h"
#include "SUV.h"
#include "TrafficController.h"
#include "Vehicle.h"


class Game {
    public:
        void start();  
    private:
        void printInstructions();
        void getInput(std::string&);
        bool isValidMove(TrafficController&, Vehicle&, std::string&);
};

#endif