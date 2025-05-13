#ifndef OTHERDVM_H
#define OTHERDVM_H

#include <string>
#include "../domain/location.h"

using namespace std;

class OtherDVM {
private:
    int dvmId;
    Location location;

public:
    OtherDVM(int id, const Location& loc);
    bool findAvailableStocks(const std::string& itemCode);
    bool askForPrepayment();
    const Location& getLocation() const;
    int getDvmId() const;
};

#endif // OTHERDVM_H