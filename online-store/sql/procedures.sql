CREATE OR REPLACE PROCEDURE create_order_procedure(
    IN user_id_param INTEGER,
    IN product_id_param INTEGER,
    IN quantity_param INTEGER,
    OUT new_order_id INTEGER,
    OUT status_message TEXT
)
LANGUAGE plpgsql
AS $$
DECLARE
    product_price DECIMAL;
    product_stock INTEGER;
BEGIN
    BEGIN
        SELECT price, stock_quantity INTO product_price, product_stock
        FROM products WHERE product_id = product_id_param;
        
        IF product_stock < quantity_param THEN
            RAISE EXCEPTION 'Недостаточно товара на складе';
        END IF;
        
        INSERT INTO orders (user_id, total_price) 
        VALUES (user_id_param, product_price * quantity_param)
        RETURNING order_id INTO new_order_id;
        
        INSERT INTO order_items (order_id, product_id, quantity, price)
        VALUES (new_order_id, product_id_param, quantity_param, product_price);
        
        UPDATE products SET stock_quantity = stock_quantity - quantity_param
        WHERE product_id = product_id_param;
        
        INSERT INTO audit_log (entity_type, entity_id, operation, performed_by, details)
        VALUES ('order', new_order_id, 'insert', user_id_param, 
                'Создан заказ с товаром #' || product_id_param);
        
        COMMIT;
        status_message := 'Заказ успешно создан';
        
    EXCEPTION WHEN OTHERS THEN
        ROLLBACK;
        new_order_id := -1;
        status_message := 'Ошибка: ' || SQLERRM;
    END;
END;
$$;
