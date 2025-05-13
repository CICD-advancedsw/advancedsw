#include "dvm.h"

#include <iostream>
#include <sstream>
using namespace std; // std namespace 사용 선언

DVM::DVM(int id, Location loc, list<OtherDVM> otherDvms, list<Item> itemList, map<Item, int> stockList, list<Sale> saleList)
    : dvmId(id), location(loc), dvms(otherDvms), items(itemList), stocks(stockList), sales(saleList) { }

string DVM::queryItems() {
    ostringstream oss;
    for (const auto& item : items) {
        oss << item.printItem() << "\n"; 
    }
    return oss.str();
}

string DVM::queryStocks(string itemCode, int count) {
    ostringstream oss;
    auto it = stocks.find(Item(itemCode, "", 0));
    
    if (it != stocks.end()) {
        int totalPrice = it->first.calculatePrice(count);
        // flag:this;item_code:xxx;total_price:xxx;item_name:xxx;count:xxx 형식으로 반환
        oss << "flag:this;"
            << "item_code:" << itemCode << ";"
            << "total_price:" << totalPrice << ";"
            << "item_name:" << it->first.printItem() << ";"
            << "count:" << count;
    } else {
        OtherDVM* nearestDvm = nullptr;
        int shortestDistance = INT_MAX;

        for (auto& dvm : dvms) {
            CheckStockRequest request{.item_code = itemCode, .item_num = count};
            CheckStockResponse response = dvm.findAvailableStocks(request);
            if (response.item_num > 0) {
                int distance = location.calculateDistance(dvm.getLocation());
                if (distance < shortestDistance) {
                    shortestDistance = distance;
                    nearestDvm = &dvm;
                }
            }
        }
        
        if (nearestDvm) {
            // flag:other;item_code:xxx;count:xxx;x:xxx;y:xxx 형식으로 반환
            oss << "flag:other;"
                << "item_code:" << itemCode << ";"
                << "count:" << count << ";"
                << "x:" << nearestDvm->getLocation().getX() << ";"
                << "y:" << nearestDvm->getLocation().getY();
        } else {
            // flag:not_available;item_code:xxx 형식으로 반환
            oss << "flag:not_available;"
                << "item_code:" << itemCode;
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
        if (dvm.getDvmId() == targetDvmId) {
            return make_pair(dvm.getLocation(), certcode);
        }
    }

    throw DVMNotFoundException(targetDvmId);
}

void DVM::saveSaleFromOther(string itemCode, int itemNum, string certCode) {
    Sale sale = Sale::createSaleUsingCertCode(SaleRequest{.itemCode = itemCode, .itemNum = itemNum}, certCode);
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