#ifndef PREPAYMENT_H
#define PREPAYMENT_H

#include <string>
#include "certificationcode.h"

class Prepayment {
private:
    int dvmId;
    std::string certCode;

public:
    Prepayment(int dvmId);
    Prepayment(int dvmId, std::string certCode);
    bool isCertificationCode();
};

#endif // PREPAYMENT_H