#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <numeric>
#include "User.h"
#include "DatabaseConnection.h"
#include "Product.h"
#include "PaymentStrategy.h"

using namespace std;

void displayMenu() {
    cout << "\n=== ИНТЕРНЕТ-МАГАЗИН ===\n";
    cout << "1. Администратор\n";
    cout << "2. Менеджер\n";
    cout << "3. Покупатель\n";
    cout << "4. Выход\n";
    cout << "Выберите роль: ";
}

void adminMenu(shared_ptr<Admin> admin) {
    int choice;
    do {
        cout << "\n=== МЕНЮ АДМИНИСТРАТОРА ===\n";
        cout << "1. Добавить товар\n";
        cout << "2. Просмотреть все заказы\n";
        cout << "3. Изменить статус заказа\n";
        cout << "4. Просмотреть историю\n";
        cout << "5. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                admin->addProduct("Новый товар", 1000, 10);
                break;
            case 2:
                admin->viewAllOrders();
                break;
            case 3:
                admin->cancelOrder(1);
                break;
            case 4:
                cout << "История просмотрена\n";
                break;
        }
    } while(choice != 5);
}

void managerMenu(shared_ptr<Manager> manager) {
    int choice;
    do {
        cout << "\n=== МЕНЮ МЕНЕДЖЕРА ===\n";
        cout << "1. Утвердить заказ\n";
        cout << "2. Просмотреть заказы\n";
        cout << "3. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                manager->approveOrder(1);
                break;
            case 2:
                manager->viewOrderStatus(1);
                break;
        }
    } while(choice != 3);
}

void customerMenu(shared_ptr<Customer> customer) {
    int choice;
    do {
        cout << "\n=== МЕНЮ ПОКУПАТЕЛЯ ===\n";
        cout << "1. Создать заказ\n";
        cout << "2. Оплатить заказ\n";
        cout << "3. Просмотреть статус\n";
        cout << "4. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                customer->createOrder();
                break;
            case 2:
                customer->makePayment(1);
                break;
            case 3:
                customer->viewOrderStatus(1);
                break;
        }
    } while(choice != 4);
}

int main() {
    cout << "Добро пожаловать в интернет-магазин!\n";
    
    auto db = make_shared<DatabaseConnection<string>>(
        "host=localhost user=postgres password=password dbname=online_store"
    );
    
    shared_ptr<Admin> admin = make_shared<Admin>(1, "Админ", "admin@mail.com");
    shared_ptr<Manager> manager = make_shared<Manager>(2, "Менеджер", "manager@mail.com");
    shared_ptr<Customer> customer = make_shared<Customer>(3, "Покупатель", "customer@mail.com");
    
    int role;
    do {
        displayMenu();
        cin >> role;
        
        switch(role) {
            case 1:
                adminMenu(admin);
                break;
            case 2:
                managerMenu(manager);
                break;
            case 3:
                customerMenu(customer);
                break;
            case 4:
                cout << "Выход из программы\n";
                break;
            default:
                cout << "Неверный выбор\n";
        }
    } while(role != 4);
    
    return 0;
}
