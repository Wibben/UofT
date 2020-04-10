SET SEARCH_PATH TO wetworldschema;

drop view if exists monitorRatings cascade;
drop view if exists siteRatings cascade;

-- Find the average rating for each monitor
create view monitorRatings as 
select m_id,avg(rating) as avg_mon_rating 
from Bookings 
group by m_id;

-- Find the average rating for each dive site
create view siteRatings as 
select s_id,avg(DiverBookings.rating) as avg_site_rating
from Bookings join DiverBookings on Bookings.b_id=DiverBookings.b_id 
group by s_id;

-- Get a list of all monitors and their dive sites and their ratings
create view ratingChart as 
select m_id,avg_mon_rating,s_id,avg_site_rating 
from BookingPrivileges natural join monitorRatings natural join siteRatings;

-- Get only the list of monitors whose average rating is higher than that of their dive sites
create view monitorList as
select m_id
from
	(select distinct m_id from ratingChart) as allMonitors
	except
	(select distinct m_id from ratingChart 
	 where avg_site_rating>avg_mon_rating);

-- Calculate the chosen monitors' average prices and get their emails
select m_id,firstname||' '||surname as name,
	(price_morning_open + price_afternoon_open + price_evening_open + 
     price_morning_cave + price_afternoon_cave + price_evening_cave + 
     price_morning_deep + price_afternoon_deep + price_evening_deep)/9 as avg_booking_fee,email 
from monitorList natural join Monitors natural join MonitorPrices 
order by m_id;

