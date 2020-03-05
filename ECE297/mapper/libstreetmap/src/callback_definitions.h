#pragma once
#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "directions.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"

// Callback function when a use clicks on the canvas
void act_on_mouse_click (ezgl::application* app, 
                         GdkEventButton* event,
                         double x, double y);

// Callback function when a user presses a key
void act_on_key_release (ezgl::application *app, 
                       GdkEventKey *event, 
                       char *key_name);

//Directions will be found between the following intersections
extern int from_intersection, to_intersection;
//Flags to control when variables are changed
extern bool do_draw_directions, do_get_from, do_get_to;