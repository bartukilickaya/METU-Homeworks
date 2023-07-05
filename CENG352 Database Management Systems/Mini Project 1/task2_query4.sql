select s.seller_id , avg(or2.review_score::decimal) as avg_rating , count(*) as review_count
from products p , order_items oi , sellers s , orders o , order_reviews or2
where p.product_id = oi.product_id and oi.seller_id = s.seller_id and o.order_id = or2.order_id and o.order_id = oi.order_id  and
	s.seller_city = 'sao paulo' and p.product_category_name = 'automotivo'
group by s.seller_id
having count(*) >= 10
order by avg_rating desc
fetch first 5 rows only;