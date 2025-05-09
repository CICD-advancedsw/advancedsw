#include "controller.h"
#include <iostream>
#include <iomanip> // for setw and left
using namespace std;

Controller::Controller(DVM* dvm) : dvm(dvm) {}

Controller::~Controller() {}

void Controller::run() {
    while (true) {
        int choice = displayMenu();
        handleMenuSelection(choice);
    }
}

int Controller::displayMenu() {
    cout << "\n안녕하세요, Team1 DVM 입니다.\n" << endl;
    cout << "희망하는 옵션을 선택해주세요.\n" << endl;
    cout << "1. 음료 구매하기" << endl;
    cout << "2. 선결제 한 음료 받아 가기\n" << endl;
    cout << "Enter menu : ";
    int choice;
    cin >> choice;
    return choice;
}

void Controller::handleMenuSelection(int choice) {
    switch (choice) {
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

void Controller::handleBeverageSelection() {
    cout << "\n구매하고 싶은 메뉴와 수량을 입력해주세요." << endl;
    cout << "ex) 02 10\n" << endl;

    string drinks = dvm->queryItems();
    cout << "----------------------------------------------------------------------" << endl;
    cout << drinks;
    cout << "----------------------------------------------------------------------\n" << endl;

    cout << "Enter menu : ";
    string menu;
    int count;
    cin >> menu >> count;

    try {
        int menuInt = stoi(menu);
        if (menuInt < 1 || menuInt > 20 || count < 1) {
            throw invalid_argument("Invalid range");
        }
        dvm->queryStocks(menu, count);
    } catch (...) {
        cout << "\n메뉴를 잘못 입력하셨습니다. 다시 입력해주세요.\n" << endl;
        return;
    }
}

void Controller::handlePrepaidPurchase() {
    cout << "\n선결제 인증코드를 입력해주세요.\n" << endl;
    cout << "Enter your prepayment code : ";
    int certCode;
    cin >> certCode;
    dvm->processPrepaidItem(to_string(certCode));
}