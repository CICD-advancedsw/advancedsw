#ifndef DVM_H
#define DVM_H

#include <string>
#include <map>
#include <list>
#include "../domain/location.h"
#include "../domain/item.h"

class Sale;
class OtherDVM;

class DVM {
private:
    int dvmId;
    Location location;
    std::map<Item, int> stocks;
    std::list<Item> items;
    std::list<Sale> sales;
    std::list<OtherDVM> dvms;

public:
    // 자판기의 아이템 목록을 조회
    std::string queryItems();
    
    // 특정 아이템의 재고를 조회
    std::string queryStocks(std::string itemCode);
    
    // 주문 요청
    void requestOrder();
    
    // 특정 자판기에 주문 요청
    std::string requestOrder(int targetDvmId);
    
    // 다른 자판기로부터의 판매 정보 저장
    void saveSaleFromOther(std::string itemCode, int itemNum, std::string certCode);
    
    // 선결제된 아이템 처리
    std::string processPrepaidItem(std::string certCode);
};

#endif // DVM_H