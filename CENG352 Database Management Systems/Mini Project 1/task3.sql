create table if not exists user_review_scores (
	product_id char(32),
	review_score float,
	primary key (product_id),
	foreign key (product_id) references products(product_id)
);

insert into user_review_scores 
select * from (select p.product_id as product_id, sum(or2.review_score::decimal)/count(*) as review_score
				from products p , order_items oi , orders o , order_reviews or2  
				where p.product_id = oi.product_id and oi.order_id = o.order_id and or2.order_id = o.order_id 
				group by p.product_id) as t

				
CREATE OR REPLACE FUNCTION func()
RETURNS TRIGGER
AS
$$
BEGIN
    IF true THEN
    UPDATE user_review_scores
    SET review_score  = (select sum(new.review_score::decimal)/count(*)
   											from products p , order_items oi , orders o , order_reviews or2
   											where p.product_id = oi.product_id and oi.order_id = o.order_id and 
   												user_review_scores.product_id  = p.product_id and or2.order_id = o.order_id and new.order_id = o.order_id  
   											group by p.product_id)  
   	where true ;
    END IF;

return user_review_scores;
END;
$$ LANGUAGE plpgsql;				
				
				
--trigger for user_review			
CREATE or replace TRIGGER trig
AFTER INSERT ON order_reviews
FOR EACH ROW  
EXECUTE PROCEDURE func();



--------------------------------------------
CREATE OR REPLACE FUNCTION zipcode()
RETURNS TRIGGER
AS
$$
BEGIN
    IF new.seller_zip_code_prefix = '00000' THEN
		RAISE EXCEPTION 'Zip code of the seller CAN not be zero!';
	else
		return new;
    END IF;

RETURN NULL;
END;
$$ LANGUAGE plpgsql;

-- trigger for zipcode
CREATE or replace TRIGGER zip
BEFORE INSERT ON sellers
FOR EACH ROW 
EXECUTE PROCEDURE zipcode();

-- This insertion throws an exception (ERROR: Zip code of the seller CAN not be zero!)
--insert into sellers(seller_id,seller_zip_code_prefix,seller_city,seller_state)
--values('00000000000000000000000000000012','00000','','');


--order product customer review standard view

create or replace view order_product_customer_review as
select o.order_id , oi.product_id , o.customer_id , or2.review_score 
from orders o ,order_items oi , order_reviews or2
where o.order_id = oi.order_id and o.order_id  = or2.order_id;

-- order product customer review materialized view

create materialized view order_product_customer_revieww as
select o.order_id , oi.product_id , o.customer_id , or2.review_score 
from orders o ,order_items oi , order_reviews or2
where o.order_id = oi.order_id and o.order_id  = or2.order_id;


-- A standard view is a virtual table that references underlying tables, while a materialized view is a physical copy of a query result set that is stored in the database.
-- The main advantage of a materialized view is that it can provide faster query performance, but at the cost of additional storage space and potentially slower data updates.
