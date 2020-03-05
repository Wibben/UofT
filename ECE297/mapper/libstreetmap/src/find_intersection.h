#pragma once
#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "popup_message.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"

class EntryData
{
public:
    GtkWidget *a;
    GtkWidget *b;
    ezgl::application * app;
    EntryData(GtkWidget* one, GtkWidget* two,ezgl::application * three) : a(one), b(two) ,app(three){};
};

gboolean press_find(GtkWidget *widget, gpointer data);
void on_dialog_response(GtkDialog *dialog, gint response_id, EntryData* entry);
int find_intersection (std::string street_1, std::string street_2);
void intersection_information_pop_up (const char* info, ezgl::application * app);
void on_text_response(GtkDialog *dialog, gint response_id, gpointer user_data);

// Dialog for displaying intersections
void display_intersections(ezgl::application *application);
gboolean press_intersection(GtkWidget *widget, gpointer data);