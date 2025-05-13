#include <string>

using namespace std;

#ifndef DTO_H
#define DTO_H

struct SaleRequest {
    string itemCode;
    int itemNum;
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