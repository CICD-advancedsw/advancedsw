#ifndef PREPAYMENT_H
#define PREPAYMENT_H

#include <string>
#include "certificationcode.h"

class Prepayment
{
private:
    int dvmId;
    CertificationCode certCode;

public:
    explicit Prepayment(int dvmId);
    explicit Prepayment(int dvmId, CertificationCode certCode);
    bool isCertificationCode(const std::string &inputCertCode);
};

#endif // PREPAYMENT_H