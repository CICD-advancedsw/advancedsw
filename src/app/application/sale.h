#ifndef SALE_H
#define SALE_H

#include <string>
#include "../domain/item.h"
#include "../domain/card.h"
#include "../domain/prepayment.h"

// Request 클래스 전방 선언
class SaleRequest;

class Sale {
private:
    std::string saleId;
    std::string datetime;
    Item item;
    int count;
    int totalAmount;
    Card card;
    Prepayment* prepayment;

public:
    // 일반 판매 요청 처리
    Sale(SaleRequest request);
    
    // 특정 DVM으로 향하는 판매 요청 처리
    Sale(SaleRequest request, int targetDvmId);
    
    // 인증 코드를 사용한 판매 요청 처리
    Sale(SaleRequest request, std::string certCode);
    
    // 선결제된 아이템 수령 처리
    bool receivePrepaidItem(std::string certCode);
};

#endif // SALE_H 