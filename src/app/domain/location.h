#ifndef LOCATION_H
#define LOCATION_H

class Location
{
private:
    int x;
    int y;

public:
    Location(int x = 0, int y = 0);
    int calculateDistance(Location otherLocation) const;
    int getX() const;
    int getY() const;
};

#endif // LOCATION_H