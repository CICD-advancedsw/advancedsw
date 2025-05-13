#include "sale.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <stdexcept>

// Helper 함수: 현재 시간을 문자열로 반환 (예: "2024-05-01 14:03:22")
static std::string getCurrentTimeAsString() {
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm localTm;
    localtime_r(&timeNow, &localTm);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTm);
    return std::string(buffer);
}

// 일반 판매 요청 - 생성자
Sale::Sale(SaleRequest request)
    : item(request.item),
      count(request.count),
      totalAmount(request.totalAmount),
      prepayment(nullptr) {

    saleId = getCurrentTimeAsString();
    datetime = getCurrentTimeAsString();

    // 외부 결제 서버에 승인 요청
}

// 특정 DVM으로 향하는 선결제 요청 처리
Sale::Sale(SaleRequest request, int targetDvmId)
    : item(request.item),
      count(request.count),
      totalAmount(request.totalAmount) {
    
    saleId = getCurrentTimeAsString();
    datetime = getCurrentTimeAsString();

    // 외부 결제 서버

    prepayment = new Prepayment(targetDvmId);
}

// 인증코드를 사용한 음료 수령 요청 처리
Sale::Sale(SaleRequest request, const std::string& certCode)
    : item(request.item),
      count(request.count),
      totalAmount(request.totalAmount) {

    saleId = getCurrentTimeAsString();
    datetime = getCurrentTimeAsString();

    prepayment = new Prepayment(request.dvmId, CertificationCode(certCode));
}


// 수령 처리 메서드
bool Sale::receivePrepaidItem(const std::string& certCode) {
    if (!prepayment) return false;
    return prepayment->isCertificationCode(certCode);
}

// 소멸자
Sale::~Sale() {
    if (prepayment != nullptr) {
        delete prepayment;
        prepayment = nullptr;
    }
}