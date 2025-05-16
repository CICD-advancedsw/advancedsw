#include "location.h"
#include <iostream>

using namespace std;

Location::Location(int x, int y) : x(x), y(y) {

}

int Location::calculateDistance(Location otherLocation) const {
    return abs(x - otherLocation.x) + abs(y - otherLocation.y);
}

int Location::getX() const {
    return x;
}

int Location::getY() const {
    return y;
}
