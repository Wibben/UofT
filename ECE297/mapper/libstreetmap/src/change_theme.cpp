#include "change_theme.h"

void press_night_mode(GtkWidget *, ezgl::application *application){
    if(colour_theme == 0){
        colour_theme = 1;
    }
    else{
        colour_theme = 0;
    }
    application->refresh_drawing();
}