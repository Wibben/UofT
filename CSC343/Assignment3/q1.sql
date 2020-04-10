SET SEARCH_PATH TO wetworldschema;

drop view if exists openWaterSites cascade;
drop view if exists caveSites cascade;
drop view if exists deepWaterSites cascade;

-- Get a list of sites that allow open water diving and how many monitors can book there
create view openWaterSites as 
select DiveSites.s_id,name,count(m_id) as Num_Monitors 
from DiveSites join BookingPrivileges on DiveSites.s_id=BookingPrivileges.s_id 
where max_capacity_morning_open_water>0 or max_capacity_evening_open_water>0 
group by DiveSites.s_id,name 
order by DiveSites.s_id;

-- Get a list of sites that allow cave diving and how many monitors can book there
create view caveSites as 
select DiveSites.s_id,name,count(m_id) as Num_Monitors 
from DiveSites join BookingPrivileges on DiveSites.s_id=BookingPrivileges.s_id 
where max_capacity_morning_cave>0 or max_capacity_evening_cave>0 
group by DiveSites.s_id,name 
order by DiveSites.s_id;

-- Get a list of sites that allow deep water diving and how many monitors can book there
create view deepWaterSites as 
select DiveSites.s_id,name,count(m_id) as Num_Monitors 
from DiveSites join BookingPrivileges on DiveSites.s_id=BookingPrivileges.s_id 
where max_capacity_morning_deep>0 or max_capacity_evening_deep>0 
group by DiveSites.s_id,name 
order by DiveSites.s_id;

-- Count the number of sites in each category that have at least 1 monitor that can book there
select * 
from 
	(select count(*) as num_open_water from openWaterSites where Num_Monitors>0) as open 
	cross join
	(select count(*) as num_cave from caveSites where Num_Monitors>0) as cave 
	cross join
	(select count(*) as num_deep_water from deepWaterSites where Num_Monitors>0) as deep;

