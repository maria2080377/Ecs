#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include "PaymentStrategy.h"
#include <memory>

using namespace std;

class Payment {
private:
    int paymentId;
    int orderId;
    double amount;
    string method;
    bool isCompleted;
    unique_ptr<PaymentStrategy> strategy;
    
public:
    Payment(int id, int oid, double amt, string meth)
        : paymentId(id), orderId(oid), amount(amt), method(meth), isCompleted(false) {}
    
    int getPaymentId() { return paymentId; }
    int getOrderId() { return orderId; }
    double getAmount() { return amount; }
    string getMethod() { return method; }
    bool getIsCompleted() { return isCompleted; }
    
    void setStrategy(unique_ptr<PaymentStrategy> strat) {
        strategy = move(strat);
    }
    
    bool processPayment() {
        if (strategy) {
            strategy->pay(amount);
            isCompleted = true;
            return true;
        }
        return false;
    }
};

#endif
