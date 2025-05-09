#ifndef LOCATION_H
#define LOCATION_H

class Location {
private:
    int x;
    int y;

public:
    int calculateDistance(Location otherLocation);
    int getX();
    int getY();
};

#endif // LOCATION_H