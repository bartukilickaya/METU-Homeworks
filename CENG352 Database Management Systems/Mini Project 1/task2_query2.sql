select averages.seller_id as seller_id, average as avg_duration
from 
	(select s2.seller_id as seller_id , avg(o2.order_delivered_carrier_date - o2.order_purchase_timestamp) as average
		from sellers s2 , order_items oi2 , orders o2, products p2 ,
		(select s.seller_id as seller_id 
			from sellers s , products p , order_items oi , orders o 
			where oi.product_id  = p.product_id and s.seller_id = oi.seller_id and o.order_id = oi.order_id and 
				p.product_category_name = 'beleza_saude' and s.seller_city = 'curitiba') as s1
				where s2.seller_id = s1.seller_id and s2.seller_id = oi2.seller_id and o2.order_id = oi2.order_id and 
					p2.product_category_name = 'beleza_saude' and oi2.product_id = p2.product_id 
				group by s2.seller_id) as averages
where average <= '2 days'--::interval
order by average desc;


