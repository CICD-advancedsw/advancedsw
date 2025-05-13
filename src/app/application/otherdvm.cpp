#include "otherdvm.h"
#include <iostream>

OtherDVM::OtherDVM(int id, const Location& loc)
    : dvmId(id), location(loc) {}

CheckStockResponse OtherDVM::findAvailableStocks(const CheckStockRequest& request) {
    // 임시 구현
    return CheckStockResponse{
        .item_code = request.item_code,
        .item_num = request.item_num,
        .coor_x = location.getX(),
        .coor_y = location.getY()
    };
}

bool OtherDVM::askForPrepayment() {
    return true; // 임시
}

const Location& OtherDVM::getLocation() const {
    return location;
}

int OtherDVM::getDvmId() const {
    return dvmId;
}