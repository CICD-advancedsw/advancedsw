#include "card.h"
#include <regex>

Card::Card(std::string number) : cardNumber(number) {}

// 올바른 카드 형식 4자리-4자리
bool Card::isValid() {
    std::regex cardFormat("^[0-9]{4}-[A-Za-z]{4}$");
    return std::regex_match(cardNumber, cardFormat);
}