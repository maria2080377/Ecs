CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    role VARCHAR(20) CHECK (role IN ('admin', 'manager', 'customer')),
    password_hash VARCHAR(255) NOT NULL,
    loyalty_level INTEGER DEFAULT 0 CHECK (loyalty_level IN (0, 1)),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL CHECK (price > 0),
    stock_quantity INTEGER NOT NULL CHECK (stock_quantity >= 0),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(user_id) ON DELETE CASCADE,
    status VARCHAR(20) DEFAULT 'pending' CHECK (status IN ('pending', 'approved', 'completed', 'canceled', 'returned')),
    total_price DECIMAL(10,2) DEFAULT 0.00,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id) ON DELETE CASCADE,
    product_id INTEGER REFERENCES products(product_id),
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    price DECIMAL(10,2) NOT NULL CHECK (price > 0)
);

CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INTEGER REFERENCES orders(order_id) ON DELETE CASCADE,
    old_status VARCHAR(20),
    new_status VARCHAR(20) NOT NULL,
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INTEGER REFERENCES users(user_id)
);

CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    entity_type VARCHAR(20) NOT NULL CHECK (entity_type IN ('user', 'product', 'order')),
    entity_id INTEGER NOT NULL,
    operation VARCHAR(20) NOT NULL CHECK (operation IN ('insert', 'update', 'delete')),
    performed_by INTEGER REFERENCES users(user_id),
    performed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    details TEXT
);
