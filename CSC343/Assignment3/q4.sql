SET SEARCH_PATH TO wetworldschema;

-- Calculate the highest, lowest, and average booking prices for each site
select s_id,max(price) as highest,min(price) as lowest,avg(price::numeric)::money as average 
from Bookings 
group by s_id 
order by s_id;
