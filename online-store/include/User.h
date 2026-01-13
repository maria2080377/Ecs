#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>
#include "Order.h"

using namespace std;

class User {
protected:
    int userId;
    string name;
    string email;
    string role;
    vector<shared_ptr<Order>> orders;
    
public:
    User(int id, string n, string e, string r)
        : userId(id), name(n), email(e), role(r) {}
    
    virtual ~User() {}
    
    virtual void createOrder() = 0;
    virtual void viewOrderStatus(int orderId) = 0;
    virtual void cancelOrder(int orderId) = 0;
    
    int getId() { return userId; }
    string getName() { return name; }
    string getEmail() { return email; }
    string getRole() { return role; }
    
    void addOrder(shared_ptr<Order> order) {
        orders.push_back(order);
    }
    
    vector<shared_ptr<Order>> getOrders() {
        return orders;
    }
    
    virtual bool hasPermission(string permission) {
        if (role == "admin") return true;
        return false;
    }
};

class Admin : public User {
public:
    Admin(int id, string n, string e) : User(id, n, e, "admin") {}
    
    void createOrder() override {
        cout << "Админ создает заказ\n";
    }
    
    void viewOrderStatus(int orderId) override {
        cout << "Админ просматривает статус заказа " << orderId << "\n";
    }
    
    void cancelOrder(int orderId) override {
        cout << "Админ отменяет заказ " << orderId << "\n";
    }
    
    void addProduct(string name, double price, int stock) {
        cout << "Админ добавляет товар: " << name << "\n";
    }
    
    void viewAllOrders() {
        cout << "Админ просматривает все заказы\n";
    }
    
    bool hasPermission(string permission) override {
        return true;
    }
};

class Manager : public User {
public:
    Manager(int id, string n, string e) : User(id, n, e, "manager") {}
    
    void createOrder() override {
        cout << "Менеджер создает заказ\n";
    }
    
    void viewOrderStatus(int orderId) override {
        cout << "Менеджер просматривает статус заказа " << orderId << "\n";
    }
    
    void cancelOrder(int orderId) override {
        cout << "Менеджер отменяет заказ " << orderId << "\n";
    }
    
    void approveOrder(int orderId) {
        cout << "Менеджер утверждает заказ " << orderId << "\n";
    }
    
    bool hasPermission(string permission) override {
        if (permission == "view_orders" || permission == "approve_orders") {
            return true;
        }
        return false;
    }
};

class Customer : public User {
private:
    int loyaltyLevel;
    
public:
    Customer(int id, string n, string e, int level = 0) 
        : User(id, n, e, "customer"), loyaltyLevel(level) {}
    
    void createOrder() override {
        cout << "Покупатель создает заказ\n";
    }
    
    void viewOrderStatus(int orderId) override {
        cout << "Покупатель просматривает статус своего заказа " << orderId << "\n";
    }
    
    void cancelOrder(int orderId) override {
        cout << "Покупатель отменяет свой заказ " << orderId << "\n";
    }
    
    void makePayment(int orderId) {
        cout << "Покупатель оплачивает заказ " << orderId << "\n";
    }
    
    bool hasPermission(string permission) override {
        if (permission == "view_own_orders" || permission == "create_order") {
            return true;
        }
        return false;
    }
    
    int getLoyaltyLevel() { return loyaltyLevel; }
};

#endif
