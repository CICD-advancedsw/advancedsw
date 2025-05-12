#ifndef SALE_H
#define SALE_H

#include <string>
#include <utility> // for std::pair
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

    // Private constructor to enforce factory method usage
    Sale(SaleRequest request);
    Sale(SaleRequest request, int targetDvmId);
    Sale(SaleRequest request, std::string certCode);

public:
    // Factory methods for creating Sale objects
    static Sale createStandaloneSale(SaleRequest request);
    static std::pair<Sale, std::string> createSaleForDvm(SaleRequest request, int targetDvmId);
    static Sale createSaleUsingCertCode(SaleRequest request, std::string certCode);

    // 선결제된 아이템 수령 처리
    bool receivePrepaidItem(std::string certCode);
};

#endif // SALE_H