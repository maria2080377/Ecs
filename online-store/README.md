Описание проекта
Создание системы интернет-магазина на языке программирования С++ с использованием принципов объектно-ориентированного программирования, умных указателей и работы с PostgreSQL. Система имеет три роли пользователя: администратор, менеджер и покупатель, каждый их них имеет разные уровни доступа к базе данных и имеют разные функции (то что они могут делать).

Архитектура проекта
DatabaseConnection (Шаблонный класс) Назначение: Управление подключением к PostgreSQL через libpqxx Ключевые методы: -executeQuery() - выполнение SELECT-запросов -executeNonQuery() - выполнение INSERT/UPDATE/DELETE -beginTransaction(), commitTransaction(), rollbackTransaction() - работа с транзакциями -createFunction(), createTrigger() - создание функция и триггеров

Абстрактный класс пользователей User User (абстрактный класс) ├── Admin ├── Manager └── Customer В данном классе реализовано наследованое и полиморфизм User: Базовый класс с чисто виртуальной функцией showMenu(), классы-наследники реализовывают эту функцию по-своему Admin: Полный доступ к системе Manager: Управление заказами и складом Customer: Создание и оплата заказов

Классы для работы с заказами Order: Основной класс заказа OrderItem: Элементы заказа (композиция с Order) Payment: Управление оплатой PaymentStrategy: Стратегия оплаты (в данном случае карта, сбп и электронный кошелек)

