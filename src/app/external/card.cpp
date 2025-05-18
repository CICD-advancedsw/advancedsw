#include "card.h"
#include <regex>

Card::Card(string number) : cardNumber(number) {}
Card::~Card(){}

// 올바른 카드 형식 4자리-4자리
bool Card::isValid() {
    regex cardFormat("^[0-9]{4}-[A-Za-z]{4}$");
    return regex_match(cardNumber, cardFormat);
}

bool Card::processPayment(int amount) {
    while (true) {
        cout << "\n카드 정보를 입력해주세요.\n\n";
        cout << "Enter your card : ";
        cin >> cardNumber;

        if (isValid()) {
            break;
        }
        cerr << "\n결제가 불가한 카드 정보입니다. 다시 입력해주세요.\n";
    }
    cout << "\n결제가 완료되었습니다.\n";
    return true;
}