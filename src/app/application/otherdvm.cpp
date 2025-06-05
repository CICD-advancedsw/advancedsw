#include <fstream>
#include "otherdvm.h"

OtherDVM::OtherDVM(int id, const Location &loc, const char* targetIp, const int port)
    : dvmId(id), location(loc), targetIp(targetIp), port(port) {}

bool OtherDVM::establishConnection(int &sock) const {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed\n";
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, targetIp, &serverAddr.sin_addr);

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));


    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection failed\n";
        close(sock);
        return false;
    }

    return true;
}

//재고 확인 요청
CheckStockResponse OtherDVM::findAvailableStocks(const CheckStockRequest &request, int senderDvmId)
{
    static ofstream logFile("client_log.txt", ios::app);
    if (!logFile) {
        cerr << "⚠️ client_log.txt 열기 실패\n";
    }

    int sock;
    if (!establishConnection(sock)) {
        return {};
    }

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    SocketMessage msg;
    msg.msg_type = "req_stock";
    msg.src_id = "T" + to_string(senderDvmId);
    msg.dst_id = "0";
    msg.msg_content["item_code"] = request.item_code;
    msg.msg_content["item_num"] = to_string(request.item_num);

    string requestStr = msg.serialize();
    send(sock, requestStr.c_str(), requestStr.size(), 0);

    char buffer[4096] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    if (bytesRead <= 0)
    {
        static int warnCount = 0;
        if (warnCount++ < 3) {
            logFile << "[CLIENT] Warning: Failed to receive response (count " << warnCount << ")" << std::endl;
        }
        logFile.flush();
        return {};
    }

    logFile << "[CLIENT] Raw response: " << buffer << std::endl;

    SocketMessage resp = SocketMessage::deserialize(buffer);

    logFile << "[CLIENT] Parsed item_code: " << resp.msg_content["item_code"] << std::endl;
    logFile << "[CLIENT] Parsed item_num: " << resp.msg_content["item_num"] << std::endl;

    int item_num = 0;
    int coor_x = 0;
    int coor_y = 0;
    try {
        item_num = stoi(resp.msg_content["item_num"]);
        coor_x = stoi(resp.msg_content["coor_x"]);
        coor_y = stoi(resp.msg_content["coor_y"]);
    } catch (const std::exception& e) {
        logFile << "[CLIENT] stoi failed: " << e.what() << std::endl;
    }
    logFile.flush();

    return CheckStockResponse{
        .dst_id = stoi(resp.dst_id.substr(1)),
        .item_code = resp.msg_content["item_code"],
        .item_num = item_num,
        .coor_x = coor_x,
        .coor_y = coor_y};
}

//선결제 요청
askPrepaymentResponse OtherDVM::askForPrepayment(const askPrepaymentRequest &request, int senderDvmId)
{
    static ofstream logFile("client_log.txt", ios::app);

    int sock;
    if (!establishConnection(sock)) {
        return {};
    }

    SocketMessage msg;
    msg.msg_type = "req_prepay";
    msg.src_id = "T" + to_string(senderDvmId);
    msg.dst_id = dvmId;
    msg.msg_content["item_code"] = request.item_code;
    msg.msg_content["item_num"] = to_string(request.item_num);
    msg.msg_content["cert_code"] = request.cert_code;

    string requestStr = msg.serialize();
    send(sock, requestStr.c_str(), requestStr.size(), 0);

    char buffer[4096] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
    close(sock);

    if (bytesRead <= 0)
    {
        logFile << "Failed to receive response\n";
        return {};
    }

    SocketMessage resp = SocketMessage::deserialize(buffer);
    return askPrepaymentResponse{
        .item_code = resp.msg_content["item_code"],
        .item_num = stoi(resp.msg_content["item_num"]),
        .availability = (resp.msg_content["availability"] == "T")};
}

const Location &OtherDVM::getLocation() const
{
    return location;
}

int OtherDVM::getDvmId() const
{
    return dvmId;
}