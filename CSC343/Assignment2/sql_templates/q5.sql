-- Q5. Flight Hopping

-- You must not change the next 2 lines or the table definition.
SET SEARCH_PATH TO air_travel, public;
DROP TABLE IF EXISTS q5 CASCADE;

CREATE TABLE q5 (
	destination CHAR(3),
	num_flights INT
);

-- Do this for each of the views that define your intermediate steps.  
-- (But give them better names!) The IF EXISTS avoids generating an error 
-- the first time this file is imported.
DROP VIEW IF EXISTS intermediate_step CASCADE;
DROP VIEW IF EXISTS day CASCADE;
DROP VIEW IF EXISTS n CASCADE;

CREATE VIEW day AS
SELECT day::date as day FROM q5_parameters;
-- can get the given date using: (SELECT day from day)

CREATE VIEW n AS
SELECT n FROM q5_parameters;
-- can get the given number of flights using: (SELECT n from n)

-- HINT: You can answer the question by writing one recursive query below, without any more views.
-- Your query that answers the question goes below the "insert into" line:
INSERT INTO q5
with recursive flight_hops as (
	-- Inital flights departing from YYZ on day	
	(select inbound as destination,s_arv, 1 as num_flights
	from flight,day
	where outbound='YYZ' and s_dep::date=day.day)

	union all
	
	-- All flights that can be hopped to
	(select inbound as destination,flight.s_arv, num_flights+1
	from flight_hops,flight,n
	where num_flights<n.n and
		destination=outbound and
		flight.s_dep>flight_hops.s_arv and
		flight.s_dep-flight_hops.s_arv<'24:00:00')
)
select destination,num_flights from flight_hops;















