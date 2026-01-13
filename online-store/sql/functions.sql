CREATE OR REPLACE FUNCTION get_order_status(order_id_param INTEGER)
RETURNS VARCHAR AS $$
DECLARE
    status_result VARCHAR;
BEGIN
    SELECT status INTO status_result FROM orders WHERE order_id = order_id_param;
    RETURN COALESCE(status_result, 'not_found');
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION get_user_order_count(user_id_param INTEGER)
RETURNS INTEGER AS $$
DECLARE
    count_result INTEGER;
BEGIN
    SELECT COUNT(*) INTO count_result FROM orders WHERE user_id = user_id_param;
    RETURN COALESCE(count_result, 0);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION get_total_spent_by_user(user_id_param INTEGER)
RETURNS DECIMAL AS $$
DECLARE
    total_result DECIMAL;
BEGIN
    SELECT COALESCE(SUM(total_price), 0) INTO total_result 
    FROM orders 
    WHERE user_id = user_id_param AND status IN ('completed', 'returned');
    RETURN total_result;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION can_return_order(order_id_param INTEGER)
RETURNS BOOLEAN AS $$
DECLARE
    order_status VARCHAR;
    order_date TIMESTAMP;
    days_passed INTEGER;
BEGIN
    SELECT status, order_date INTO order_status, order_date 
    FROM orders WHERE order_id = order_id_param;
    
    IF order_status != 'completed' THEN
        RETURN FALSE;
    END IF;
    
    days_passed := EXTRACT(DAY FROM (CURRENT_TIMESTAMP - order_date));
    RETURN days_passed <= 30;
END;
$$ LANGUAGE plpgsql;
