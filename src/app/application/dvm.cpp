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

string DVM::buildThisResponse(const string& itemCode, int count) {
    Item item = findItem(itemCode);
    int totalPrice = item.calculatePrice(count);
    ostringstream oss;
    oss << "flag:this;"
        << "item_code:" << itemCode << ";"
        << "total_price:" << totalPrice << ";"
        << "item_name:" << item.printItem() << ";"
        << "count:" << count;
    return oss.str();
}

string DVM::buildOtherResponse(const string& itemCode, int count, OtherDVM* nearestDvm) {
    if (!nearestDvm) {
        return "flag:not_available;item_code:" + itemCode;
    }
    
    ostringstream oss;
    oss << "flag:other;"
        << "item_code:" << itemCode << ";"
        << "count:" << count << ";"
        << "x:" << nearestDvm->getLocation().getX() << ";"
        << "y:" << nearestDvm->getLocation().getY() << ";"
        << "target: " << nearestDvm->getDvmId();
    return oss.str();
}

OtherDVM* DVM::findNearestDvmWithStock(const string& itemCode, int count) {
    OtherDVM* nearestDvm = nullptr;
    int shortestDistance = INT_MAX;

    for (auto& dvm : dvms) {
        CheckStockRequest request{.item_code = itemCode, .item_num = count};
        CheckStockResponse response = dvm.findAvailableStocks(request, dvmId);
        
        if (response.item_num <= 0) {
            continue;
        }
        
        int distance = location.calculateDistance(dvm.getLocation());
        if (distance < shortestDistance) {
            shortestDistance = distance;
            nearestDvm = &dvm;
        }
    }
    
    return nearestDvm;
}

string DVM::queryStocks(string itemCode, int count) {
    auto it = stocks.find(Item(itemCode, "", 0));
    
    if (it != stocks.end() && it->second >= count) {
        return buildThisResponse(itemCode, count);
    }
    
    OtherDVM* nearestDvm = findNearestDvmWithStock(itemCode, count);
    return buildOtherResponse(itemCode, count, nearestDvm);
}

void DVM::requestOrder(SaleRequest request) {
    Item item = findItem(request.itemCode);
    request.item = item;
    decreaseStock(request.itemCode, request.itemNum);
    Sale sale = Sale::createStandaloneSale(request);
    sales.push_back(sale);
}

OtherDVM* DVM::findDvmById(int targetDvmId) {
    for (auto& dvm : dvms) {
        if (dvm.getDvmId() == targetDvmId) {
            return &dvm;
        }
    }
    return nullptr;
}

pair<Location, string> DVM::requestOrder(int targetDvmId, SaleRequest request) {
    OtherDVM* targetDvm = findDvmById(targetDvmId);
    if (!targetDvm) {
        throw DVMNotFoundException(targetDvmId);
    }
    
    auto [sale, certcode] = Sale::createSaleForDvm(request, targetDvmId);
    askPrepaymentRequest askRequest{
        .item_code = request.itemCode,
        .item_num = request.itemNum,
        .cert_code = certcode
    };
    askPrepaymentResponse response = targetDvm->askForPrepayment(askRequest, dvmId);
    if (!response.availability) {
        throw runtime_error("Prepayment not available");
    }
    return make_pair(targetDvm->getLocation(), certcode);
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
