#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

using namespace std;

class Card {
    private:
        string cardNumber;
    
    public:
        Card(string number);
        ~Card();
        bool isValid();
    
        bool processPayment(int amount);
    };

#endif // CARD_H