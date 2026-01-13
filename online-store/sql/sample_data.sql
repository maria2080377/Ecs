INSERT INTO users (name, email, role, password_hash, loyalty_level) VALUES
('Администратор', 'admin@store.com', 'admin', 'hash1', 1),
('Менеджер', 'manager@store.com', 'manager', 'hash2', 1),
('Иван Покупатель', 'customer@store.com', 'customer', 'hash3', 0);

INSERT INTO products (name, price, stock_quantity) VALUES
('Ноутбук Lenovo', 50000.00, 10),
('Смартфон Samsung', 30000.00, 25),
('Наушники Sony', 5000.00, 50);
