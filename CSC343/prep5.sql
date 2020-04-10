SET search_path TO world;
SELECT count(*) as "Countries that speak English" FROM countrylanguage WHERE countrylanguage='English';
SELECT count(countrylanguage) as "Languages spoken in Mozambique" from countrylanguage,country WHERE countrycode=code AND name='Mozambique';
