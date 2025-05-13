#ifndef CERTIFICATIONCODE_H
#define CERTIFICATIONCODE_H

#include <string>

class CertificationCode {
private:
    std::string value;
    bool isUsed;

public:
    CertificationCode();
    CertificationCode(std::string value);
    bool markUsed(const std::string& certCode);
};

#endif // CERTIFICATIONCODE_H