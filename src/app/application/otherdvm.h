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
    OtherDVM(int id, Location loc);
    bool findAvailableStocks(string itemCode, int itemNum);
    bool askForPrepayment();
    Location getLocation();
    int getDvmId(); // DVM ID를 반환하는 메서드

    // getId 메서드 추가
    int getId() const { return dvmId; }
};

#endif // OTHERDVM_H