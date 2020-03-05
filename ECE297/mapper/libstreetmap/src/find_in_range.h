#pragma once

#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "popup_message.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include <sstream>
#include <string>

// POI class to store all relevant information on POIs: ID, name, distance
// as well as to provide custom compare functions to sort by different things
class POIData
{  
public:
    POIIndex ID;
    std::string name,type;
    double dist;
    
    // Constructor
    POIData(POIIndex id, std::string n, std::string t, double d) : 
        ID(id),name(n),type(t),dist(d) {}
};

// Comparison Functions, will sort from least to greatest
bool cmp_ID(const POIData &a, const POIData &b); // Compare by ID
bool cmp_name(const POIData &a, const POIData &b); // Compare by name
bool cmp_type(const POIData &a, const POIData &b); // Compare by type
bool cmp_dist(const POIData &a, const POIData &b); // Compare by distance

// Quick class for passing in entry data and application
class POIListData
{
public:
    GtkEntry* entry;
    ezgl::application* application;
    
    POIListData(GtkEntry* e, ezgl::application* app) : entry(e), application(app) {}
};

// Opens window to prompt user for range
void press_pois_in_range(GtkWidget *, ezgl::application *application);
void on_prompt_response(GtkDialog *dialog, gint response_id, POIListData *entry);

// Generates and displays a list of POIs within a range
void generate_poi_table(ezgl::application *application);
// Responses for sorting by the different buttons
void press_ID(GtkWidget *, gint, gpointer);
void press_name(GtkWidget *, gint, gpointer);
void press_type(GtkWidget *, gint, gpointer);
void press_dist(GtkWidget *, gint, gpointer);
// Response for pressing the id of the POI to display
void press_poi_ID(GtkWidget *ID_button, gint, gpointer);

void update_and_show_table();

// Returns all points of interests within range of a given position
// range is in meters
std::vector<POIData> find_poi_in_range(LatLon position);
extern int find_poi_range;