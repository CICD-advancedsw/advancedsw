#include <string>
#include <sstream> 
#include <map>
#include "domain/item.h"

using namespace std;

#ifndef DTO_H
#define DTO_H

struct Config
{
    const char* target_ip = "127.0.0.1";
    int port = 9000;
    static Config &get()
    {
        static Config instance;
        return instance;
    }
};

struct SocketMessage
{
    string msg_type;
    string src_id;
    string dst_id;
    map<string, string> msg_content;

    string serialize() const
    {
        ostringstream oss;
        oss << "msg_type:" << msg_type << ";"
            << "src_id:" << src_id << ";"
            << "dst_id:" << dst_id << ";";
        for (const auto &[key, value] : msg_content)
        {
            oss << key << ":" << value << ";";
        }
        return oss.str();
    }

    static SocketMessage deserialize(const string &raw)
    {
        SocketMessage msg;
        istringstream iss(raw);
        string token;
        while (getline(iss, token, ';'))
        {
            size_t pos = token.find(':');
            if (pos == string::npos)
                continue;
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            if (key == "msg_type")
                msg.msg_type = value;
            else if (key == "src_id")
                msg.src_id = value;
            else if (key == "dst_id")
                msg.dst_id = value;
            else
                msg.msg_content[key] = value;
        }
        return msg;
    }
};

struct SaleRequest {
    string itemCode;
    int itemNum;
    Item item;  // 판매할 아이템 정보
};

struct CheckStockRequest {
    string item_code;
    int item_num;
};

struct CheckStockResponse {
    int dst_id;
    string item_code;
    int item_num;
    int coor_x;
    int coor_y;
};

struct askPrepaymentRequest {
    string item_code;
    int item_num;
    string cert_code;
};

struct askPrepaymentResponse {
    string item_code;
    int item_num;
    bool availability;
};

#endif 