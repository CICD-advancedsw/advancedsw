#ifndef DVM_H
#define DVM_H

#include <string>
#include <map>
#include <list>
#include "../domain/location.h"
#include "../domain/item.h"
#include "../domain/sale.h"
#include "../dto/salerequest.h"
#include "../exception/dvmexception.h"
#include "otherdvm.h"

using namespace std; // std namespace 사용 선언

class Sale;
class SaleRequest; // SaleRequest 클래스 전방 선언

class DVM {
private:
    int dvmId;
    Location location;
    map<Item, int> stocks;
    list<Item> items;
    list<Sale> sales;
    list<OtherDVM> dvms;

public:
    DVM(int id, Location loc, list<OtherDVM> otherDvMs, list<Item> itemList, map<Item, int> stockList, list<Sale> saleList); // 생성자를 통한 의존성 주입
    
    // 자판기의 아이템 목록을 조회
    string queryItems();
    
    // 특정 아이템의 재고를 조회
    string queryStocks(string itemCode, int count);
    
    // 주문 요청
    void requestOrder(SaleRequest request); // SaleRequest를 파라미터로 받는 메서드로 수정
    
    // 특정 자판기에 주문 요청
    pair<Location, string> requestOrder(int targetDvmId, SaleRequest request); // SaleRequest를 파라미터로 받는 메서드로 수정
    
    // 다른 자판기로부터의 판매 정보 저장
    void saveSaleFromOther(string itemCode, int itemNum, string certCode);
    
    // 선결제된 아이템 처리
    bool processPrepaidItem(string certCode);
};

#endif // DVM_H