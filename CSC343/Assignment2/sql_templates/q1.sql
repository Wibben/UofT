-- Q1. Airlines

-- You must not change the next 2 lines or the table definition.
SET SEARCH_PATH TO air_travel, public;
DROP TABLE IF EXISTS q1 CASCADE;

CREATE TABLE q1 (
    pass_id INT,
    name VARCHAR(100),
    airlines INT
);

-- Do this for each of the views that define your intermediate steps.  
-- (But give them better names!) The IF EXISTS avoids generating an error 
-- the first time this file is imported.
DROP VIEW IF EXISTS flightsTaken CASCADE;

-- Define views for your intermediate steps here:
-- Which flight each passenger has taken
create view flightsTaken as
select passenger.id, firstname||' '||surname as name, flight_id
from passenger left join booking on passenger.id=pass_id
order by passenger.id;

-- Your query that answers the question goes below the "insert into" line:
INSERT INTO q1
select flightsTaken.id, name, count(distinct airline)
from flightsTaken left join flight on flight_id = flight.id
group by flightsTaken.id, name;
