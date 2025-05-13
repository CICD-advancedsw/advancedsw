#include "sale.h"

#include "../dto/salerequest.h"
#include <ctime>
  
  #include <chrono>

#include <sstream>
#include <stdexcept>
#include <chrono>
  
static std::string getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm localTm;
    localtime_r(&timeNow, &localTm);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTm);
    return std::string(buffer);
}

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

// 소멸자
Sale::~Sale() {
    if (prepayment != nullptr) {
        delete prepayment;
        prepayment = nullptr;
    }
}
