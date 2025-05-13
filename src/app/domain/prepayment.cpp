#include "prepayment.h"

// 인증코드 자동 생성
Prepayment::Prepayment(int dvmId) : dvmId(dvmId), certCode() {
    // 기본 생성자로 랜덤 인증코드 생성
}

// 외부에서 인증코드를 받는 경우
Prepayment::Prepayment(int dvmId, CertificationCode certCode)
    : dvmId(dvmId), certCode(certCode) {}

// 인증코드 일치 여부 + 사용되지 않았는지 확인
bool Prepayment::isCertificationCode(std::string inputCode) {
    return certCode.matches(inputCode);
}
