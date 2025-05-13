#include <string>
#include "domain/item.h"

using namespace std;

#ifndef DTO_H
#define DTO_H

struct SaleRequest {
    string itemCode;
    int itemNum;
    Item item;  // 판매할 아이템 정보
};

struct CheckStockRequest {
    string item_code;
    int item_num;
};

struct CheckStockResponse {
    string item_code;
    int item_num;
    int coor_x;
    int coor_y;
};

#endif 