select t1.product_id as product_id, t1.countt as total_sales,RANK() OVER(PARTITION BY t1.product_category_name 
																			ORDER BY t1.countt DESC) sales_rank , t1.product_category_name as product_category_name 
from (select p.product_id , count(*) as countt, p.product_category_name 
	from products p , order_items oi 
	where p.product_id = oi.product_id and p.product_category_name != ''
	group by p.product_id 
	having count(*) >= 10) as t1
--where
order by t1.product_category_name asc,total_sales desc;

