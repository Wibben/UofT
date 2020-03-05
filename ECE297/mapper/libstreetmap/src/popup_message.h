#pragma once

#include "m1.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"

// Displays an error message when the user does something not allowed
void popup_message(GtkWindow* parent, const char* title, const char* message);
void on_popup_response(GtkDialog *dialog, gint response_id, gpointer user_data);