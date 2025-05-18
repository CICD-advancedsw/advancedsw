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
  Config::get().setPort(9000);
  Location loc(1, 2);

  // 더미 OtherDVM 리스트
  list<OtherDVM> otherDvms;
  otherDvms.emplace_back(2, Location(3, 4), "127.0.0.1", 9001);

  // 더미 Item 리스트
  std::list<Item> itemList;
  itemList.emplace_back("01", "콜라", 1500);
  itemList.emplace_back("02", "사이다", 1400);

  // 더미 stockList
  std::map<Item, int> stockList;
  for (const auto& item : itemList) {
      stockList[item] = 10;
  }

  // 빈 Sale 리스트
  std::list<Sale> saleList;

  DVM dvm(1, loc, stockList, itemList, saleList, otherDvms);
  Controller controller(&dvm);

  // controller 실행
  std::thread serverThread(&Controller::runServer, &controller);
  controller.run();
  serverThread.join();

  return 0;
}
