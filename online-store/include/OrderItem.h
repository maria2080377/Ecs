#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>

using namespace std;

class OrderItem {
private:
    int itemId;
    int orderId;
    int productId;
    int quantity;
    double price;
    
public:
    OrderItem(int id, int oid, int pid, int qty, double pr)
        : itemId(id), orderId(oid), productId(pid), quantity(qty), price(pr) {}
    
    int getItemId() { return itemId; }
    int getOrderId() { return orderId; }
    int getProductId() { return productId; }
    int getQuantity() { return quantity; }
    double getPrice() { return price; }
    
    void setQuantity(int qty) {
        quantity = qty;
    }
    
    void setPrice(double pr) {
        price = pr;
    }
    
    double getSubtotal() {
        return quantity * price;
    }
};

#endif
