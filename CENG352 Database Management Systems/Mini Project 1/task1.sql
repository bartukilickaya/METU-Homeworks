create table if not exists product_category (
	product_category_name varchar(80),
	product_category_name_english varchar(80),
	primary key (product_category_name)
);

create table if not exists customers (
	customer_id char(32),
	customer_unique_id char(32),
	customer_zip_code_prefix varchar(10),
	customer_city varchar(32),
	customer_state varchar(5),
	primary key (customer_id)
);

create table if not exists geolocation (
	geolocation_zip_code_prefix varchar(10),
	geolocation_lat varchar(32),
	geolocation_lng varchar(32),
	geolocation_city varchar(80),
	geolocation_state varchar(10)
);

create table if not exists orders (
	order_id char(32),
	customer_id char(32),
	order_status varchar(32),
	order_purchase_timestamp timestamp,
	order_approved_at timestamp,
	order_delivered_carrier_date timestamp,
	order_delivered_customer_date timestamp,
	order_estimated_delivery_date timestamp,
	primary key (order_id),
	foreign key (customer_id) references customers(customer_id)
);

create table if not exists order_payments (
	order_id char(32),
	payment_sequential varchar(10),
	payment_type varchar(20),
	payment_installments varchar(10),
	payment_value varchar(32),
	foreign key (order_id) references orders(order_id)
);

create table if not exists sellers (
	seller_id char(32),
	seller_zip_code_prefix varchar(20),
	seller_city varchar(80),
	seller_state varchar(10),
	primary key (seller_id)
);

create table if not exists products (
	product_id char(32),
	product_category_name varchar(80),
	product_name_length varchar(20),
	product_description_length varchar(20),
	product_photos_qty varchar(20),
	product_weight_g varchar(20),
	product_length_cm varchar(20),
	product_height_cm varchar(20),
	product_width_cm varchar(20),
	primary key (product_id)
);


create table if not exists order_items (
	order_id char(32),
	order_item_id char(32),
	product_id char(32),
	seller_id char(32),
	shipping_limit_date timestamp,
	price varchar(20),
	freight_value varchar(10),
	foreign key (order_id) references orders(order_id),
	foreign key (product_id) references products(product_id),
	foreign key (seller_id) references sellers(seller_id)
);

create table if not exists order_reviews(
	review_id char(32),
	order_id char(32),
	review_score varchar(10),
	review_comment_title varchar(32),
	review_comment_message varchar(500),
	review_creation_date timestamp,
	review_answer_timestamp timestamp,
	primary key (review_id,order_id),
	foreign key (order_id) references orders(order_id)
);
