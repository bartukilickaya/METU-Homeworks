select p.product_id , (sum(or2.review_score::decimal) / count(*)) as review_avg, count(*) as review_count
from products p , order_items oi , orders o , order_reviews or2 
where p.product_id = oi.product_id and oi.order_id = o.order_id and o.order_id = or2.order_id and p.product_category_name = 'eletronicos'
group by p.product_id 
having count(*) >= 5
order by review_avg desc,review_count desc
limit 10;