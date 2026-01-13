#include "User.h"
#include <iostream>

using namespace std;

void Admin::addProduct(string name, double price, int stock) {
    cout << "Администратор добавляет товар: " << name 
         << ", цена: " << price << ", количество: " << stock << "\n";
}
