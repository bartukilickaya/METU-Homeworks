select s.seller_id as seller_id , extract(month from oi.shipping_limit_date) as month, sum(oi.freight_value::decimal) as sum
	from order_items oi , sellers s 
	where oi.seller_id = s.seller_id and extract(year from oi.shipping_limit_date) = '2018'
	group by (s.seller_id, month)
union
select t1.seller_id as seller_id , null, sum(total_freight) as total_freight 
from (select s.seller_id as seller_id , extract(month from oi.shipping_limit_date) as month, sum(oi.freight_value::decimal) as total_freight
	from order_items oi , sellers s 
	where oi.seller_id = s.seller_id and extract(year from oi.shipping_limit_date) = '2018'
	group by (s.seller_id, month)) as t1
group by t1.seller_id
union
select null , extract(month from oi.shipping_limit_date) as month, sum(oi.freight_value::decimal)
from sellers s , order_items oi 
where oi.seller_id = s.seller_id and extract(year from oi.shipping_limit_date) = '2018'
group by month
union
select null,null,sum(oi.freight_value::decimal)
from sellers s, order_items oi
where s.seller_id = oi.seller_id and extract(year from oi.shipping_limit_date) = '2018'
order by seller_id asc, month asc;