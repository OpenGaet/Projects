#include <iostream>

#include "Semi.h"


bool Semi::turnRight() { 
    if (state == Direction::stop) {
        turnErr();
        return false;
    }
    else {
        state = Direction::right;
        return true;
    }
}

bool Semi::turnLeft() {
    if (state == Direction::stop) {
        turnErr();
        return false;
    }
    else {
        state = Direction::left;
        return true;
    }
}