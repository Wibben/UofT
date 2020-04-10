SET SEARCH_PATH TO wetworldschema;

insert into Divers (d_id,firstname,surname,birth_date,email,certification) values
-- Joseph Joestar, born August 23, 1968, is certified
(1,'Joseph','Joestar','1968-08-23','jojo2@bizarreadventure.com',true),
-- Yukihira Soma, born March 13, 2004, is certified
(2,'Yukihira','Soma','2004-03-13','yukihira@polarstar.com',true),
-- John Cena, born November 11, 1989, is certified
(3,'John','Cena','1989-11-11','cenaStar@wwe.com',false),
-- Kuroko Tetsuya, born September 11, 2004, is certified
(4,'Kuroko','Tetsuya','2004-09-11','ilovebasketball@seirin.com',false),
-- Kyrie Irving, born December 25, 1997, is not certified
(5,'Kyrie','Irving','1997-12-25','ball-handler@flatearth.com',false),
-- Connor McGregor, born January 31, 1988, is certified
(6,'Connor','McGregor','1988-01-31','bigfighterboi@mma.com',false),
-- Maury Povich, born September 9, 1991, is certified
(7,'Maury','Povich','1991-09-09','areyouthefather@mauryshow.com',true);

insert into Monitors (m_id,firstname,surname,email,
    max_group_size_open_water,max_group_size_cave,max_group_size_deep) values
-- Lisa Lisa can monitor 5 people in open water, 4 in caves, and 3 in deep water
(1,'Lisa','Lisa','lisalisa@bizarreadventure.com',5,4,3),
-- Lebron James can monitor 8 people in open water, 4 in caves, and 5 in deep water
(2,'Lebron','James','kingjames@tacotuesday.com',8,4,5),
-- Adam Silver can monitor 3 people in open water, 2 in caves, and 1 in deep water
(3,'Adam','Silver','endedtanking@NOL1stpick.com',3,2,1),
-- Jim Halpert can monitor 5 people in open water, 5 in caves, and 3 in deep water
(4,'Jim','Halpert','asianjim@dundermifflin.com',5,5,3);

insert into MonitorPrices (m_id,
	price_morning_open,price_afternoon_open,price_evening_open,
    price_morning_cave,price_afternoon_cave,price_evening_cave,
    price_morning_deep,price_afternoon_deep,price_evening_deep) values
-- Pricings for Lisa Lisa:
-- Open water: morning - 10, afternoon - 10, evening - 10
-- Caves: morning - 20, afternoon - 20, evening - 20
-- Deep water: morning - 20, afternoon - 20, evening - 20
(1,10,10,10,20,20,20,20,20,20),
-- Pricings for Lebron James
-- Open water: morning - 10, afternoon - 15, evening - 20
-- Caves: morning - 15, afternoon - 25, evening - 35
-- Deep water: morning - 15, afternoon - 20, evening - 25
(2,10,15,20,15,25,35,15,20,25),
-- Pricings for Adam Silver
-- Open water: morning - 15, afternoon - 20, evening - 20
-- Caves: morning - 20, afternoon - 25, evening - 25
-- Deep water: morning - 25, afternoon - 30, evening - 30
(3,15,20,20,20,25,25,25,30,30),
-- Pricings for Jim Halpert
-- Open water: morning - 15, afternoon - 15, evening - 15
-- Caves: morning - 20, afternoon - 20, evening - 20
-- Deep water: morning - 25, afternoon - 25, evening - 25
(4,15,15,15,20,20,20,25,25,25);

insert into DiveSites (s_id,name,
	max_capacity_morning_open_water,max_capacity_evening_open_water,
    max_capacity_morning_cave,max_capacity_evening_cave,
    max_capacity_morning_deep,max_capacity_evening_deep) values
