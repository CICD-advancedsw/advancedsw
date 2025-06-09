#include "dvm.h"
#include <climits>

DVM::DVM(int id, Location loc, map<Item, int> stockList, list<Item> itemList, list<Sale> saleList, list<OtherDVM> otherDvMs)
    : dvmId(id), location(loc), stocks(stockList), items(itemList), sales(saleList), dvms(otherDvMs) { }

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
    for (const auto& [code, name] : ItemDictionary::get()) {
        Item item(code, name, 0);
        auto it = stocks.find(item);
        if (it != stocks.end()) {
            oss << it->first.printItem() << "\n";
        } else {
            oss << item.printItem() << "\n";
        }
    }
    return oss.str();
}

string DVM::queryStocks(string itemCode, int count) {
    ostringstream oss;
    auto it = stocks.find(Item(itemCode, "", 0));
    
    if (it != stocks.end() && it->second >= count) {
        Item item = findItem(itemCode);  // 정확한 가격과 이름 가진 item을 리턴
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
            //target 추가
            // flag:other;item_code:xxx;count:xxx;x:xxx;y:xxx;target:xxx 형식으로 반환
            oss << "flag:other;"
                << "item_code:" << itemCode << ";"
                << "count:" << count << ";"
                << "x:" << nearestDvm->getLocation().getX() << ";"
                << "y:" << nearestDvm->getLocation().getY() << ";"
                << "target: " << nearestDvm->getDvmId();
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
    for (auto& dvm : dvms) {
        if (dvm.getDvmId() == targetDvmId) {
            auto [sale, certcode] = Sale::createSaleForDvm(request, targetDvmId);
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

Location DVM::getLocation() const{
    return location;
}
const map<Item, int>& DVM::getStocks() const{
    return stocks;
}
const int DVM::getDvmId() const{
    return dvmId;
}
