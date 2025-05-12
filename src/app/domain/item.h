#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
private:
    std::string itemCode;
    std::string name;
    int price;

public:
    Item(std::string itemCode, std::string name, int price);
    std::string printItem() const;
    int calculatePrice(int num);

    // Overload the < operator for comparison => list 사용을 위한 함수 정의라 설계에 안 넣어도 되지 않나
    bool operator<(const Item& other) const {
        return itemCode < other.itemCode; // Compare based on id
    }
};

#endif // ITEM_H