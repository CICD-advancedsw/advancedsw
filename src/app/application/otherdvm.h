#ifndef OTHERDVM_H
#define OTHERDVM_H

#include <string>
#include "../domain/location.h"

class OtherDVM {
private:
    int dvmId;
    Location location;

public:
    bool findAvailableStocks();
    bool askForPrepayment();
    Location getLocation();
    int getDvmId();
};

#endif // OTHERDVM_H