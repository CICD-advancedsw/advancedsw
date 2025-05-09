#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include "../domain/location.h"
#include "../domain/item.h"
#include "../application/dvm.h"

class Controller {
private:
    Location location;
    std::map<Item, int> stocks;
    DVM* dvm;

public:
    Controller(DVM* dvm);
    ~Controller();
    void run();
};

#endif