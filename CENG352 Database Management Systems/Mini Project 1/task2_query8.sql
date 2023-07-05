select p3.product_category_name,count(*) as past_order_count
from customers c ,orders o ,order_items oi2 ,products p3 , (select distinct p2.product_category_name as namee
		from products p2
		where p2.product_category_name not in (select distinct p.product_category_name
												from products p, order_items oi , sellers s 
												where p.product_id = oi.product_id and s.seller_id = oi.seller_id and s.seller_city = 'sao paulo')) as prods
where c.customer_id = o.customer_id and o.order_id = oi2.order_id and oi2.product_id = p3.product_id and prods.namee = p3.product_category_name and 
	c.customer_city = 'sao paulo'
group by p3.product_category_name 
having count(*) >= 10
order by p3.product_category_name asc;