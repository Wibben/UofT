set search_path to World;
insert into country values
('BOR','Borduria','Oceania',1000),
('CAG','Cagliostro','Oceania',250),
('QUM','Qumar','Oceania',3380);
select sum(population) from country where continent='Oceania';

update country set continent = 'Europe' where code = 'BOR';
select count(*) as "Small European Countries" from country where continent = 'Europe' and population < 50000;

insert into countrylanguage values
('DEU','Spanish','f',1),
('BOL','Arabic','f',1);
select count(*) as "Number of Languages Spoken in Germany" 
from countrylanguage,country where name='Germany' and countrycode=code;
select count(*) as "Number of Continents That speak Arabic" from 
(select continent
from countrylanguage,country where countrylanguage='Arabic' and countrycode=code
group by continent) as ArabicInEachContinent;

delete from countrylanguage where isofficial='f';
select count(*) as "Number of Languages Spoken in Africa" from
(select countrylanguage
from countrylanguage,country 
where continent='Africa' and countrycode=code) as OfficialLanguagesInAfrica;
