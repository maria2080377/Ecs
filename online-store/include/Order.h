#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include "OrderItem.h"
#include "Payment.h"

using namespace std;

class Order {
private:
    int orderId;
    int userId;
    string status;
    double totalPrice;
    time_t orderDate;
    vector<unique_ptr<OrderItem>> items;
    unique_ptr<Payment> payment;
    
public:
    Order(int id, int uid, double price = 0.0)
        : orderId(id), userId(uid), status("pending"), totalPrice(price) {
        orderDate = time(nullptr);
    }
    
    ~Order() {
        items.clear();
    }
    
    int getOrderId() { return orderId; }
    int getUserId() { return userId; }
    string getStatus() { return status; }
    double getTotalPrice() { return totalPrice; }
    time_t getOrderDate() { return orderDate; }
    
    void setStatus(string newStatus) {
        status = newStatus;
    }
    
    void setTotalPrice(double price) {
        totalPrice = price;
    }
    
    void addItem(unique_ptr<OrderItem> item) {
        items.push_back(move(item));
    }
    
    void removeItem(int productId) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->getProductId() == productId) {
                items.erase(it);
                break;
            }
        }
    }
    
    vector<unique_ptr<OrderItem>>& getItems() {
        return items;
    }
    
    void setPayment(unique_ptr<Payment> pay) {
        payment = move(pay);
    }
    
    Payment* getPayment() {
        return payment.get();
    }
};

#endif
