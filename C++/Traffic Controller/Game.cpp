#include <iostream>
#include <string>

#include "Game.h"


// Print Game Instructions
void Game::printInstructions() {
    std::cout << std::endl << "Rules:" << std::endl << std::endl;
    std::cout << "1)  Valid Traffic Control Commands: 'green', 'yellow', 'red', 'left turn green', or enter to skip" << std::endl;
    std::cout << "2)  Valid Semi Commands: 'forward', 'right', 'left', 'stop', or enter to skip" << std::endl;
    std::cout << "3)  Valid SUV Commands: 'forward', 'right', 'left', 'stop', 'run over pinto', or enter to skip" << std::endl;
    std::cout << "4)  If the vehicle is already moving forward, you cannot issue another forward command" << std::endl;
    std::cout << "5)  The vehicle cannot move forward if the light is red" << std::endl;
    std::cout << "6)  Only one traffic signal can be on at a given time" << std::endl; 
    std::cout << "7)  The vehicle can only turn left if the light is left turn green" << std::endl;
    std::cout << "8)  After coming to a stop, the Semi can only move forward" << std::endl;
    std::cout << "9)  The SUV can only run over a Ford Pinto if it was already moving forward" << std::endl;
    std::cout << "10) After each vehicle command, the traffic light will be reset to green" << std::endl;
    std::cout << "11) Enter 'quit' to exit this hyper-realistic simulation!" << std::endl << std::endl;
}

// Assign user input to variable and exit program if 'quit' is entered
void Game::getInput(std::string& input) {
    std::getline(std::cin, input);
    if (input == "quit")
        exit(0);
}

// Checks input against game and vehicle rules
bool Game::isValidMove(TrafficController& tc, Vehicle& v, std::string& input) {
    if (tc.getLightColor() != leftGreen && input == "left") {
        v.turnErr();
        return false;
    }
    else if (tc.getLightColor() == red && input == "forward") {
        v.moveErr();
        return false;
    }
    else if (!v.setState(input)) {
        return false;
    }
    else {
        return true;
    }
}

// Runs the game
void Game::start() {
    std::cout << std::endl << "\t\tWelcome to 'Driving Sim 2020: Endless Nightmare'" << std::endl;
    printInstructions();

    SUV suv;
    Semi semi;
    
    TrafficController tc;
    std::string in,dir,light;

    // Game Loop
    while (1) {

        // Get traffic control command
        do {
            std::cout << "Traffic Light Command: ";
            getInput(light);
        } while (!tc.setState(light));

        // Get semi direction command
        do {
            std::cout << semi.getType() << " Direction: ";
            getInput(dir);
        } while (!isValidMove(tc, semi, dir));

        // Get suv direction command
        do {
            std::cout << suv.getType() << " Direction: ";
            getInput(dir);
        } while (!isValidMove(tc, suv, dir));

        // Print Game State
        std::cout << std::endl;
        tc.printState();
        semi.printState();
        suv.printState();
        std::cout << std::endl;

        // Reset light to green
        tc.setState("green");
    }
}