#ifndef CERTIFICATIONCODE_H
#define CERTIFICATIONCODE_H

#include <string>
#include <iostream>

class CertificationCode {
private:
    std::string value;
    bool isUsed;

public:
    CertificationCode();
    CertificationCode(std::string value);
    bool markUsed(const std::string& certCode);

    friend std::ostream& operator<<(std::ostream& os, const CertificationCode& code) {
        return os << code.value;
    }
};

#endif // CERTIFICATIONCODE_H