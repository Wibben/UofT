#pragma once

#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "popup_message.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"

// Array that holds all of the data paths
// Declared static as this is basically a reference
static const class MapPath
{
public:
    const char *name,*file_path;
    
    // Constructor
    MapPath(const char* n, const char* f) : name(n), file_path(f) {}
} 
map_list[] = {
    MapPath("Beijing, China"            , "beijing_china.streets.bin"),
    MapPath("Cairo, Egypt"              , "cairo_egypt.streets.bin"),
    MapPath("Cape Town, South Africa"   , "cape-town_south-africa.streets.bin"),
    MapPath("Golden Horseshoe, Canada"  , "golden-horseshoe_canada.streets.bin"),
    MapPath("Hamilton, Canada"          , "hamilton_canada.streets.bin"),
    MapPath("Hong Kong, China"          , "hong-kong_china.streets.bin"),
    MapPath("Iceland"                   , "iceland.streets.bin"),
    MapPath("Interlaken, Switzerland"   , "interlaken_switzerland.streets.bin"),
    MapPath("London, England"           , "london_england.streets.bin"),
    MapPath("Moscow, Russia"            , "moscow_russia.streets.bin"),
    MapPath("New-Delhi, India"          , "new-delhi_india.streets.bin"),
    MapPath("New York, USA"             , "new-york_usa.streets.bin"),
    MapPath("Rio-de-Janeiro, Brazil"    , "rio-de-janeiro_brazil.streets.bin"),
    MapPath("St. Helena"                , "saint-helena.streets.bin"),
    MapPath("Singapore"                 , "singapore.streets.bin"),
    MapPath("Sydney, Australia"         , "sydney_australia.streets.bin"),
    MapPath("Tehran, Iran"              , "tehran_iran.streets.bin"),
    MapPath("Tokyo, Japan"              , "tokyo_japan.streets.bin"),
    MapPath("Toronto, Canada"           , "toronto_canada.streets.bin")
};

// Simple class to pass in GtkComboBox and ezgl::application into
// on_load_response so redraws are possible
class LoadMapData
{
public:
    GtkComboBox * combo_box;
    ezgl::application * application;
    
    LoadMapData(GtkComboBox* c_box, ezgl::application* app) :
        combo_box(c_box), application(app) {}
};

// Opens a dialog to select a new map
void press_load_map(GtkWidget *widget, ezgl::application *application);
void on_load_response(GtkDialog *dialog, gint response_id, LoadMapData * entry);

