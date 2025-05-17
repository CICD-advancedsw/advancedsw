#include "sale.h"

#include <ctime>  
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <chrono>

using namespace std;
  
// Private constructors
Sale::Sale(SaleRequest request) 
    : saleId(""), datetime(""), item(request.item), 
    count(request.itemNum), totalAmount(0), prepayment(nullptr) {
    time_t now = time(0);
    datetime = ctime(&now);
    saleId = "SALE_" + to_string(now);
    totalAmount = item.calculatePrice(count);
}

Sale::Sale(SaleRequest request, int targetDvmId) 
    : Sale(request) {
    prepayment = new Prepayment(targetDvmId);
}

Sale::Sale(SaleRequest request, const CertificationCode& certCode) 
    : Sale(request) {
    prepayment = new Prepayment(0, certCode);
}

// Copy constructor
Sale::Sale(const Sale& other)
    : saleId(other.saleId), datetime(other.datetime),
      item(other.item), count(other.count),
      totalAmount(other.totalAmount), prepayment(nullptr) {
    if (other.prepayment) {
        prepayment = new Prepayment(*other.prepayment);
    }
}

// Assignment operator
Sale& Sale::operator=(const Sale& other) {
    if (this != &other) {
        saleId = other.saleId;
        datetime = other.datetime;
        item = other.item;
        count = other.count;
        totalAmount = other.totalAmount;
        
        delete prepayment;
        prepayment = nullptr;
        
        if (other.prepayment) {
            prepayment = new Prepayment(*other.prepayment);
        }
    }
    return *this;
}

// Factory methods
Sale Sale::createStandaloneSale(SaleRequest request) {
    return Sale(request);
}

pair<Sale, string> Sale::createSaleForDvm(SaleRequest request, int targetDvmId) {
    CertificationCode certCode;
    Sale sale(request, targetDvmId);
    stringstream ss;
    ss << certCode;
    return make_pair(sale, ss.str());
}

Sale Sale::createSaleUsingCertCode(SaleRequest request, string certCode) {
    CertificationCode certCodeObj(certCode);
    return Sale(request, certCodeObj);
}

// Public methods
bool Sale::receivePrepaidItem(const string& certCode) {
    if (prepayment && prepayment->isCertificationCode(certCode)) {
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
