#include "controller.h"
#include <fstream>
#include <poll.h>
#include <unistd.h>

Controller::Controller(DVM *dvm) : dvm(dvm), location(dvm->getLocation()), stocks(dvm->getStocks()), dvmId(dvm->getDvmId())
{
}

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
    server_addr.sin_port = htons(Config::get().port);

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

    static std::ofstream logFile("server_log.txt", std::ios::app);

    // cout << "DVM Controller Server is running on port 9000...\n";

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
            logFile << "[SERVER] Received: " << request << std::endl;
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

            logFile << "[SERVER] Response generated: " << response << std::endl;
            ssize_t sent = ::send(client_fd, response.c_str(), response.size(), 0);
            if (sent < 0)
            {
                logFile << "[SERVER] Send failed" << std::endl;
            }
            // ::send(client_fd, response.c_str(), response.size(), 0);
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
    cout << "2. 선결제 한 음료 받아 가기" << endl;
    cout << "0. dvm 종료\n"
         << endl;

    int choice;
    while (true)
    {
        cout << "Enter menu : ";
        if (cin >> choice)
        {
            if (choice == 0)
            {
                exit(0);
            }
            if (choice < 1 || choice > 2)
            {
                cout << "옵션을 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                     << endl;
                continue;
            }
            return choice;
        }
        else
        {
            // 입력 스트림 정리
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "옵션을 잘못 입력하셨습니다. 다시 입력해주세요.\n";
        }
    }
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

    while (true)
    {
        string drinks = dvm->queryItems();
        cout << "----------------------------------------------------------------------" << endl;
        cout << drinks;
        cout << "----------------------------------------------------------------------\n"
             << endl;
        cout << "Enter menu : ";
        cout.flush();

        struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
        int ret = poll(&pfd, 1, 20000);

        if (ret == 0)
        {
            cout << "\n입력 시간이 초과되어 초기화면으로 돌아갑니다.\n";
            break;
        }
        else if (ret < 0)
        {
            perror("poll 실패");
            break;
        }

        string menu;
        int count;
        if (!(cin >> menu >> count))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\n잘못된 입력입니다. 다시 입력해주세요.\n"
                 << endl;
            continue;
        }
        if (menu == "00")
        {
            exit(0);
        }
        try
        {
            int menuInt = stoi(menu);
            if (menu.length() != 2 || menuInt < 1 || menuInt > 20)
            {
                cout << "\n메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                     << endl;
                continue;
            }
            if (count < 1)
            {
                cout << "\n수량을 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                     << endl;
                continue;
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
                cout << "(" << x << ", " << y << ") 위치의 자판기에서 구매가 가능합니다.\n"
                     << endl;
                try
                {
                    pair<Location, string> response = dvm->requestOrder(stoi(target), request);
                    Card card("");
                    if (!card.processPayment(total_price))
                    {
                        cout << "결제에 실패하였습니다. 메인 화면으로 돌아갑니다.\n";
                        cout << "계속하려면 Enter를 누르세요..." << endl;
                        cin.ignore();
                        cin.get();
                        continue;
                    }
                    Location loc = response.first;
                    string certCode = response.second;
                    cout << "=====================" << endl;
                    cout << "자판기 위치 : (" << loc.getX() << ", " << loc.getY() << ")" << endl;
                    cout << "인증코드 : " << certCode << endl;
                    cout << "=====================" << endl;
                }
                catch (const std::exception &e)
                {
                    cout << "[ERROR] 주문 처리 중 문제가 발생했습니다: " << e.what() << endl;
                }
                cout << "\n메인 화면으로 돌아갑니다." << endl;
                cout << "계속하려면 Enter를 누르세요..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            else if (flag == "this")
            {
                cout << "음료 가격 총 " << total_price << "원 (" << item_name << " " << countParsed << "개 ";
                cout << total_price / count << "원 * " << count << ")" << endl;
                Card card("");
                if (!card.processPayment(total_price))
                {
                    cout << "결제에 실패하였습니다. 메인 화면으로 돌아갑니다.\n";
                    cout << "계속하려면 Enter를 누르세요..." << endl;
                    cin.ignore();
                    cin.get();
                    continue;
                }
                card.~Card();
                dvm->requestOrder(request);
                cout << "\n메인 화면으로 돌아갑니다." << endl;
                cout << "계속하려면 Enter를 누르세요..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
            else if (flag == "not_available")
            {
                cout << "\n요청하신 음료(" << item_code << ")는 현재 재고가 없습니다.\n";
                cout << "\n메인 화면으로 돌아갑니다." << endl;
                cout << "계속하려면 Enter를 누르세요..." << endl;
                cin.ignore();
                cin.get();
                return;
            }
        }
        catch (...)
        {
            cout << "\n메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                 << endl;
            continue;
        }
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
            if (certCode == "00")
            {
                exit(0);
            }

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
                cout << "선결제 한 음료가 제공되었습니다.\n"
                     << endl;
                break;
            }
        }
        catch (const exception &e)
        {
            cout << "인증코드를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                 << endl;
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
            item_num = stoi(value);
        else if (key == "src_id")
            src_id = value;
    }

    string result = dvm->queryStocks(item_code, item_num);
    static std::ofstream logFile("server_log.txt", std::ios::app);
    logFile << "[SERVER] queryStocks: " << result << std::endl;
    auto parsed = parseStockResponse(result);

    string resp_num = ""; // 초기화는 명시적으로

    if (parsed["flag"] == "other")
    {
        resp_num = "0";
    }
    else if (parsed["flag"] == "this")
    {
        if (parsed.count("count"))
            resp_num = parsed["count"];
        else
            resp_num = "0";
    }
    else
    {
        resp_num = "0";
    }

    ostringstream oss;
    oss << "msg_type:resp_stock;"
        << "src_id:T" << dvmId << ";"
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

    if (parsed["flag"] == "this")
    {
        availability = "T";
        dvm->saveSaleFromOther(item_code, item_num, cert_code);
    }

    ostringstream oss;
    oss << "msg_type:resp_prepay;"
        << "src_id:T" << dvmId << ";"
        << "dst_id:T" << src_id << ";"
        << "item_code:" << item_code << ";"
        << "item_num:" << item_num << ";"
        << "availability:" << availability << ";";

    return oss.str();
}