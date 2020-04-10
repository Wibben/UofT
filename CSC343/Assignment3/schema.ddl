drop schema if exists wetworldschema cascade;
create schema wetworldschema;
set search_path to wetworldschema;

-- Divers.
CREATE TABLE Divers (
  d_id INT PRIMARY KEY,
  -- The first name of the diver.
  firstname VARCHAR(50) NOT NULL,
  -- The surname of the diver.
  surname VARCHAR(50) NOT NULL,
  -- The birthdate of the diver
  birth_date DATE NOT NULL,
  -- The email of the diver.
  email varchar(30) NOT NULL,
  -- Certification status of the diver
  certification BOOLEAN
);

-- Dive Monitors.
CREATE TABLE Monitors (
  m_id INT PRIMARY KEY,
  -- The first name of the monitor.
  firstname VARCHAR(50) NOT NULL,
  -- The surname of the monitor.
  surname VARCHAR(50) NOT NULL,
  -- The email of the monitor.
  email varchar(30) NOT NULL,
  -- Max group size open water.
  max_group_size_open_water INT DEFAULT 0,
  -- Max group size cave.
  max_group_size_cave INT DEFAULT 0,
  -- Max group size deeper than 30.
  max_group_size_deep INT DEFAULT 0
); 

CREATE TABLE MonitorPrices(
  m_id INT PRIMARY KEY,
  -- price depending on time and type of dive
  price_morning_open MONEY NOT NULL,
  price_afternoon_open MONEY NOT NULL,
  price_evening_open MONEY NOT NULL,
  price_morning_cave MONEY NOT NULL,
  price_afternoon_cave MONEY NOT NULL,
  price_evening_cave MONEY NOT NULL,
  price_morning_deep MONEY NOT NULL,
  price_afternoon_deep MONEY NOT NULL,
  price_evening_deep MONEY NOT NULL,  

  FOREIGN KEY (m_id) references Monitors					
);

-- Dive Sites.
CREATE TABLE DiveSites (
  s_id INT PRIMARY KEY,
  -- The first name of the monitor.
  name VARCHAR(50) NOT NULL,
  -- Max group size open water in the morning.
  max_capacity_morning_open_water INT DEFAULT 0,
  -- Max group size open water in the evening.
  max_capacity_evening_open_water INT DEFAULT 0,
  -- Max group size cave in the morning.
  max_capacity_morning_cave INT DEFAULT 0,
  -- Max group size cave in the evening.
  max_capacity_evening_cave INT DEFAULT 0,
  -- Max group size deeper than 30 in the morning.
  max_capacity_morning_deep INT DEFAULT 0,
  -- Max group size deeper than 30 in the evening.
  max_capacity_evening_deep INT DEFAULT 0
); 

-- Monitor Booking Privileges.
CREATE TABLE BookingPrivileges (
  m_id INT,
  s_id INT,
  PRIMARY KEY (m_id, s_id),
  FOREIGN KEY (m_id) references Monitors,
  FOREIGN KEY (s_id) references DiveSites	
);  

-- Table to hold booking information.
CREATE TYPE type AS ENUM ('Open Water', 'Cave', 'Deeper Than 30');
CREATE TYPE tod AS ENUM ('Morning', 'Afternoon', 'Evening');
CREATE TABLE Bookings (
  b_id INT PRIMARY KEY,
  -- Lead diver id
  d_id INT,
  -- Dive site id
  s_id INT,
  -- Monitor id 
  m_id INT,
  -- Date of the dive
  datetime timestamp NOT NULL,
  -- Type of Dive
  dive_type type NOT NULL,
  -- Time of day 
  time_of_day tod NOT NULL,
  -- Price paid
  price MONEY NOT NULL,
  -- Credit card information
  credit_card INT NOT NULL,
  -- Rating of Monitor
  rating INT CHECK (rating>=0 and rating<=5),
  FOREIGN KEY(d_id) references Divers,
  FOREIGN KEY(s_id) references DiveSites,
  FOREIGN KEY(m_id) references Monitors
);

-- Table that holds bookings of all divers 
CREATE TABLE DiverBookings(
  b_id INT,
  d_id INT,
  -- Rating of dive site
  rating INT CHECK (rating>=0 and rating<=5),
  PRIMARY KEY (b_id, d_id),
  FOREIGN KEY(d_id) references Divers,
  FOREIGN KEY(b_id) references Bookings
);			   
