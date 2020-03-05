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

void press_night_mode(GtkWidget *, ezgl::application *application);

extern int colour_theme;