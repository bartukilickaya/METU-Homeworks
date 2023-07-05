select distinct c.customer_city 
from customers c , orders o , order_payments op 
where o.customer_id = o.customer_id and o.order_id = op.order_id and op.payment_type = 'voucher' and c.customer_id not in (select c2.customer_id 
																							from customers c2 , orders o2 ,order_payments op2 
																							where c2.customer_id = o2.customer_id and o2.order_id = op2.order_id
																								and op2.payment_type != 'voucher')
order by c.customer_city asc;