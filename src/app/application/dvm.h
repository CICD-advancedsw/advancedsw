#ifndef DVM_H
#define DVM_H

#include <string>
#include <map>
#include <list>
#include "sale.h"
#include "../domain/location.h"
#include "../domain/item.h"
#include "otherdvm.h"

class Sale;

class DVM {
private:
    int dvmId;
    Location location;
    std::map<Item, int> stocks;
    std::list<Item> items;
    std::list<Sale> sales;
    std::list<OtherDVM> dvms;
    // question: 내부에서만 사용되는 메서드 추가 제안
    void initializeItems();
    void initializeStocks();
    Item findItemByCode(const std::string& itemCode);
public:
    // 개발을 위한 기본 생성자 임시 추가
    DVM(); 
    DVM(int id, Location loc, std::list<OtherDVM> otherDvms); // 생성자를 통한 의존성 주입
    
    // 자판기의 아이템 목록을 조회
    std::string queryItems();
    
    // 특정 아이템의 재고를 조회
    std::string queryStocks(std::string itemCode, int count);
    
    // 주문 요청
    void requestOrder();
    
    // 특정 자판기에 주문 요청
    std::string requestOrder(int targetDvmId);
    
    // 다른 자판기로부터의 판매 정보 저장
    void saveSaleFromOther(std::string itemCode, int itemNum, std::string certCode);
    
    // 선결제된 아이템 처리
    std::string processPrepaidItem(std::string certCode);
    
    // 다른 DVM 추가 메서드
    void addOtherDVM(OtherDVM dvm);
};

#endif // DVM_H