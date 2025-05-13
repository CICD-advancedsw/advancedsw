#ifndef PREPAYMENT_H
#define PREPAYMENT_H

#include <string>
#include "certificationcode.h"

class Prepayment {
private:
    int dvmId;
    CertificationCode certCode;

public:
    Prepayment(int dvmId);
    Prepayment(int dvmId, CertificationCode certCode);
    bool isCertificationCode(std::string inputCode);
};

#endif // PREPAYMENT_H