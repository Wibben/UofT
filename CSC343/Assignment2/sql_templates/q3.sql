-- Q3. North and South Connections

-- You must not change the next 2 lines or the table definition.
SET SEARCH_PATH TO air_travel, public;
DROP TABLE IF EXISTS q3 CASCADE;

CREATE TABLE q3 (
    outbound VARCHAR(30),
    inbound VARCHAR(30),
    direct INT,
    one_con INT,
    two_con INT,
    earliest timestamp
);

-- Do this for each of the views that define your intermediate steps.  
-- (But give them better names!) The IF EXISTS avoids generating an error 
-- the first time this file is imported.
DROP VIEW IF EXISTS direct CASCADE;
DROP VIEW IF EXISTS cities CASCADE;


-- Define views for your intermediate steps here:

-- All direct flights on April 30, 2020
create view direct as
select a1.city as outbound, a2.city as inbound,s_dep,s_arv
from flight join airport a1 on outbound=a1.code
	join airport a2 on inbound=a2.code
where s_dep >= '2020-04-30' and s_arv < '2020-05-01';

-- All one-connection flights on April 30, 2020
create view one_con as
select f1.outbound,f2.inbound,f1.s_dep,f2.s_arv
from direct f1,direct f2
where f1.inbound=f2.outbound and f2.s_dep-f1.s_arv > '00:30:00';

-- All two-connection flights on April 30, 2020
create view two_con as
select f1.outbound, f2.inbound,f1.s_dep,f2.s_arv
from one_con f1,direct f2
where f1.inbound=f2.outbound and f2.s_dep-f1.s_arv > '00:30:00'
union all
select f1.outbound, f2.inbound,f1.s_dep,f2.s_arv
from direct f1,one_con f2
where f1.inbound=f2.outbound and f2.s_dep-f1.s_arv > '00:30:00';

-- Get a list of ann USA-Canada and Canada-USA airport combinations
create view cities as
select distinct a1.city as outbound,a2.city as inbound
from airport a1 join airport a2 on a1.city!=a2.city
where (a1.country='Canada' and a2.country='USA') or
	(a1.country='USA' and a2.country='Canada');

-- Count how many flights go from inbound to outbound for each type 
-- and record earliest arrival time
create view count_dir as
select cities.outbound,cities.inbound,count(s_arv) as num_flights,min(s_arv) as earliest
from cities left join direct 
	on cities.outbound=direct.outbound and cities.inbound=direct.inbound
group by cities.outbound,cities.inbound;

create view count_1 as
select cities.outbound,cities.inbound,count(s_arv) as num_flights,min(s_arv) as earliest
from cities left join one_con 
	on cities.outbound=one_con.outbound and cities.inbound=one_con.inbound
group by cities.outbound,cities.inbound;

create view count_2 as
select cities.outbound,cities.inbound,count(s_arv) as num_flights,min(s_arv) as earliest
from cities left join two_con 
	on cities.outbound=two_con.outbound and cities.inbound=two_con.inbound
group by cities.outbound,cities.inbound;

-- Your query that answers the question goes below the "insert into" line:
INSERT INTO q3
select cities.outbound,cities.inbound,
	count_dir.num_flights as direct, 
	count_1.num_flights as one_con, 
	count_2.num_flights as two_con,
	case
		when count_dir.earliest is not null and 
			(count_1.earliest is null or count_dir.earliest<=count_1.earliest) and
			(count_2.earliest is null or count_dir.earliest<=count_2.earliest)
			then count_dir.earliest
		when count_1.earliest is not null and 
			(count_dir.earliest is null or count_1.earliest<=count_dir.earliest) and
			(count_2.earliest is null or count_1.earliest<=count_2.earliest)
			then count_1.earliest
		when count_2.earliest is not null and 
			(count_dir.earliest is null or count_2.earliest<=count_dir.earliest) and
			(count_1.earliest is null or count_2.earliest<=count_1.earliest)
			then count_1.earliest
		else null
	end as earliest
from cities
	left join count_dir using(outbound,inbound)
	left join count_1 using(outbound,inbound)
	left join count_2 using(outbound,inbound);


