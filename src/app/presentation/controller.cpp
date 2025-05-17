#include "controller.h"

Controller::Controller(DVM *dvm) : dvm(dvm) {}

Controller::~Controller() {}

map<string, string> Controller::parseStockResponse(const string &response)
{
    map<string, string> parsed;
    istringstream stream(response);
    string token;
    while (getline(stream, token, ';'))
    {
        size_t pos = token.find(':');
        if (pos != string::npos)
        {
            string key = token.substr(0, pos);
            string value = token.substr(pos + 1);
            parsed[key] = value;
        }
    }
    return parsed;
}

void Controller::run()
{
    while (true)
    {
        int choice = displayMenu();
        handleMenuSelection(choice);
    }
}

void Controller::runServer()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        cerr << "Failed to create server socket\n";
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9000);

    if (::bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Bind failed\n";
        return;
    }

    if (::listen(server_fd, 5) < 0)
    {
        cerr << "Listen failed\n";
        return;
    }

    cout << "DVM Controller Server is running on port 9000...\n";

    while (true)
    {
        int client_fd = ::accept(server_fd, nullptr, nullptr);
        if (client_fd < 0)
        {
            cerr << "Accept failed\n";
            continue;
        }

        char buffer[1024] = {0};
        int bytesRead = ::read(client_fd, buffer, sizeof(buffer));
        if (bytesRead > 0)
        {
            string request(buffer);
            string response;

            if (request.find("msg_type:req_stock") != string::npos)
            {
                response = handleCheckStockRequest(request);
            }
            else if (request.find("msg_type:req_prepay") != string::npos)
            {
                response = handlePrepaymentRequest(request);
            }
            else
            {
                response = "msg_type:error;detail:unknown_request;";
            }

            ::send(client_fd, response.c_str(), response.size(), 0);
        }

        ::close(client_fd);
    }

    ::close(server_fd);
}

int Controller::displayMenu()
{
    cout << "\n안녕하세요, Team1 DVM 입니다.\n"
         << endl;
    cout << "희망하는 옵션을 선택해주세요.\n"
         << endl;
    cout << "1. 음료 구매하기" << endl;
    cout << "2. 선결제 한 음료 받아 가기\n"
         << endl;
    cout << "Enter menu : ";
    int choice;
    cin >> choice;
    return choice;
}

void Controller::handleMenuSelection(int choice)
{
    switch (choice)
    {
    case 1:
        handleBeverageSelection();
        break;
    case 2:
        handlePrepaidPurchase();
        break;
    default:
        cout << "옵션을 잘못 입력하셨습니다. 다시 입력해주세요." << endl;
    }
}

void Controller::handleBeverageSelection()
{
    cout << "\n구매하고 싶은 메뉴와 수량을 입력해주세요." << endl;
    cout << "ex) 02 10\n"
         << endl;

    string drinks = dvm->queryItems();
    cout << "----------------------------------------------------------------------" << endl;
    cout << drinks;
    cout << "----------------------------------------------------------------------\n"
         << endl;

    cout << "Enter menu : ";
    string menu;
    int count;
    cin >> menu >> count;
    try
    {
        int menuInt = stoi(menu);
        if (menuInt < 1 || menuInt > 20)
        {
            cout << "\n메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                 << endl;
            return;
        }
        if (count < 1)
        {
            cout << "\n수량을 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                 << endl;
            return;
        }
        SaleRequest request{menu, count, Item("", "", 0)};
        string msg = dvm->queryStocks(menu, count);
        auto parsed = parseStockResponse(msg);

        string flag = parsed["flag"];
        string item_code = parsed["item_code"];
        int countParsed = parsed.count("count") ? stoi(parsed["count"]) : 0;
        string target = parsed["target"];
        string item_name = parsed["item_name"];
        int total_price = parsed.count("total_price") ? stoi(parsed["total_price"]) : 0;
        int x = parsed.count("x") ? stoi(parsed["x"]) : 0;
        int y = parsed.count("y") ? stoi(parsed["y"]) : 0;

        if (flag == "other")
        {
            cout << "\n현재 해당 자판기에서 구매가 불가합니다.\n";
            cout << "(" << x << ", " << y << ") 위치의 자판기에서 구매가 가능합니다.\n" << endl;
            cout << "음료 가격 총 " << total_price << "원 (" << item_name << " " << countParsed << "개 ";
            cout << total_price / count << "원 * " << count << ")" << endl; 
            pair<Location, string> response = dvm->requestOrder(stoi(target), request);
            cout << "\n결제가 완료되었습니다." << endl;
        }
        else if (flag == "this")
        {
            cout << "음료 가격 총 " << total_price << "원 (" << item_name << " " << countParsed << "개 ";
            cout << total_price / count << "원 * " << count << ")" << endl; 
            dvm->requestOrder(request);
            cout << "\n결제가 완료되었습니다." << endl;
        }
        else if (flag == "not_available")
        {
            cout << "\n요청하신 음료(" << item_code << ")는 현재 재고가 없습니다.\n";
        }

        cout << "\n메인 화면으로 돌아갑니다." << endl;
        cout << "계속하려면 Enter를 누르세요..." << endl;
        cin.ignore();
        cin.get();
        return;
    }
    catch (...)
    {
        cout << "\n메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
             << endl;
        return;
    }
}

