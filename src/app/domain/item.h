#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
    std::string itemCode;
    std::string name;
    int price;

public:
    void printItem();
    int calculatePrice(int num);
};

#endif // ITEM_H