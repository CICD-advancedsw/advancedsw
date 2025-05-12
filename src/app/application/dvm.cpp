#include "dvm.h"
#include "../dto/salerequest.h" // SaleRequest 헤더 추가
#include "sale.h" // Sale 클래스 헤더 추가
#include "dvmexception.h" // DVMNotFoundException 헤더 추가
#include <iostream>
#include <sstream>
using namespace std; // std namespace 사용 선언

DVM::DVM(int id, Location loc, list<OtherDVM> otherDvms, list<Item> itemList, map<Item, int> stockList, list<Sale> saleList)
    : dvmId(id), location(loc), dvms(otherDvms), items(itemList), stocks(stockList), sales(saleList) { }

string DVM::queryItems() {
    ostringstream oss;
    for (const auto& item : items) {
        oss << item << "\n"; 
    }
    return oss.str();
}

string DVM::queryStocks(string itemCode, int count) {
    ostringstream oss;
    auto it = stocks.find(Item(itemCode, "", 0)); // 빈 이름과 가격 0으로 Item 생성
    if (it != stocks.end()) {
        int totalPrice = it->first.calculatePrice(count);
        oss << "음료 가격 총 " << totalPrice << "원 (" << it->first << "* " << count << ")";
    } else {
        oss << "현재 해당 자판기에서 구매가 불가합니다.\n";

        OtherDVM* nearestDvm = nullptr;
        int shortestDistance = INT_MAX;

        for (auto& dvm : dvms) {
            if (dvm.findAvailableStocks(itemCode, count)) {
                int distance = location.calculateDistance(dvm.getLocation());
                if (distance < shortestDistance) {
                    shortestDistance = distance;
                    nearestDvm = &dvm;
                }
            }
        }
        if (nearestDvm) {
            int x = nearestDvm->getLocation().getX();
            int y = nearestDvm->getLocation().getY();
            oss << "(" << x << "," << y << ") 위치의 자판기에서 구매가 가능합니다.\n";
        } else {
            oss << "해당 음료를 제공할 수 있는 자판기가 없습니다.\n";
        }
    }
    return oss.str();
}

void DVM::requestOrder(SaleRequest request) {
    Sale sale = Sale::createStandaloneSale(request);
    sales.push_back(sale);
}

pair<Location, string> DVM::requestOrder(int targetDvmId, SaleRequest request) {
    auto [sale, certcode] = Sale::createSaleForDvm(request, targetDvmId);
    sales.push_back(sale);

    for (const auto& dvm : dvms) {
        if (dvm.getId() == targetDvmId) {
            return make_pair(dvm.getLocation(), certcode);
        }
    }

    throw DVMNotFoundException(targetDvmId);
}

void DVM::saveSaleFromOther(string itemCode, int itemNum, string certCode) {
    Sale sale = Sale::createSaleUsingCertCode(SaleRequest{.itemCode = itemCode, .itemNum = itemNum, .card = ""}, certCode);
    sales.push_back(sale);
}

bool DVM::processPrepaidItem(string certCode) {
    for (auto& sale : sales) {
        if (sale.receivePrepaidItem(certCode)) {
            return true;
        }
    }
    return false;
}

