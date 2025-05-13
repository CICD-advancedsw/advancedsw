#include "otherdvm.h"
#include <iostream>

OtherDVM::OtherDVM(int id, const Location& loc)
    : dvmId(id), location(loc) {}

bool OtherDVM::findAvailableStocks(const std::string& itemCode) {
    return true; // 임시
}

bool OtherDVM::askForPrepayment() {
    return true; // 임시
}

const Location& OtherDVM::getLocation() const {
    return location;
}