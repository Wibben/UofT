#include "callback_definitions.h" 
#include <cstring>

// Defines what happens when a user clicks on the map
// Left click: place marker in location and highlight nearest intersection
// Middle click: panning, handled by default
// Right click: TBD
void act_on_mouse_click (ezgl::application* app, 
                         GdkEventButton* event,
                         double x, double y){
    // Selection of intersections and points is done with left mouse click
    if(event->button == 1) {
        LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
        int id = find_closest_intersection(pos);
        app->update_message("Closest Intersection: "+getIntersectionName(id));

        // Set the marker to the new location and redraw
        reset_highlighted_intersection_ID();
        set_highlighted_intersection_ID(id); // Nearest intersection
        set_marker_points(
                x,y,app->get_canvas(app->get_main_canvas_id()),true); // Mouse position
        
        //The intersections for directions are changed only when the appropriate
        //flags are set after the directions button is clicked
        if(do_get_from){
            from_intersection = id;
            update_from_entry(getIntersectionName(id));
        }
        if(do_get_to){
            to_intersection = id;
            update_to_entry(getIntersectionName(id));
        }
        
        // Update map
        app->refresh_drawing();
    }
}

// Defines what happens when a user presses (and releases) a key on the keyboard
// Set the map to update upon key release so holding down the key 
// doesn't end up crashing the program or cause multiple events
// Left, right, up, down: panning (replaces panning buttons from default UI)
void act_on_key_release (ezgl::application *app, 
                         GdkEventKey *, 
                         char *key_name)
{
    // Get the current canvas
    ezgl::canvas * canvas = app->get_canvas(app->get_main_canvas_id());
        
    // Set up the keys for translation
    if(strcmp(key_name,"Left")==0) ezgl::translate_left(canvas, 5.0);
    else if(strcmp(key_name,"Right")==0) ezgl::translate_right(canvas, 5.0);
    else if(strcmp(key_name,"Up")==0) ezgl::translate_up(canvas, 5.0);
    else if(strcmp(key_name,"Down")==0) ezgl::translate_down(canvas, 5.0);
    
    // Update map
    app->refresh_drawing();
}