Принципы ООП
Наследование Admin, Manager, Customer - классы наследники от User
Полиморфизм В классе User virtual void showMenu() = 0; - Чисто виртуальная функция, а в классе Admin, Manager, Customer реализована эта функция: void showMenu() override;
Композиция реализована в классе Order class Order { vector<unique_ptr> items; // Композиция unique_ptr payment; // Композиция };
Агрегация в классе User class User { DatabaseConnection* db; // Агрегация vector<shared_ptr> orders; // Агрегация };
Работа с базой данных
Структура таблиц PostgreSQL

users - пользователи системы -user_id (INTEGER, PRIMARY KEY) -name (VARCHAR) -email (VARCHAR) -role (VARCHAR: 'admin', 'manager', 'customer') -password_hash (VARCHAR) -loyalty_level (INTEGER: 0 — обычный, 1 — премиум)
products - каталог товаров -product_id (INTEGER, PRIMARY KEY) -name (VARCHAR) -price (DECIMAL) -stock_quantity (INTEGER)
orders – заказы -order_id (INTEGER, PRIMARY KEY) -user_id (INTEGER, FOREIGN KEY, ссылается на users) -status (VARCHAR: 'pending', 'completed', 'canceled', 'returned') -total_price (DECIMAL) -order_date (TIMESTAMP)
order_items - элементы заказов -order_item_id (INTEGER, PRIMARY KEY) -order_id (INTEGER, FOREIGN KEY, ссылается на orders) -product_id (INTEGER, FOREIGN KEY, ссылается на products) -quantity (INTEGER) -price (DECIMAL) 5.order_status_history - история изменений статусов -history_id (INTEGER, PRIMARY KEY) -order_id (INTEGER, FOREIGN KEY → orders) -old_status (VARCHAR) -new_status (VARCHAR) -changed_at (TIMESTAMP) -changed_by (INTEGER, FOREIGN KEY → users)
audit_log - журнал аудита действий -log_id (INTEGER, PRIMARY KEY) -entity_type (VARCHAR: 'order', 'product', 'user') -entity_id (INTEGER) -operation (VARCHAR: 'insert', 'update', 'delete') -performed_by (INTEGER) -performed_at (TIMESTAMP)
Хранимые процедуры и функции
-- Создание функции обновления статуса CREATE OR REPLACE FUNCTION update_order_status( p_order_id INTEGER, p_new_status VARCHAR, p_user_id INTEGER ) RETURNS VOID AS $$

-- Функция проверки возможности возврата CREATE OR REPLACE FUNCTION can_return_order(p_order_id INTEGER) RETURNS BOOLEAN AS $$

-- Функция получения истории статусов CREATE OR REPLACE FUNCTION get_order_status_history(p_order_id INTEGER) RETURNS TABLE(...) AS $$

Триггеры
-Обновление даты заказа при изменении статуса -Запись истории статусов в order_status_history -Аудит действий в audit_log

Умные указатели и STL
Использование умных указателей // unique_ptr для эксклюзивного владения unique_ptr strategy; unique_ptr item;

// shared_ptr для разделяемого владения vector<shared_ptr> orders;

Алгоритмы STL и лямбда-выражения // Фильтрация заказов по статусу vector<shared_ptr> filtered; copy_if(orders.begin(), orders.end(), back_inserter(filtered), [&status](const shared_ptr& order) { return order->getStatus() == status; });

// Подсчет общей суммы double total = accumulate(orders.begin(), orders.end(), 0.0, [](double sum, const shared_ptr& order) { return sum + order->getTotalPrice(); });

// Проверка прав доступа auto permission_check = [](const string& role, const string& required) -> bool { if (required == "admin") return role == "admin"; if (required == "manager") return role == "admin" || role == "manager"; return true; };

Логика ролей и прав доступа
Администратор

Полный доступ ко всем функциям системы
Управление продуктами: добавление, обновление, удаление
Просмотр всех заказов и их истории
Генерация отчетов в CSV формате
Доступ к полному журналу аудита
Менеджер

Утверждение заказов
Обновление информации о наличии товаров
Просмотр заказов в обработке
Ограниченный доступ к аудиту (только операции с заказами)
Покупатель

Создание и управление своими заказами
Оплата заказов через выбранную стратегию
Возврат заказов (при соблюдении условий)
Доступ только к своей истории заказов
Аудит и история изменений
Таблица order_status_history

Сохраняет полную историю изменений статусов заказов:

Старый и новый статус
Дата и время изменения
Пользователь, выполнивший изменение
Таблица audit_log

Фиксирует все критичные операции в системе:

Тип операции (insert, update, delete)
Тип сущности (order, product, user)
Идентификатор сущности
Пользователь, выполнивший операцию
Временная метка
Отчет в формате CSV
SQL-запрос для формирования отчета SELECT o.order_id, o.user_id, o.status, o.total_price, o.order_date, osh.old_status, osh.new_status, osh.changed_at, osh.changed_by, al.operation, al.performed_at FROM orders o LEFT JOIN order_status_history osh ON o.order_id = osh.order_id LEFT JOIN audit_log al ON al.entity_type = 'order' AND al.entity_id = o.order_id ORDER BY o.order_id, osh.changed_at, al.performed_at

Пример содержимого CSV: OrderID,UserID,Status,TotalPrice,OrderDate,OldStatus,NewStatus,ChangedAt,ChangedBy,Operation,PerformedAt 1,3,completed,50000.00,2026-01-12 17:10:51.32913,pending,completed,2026-01-12 16:08:43.105583,1,, 1,3,completed,50000.00,2026-01-12 17:10:51.32913,completed,returned,2026-01-12 17:07:29.420137,3,, 1,3,completed,50000.00,2026-01-12 17:10:51.32913,returned,completed,2026-01-12 17:10:51.32913,1,,

Сборка и запуск проекта
Требования к окружению

Компилятор: C++20
База данных: PostgreSQL 12+
Библиотеки: libpqxx 7.0+
Операционная система: Windows/Linux/macOS
Инструкция по сборке

Клонирование репозитория git clone cd online-store
-Создание директории сборки mkdir build cd build

Генерация проектных файлов cmake ..

Сборка проекта cmake --build .

Настройка базы данных

Создание базы данных createdb online_store3

Выполнение SQL-скриптов psql -d online_store3 -f sql/database_setup.sql

Запуск программы ./online_store

Пример работы
Выбор роли: СИСТЕМА ИНТЕРНЕТ-МАГАЗИНА

Войти как Администратор
Войти как Менеджер
Войти как Покупатель
Выход Выберите роль: 1 Меню администратора: АДМИН МЕНЮ
Добавить новый продукт
Обновить информацию о продукте
Удалить продукт
Просмотр всех заказов
Просмотр деталей заказа
Изменить статус заказа
Просмотр истории статусов заказа
Просмотр журнала аудита
Сформировать отчёт (CSV)
Выход
Пример транзацкии // Добавление продукта с транзакцией void addProduct(const string& name, double price, int stock) { try { db->beginTransaction(); string query = "INSERT INTO products VALUES (...)"; db->executeInTransaction(query); db->commitTransaction(); cout << "Продукт добавлен успешно" << endl; } catch (const exception& e) { db->rollbackTransaction(); cerr << "Ошибка: " << e.what() << endl; } }

Заключение
Проект демонстрирует применение современных практик C++ разработки:

Объектно-ориентированное проектирование
Управление ресурсами через умные указатели
Работа с реляционными базами данных
Реализация бизнес-логики с транзакциями
Система аудита и мониторинга
