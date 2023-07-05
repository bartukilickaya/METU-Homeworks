select o1.customer_id, extract(year from  o1.order_purchase_timestamp ) as year, extract(month from o1.order_purchase_timestamp) as month, card.total as max_total_for_month
from
    (select oi.order_id, sum(oi.price::float) as total
     from order_items oi
     group by oi.order_id) as card
	JOIN orders o1 ON card.order_id = o1.order_id
where not exists ( 
    select customer_total.total
    from (select oi.order_id, sum(oi.price::float) as total
         from order_items oi
         group by oi.order_id) as customer_total
    JOIN orders o ON customer_total.order_id=o.order_id
    where extract(year from o.order_purchase_timestamp ) = extract(year from o1.order_purchase_timestamp) and 
    	extract(month from o.order_purchase_timestamp) = extract(month from o1.order_purchase_timestamp) and card.total < customer_total.total
    )
order by year asc , month asc,o1.customer_id asc;