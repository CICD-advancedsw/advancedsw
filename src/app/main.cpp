#include "application/dvm.h"
#include "presentation/controller.h"
#include <list>
#include <map>

int main(void) {
    // 더미 Location
    Location loc(1, 2);

    // 더미 OtherDVM 리스트
    std::list<OtherDVM> otherDvms;
    otherDvms.emplace_back(2, Location(3, 4));
    otherDvms.emplace_back(3, Location(5, 6));

    // 더미 Item 리스트
    std::list<Item> itemList;
    itemList.emplace_back("A001", "콜라", 1500);
    itemList.emplace_back("A002", "사이다", 1400);

    // 더미 stockList
    std::map<Item, int> stockList;
    for (const auto& item : itemList) {
        stockList[item] = 10;
    }

    // 빈 Sale 리스트
    std::list<Sale> saleList;

    DVM dvm(1, loc, otherDvms, itemList, stockList, saleList);
    Controller controller(&dvm);
    controller.run(); 

    return 0;
}