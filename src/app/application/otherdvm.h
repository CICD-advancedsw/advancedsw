#ifndef OTHERDVM_H
#define OTHERDVM_H

#include <string>
#include "../domain/location.h"
#include "../dto.h"

using namespace std;

class OtherDVM {
private:
    int dvmId;
    Location location;

public:
    OtherDVM(int id, const Location& loc);
    CheckStockResponse findAvailableStocks(const CheckStockRequest& request);
    askPrepaymentResponse askForPrepayment(const askPrepaymentRequest& request);
    const Location& getLocation() const;
    int getDvmId() const;
};

#endif // OTHERDVM_H