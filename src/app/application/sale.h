#ifndef SALE_H
#define SALE_H

#include <string>
#include <utility> // for std::pair
#include "../domain/item.h"
#include "../domain/prepayment.h"

// DTO
struct SaleRequest {
    Item item;
    int count;
    int totalAmount;
    std::string cardNumber;
    int dvmId;
};

class Sale {
private:
    std::string saleId;
    std::string datetime;
    Item item;
    int count;
    int totalAmount;
    Prepayment* prepayment;

    // Private constructor to enforce factory method usage
    Sale(SaleRequest request);
    Sale(SaleRequest request, int targetDvmId);
    
    // 인증 코드를 사용한 판매 요청 처리
    Sale(SaleRequest request, const std::string& certCode);

public:
    // Factory methods for creating Sale objects
    static Sale createStandaloneSale(SaleRequest request);
    static std::pair<Sale, std::string> createSaleForDvm(SaleRequest request, int targetDvmId);
    static Sale createSaleUsingCertCode(SaleRequest request, std::string certCode);
  
    // 선결제된 아이템 수령 처리
    bool receivePrepaidItem(const std::string& certCode);

    ~Sale();
};

#endif // SALE_H