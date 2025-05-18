#ifndef DVM_H
#define DVM_H

#include <string>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include "sale.h"
#include "../domain/location.h"
#include "../domain/item.h"
#include "sale.h"
#include "../dto.h"
#include "../exception/dvmexception.h"
#include "otherdvm.h"

using namespace std; // std namespace 사용 선언

class DVM {
private:
    int dvmId;
    Location location;
    map<Item, int> stocks;
    list<Item> items;
    list<Sale> sales;
    list<OtherDVM> dvms;

    // 아이템 코드로 재고에서 아이템을 찾는 메서드
    Item findItem(const string& itemCode) const;
    
    // 재고를 감소시키는 메서드
    void decreaseStock(const string& itemCode, int count);

public:
    DVM(int id, Location loc, map<Item, int> stockList, list<Item> itemList, list<Sale> saleList, list<OtherDVM> otherDvMs); // 생성자를 통한 의존성 주입    
    // 자판기의 아이템 목록을 조회
    string queryItems();
    
    // 특정 아이템의 재고를 조회
    string queryStocks(string itemCode, int count);
    
    // 주문 요청
    void requestOrder(SaleRequest request);
    
    // 특정 자판기에 주문 요청
    pair<Location, string> requestOrder(int targetDvmId, SaleRequest request);
    
    // 다른 자판기로부터의 판매 정보 저장
    void saveSaleFromOther(string itemCode, int itemNum, string certCode);
    
    // 선결제된 아이템 처리
    bool processPrepaidItem(string certCode);

    // getter 추가
    Location getLocation() const;
    const map<Item, int>& getStocks() const;
    const int getDvmId() const;
};

#endif // DVM_H