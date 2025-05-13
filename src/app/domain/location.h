#ifndef LOCATION_H
#define LOCATION_H

class Location {
private:
    int x;
    int y;

public:
    // todo : 우리 좌표로 변경 필요
    Location(int x = 0, int y = 0);
    int calculateDistance(Location otherLocation);
    int getX();
    int getY();
};

#endif // LOCATION_H