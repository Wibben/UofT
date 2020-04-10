-- Q4. Plane Capacity Histogram

-- You must not change the next 2 lines or the table definition.
SET SEARCH_PATH TO air_travel, public;
DROP TABLE IF EXISTS q4 CASCADE;

CREATE TABLE q4 (
	airline CHAR(2),
	tail_number CHAR(5),
	very_low INT,
	low INT,
	fair INT,
	normal INT,
	high INT
);

-- Do this for each of the views that define your intermediate steps.  
-- (But give them better names!) The IF EXISTS avoids generating an error 
-- the first time this file is imported.
DROP VIEW IF EXISTS capacity_data CASCADE;

-- Define views for your intermediate steps here:
-- Get a list of all the flights (that have departed) from each plane
create view capacity_data as
select flight.id,plane.airline,tail_number,
	100*count(seat_class)/(capacity_economy+capacity_business+capacity_first) as percent_capacity
from plane join flight on plane.tail_number=flight.plane
	join departure on flight.id=departure.flight_id
	left join booking on flight.id=booking.flight_id
group by flight.id,plane.airline,tail_number
order by flight.id;

-- Your query that answers the question goes below the "insert into" line:
INSERT INTO q4
select airline,tail_number,
	sum(case when percent_capacity<20 then 1 else 0 end) as very_low,
	sum(case when percent_capacity>=20 and percent_capacity<40 then 1 else 0 end) as low,
	sum(case when percent_capacity>=40 and percent_capacity<60 then 1 else 0 end) as fair,
	sum(case when percent_capacity>=60 and percent_capacity<80 then 1 else 0 end) as normal,
	sum(case when percent_capacity>=80 then 1 else 0 end) as high
from capacity_data
group by airline,tail_number;