-- Niagara Falls can have the following divers in the day:
-- 10 in open water, 8 in caves, 8 in deep water
-- For the evening: 5 in open water, 4 in caves, 4 in deep water
(1,'Niagara Falls',10,5,8,4,8,4), -- 15 per diver
-- Doki Doki Lit Club can have the following divers in the day:
-- 6 in open water, 5 in caves, 6 in deep water
-- For the evening: 5 in open water, 4 in caves, 5 in deep water
(2,'Doki Doki Lit Club',6,5,5,4,6,5), -- 20 per diver
-- Skydome can have the following divers in the day:
-- 15 in open water, no cave dives, 12 in deep water
-- For the afternoon: 10 in open water, no cave dives, 8 in deep water
(3,'Skydome',15,10,0,0,12,8), -- 15 per diver
-- Oracle Arena can have the following divers in the day:
-- 4 in open water, no cave or deep water dives
-- For the evening: 4 in open water, no cave or deep water dives
(4,'Oracle Arena',4,4,0,0,0,0); -- 10 per diver

insert into BookingPrivileges (m_id,s_id) values
-- Lisa Lisa can monitor in Niagara Falls, Doki Doki Lit Club, and Skydome
(1,1),
(1,2),
(1,3),
-- Lebron James can monitor in Niagara Falls, Skydome, and Oracle Arena
(2,1),
(2,3),
(2,4),
-- Adam Silver can monitor in Doki Doki Lit CLub and Skydome
(3,2),
(3,3),
-- Jim Halpert can monitor in Skydome and Oracle Arena
(4,3),
(4,4);

-- Rating is of monitors
insert into Bookings (b_id,d_id,s_id,m_id,datetime,
    dive_type,time_of_day,price,credit_card,rating) values
-- Joseph Joestar booked a evening deep dive with Lisa Lisa at Doki Doki Lit Club
-- on April 20, 2020, he paid $100 and rated Lisa 5 stars
(1,1,2,1,'2020-04-20 20:00','Deeper Than 30','Evening',100,1234567,5),
-- Yukihira Soma booked a morning cave dive with Lisa Lisa at Doki Doki Lit Club
-- on April 20, 2020, he paid $90 and did not rate Lisa
(2,2,2,1,'2020-04-20 10:00','Cave','Morning',90,3334445,NULL),
-- Yukihira Soma booked an afternoon deep water dive with Adam Silver at Skydome
-- on April 20, 2020, he paid $40 and rated Adam 0 stars
(3,2,3,3,'2020-04-20 16:00','Deeper Than 30','Afternoon',40,3334445,0),
-- Joseph Joestar booked a morning open water dive with Lebron James at Niagara Falls
-- on July 1, 2020, he paid $75 and rated Lebron 4 stars
(4,1,1,2,'2020-07-01 11:00','Open Water','Morning',75,1234567,4),
-- Maury Povich booked an afternoon open water dive with Jim Halpert at Oracle Arena
-- on August 4, 2020, he paid $50 and rated Jim 4 stars
(5,7,4,4,'2020-08-04 13:00','Open Water','Afternoon',50,1231231,4);

-- Rating is of dive sites
insert into DiverBookings (b_id,d_id,rating) values
-- Joseph (3 stars) has John (4 stars) and Kuroko (2 stars) 
-- in his group for his April 20 evening dive at Doki Doki Lit Club
(1,1,3),
(1,3,4),
(1,4,2),
-- Yukihira (Did Not Rate) has Connor (2 stars) and Maury (1 star) 
-- in his group for his April 20 morning dive ar Doki Doki Lit Club
(2,2,NULL),
(2,6,2),
(2,7,1),
-- Yukihira Soma (1 star) has only himself 
-- in his group for his April 20 afternoon dive at Skydome
(3,2,1),
-- Joesph (5 stars) has Yukihira (4 stars), John (4 stars), Kuroko (3 stars), and Maury (5 stars) -- in his group for his July 1 morning dive at Niagara Falls
(4,1,5),
(4,2,4),
(4,3,4),
(4,4,3),
(4,7,5),
-- Maury (3 stars) has Joseph (4 stars)
-- in his group for his August 4 afternoon dive at Oracle Arena
(5,7,3),
(5,1,4);

