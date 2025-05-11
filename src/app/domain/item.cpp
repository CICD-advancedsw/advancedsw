#include "item.h"

#include <iostream>

using namespace std;

Item::Item(string itemCode, string name, int price) : itemCode(itemCode), name(name), price(price)
{
    // 생성자
}

std::string Item::printItem() const
{
    return name + " (" + itemCode + ")";
}

int Item::calculatePrice(int num)
{
    return price * num;
}