void Controller::handlePrepaidPurchase()
{
    cout << "\n선결제 인증코드를 입력해주세요.\n"
         << endl;

    regex certCodePattern("^[a-zA-Z0-9]{5}$"); // 인증코드 형식: 영문자와 숫자로 구성된 5자리

    while (true)
    {
        try
        {
            cout << "Enter your prepayment code : ";
            string certCode;
            cin >> certCode;

            if (!regex_match(certCode, certCodePattern))
            {
                throw invalid_argument("인증코드를 잘못 입력하셨습니다. 다시 입력해주세요.");
            }

            bool result = dvm->processPrepaidItem(certCode);
            if (!result)
            {
                cout << "이 자판기에서 선결제한 음료가 없습니다.\n"
                     << endl;
            }
            else
            {
                cout << "선결제 한 음료가 제공되었습니다.\n" << endl;
                break;
            }
        }
        catch (const exception &e)
        {
            cout << "인증코드를 잘못 입력하셨습니다. 다시 입력해주세요.\n" << endl;
        }
    }

    cout << "\n메인 화면으로 돌아갑니다." << endl;
    cout << "계속하려면 Enter를 누르세요..." << endl;
    cin.ignore();
    cin.get();
}

string Controller::handleCheckStockRequest(const string &msg)
{
    istringstream iss(msg);
    string token, item_code, src_id;
    int item_num = 0;

    while (getline(iss, token, ';'))
    {
        size_t pos = token.find(':');
        if (pos == string::npos)
            continue;
        string key = token.substr(0, pos);
        string value = token.substr(pos + 1);
        if (key == "item_code")
            item_code = value;
        else if (key == "item_num")
            src_id = value;
        else if (key == "src_id")
            src_id = value;
        else if (key == "item_num")
            item_num = stoi(value);
    }
    
    string result = dvm->queryStocks(item_code, item_num);
    auto parsed = parseStockResponse(result);

    string resp_num;
    if(parsed["flag"] == "other")
    {
        resp_num = "0";
    }
    else if(parsed["flag"] == "this")
    {
        if(parsed.count("count"))
            resp_num = parsed["count"];
    }

    ostringstream oss;
    oss << "msg_type:resp_stock;"
        << "src_id:" << dvmId << ";"
        << "dst_id:" << src_id << ";"
        << "item_code:" << item_code << ";"
        << "item_num:" << resp_num << ";"
        << "coor_x:" << location.getX() << ";"
        << "coor_y:" << location.getY() << ";";

    return oss.str();
}

string Controller::handlePrepaymentRequest(const string &msg)
{
    istringstream iss(msg);
    string token, item_code, src_id;
    int item_num = 0;
    string cert_code;

    while (getline(iss, token, ';'))
    {
        size_t pos = token.find(':');
        if (pos == string::npos)
            continue;
        string key = token.substr(0, pos);
        string value = token.substr(pos + 1);
        if (key == "item_code")
            item_code = value;
        else if (key == "item_num")
            item_num = stoi(value);
        else if (key == "cert_code")
            cert_code = value;
        else if (key == "src_id")
            src_id = value;
    }

    string result = dvm->queryStocks(item_code, item_num);
    auto parsed = parseStockResponse(result);
    string availability = "F";

    if(parsed["flag"] == "this")
    {
        availability = "T";
    }

    ostringstream oss;
    oss << "msg_type:resp_prepay;"
        << "src_id:T" << dvmId << ";"
        << "dst_id:" << src_id << ";"
        << "item_code:" << item_code << ";"
        << "item_num:" << item_num << ";"
        << "availability:" << availability << ";";

    return oss.str();
}