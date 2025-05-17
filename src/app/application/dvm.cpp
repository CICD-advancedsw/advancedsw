#include "dvm.h"

#include <iostream>
#include <sstream>
using namespace std; // std namespace 사용 선언

DVM::DVM(int id, Location loc, list<OtherDVM> otherDvms, list<Item> itemList, map<Item, int> stockList, list<Sale> saleList)
    : dvmId(id), location(loc), dvms(otherDvms), items(itemList), stocks(stockList), sales(saleList) { }

// Private methods
void DVM::decreaseStock(const string& itemCode, int count) {
    auto it = stocks.find(Item(itemCode, "", 0));
    if (it == stocks.end() || it->second < count) {
        throw runtime_error("Insufficient stock");
    }
    stocks[it->first] -= count;
}

Item DVM::findItem(const string& itemCode) const {
    auto it = stocks.find(Item(itemCode, "", 0));
    if (it == stocks.end()) {
        throw runtime_error("Item not found");
    }
    return it->first;
}

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
    
    if (it != stocks.end() && it->second >= count) {
        Item item = it->first;
        int totalPrice = item.calculatePrice(count);
        // flag:this;item_code:xxx;total_price:xxx;item_name:xxx;count:xxx 형식으로 반환
        oss << "flag:this;"
            << "item_code:" << itemCode << ";"
            << "total_price:" << totalPrice << ";"
            << "item_name:" << item.printItem() << ";"
            << "count:" << count;
    } else {
        OtherDVM* nearestDvm = nullptr;
        int shortestDistance = INT_MAX;

        for (auto& dvm : dvms) {
            CheckStockRequest request{.item_code = itemCode, .item_num = count};
            CheckStockResponse response = dvm.findAvailableStocks(request, dvmId);
            if (response.item_num > 0) {
                int distance = location.calculateDistance(dvm.getLocation());
                if (distance < shortestDistance) {
                    shortestDistance = distance;
                    nearestDvm = &dvm;
                }
            }
        }
        
        if (nearestDvm) {
            // 지원 수정 : targetDvmid를 반환해줘야 함
            // flag:other;item_code:xxx;count:xxx;target:xxx 형식으로 반환
            oss << "flag:other;"
                << "item_code:" << itemCode << ";"
                << "count:" << count << ";"
                << "target:" << nearestDvm->getDvmId() << ";";
        } else {
            // flag:not_available;item_code:xxx 형식으로 반환
            oss << "flag:not_available;"
                << "item_code:" << itemCode;
        }
    }
    return oss.str();
}

void DVM::requestOrder(SaleRequest request) {
    Item item = findItem(request.itemCode);
    request.item = item;
    decreaseStock(request.itemCode, request.itemNum);
    Sale sale = Sale::createStandaloneSale(request);
    sales.push_back(sale);
}

pair<Location, string> DVM::requestOrder(int targetDvmId, SaleRequest request) {
    Item item = findItem(request.itemCode);
    request.item = item;
    decreaseStock(request.itemCode, request.itemNum);
    auto [sale, certcode] = Sale::createSaleForDvm(request, targetDvmId);
    sales.push_back(sale);

    for (auto& dvm : dvms) {
        if (dvm.getDvmId() == targetDvmId) {
            askPrepaymentRequest askRequest{
                .item_code = request.itemCode,
                .item_num = request.itemNum,
                .cert_code = certcode
            };
            askPrepaymentResponse response = dvm.askForPrepayment(askRequest, dvmId);
            if (!response.availability) {
                throw runtime_error("Prepayment not available");
            }
            return make_pair(dvm.getLocation(), certcode);
        }
    }
    throw DVMNotFoundException(targetDvmId);
}

void DVM::saveSaleFromOther(string itemCode, int itemNum, string certCode) {
    Item item = findItem(itemCode);
    decreaseStock(itemCode, itemNum);
    SaleRequest request{
        .itemCode = itemCode,
        .itemNum = itemNum,
        .item = item
    };
    
    Sale sale = Sale::createSaleUsingCertCode(request, certCode);
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