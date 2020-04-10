-- Q2. Refunds!

-- You must not change the next 2 lines or the table definition.
SET SEARCH_PATH TO air_travel, public;
DROP TABLE IF EXISTS q2 CASCADE;

CREATE TABLE q2 (
    airline CHAR(2),
    name VARCHAR(50),
    year CHAR(4),
    seat_class seat_class,
    refund REAL
);

-- Do this for each of the views that define your intermediate steps.  
-- (But give them better names!) The IF EXISTS avoids generating an error 
-- the first time this file is imported.
DROP VIEW IF EXISTS flightData CASCADE;

-- Define views for your intermediate steps here:
-- Required data from each flight
create view flightData as
select id,flight.airline,airline.name,
	s_dep,departure.datetime as dep,s_arv,arrival.datetime as arv,
	a1.country = a2.country as isDomestic,EXTRACT(year from s_dep) as year
from (((flight join airport a1 on outbound=a1.code) 
	join airport a2 on inbound=a2.code)
	join airline on flight.airline = airline.code)
	join departure on flight.id=departure.flight_id
	join arrival on flight.id=arrival.flight_id;

select * from flightData;

-- Domestic flights that refund 50%
create view domestic50 as
select id
from flightData
where isDomestic and
	dep-s_dep > '10:00:00' and
	arv-s_arv > (dep-s_dep)/2;

create view dom50refund as
select airline,name,year,seat_class,sum(price)*0.5 as refund
from domestic50 natural join flightData join booking on flightData.id=booking.flight_id
group by airline,name,year,seat_class;

-- Domestic flights that refund 35%
create view domestic35 as
select id
from flightData
where isDomestic and
	dep-s_dep > '4:00:00' and
	arv-s_arv > (dep-s_dep)/2
except 
select id from domestic50;

create view dom35refund as
select airline,name,year,seat_class,sum(price)*0.35 as refund
from domestic35 natural join flightData join booking on flightData.id=booking.flight_id
group by airline,name,year,seat_class;

-- International flights that refund 50%
create view international50 as
select id
from flightData
where NOT isDomestic and
	dep-s_dep > '00:20:00' and
	arv-s_arv > (dep-s_dep)/2;

create view int50refund as
select airline,name,year,seat_class,sum(price)*0.5 as refund
from international50 natural join flightData join booking on flightData.id=booking.flight_id
group by airline,name,year,seat_class;

-- International flights that refund 35%
create view international35 as
select id
from flightData
where NOT isDomestic and
	dep-s_dep > '00:05:00' and
	arv-s_arv > (dep-s_dep)/2
except
select id from international50;

create view int35refund as
select airline,name,year,seat_class,sum(price)*0.35 as refund
from international35 natural join flightData join booking on flightData.id=booking.flight_id
group by airline,name,year,seat_class;

-- Your query that answers the question goes below the "insert into" line:
INSERT INTO q2
select airline,name,year,seat_class,sum(refund)
from (select * from dom50refund union all 
	select * from dom35refund union all 
	select * from int50refund union all 
	select * from int35refund) as allRefunds
group by airline,name,year,seat_class;
