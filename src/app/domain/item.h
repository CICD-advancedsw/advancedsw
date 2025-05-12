#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

class Item {
private:
    std::string itemCode;
    std::string name;
    int price;

public:
    Item(const std::string& code, const std::string& name, int price)
        : itemCode(code), name(name), price(price) {}

    void printItem();
    int calculatePrice(int num) const;

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << "ItemCode: " << item.itemCode << ", Name: " << item.name << ", Price: " << item.price;
        return os;
    }
};

#endif // ITEM_H