from seller import Seller
import datetime
import uuid
import psycopg2
import configparser
from messages import *

"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""
def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]

class Mp2Client:
    def __init__(self, config_filename):
        cfg = configparser.ConfigParser()
        cfg.read(config_filename)

        self.db_conn_params = cfg["postgresql"]
        self.conn = None

    """
        Connects to PostgreSQL database and returns connection object.
    """
    def connect(self):
        self.conn = psycopg2.connect(**self.db_conn_params)
        self.conn.autocommit = False

    """
        Disconnects from PostgreSQL database.
    """
    def disconnect(self):
        self.conn.close()

    """
        Prints list of available commands of the software.
    """
    def help(self):
        # prints the choices for commands and parameters
        print("\n*** Please enter one of the following commands ***")
        print("> help")
        print("> sign_up <seller_id> <subscriber_key> <zip_code> <city> <state> <plan_id>")
        print("> sign_in <seller_id> <subscriber_key>")
        print("> sign_out")
        print("> show_plans")
        print("> show_subscription")
        print("> change_stock <product_id> <add or remove> <amount>")
        print("> show_quota")
        print("> subscribe <plan_id>")
        print("> ship <product_id_1> <product_id_2> <product_id_3> ... <product_id_n>")
        print("> calc_gross")
        print("> show_cart <customer_id>")
        print("> change_cart <customer_id> <product_id> <seller_id> <add or remove> <amount>")
        print("> purchase_cart <customer_id>")
        print("> quit")
    
    """
        Saves seller with given details.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_up(self, seller_id, sub_key, zip, city, state, plan_id):
        seller_query = "INSERT INTO sellers (seller_id, seller_zip_code_prefix, seller_city, seller_state) VALUES(%s, %s, %s, %s);"
        seller_subscription_query = "INSERT INTO seller_subscription (seller_id, subscriber_key, session_count, plan_id) VALUES(%s, %s, %s, %s);"
        try:
            cursor = self.conn.cursor()
            cursor.execute(seller_query,(seller_id,zip,city,state))
            cursor.execute(seller_subscription_query,(seller_id,sub_key,0,plan_id))
            self.conn.commit()
            return (True,CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)



    """
        Retrieves seller information if seller_id and subscriber_key is correct and seller's session_count < max_parallel_sessions.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If seller_id or subscriber_key is wrong, return tuple (None, USER_SIGNIN_FAILED).
        - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
    """
    def sign_in(self, seller_id, sub_key):
        get_seller_query = "SELECT seller_id,session_count,plan_id FROM seller_subscription WHERE seller_id = %s and subscriber_key = %s;"
        check_query = "SELECT COUNT(*) FROM seller_subscription WHERE seller_id = %s and subscriber_key = %s;"
        seller_subscription_query = "SELECT session_count FROM seller_subscription WHERE seller_id = %s and subscriber_key = %s;"
        subscription_plans_query = "SELECT sp.max_parallel_sessions FROM subscription_plans sp JOIN seller_subscription s ON s.plan_id = sp.plan_id WHERE s.seller_id = %s and s.subscriber_key = %s"
        increment_sessions_query = "UPDATE seller_subscription SET session_count = session_count + 1 WHERE seller_id = %s and subscriber_key = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(check_query,(seller_id,sub_key,))
            self.conn.commit()
            is_subscribed = cursor.fetchone()[0]
            if is_subscribed < 1:
                return (None,USER_SIGNIN_FAILED)
            cursor.execute(seller_subscription_query,(seller_id,sub_key,))
            self.conn.commit()
            current_session_count = cursor.fetchone()[0]
            cursor.execute(subscription_plans_query,(seller_id,sub_key,))
            self.conn.commit()
            max_session_count = cursor.fetchone()[0]
            if current_session_count >= max_session_count:
                return (None,USER_ALL_SESSIONS_ARE_USED)
            else:
                cursor.execute(increment_sessions_query,(seller_id,sub_key,))
                self.conn.commit()
                cursor.execute(get_seller_query,(seller_id,sub_key,))
                self.conn.commit()
                sid,count,pid = cursor.fetchone()
                seller = Seller(sid,count,pid)
                return (seller,CMD_EXECUTION_SUCCESS)

        except Exception:
            self.conn.rollback()
            return USER_SIGNIN_FAILED



    """
        Signs out from given seller's account.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Decrement session_count of the seller in the database.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_out(self, seller):
        session_query = "UPDATE seller_subscription SET session_count = (CASE when session_count >= 1 THEN (session_count - 1) ELSE 0 end) WHERE seller_id= %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(session_query,(seller.seller_id,))
            self.conn.commit()
            return (True,CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)


    """
        Quits from program.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Remember to sign authenticated user out first.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def quit(self, seller):
        if seller is None:
            return (True,CMD_EXECUTION_SUCCESS)
        else:
            try:
                (stat,msg) = self.sign_out(seller)
                if stat is True:
                    return (True,CMD_EXECUTION_SUCCESS)
                else:
                    return (False,CMD_EXECUTION_FAILED)
            except Exception:
                self.conn.rollback()
                return (False,CMD_EXECUTION_FAILED)



    """
        Retrieves all available plans and prints them.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print available plans and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        #|Name|Max Sessions|Max Stocks Per Product
        1|Basic|2|4
        2|Advanced|4|8
        3|Premium|6|12
    """
    def show_plans(self):
        plans_query = "SELECT plan_id, plan_name, max_parallel_sessions, max_stock_per_product FROM subscription_plans ORDER BY plan_id ASC;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(plans_query)
            self.conn.commit()
            all_plans = cursor.fetchall()
            print("#|Name|Max Sessions|Max Stocks Per Product")
            for pid, name, maxp, maxs in all_plans:
                print(f"{pid}|{name}|{maxp}|{maxs}")
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)
    
    def show_subscription(self, seller):
        subscription_query = "SELECT sp.plan_id, sp.plan_name, sp.max_parallel_sessions, sp.max_stock_per_product FROM subscription_plans sp WHERE %s = sp.plan_id;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(subscription_query,(seller.plan_id,))
            self.conn.commit()
            plans = cursor.fetchall()
            print("#|Name|Max Sessions|Max Stocks Per Product")
            for pid, name, maxp, maxs in plans:
                print(f"{pid}|{name}|{maxp}|{maxs}")
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)
    """
        Change stock count of a product.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If target product does not exist on the database, return tuple (False, PRODUCT_NOT_FOUND).
        - If target stock count > current plan's max_stock_per_product, return tuple (False, QUOTA_LIMIT_REACHED).
        - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def change_stock(self, seller, product_id, change_amount):
        product_found_query = "SELECT COUNT(*) FROM seller_stocks WHERE seller_id = %s and product_id = %s;"
        product_query = "SELECT stock_count FROM seller_stocks WHERE seller_id = %s and product_id = %s;"
        max_stock_query = "SELECT max_stock_per_product FROM subscription_plans WHERE plan_id = %s;"
        update_query = "UPDATE seller_stocks SET stock_count = stock_count + %s WHERE seller_id = %s and product_id = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(product_found_query,(seller.seller_id,product_id,))
            self.conn.commit()
            count = cursor.fetchone()[0]
            if count == 0:
                return (False, PRODUCT_NOT_FOUND)
            else:
                cursor.execute(product_query,(seller.seller_id,product_id,))
                self.conn.commit()
                stock_count = cursor.fetchone()[0]
                cursor.execute(max_stock_query,(seller.plan_id,))
                self.conn.commit()
                max_stock = cursor.fetchone()[0]
                if (int(stock_count) + int(change_amount) > int(max_stock)) or ( int(stock_count) + int(change_amount) < 0):
                    return (False, STOCK_UPDATE_FAILURE)

                cursor.execute(update_query,(int(change_amount),seller.seller_id,product_id,))
                self.conn.commit()
                return (seller,CMD_EXECUTION_SUCCESS)

        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)


    """
        Retrieves authenticated seller's remaining quota for stocks and prints it. 
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print the authenticated seller's quota and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).

        If the seller is subscribed to a plan with max_stock_per_product = 12 and
        the current stock for product 92bf5d2084dfbcb57d9db7838bac5cd0 is 10, then output should be like:
        
        Product Id|Remaining Quota
        92bf5d2084dfbcb57d9db7838bac5cd0|2

        If the seller does not have a stock, print 'Quota limit is not activated yet.'
    """
    def show_quota(self, seller):
        count_query = "SELECT COUNT(*) FROM seller_stocks WHERE seller_id = %s;"
        quota_query = "SELECT product_id,stock_count FROM seller_stocks WHERE seller_id = %s ORDER BY product_id ASC;"
        max_stock_query = "SELECT max_stock_per_product FROM subscription_plans WHERE plan_id = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(count_query,(seller.seller_id,))
            self.conn.commit()
            count = cursor.fetchone()[0]
            if count == 0:
                print(QUOTA_INACTIVE)
                return (True,CMD_EXECUTION_SUCCESS)
            else:
                cursor.execute(quota_query,(seller.seller_id,))
                self.conn.commit()
                stocks = cursor.fetchall()
                cursor.execute(max_stock_query,(seller.plan_id,))
                self.conn.commit()
                max_stock = cursor.fetchone()[0]
                print("Product Id|Remaining Quota")
                for product_id,stock_count in stocks:
                    remaining_quota = int(max_stock) - int(stock_count)
                    print(f"{product_id}|{remaining_quota}")
                return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)

    """
        Subscribe authenticated seller to new plan.
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If target plan does not exist on the database, return tuple (None, PRODUCT_NOT_FOUND).
        - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
        - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
    """
    def subscribe(self, seller, plan_id):
        exist_query = "SELECT COUNT(*) FROM subscription_plans WHERE plan_id = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(exist_query, (plan_id,))
            self.conn.commit()
            if cursor.fetchone()[0] != 1:
                return (None, SUBSCRIBE_PLAN_NOT_FOUND)
            subscription_query = "SELECT sp.plan_id, sp.plan_name, sp.max_parallel_sessions, sp.max_stock_per_product FROM subscription_plans sp WHERE %s = sp.plan_id;"
            cursor.execute(subscription_query,(plan_id,))
            self.conn.commit()
            next_plan_id, next_plan_name, next_max_session_count, next_max_product = cursor.fetchone()
            prev_subscription = "SELECT sp.plan_id, sp.plan_name, sp.max_parallel_sessions, sp.max_stock_per_product FROM subscription_plans sp WHERE %s = sp.plan_id;"
            cursor.execute(prev_subscription,(seller.plan_id,))
            self.conn.commit()
            curr_plan_id, curr_plan_name, curr_max_session_count, curr_max_product = cursor.fetchone()
            if curr_max_session_count <= next_max_session_count:
                update_query = "UPDATE seller_subscription SET plan_id = %s WHERE seller_id = %s;"
                cursor.execute(update_query,(plan_id,seller.seller_id,))
                self.conn.commit()
                seller.plan_id = plan_id
                return (seller,CMD_EXECUTION_SUCCESS)
            else:
                return (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE)
        except Exception:
            self.conn.rollback()
            return (None, CMD_EXECUTION_FAILED)

    """
        Change stock amounts for multiple distinct products.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If everything is OK and the operation is successful, return (True, CMD_EXECUTION_SUCCESS).
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any one of the product ids is incorrect; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - If any one of the products is not in the stock; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def ship(self, seller, product_ids):
        product_found_query = "SELECT COUNT(*) FROM seller_stocks WHERE seller_id = %s and product_id = %s;"
        product_query = "SELECT stock_count FROM seller_stocks WHERE seller_id = %s and product_id = %s;"
        update_query = "UPDATE seller_stocks SET stock_count = stock_count - 1 WHERE seller_id = %s and product_id = %s;"
        try:
            cursor = self.conn.cursor()
            for product_id in product_ids:
                cursor.execute(product_found_query,(seller.seller_id,product_id,))
                self.conn.commit()
                count = int(cursor.fetchone()[0])
                if count == 0:
                    return (False,CMD_EXECUTION_FAILED)

                cursor.execute(product_query,(seller.seller_id,product_id,))
                self.conn.commit()
                stock_count = int(cursor.fetchone()[0])
                if stock_count == 0:
                    return (False,CMD_EXECUTION_FAILED)

            for product_id in product_ids:
                cursor.execute(update_query,(seller.seller_id,product_id,))
                self.conn.commit()

            return (True,CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False,CMD_EXECUTION_FAILED)

    

    """
        Retrieves the gross income per product category for every month.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print the results and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        Gross Income|Year|Month
        123.45|2018|1
        67.8|2018|2
    """
    def calc_gross(self, seller):
        count_query = "SELECT COUNT(*) FROM order_items WHERE seller_id = %s;"
        order_items_query = "SELECT order_id,price FROM order_items WHERE seller_id = %s;"
        orders_query = "SELECT order_purchase_timestamp FROM orders WHERE order_id = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(count_query,(seller.seller_id,))
            self.conn.commit()
            count = cursor.fetchone()[0]
            if count == 0:
                print("Gross Income: 0")
                return (True, CMD_EXECUTION_SUCCESS)

            cursor.execute(order_items_query,(seller.seller_id,))
            self.conn.commit()
            print("Gross Income|Year|Month")
            values = cursor.fetchall()
            dates = {}
            for order_id, price in values:
                cursor.execute(orders_query,(order_id,))
                self.conn.commit()
                date = cursor.fetchone()[0]
                date = str(date)
                year = date[0:4]
                month = date[5:7]
                if (year,month) not in dates.keys():
                    dates[(year,month)] = float(price)
                else:
                    dates[(year,month)] += float(price)

            for year,month in dates.keys():
                print(f"{dates[(year,month)]}|{year}|{month}")
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)

    """
        Retrieves items on the customer's shopping cart
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print items on the cart and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        Seller Id|Product Id|Amount
        dd7ddc04e1b6c2c614352b383efe2d36|e5f2d52b802189ee658865ca93d83a8f|2
        5b51032eddd242adc84c38acab88f23d|c777355d18b72b67abbeef9df44fd0fd|3
        df560393f3a51e74553ab94004ba5c87|ac6c3623068f30de03045865e4e10089|1
    """
    def show_cart(self, customer_id):
        count_query = "SELECT COUNT(*) FROM customer_carts WHERE customer_id = %s;"
        cart_query = "SELECT seller_id, product_id, amount FROM customer_carts WHERE customer_id = %s ORDER BY seller_id ASC;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(count_query,(customer_id,))
            self.conn.commit()
            count = cursor.fetchone()[0]
            if count == 0:
                return (False,CUSTOMER_NOT_FOUND)

            cursor.execute(cart_query,(customer_id,))
            self.conn.commit()
            rows = cursor.fetchall()
            print("Seller Id|Product Id|Amount")
            for seller_id, product_id, amount in rows:
                print(f"{seller_id}|{product_id}|{amount}")
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)
        
    """
        Change count of items in shopping cart
        - Return type is a tuple, 1st element is a seller object and 2nd element is the response message from messages.py.
        - If customer does not exist on the database, return tuple (False, CUSTOMER_NOT_FOUND).
        - If target product does not exist on the database, return tuple (False, PRODUCT_NOT_FOUND).
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
        - Consider stocks of sellers when you add items to the cart.
    """
    def change_cart(self, customer_id, product_id, seller_id, change_amount):
        customer_query = "SELECT COUNT(*) FROM customers WHERE customer_id = %s;"
        product_query = "SELECT COUNT(*) FROM products WHERE product_id = %s;"
        cart_exist_query = "SELECT COUNT(*) FROM customer_carts WHERE seller_id = %s and product_id = %s and customer_id = %s;"
        update_query = "UPDATE customer_carts SET amount = amount + %s WHERE seller_id = %s and product_id = %s and customer_id = %s;"
        insert_query = "INSERT INTO customer_carts (customer_id,seller_id,product_id,amount) VALUES(%s, %s, %s, %s);"
        remove_query = "DELETE FROM customer_carts WHERE customer_id = %s and seller_id = %s and product_id = %s;"
        stocks_query = "SELECT stock_count FROM seller_stocks WHERE seller_id = %s and product_id = %s;"
        amount_query = "SELECT amount FROM customer_carts WHERE seller_id = %s and product_id = %s and customer_id = %s;"
        try:
            cursor = self.conn.cursor()
            cursor.execute(customer_query,(customer_id,))
            self.conn.commit()
            customer_count = cursor.fetchone()[0]
            if customer_count == 0:
                return (False, CUSTOMER_NOT_FOUND)

            cursor.execute(product_query,(product_id,))
            self.conn.commit()
            product_count = cursor.fetchone()[0]
            if product_count == 0:
                return (False, PRODUCT_NOT_FOUND)

            cursor.execute(stocks_query,(seller_id,product_id,))
            self.conn.commit()
            stock_count = int(cursor.fetchone()[0])
            cursor.execute(amount_query,(seller_id,product_id,customer_id,))
            self.conn.commit()
            c = cursor.fetchone()
            if c is not None:
                amount = int(c[0])
            cursor.execute(cart_exist_query,(seller_id,product_id,customer_id,))
            self.conn.commit()
            exist = cursor.fetchone()[0]
            if exist == 0 and int(change_amount) >= 0 and int(change_amount) <= stock_count: # no entry and add
                cursor.execute(insert_query,(customer_id,seller_id,product_id,change_amount))
                self.conn.commit()
                return (True,CMD_EXECUTION_SUCCESS)
            elif exist == 0 and int(change_amount) <= 0: # no entry and remove
                return (False,CMD_EXECUTION_FAILED)
            elif exist == 0 and int(change_amount) > stock_count:
                return (False,STOCK_UNAVAILABLE)
            elif amount + int(change_amount) > stock_count: # higher than stock_count
                return (False, STOCK_UNAVAILABLE)
            elif amount + int(change_amount) <= 0: # remove row since no amount left
                cursor.execute(remove_query,(customer_id,seller_id,product_id,))
                self.conn.commit()
                return (True, CMD_EXECUTION_SUCCESS)
            else: # update row
                cursor.execute(update_query,(change_amount,seller_id,product_id,customer_id,))
                self.conn.commit()
                return (True,CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)
    
    """
        Purchases items on the cart
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Actions:
        - Change stocks on seller_stocks table
        - Remove entries from customer_carts table
        - Add entries to order_items table
        - Add single entry to order table
    """
    def purchase_cart(self, customer_id):
        customer_query = "SELECT COUNT(*) FROM customers WHERE customer_id = %s;"
        amount_query = "SELECT c.seller_id,c.product_id,c.amount,s.stock_count FROM customer_carts c,seller_stocks s WHERE s.seller_id = c.seller_id and s.product_id = c.product_id and c.customer_id = %s;"
        update_stocks_query = "UPDATE seller_stocks SET stock_count = %s WHERE seller_id = %s and product_id = %s;"
        remove_cart_query = "DELETE FROM customer_carts WHERE customer_id = %s and seller_id = %s and product_id = %s;"
        orders_query = "INSERT INTO orders (order_id,customer_id,order_status,order_purchase_timestamp,order_approved_at,order_delivered_carrier_date,order_delivered_customer_date,order_estimated_delivery_date) VALUES(%s, %s, %s, %s, %s, %s, %s, %s);"
        order_items_query = "INSERT INTO order_items (order_id,order_item_id,product_id,seller_id,shipping_limit_date,price,freight_value) VALUES(%s, %s, %s, %s, %s, %s, %s);"
        try:
            cursor = self.conn.cursor()
            cursor.execute(customer_query,(customer_id,))
            self.conn.commit()
            count = cursor.fetchone()[0]
            if count == 0:
                return (False,CUSTOMER_NOT_FOUND)


            cursor.execute(amount_query,(customer_id,))
            self.conn.commit()
            values = cursor.fetchall()
            for seller_id,product_id,amount,stock in values:
                if int(amount) > int(stock):
                    return (False,STOCK_UNAVAILABLE)

            order_item_id = 1
            order_id = uuid.uuid4().hex  # random 32
            for seller_id, product_id, amount, stock in values:
                cursor.execute(update_stocks_query,(int(stock) - int(amount),seller_id,product_id,))
                self.conn.commit()
                cursor.execute(remove_cart_query,(customer_id,seller_id,product_id,))
                self.conn.commit()
                cursor.execute(order_items_query,(order_id,order_item_id,product_id,seller_id,None,None,None))
                self.conn.commit()

                order_item_id += 1

            cursor.execute(orders_query, (order_id, customer_id, None, datetime.datetime.now(), None, None, None, None))
            self.conn.commit()
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)
