SET SEARCH_PATH TO wetworldschema;

drop view if exists numDiversPerBooking cascade;
drop view if exists capacityPerBooking cascade;

-- Get the number of divers for each dive/booking, add 1 for the monitor
create view numDiversPerBooking as 
select b_id,count(d_id)+1 as num_divers 
from DiverBookings
group by b_id;

-- Get the capacity for each booking based on type of dive
create view capacityPerBooking as
select b_id,s_id,price,
	(case
	 	when dive_type='Open Water' and (time_of_day='Morning' or time_of_day='Afternoon') 
			then max_capacity_morning_open_water 
		when dive_type='Open Water' and time_of_day='Evening' 
			then max_capacity_evening_open_water 
		when dive_type='Cave' and (time_of_day='Morning' or time_of_day='Afternoon') 
			then max_capacity_morning_cave 
		when dive_type='Cave' and time_of_day='Evening' 
			then max_capacity_evening_cave 
		when dive_type='Deeper Than 30' and (time_of_day='Morning' or time_of_day='Afternoon')  			then max_capacity_morning_deep 
		when dive_type='Deeper Than 30' and time_of_day='Evening' 
			then max_capacity_evening_deep 
	 end) as site_capacity
from Bookings natural join DiveSites;

-- Calculate average fullness of each dive site (i.e. average of num_divers/site_capacity)
create view siteFullness as 
select s_id,avg(1.0*num_divers/site_capacity) as avg_fullness,
	sum(price) as total_price,count(b_id) as num_dives
from numDiversPerBooking natural join capacityPerBooking 
group by s_id;

-- Calculate average price per dive for dive sites that are more than half full on average
-- and for dive sites that ate less than or equal to half full on average
select *
from 
	(select sum(total_price)/sum(num_dives) as average_dive_fee_more_full 
	 from siteFullness 
	 where avg_fullness>0.5) as more_than_half
	cross join
	(select sum(total_price)/sum(num_dives) as average_dive_fee_less_full 
	 from siteFullness 
	 where avg_fullness<=0.5) as less_than_half;

