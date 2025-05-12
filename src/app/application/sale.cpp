#include "sale.h"
#include "../dto/salerequest.h"
#include <ctime>

// Private constructors
Sale::Sale(SaleRequest request) 
    : saleId(""), datetime(""), item(Item(request.itemCode, "", 0)), 
    count(request.itemNum), totalAmount(0), 
    card(Card(request.card)), prepayment(nullptr) {
    time_t now = time(0);
    datetime = ctime(&now);
    saleId = "SALE_" + std::to_string(now);
}

Sale::Sale(SaleRequest request, int targetDvmId) 
    : Sale(request) {
    prepayment = new Prepayment(targetDvmId);
}

Sale::Sale(SaleRequest request, std::string certCode) 
    : Sale(request) {
    prepayment = new Prepayment(0, certCode);
}

// Factory methods
Sale Sale::createStandaloneSale(SaleRequest request) {
    return Sale(request);
}

std::pair<Sale, std::string> Sale::createSaleForDvm(SaleRequest request, int targetDvmId) {
    Sale sale(request, targetDvmId);
    // Prepayment에도 FACTORY 패턴 필요할 거 같은데 이야기해보기
    return std::make_pair(sale, sale.prepayment->isCertificationCode() ? "CERT_CODE" : "");
}

Sale Sale::createSaleUsingCertCode(SaleRequest request, std::string certCode) {
    return Sale(request, certCode);
}

// Public methods
bool Sale::receivePrepaidItem(std::string certCode) {
    if (prepayment && prepayment->isCertificationCode()) {
        return true;
    }
    return false;
}


