select s1.seller_id as seller_id, (s1.total + s2.total + s3.total) as total_earned
from (select oi.seller_id as seller_id, sum(oi.price::decimal) as total, count(*) as count
		from order_items oi , orders o 
		where o.order_id = oi.order_id and o.order_purchase_timestamp < '2018-02-01' and o.order_purchase_timestamp >= '2018-01-01'
		group by oi.seller_id) as s1,
	(select oi.seller_id as seller_id, sum(oi.price::float) as total, count(*) as count
		from order_items oi , orders o 
		where o.order_id = oi.order_id and o.order_purchase_timestamp < '2018-03-01' and o.order_purchase_timestamp >= '2018-02-01'
		group by oi.seller_id) as s2,
	(select oi.seller_id as seller_id, sum(oi.price::float) as total, count(*) as count
		from order_items oi , orders o 
		where o.order_id = oi.order_id and o.order_purchase_timestamp < '2018-04-01' and o.order_purchase_timestamp >= '2018-03-01'
		group by oi.seller_id) as s3
where s1.seller_id = s2.seller_id and s2.seller_id = s3.seller_id and s1.count >= 50 and s2.count >= 50 and s3.count >= 50
--group by s1.seller_id
--having count(s3) >= 50 and count(s2) >= 50 and count(s1) >= 50
order by total_earned desc;