

#include <iostream>
#include <thread>
#include "application/otherdvm.h"
#include "application/sale.h"
#include "application/dvm.h"
#include "presentation/controller.h"
#include <list>
#include <map>

using namespace std;

int main()
{
    Config::get().setPort(9001);  // 예: 다른 DVM은 9001 포트 사용
    Location loc(3, 4);

    // 더미 OtherDVM 리스트
    list<OtherDVM> otherDvms;
    otherDvms.emplace_back(1, Location(1, 2), "127.0.0.1", 9000);

    // 더미 Item 리스트
    std::list<Item> itemList;
    itemList.emplace_back("01", "콜라", 1500);
    itemList.emplace_back("03", "녹차", 1300);

    // 더미 stockList
    std::map<Item, int> stockList;
    for (const auto& item : itemList) {
        stockList[item] = 5;
    }

    // 빈 Sale 리스트
    std::list<Sale> saleList;

    DVM dvm(2, loc, stockList, itemList, saleList, otherDvms);
    Controller controller(&dvm);

    // controller 실행
    std::thread serverThread(&Controller::runServer, &controller);
    controller.run();
    serverThread.join();

    return 0;
}