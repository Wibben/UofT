import java.sql.*;
import java.util.Date;
import java.util.Arrays;
import java.util.List;

public class Assignment2 {

	// A connection to the database
	Connection connection;

	// Can use if you wish: seat letters
	List<String> seatLetters = Arrays.asList("A", "B", "C", "D", "E", "F");

	Assignment2() throws SQLException {
		try {
			Class.forName("org.postgresql.Driver");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

  /**
	* Connects and sets the search path.
	*
	* Establishes a connection to be used for this session, assigning it to
	* the instance variable 'connection'.  In addition, sets the search
	* path to 'air_travel, public'.
	*
	* @param  url		 the url for the database
	* @param  username  the username to connect to the database
	* @param  password  the password to connect to the database
	* @return			  true if connecting is successful, false otherwise
	*/
	public boolean connectDB(String URL, String username, String password) {
		try {
			// Establish connection
			connection = DriverManager.getConnection(URL,username,password);
			System.out.printf("Connected to %s\n",URL);
			
			// Also set search path
			String updateString = "SET SEARCH_PATH TO air_travel, public";
			PreparedStatement ps = connection.prepareStatement(updateString);
			ps.executeUpdate();
			
			return true;
		} catch (SQLException se) {
			System.err.println("SQL Exception: " + se.getMessage());
			return false;
		}
	}

  /**
	* Closes the database connection.
	*
	* @return true if the closing was successful, false otherwise
	*/
	public boolean disconnectDB() {
		try {
			connection.close();
			System.out.println("Connection closed");
			return true;
		} catch(SQLException e) {
			System.err.println("Exception while disconnecting: "+ e.getMessage());
			return false;
		}
	}
	
	/* ======================= Airline-related methods ======================= */

	/**
	 * Attempts to book a flight for a passenger in a particular seat class. 
	 * Does so by inserting a row into the Booking table.
	 *
	 * Read handout for information on how seats are booked.
	 * Returns false if seat can't be booked, or if passenger or flight cannot be found.
	 *
	 * 
	 * @param  passID	  id of the passenger
	 * @param  flightID	id of the flight
	 * @param  seatClass  the class of the seat (economy, business, or first) 
	 * @return				true if the booking was successful, false otherwise. 
	 */
	public boolean bookSeat(int passID, int flightID, String seatClass) {
		try {
			String queryString;
			PreparedStatement ps;
			
			// Check if plane has already departed
			queryString = "select count(*) from departure where flight_id="+flightID;
			ps = connection.prepareStatement(queryString);
			ResultSet flightLeft = ps.executeQuery();
			flightLeft.next();
			if(flightLeft.getInt("count")>0) {
				System.out.println("Cannot book for flight "+flightID+", it has already departed");
				return false;
			}
			
			// Drop views
			queryString = "DROP VIEW IF EXISTS bookedSeats CASCADE";
			ps = connection.prepareStatement(queryString);
			ps.executeUpdate();
			
			// Get capacity data for flight to book
			queryString = 
				"select " +
					"capacity_"+seatClass+
						"-sum(case when seat_class='"+seatClass+"' then 1 else 0 end) " + 							"as leftover " +
				"from flight join plane on flight.plane=tail_number " +
					"left join booking on flight.id=booking.flight_id " +
				"where flight.id="+flightID + " " +
				"group by flight.id,capacity_"+seatClass;
			ps = connection.prepareStatement(queryString);
			ResultSet flightData = ps.executeQuery();
			flightData.next();
			
			// Get current booked seats for the flight for the class
			// Create a view to check if seat number is valid later
			queryString = 
				"create view bookedSeats as " +
				"select row||letter as seat " +
				"from booking " +
				"where flight_id="+flightID + " " +
					"and seat_class='"+seatClass+"'";
			ps = connection.prepareStatement(queryString);
			ps.executeUpdate();
			
			// Get number of next booking id
			queryString = "select max(id) as num_bookings from booking";
			ps = connection.prepareStatement(queryString);
			ResultSet num_bookings = ps.executeQuery();
			num_bookings.next();
			int next_id = num_bookings.getInt("num_bookings") + 1;
			
			// Get capacities for current flight
			queryString = 
				"select capacity_economy,capacity_business,capacity_first " +
				"from flight join plane on flight.plane=tail_number " +
				"where flight.id="+flightID;
			ps = connection.prepareStatement(queryString);
			ResultSet capacityData = ps.executeQuery();
			capacityData.next();
			int capacity_economy = capacityData.getInt("capacity_economy");
			int capacity_business = capacityData.getInt("capacity_business");
			int capacity_first = capacityData.getInt("capacity_first");
			
			// Calculate which row booking begins
			int initial_row = 1; // First class
			if(seatClass=="business") initial_row = capacity_first/6 + 2;
			else if(seatClass=="economy") initial_row = capacity_first/6 + capacity_business/6 + 3;

			// Find an "available" seat, if overbooked, this will be overriden later
			// If cannot be booked, will return early and value discarded
			int row=0,letter=0;
			boolean seatFound = false;
			for(row=initial_row; !seatFound; row++) {
				for(letter=0; letter<6 && !seatFound; letter++) {
					queryString = 
						"select count(*) as count " +
						"from bookedSeats " +
						"where seat='"+row+seatLetters.get(letter)+"'";
					ps = connection.prepareStatement(queryString);
					ResultSet if_exists = ps.executeQuery();
					if_exists.next();
					if(if_exists.getInt("count")==0) seatFound = true;
				}
			}
			row--;
			letter--;

			// Get price of the seat
			queryString = "select "+seatClass+" from price where flight_id="+flightID;
			ps = connection.prepareStatement(queryString);
			ResultSet cost = ps.executeQuery();
			cost.next();
			int price = cost.getInt(seatClass);
			
			// Check if cannot book or need to overbook
			if(seatClass=="economy" && flightData.getInt("leftover")>-10) { // Overbooking
				// Seat is null
				row = -1;
				letter = -1;
			} else if(flightData.getInt("leftover")<=0) { // No more space
				System.out.println("Can't book for "+seatClass+" class on flight "+flightID);
				return false;
			}
			
			// Create a new booking
			if(row==-1) { // Overbooking books null for row and letter
				queryString = 
					"insert into booking values " +
					"("+next_id+","+passID+","+flightID+","+
					"date_trunc('second', timestamp '"+getCurrentTimeStamp()+"'),"+
					price+",'"+seatClass+"',null,null)";
			} else { // Otherwise an actual seat is booked
				queryString = 
					"insert into booking values " +
					"("+next_id+","+passID+","+flightID+","+
					"date_trunc('second', timestamp '"+getCurrentTimeStamp()+"'),"+
					price+",'"+seatClass+"',"+row+",'"+seatLetters.get(letter)+"')";
			}
			ps = connection.prepareStatement(queryString);
			ps.executeUpdate();
			
			System.out.println("Booking done for "+seatClass+" class on flight "+flightID);
			/*
			queryString = 
				"select " +
					"capacity_economy-sum(case when seat_class='economy' then 1 else 0 end) " + 						"as leftover_economy," +
					"capacity_business-sum(case when seat_class='business' then 1 else 0 end) " + 							"as leftover_business," +
					"capacity_first-sum(case when seat_class='first' then 1 else 0 end) " + 						"as leftover_first " +
				"from flight join plane on flight.plane=tail_number " +
					"left join booking on flight.id=booking.flight_id " +
				"where flight.id="+flightID + " " +
				"group by flight.id,capacity_economy,capacity_business,capacity_first";
			ps = connection.prepareStatement(queryString);
			flightData = ps.executeQuery();
			
			System.out.printf(
				"left_econ\tleft_busi\tleft_first\n");
			while(flightData.next()) {
				System.out.printf("%d\t\t%d\t\t%d\n",
					flightData.getInt("leftover_economy"),
					flightData.getInt("leftover_business"),
					flightData.getInt("leftover_first")
				);
			}
			*/
			return true;
		} catch (SQLException se) {
			System.err.println("SQL Exception: " + se.getMessage());
			return false;
		}
	}

	/**
	 * Attempts to upgrade overbooked economy passengers to business class
	 * or first class (in that order until each seat class is filled).
	 * Does so by altering the database records for the bookings such that the
	 * seat and seat_class are updated if an upgrade can be processed.
	 *
	 * Upgrades should happen in order of earliest booking timestamp first.
	 *
	 * If economy passengers left over without a seat (i.e. more than 10 overbooked passengers or not enough higher class seats), 
	 * remove their bookings from the database.
	 * 
	 * @param  flightID  The flight to upgrade passengers in.
	 * @return			  the number of passengers upgraded, or -1 if an error occured.
	 */
	public int upgrade(int flightID) {
		try {
			String queryString;
			PreparedStatement ps;
			int num_upgraded = 0;
		
			// Drop views
			/*queryString = "DROP VIEW IF EXISTS bookedSeats CASCADE";
			ps = connection.prepareStatement(queryString);
			ps.executeUpdate();
			*/
			// Get capacities for current flight
			queryString = 
				"select capacity_economy,capacity_business,capacity_first " +
				"from flight join plane on flight.plane=tail_number " +
				"where flight.id="+flightID;
			ps = connection.prepareStatement(queryString);
			ResultSet capacityData = ps.executeQuery();
			capacityData.next();
			int capacity_economy = capacityData.getInt("capacity_economy");
			int capacity_business = capacityData.getInt("capacity_business");
			int capacity_first = capacityData.getInt("capacity_first");
			
			// Get a list of all overbooked passengers on flight
			queryString = 
				"select * " +
				"from booking " +
				"where flight_id="+flightID + " and row is null " +
				"order by datetime";
			ps = connection.prepareStatement(queryString);
			ResultSet overbooked = ps.executeQuery();
			
			// Get remaining seats that can be bookes on flight
			queryString = 
				"select " +
					"capacity_business-sum(case when seat_class='business' then 1 else 0 end) " + 							"as leftover_business," +
					"capacity_first-sum(case when seat_class='first' then 1 else 0 end) " + 						"as leftover_first " +
				"from flight join plane on flight.plane=tail_number " +
					"left join booking on flight.id=booking.flight_id " +
				"where flight.id="+flightID + " " +
				"group by flight.id,capacity_business,capacity_first";
			ps = connection.prepareStatement(queryString);
			ResultSet seatData = ps.executeQuery();
			seatData.next();
			int leftover_business = seatData.getInt("leftover_business");
			int leftover_first = seatData.getInt("leftover_first");
			boolean canBook = true; // Can still upgrade
			
			// Go through each overbooking and try to upgrade the class
			while(overbooked.next() && num_upgraded<10 && canBook) {
				int booking_id = overbooked.getInt("id");
				// Drop views
				queryString = "DROP VIEW IF EXISTS bookedSeats CASCADE";
				ps = connection.prepareStatement(queryString);
				ps.executeUpdate();
			
				// Check if can upgrade to business
				if(leftover_business>0) {
					// Get current booked seats for the flight for the class
					// Create a view to check if seat number is valid later
					queryString = 
						"create view bookedSeats as " +
						"select row||letter as seat " +
						"from booking " +
						"where flight_id="+flightID + " " +
							"and seat_class='business'";
					ps = connection.prepareStatement(queryString);
					ps.executeUpdate();
				
					// Find an available seat
					int row=0,letter=0;
					boolean seatFound = false;
					for(row=capacity_first/6 + 2; !seatFound; row++) {
						for(letter=0; letter<6 && !seatFound; letter++) {
							queryString = 
								"select count(*) as count " +
								"from bookedSeats " +
								"where seat='"+row+seatLetters.get(letter)+"'";
							ps = connection.prepareStatement(queryString);
							ResultSet if_exists = ps.executeQuery();
							if_exists.next();
							if(if_exists.getInt("count")==0) seatFound = true;
						}
					}
					row--;
					letter--;
					
					// Update booking
					queryString = 
						"update booking " +
						"set seat_class='business'," +
						"row="+row + "," +
						"letter='"+seatLetters.get(letter)+"' "+
						"where id="+booking_id;
					ps = connection.prepareStatement(queryString);
					ps.executeUpdate();
					
					// Booking would leave one less leftover
					num_upgraded++;
					leftover_business--;
				} else if(leftover_first>0) {
					// Get current booked seats for the flight for the class
					// Create a view to check if seat number is valid later
					queryString = 
						"create view bookedSeats as " +
						"select row||letter as seat " +
						"from booking " +
						"where flight_id="+flightID + " " +
							"and seat_class='business'";
					ps = connection.prepareStatement(queryString);
					ps.executeUpdate();
				
					// Find an available seat
					int row=0,letter=0;
					boolean seatFound = false;
					for(row=capacity_first/6 + capacity_business/6 + 3; !seatFound; row++) {
						for(letter=0; letter<6 && !seatFound; letter++) {
							queryString = 
								"select count(*) as count " +
								"from bookedSeats " +
								"where seat='"+row+seatLetters.get(letter)+"'";
							ps = connection.prepareStatement(queryString);
							ResultSet if_exists = ps.executeQuery();
							if_exists.next();
							if(if_exists.getInt("count")==0) seatFound = true;
						}
					}
					row--;
					letter--;
					
					// Update booking
					queryString = 
						"update booking " +
						"set seat_class='first'," +
						"row="+row + "," +
						"letter='"+seatLetters.get(letter)+"' "+
						"where id="+booking_id;
					ps = connection.prepareStatement(queryString);
					ps.executeUpdate();
					
					// Booking would leave one less leftover
					num_upgraded++;
					leftover_first--;
				} else canBook = false;
			}
			
			// Remove the leftover overbooked passengers from the booking
			queryString = 
				"delete from booking " +
				"where flight_id="+flightID + " and row is null ";
			ps = connection.prepareStatement(queryString);
			ps.executeUpdate();
			
			System.out.println("Upgraded "+num_upgraded+" passengers on flight "+flightID);
			return num_upgraded;
		} catch (SQLException se) {
			System.err.println("SQL Exception: " + se.getMessage());
			return -1;
		}
	}


	/* ----------------------- Helper functions below  ------------------------- */

	 // A helpful function for adding a timestamp to new bookings.
	 // Example of setting a timestamp in a PreparedStatement:
	 // ps.setTimestamp(1, getCurrentTimeStamp());

	 /**
	 * Returns a SQL Timestamp object of the current time.
	 * 
	 * @return			  Timestamp of current time.
	 */
	private java.sql.Timestamp getCurrentTimeStamp() {
		java.util.Date now = new java.util.Date();
		return new java.sql.Timestamp(now.getTime());
	}

	// Add more helper functions below if desired.
	// oops

  
  /* ----------------------- Main method below  ------------------------- */

	public static void main(String[] args) throws Exception {
		// You can put testing code in here. It will not affect our autotester.
		System.out.println("Running the code!");
		
		Assignment2 a2 = new Assignment2();
		
		if(a2.connectDB("jdbc:postgresql://localhost:5432/csc343h-libingra","libingra","")) {
			a2.bookSeat(1,5,"economy");
			for(int i=0; i<11; i++) {
				a2.bookSeat(1,10,"economy");
			}
			a2.upgrade(10);
			a2.disconnectDB();
		}
	}

}
