#ifndef OTHERDVM_H
#define OTHERDVM_H

#include <string>
#include "../domain/location.h"
#include "../dto.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;

class OtherDVM
{
private:
    int dvmId;
    Location location;

    // 추가
    const char* targetIp;
    int port;
    bool establishConnection(int &sock) const;
public:
    OtherDVM(int id, const Location &loc);
    CheckStockResponse findAvailableStocks(const CheckStockRequest &request,int senderDvmId);
    askPrepaymentResponse askForPrepayment(const askPrepaymentRequest &request, int senderDvmId);
    const Location &getLocation() const;
    int getDvmId() const;
};

#endif // OTHERDVM_H