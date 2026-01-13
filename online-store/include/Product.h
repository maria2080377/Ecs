#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <memory>

using namespace std;

class Product {
private:
    int productId;
    string name;
    double price;
    int stockQuantity;
    
public:
    Product(int id, string n, double pr, int stock)
        : productId(id), name(n), price(pr), stockQuantity(stock) {}
    
    int getProductId() { return productId; }
    string getName() { return name; }
    double getPrice() { return price; }
    int getStockQuantity() { return stockQuantity; }
    
    void setName(string n) {
        name = n;
    }
    
    void setPrice(double pr) {
        if (pr > 0) {
            price = pr;
        }
    }
    
    void setStockQuantity(int stock) {
        if (stock >= 0) {
            stockQuantity = stock;
        }
    }
    
    void decreaseStock(int amount) {
        if (stockQuantity >= amount) {
            stockQuantity -= amount;
        }
    }
    
    void increaseStock(int amount) {
        stockQuantity += amount;
    }
    
    bool isAvailable(int requested) {
        return stockQuantity >= requested;
    }
};

#endif
