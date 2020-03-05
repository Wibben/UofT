#pragma once

#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "draw_functions.h"
#include "popup_message.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include <sstream>
#include <string>
#include <queue>
#include <boost/format.hpp>

// Quick class for passing in entry data and application
class PathIntersectionData
{
public:
    GtkEntry* entry1;
    GtkEntry* entry2;
    ezgl::application* application;
    
    PathIntersectionData(GtkEntry* e1, GtkEntry* e2, ezgl::application* app) : entry1(e1), entry2(e2), application(app) {}
};

// Opens window to prompt user for the starting intersection
void press_directions_from(GtkWidget *, ezgl::application *application);
// Opens window to prompt user for the destination intersection
void press_directions_to(ezgl::application *application);
// Opens window to prompt user to clear the path directions
void press_directions_clear(ezgl::application *application);

//Actions to be done based on different user responses to the starting prompt
void on_prompt_response_directions_from(GtkDialog *dialog, gint response_id, PathIntersectionData *entry);
//Actions to be done based on different user responses to the destination prompt
void on_prompt_response_directions_to(GtkDialog *dialog, gint response_id, PathIntersectionData *entry);
//Actions to be done based on different user responses to the clear prompt
void on_prompt_response_directions_clear(GtkDialog *dialog, gint response_id, PathIntersectionData *entry);

//Returns the other intersection at the street segment
unsigned get_other_intersection(unsigned street_segment_id, unsigned from_or_to);
//Check if the street segment can be legally traveled (in case of one way)
bool is_street_segment_valid(int street_segment_id, int from_or_to);

// Update the entry_from EntryBox
void update_from_entry(std::string text);
// Update the entry_to EntryBox
void update_to_entry(std::string text);
// Splits the user input into partial street names
int split (std::string name);
// Find the intersection corresponding to the partial street names
int find_intersection_directions (std::string street_1, std::string street_2);

extern std::vector<std::vector<unsigned>> street_segments_from_intersection;
