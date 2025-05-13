#include "certificationcode.h"
#include <random>
#include <chrono>

CertificationCode::CertificationCode()
{
    // 랜덤 10자리 인증코드 생성
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int codeLength = 5;

    std::mt19937 rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, characters.size() - 1);

    value = "";

    for (int i = 0; i < codeLength; ++i)
    {
        value += characters[dist(rng)];
    }

    isUsed = false;
}

CertificationCode::CertificationCode(std::string value) : value(value), isUsed(false)
{
    // 외부에서 지정한 코드로 초기화 (특정 인증코드로 초기화)
}

bool CertificationCode::markUsed()
{
    if (isUsed)
        return false;
    isUsed = true;
    return true;
}

bool CertificationCode::matches(const std::string& inputCode) const {
    return value == inputCode && !isUsed;
}
