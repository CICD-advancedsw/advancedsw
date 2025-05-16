#include "controller.h"
#include <iostream>
#include <regex>

using namespace std;

Controller::Controller(DVM *dvm) : dvm(dvm) {}

Controller::~Controller() {}

void Controller::run()
{
    while (true)
    {
        int choice = displayMenu();
        handleMenuSelection(choice);
    }
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
        string msg = dvm->queryStocks(menu, count);
        if (msg == "재고 없음")
        {
            // todo: 선결제 로직 추가
        }
        else
        {
            cout << msg << endl;
            SaleRequest request{menu, count, Item("", "", 0)};
            dvm->requestOrder(request);
            cout << "\n결제가 완료되었습니다." << endl;
            cout << "\n메인 화면으로 돌아갑니다." << endl;
            cout << "계속하려면 Enter를 누르세요..." << endl;
            cin.ignore();
            cin.get(); // Enter 키 입력 대기
            return;
        }
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

    std::regex certCodePattern("^[a-zA-Z0-9]{5}$"); // 인증코드 형식: 영문자와 숫자로 구성된 5자리

    while (true)
    {
        cout << "Enter your prepayment code : ";
        string certCode;
        cin >> certCode;

        if (!std::regex_match(certCode, certCodePattern))
        {
            cout << "인증코드를 잘못 입력하셨습니다. 다시 입력해주세요.\n"
                 << endl;
            continue;
        }

        bool result = dvm->processPrepaidItem(certCode);
        // todo: result에 따른 처리 로직
        //  if (!result)
        //  {
        //      cout << "인증코드를 잘못 입력하셨습니다. 다시 입력해주세요\n"
        //           << endl;
        //  }
        //  else
        //  {
        //      cout << result << endl;
        //      break;
        //  }
    }

    return;
}