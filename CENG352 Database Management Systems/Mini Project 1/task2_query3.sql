select p.product_category_name , count(*) as late_delivery_count
from products p , order_items oi , orders o , customers c 
where p.product_id = oi.product_id and oi.order_id = o.order_id and o.customer_id = c.customer_id and c.customer_city = 'rio de janeiro' and 
	o.order_delivered_customer_date  > o.order_estimated_delivery_date and o.order_purchase_timestamp > '2018-06-01' and 
	o.order_purchase_timestamp < '2018-09-01' and o.order_status = 'delivered'
group by p.product_category_name 
order by late_delivery_count desc, p.product_category_name asc;