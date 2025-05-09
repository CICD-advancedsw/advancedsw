#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include "../domain/location.h"
#include "../domain/item.h"

class Controller {
private:
    Location location;
    std::map<Item, int> stocks;

public:
    Controller();
    ~Controller();
    void run();
};

#endif