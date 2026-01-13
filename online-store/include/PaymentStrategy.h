#ifndef PAYMENTSTRATEGY_H
#define PAYMENTSTRATEGY_H

#include <iostream>

using namespace std;

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата банковской картой: " << amount << " руб.\n";
    }
};

class WalletPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата электронным кошельком: " << amount << " руб.\n";
    }
};

class SBPPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Оплата через СБП: " << amount << " руб.\n";
    }
};

#endif
