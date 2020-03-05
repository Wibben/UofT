#include "directions.h"
#include <boost/algorithm/string.hpp>

int from_intersection, to_intersection;
std::vector<unsigned> shortest_path;
bool do_draw_directions, do_get_from, do_get_to;
GtkWidget *entry_from,*entry_to;
std::string name_from = "",name_to = "";
std::vector<int> information;

// Opens window to prompt user for the starting intersection
void press_directions_from(GtkWidget *, ezgl::application *application)
{
    // Disable the directions button so the user can't press it twice
    GObject *directions_button = application->get_object("DirectionsButton");
    gtk_widget_set_sensitive((GtkWidget*)directions_button,false);
    
    GObject *window; // Parent window for the dialog
    GtkWidget *content_area;
    GtkWidget *prompt;
    GtkWidget *prompt_dialog; // Prompt dialog box
    
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Update canvas information
    ezgl::canvas *canvas = application->get_canvas(application->get_main_canvas_id());
    canvas_width = canvas->width();
    canvas_height = canvas->height();
    
    // Create the prompt
    prompt_dialog = gtk_dialog_new_with_buttons(
        "Directions",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL, // Do not allow interaction with main window
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );
    
    gtk_window_set_modal((GtkWindow*)prompt_dialog,false);
    
    // Prompt the user to click on the starting intersection
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(prompt_dialog));
    prompt = gtk_label_new(
        "Click on the starting intersection or enter the intersection name\n"
        "then press OK when ready. Separate street names with '&', ',', or 'and'\n\n"
        "Starting intersection name:");
    entry_from = gtk_entry_new();
    entry_to = gtk_entry_new();
    gtk_entry_set_text((GtkEntry*)entry_from,name_from.c_str()); // Default range
    gtk_entry_set_text((GtkEntry*)entry_to,name_to.c_str()); // Default range
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), prompt);
    gtk_container_add(GTK_CONTAINER(content_area), entry_from);
    gtk_widget_show_all(prompt_dialog);
    
    //Move the prompt window to the top left corner of the map window
    int windowx, windowy;
    gtk_window_get_position((GtkWindow*) window,&windowx,&windowy);
    gtk_window_move ((GtkWindow*) prompt_dialog,windowx+5,windowy+65);
    
    PathIntersectionData* entry = new PathIntersectionData(NULL, NULL, application);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        GTK_DIALOG(prompt_dialog),
        "response",
        G_CALLBACK(on_prompt_response_directions_from),
            entry
    );
}

//Actions to be done based on different user responses to the starting prompt
void on_prompt_response_directions_from(GtkDialog *dialog, gint response_id, PathIntersectionData *entry) 
{
    GObject *directions_button = entry->application->get_object("DirectionsButton");
    std::string text_from = gtk_entry_get_text((GtkEntry*) entry_from);
    bool can_close = false;
    information.clear();
    
    //Responses for user interactions with the prompt
    switch (response_id) {
        case GTK_RESPONSE_ACCEPT:
            if(from_intersection==-1 && text_from=="") {
                popup_message((GtkWindow*)dialog, "ERROR","Please select an intersection");
            } else {
                if(from_intersection!=-1 && getIntersectionName(from_intersection)==text_from)
                {
                    do_get_from = false;
                    //When OK is clicked the next prompt window for the TO intersection opens
                    press_directions_to(entry->application);
                    can_close = true;
                }
                else{
                    int state = split(text_from);
                    if(state == 4 || state == 3){
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "Please enter a valid intersection");
                    }
                    else if (state == 1){
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "There is no intersection with that name");
                    }
                    else if (state == 0){
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "Please enter a unique intersection name");
                    }    
                    else if (state == 2){
                        from_intersection = information[0];
                        name_from = getIntersectionName(from_intersection);
                        do_get_from = false;
                        //When OK is clicked the next prompt window for the TO intersection opens
                        press_directions_to(entry->application);
                        can_close = true;
                    }
                }
            }
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            // Re-enable the directions button once user exits directions function
            gtk_widget_set_sensitive((GtkWidget*)directions_button,true);
            entry_from = entry_to = NULL;
            can_close = true;
            break;
        case GTK_RESPONSE_REJECT:
            // Re-enable the directions button once user exits directions function
            gtk_widget_set_sensitive((GtkWidget*)directions_button,true);
            entry_from = entry_to = NULL;
            can_close = true;
            break;
        default:
            break;
    }
    
    if(can_close) gtk_widget_destroy(GTK_WIDGET(dialog));
}

// Opens window to prompt user for the destination intersection
void press_directions_to(ezgl::application *application)
{
    
    GObject *window; // Parent window for the dialog
    GtkWidget *content_area;
    GtkWidget *prompt;
    GtkWidget *prompt_dialog; // Prompt dialog box
    
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the prompt
    prompt_dialog = gtk_dialog_new_with_buttons(
        "Directions",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL, // Do not allow interaction with main window
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );
    
    gtk_window_set_modal((GtkWindow*)prompt_dialog,false);
    
    // Prompt the user to click on the destination intersection
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(prompt_dialog));
    prompt = gtk_label_new(
        "Click on the destination intersection or enter the intersection name\n"
        "then press OK when ready. Separate street names with '&', ',', or 'and'\n\n"
        "Destination intersection name:");
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), prompt);
    gtk_container_add(GTK_CONTAINER(content_area), entry_to);
    gtk_widget_show_all(prompt_dialog);
    
    //Move the prompt window to the top left corner of the map window
    int windowx, windowy;
    gtk_window_get_position((GtkWindow*) window,&windowx,&windowy);
    gtk_window_move ((GtkWindow*) prompt_dialog,windowx+5,windowy+65);
    
    PathIntersectionData* entry = new PathIntersectionData(NULL, NULL, application);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        GTK_DIALOG(prompt_dialog),
        "response",
        G_CALLBACK(on_prompt_response_directions_to),
        entry
    );
}

//Actions to be done based on different user responses to the destination prompt
void on_prompt_response_directions_to(GtkDialog *dialog, gint response_id, PathIntersectionData *entry) 
{
    GObject *directions_button = entry->application->get_object("DirectionsButton");
    std::string text_to = gtk_entry_get_text((GtkEntry*) entry_to);
    bool can_close = false;
    information.clear();
    
    switch (response_id) {
        case GTK_RESPONSE_ACCEPT:
            if(from_intersection==to_intersection) {
                popup_message((GtkWindow*)dialog, "ERROR",
                    "Please select a different intersection\nthan your starting one");
            } else if(to_intersection==-1 && text_to=="") {
                popup_message((GtkWindow*)dialog, "ERROR","Please select an intersection");
            } else {
                if(to_intersection!=-1 && getIntersectionName(to_intersection)==text_to) {
                    //The TO intersection can no longer be changed.
                    do_get_to = false;
                    //The path can now be drawn
                    do_draw_directions = true;

                    shortest_path = find_path_between_intersections(from_intersection, to_intersection, 0, 0);

                    if(shortest_path.size()==0) {
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "You cannot move between these intersections.");
                        gtk_widget_set_sensitive((GtkWidget*)directions_button,true);//Pressing anything will clear the directions path
                        do_draw_directions = false;
                        do_get_from = do_get_to = true; // Allow for the updating of intersections
                        entry_from = entry_to = NULL;
                        // Set FROM intersection to intersection closest to current mouse position
                        LatLon pos = LatLon(lat_from_y(mouse_y), lon_from_x(mouse_x));
                        from_intersection = find_closest_intersection(pos);
                        name_from = name_to = getIntersectionName(from_intersection);
                    } else {
                        //Draw the path
                        entry->application->refresh_drawing();
                        //Open the new prompt to allow the user to clear the path
                        press_directions_clear(entry->application);
                    }
                    can_close = true;
                } else {
                    int state = split(text_to);
                    if(state == 4 || state == 3){
                        popup_message((GtkWindow*)dialog,
                            "ERROR","Please enter a valid intersection");
                    }
                    else if (state == 1){
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "There is no intersection with that name");
                    }
                    else if (state == 0){
                        popup_message((GtkWindow*)dialog, "ERROR",
                            "Please enter a unique intersection name");
                    }    
                    else if (state == 2){
                        to_intersection = information[0];
                        name_to = getIntersectionName(to_intersection);
                        //The TO intersection can no longer be changed.
                        do_get_to = false;
                        //The path can now be drawn
                        do_draw_directions = true;

                        shortest_path = find_path_between_intersections(from_intersection, to_intersection, 0, 0);

                        if(shortest_path.size()==0) {
                            popup_message((GtkWindow*)dialog, "ERROR",
                                "You cannot move between these intersections.");
                            gtk_widget_set_sensitive((GtkWidget*)directions_button,true);//Pressing anything will clear the directions path
                            do_draw_directions = false;
                            do_get_from = do_get_to = true; // Allow for the updating of intersections
                            entry_from = entry_to = NULL;
                            // Set FROM intersection to intersection closest to current mouse position
                            LatLon pos = LatLon(lat_from_y(mouse_y), lon_from_x(mouse_x));
                            from_intersection = find_closest_intersection(pos);
                            name_from = name_to = getIntersectionName(from_intersection);
                        } else {
                            //Draw the path
                            entry->application->refresh_drawing();
                            //Open the new prompt to allow the user to clear the path
                            press_directions_clear(entry->application);
                        }
                        can_close = true;
                    }
                }
            }
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            // Re-enable the directions button once user exits directions function
            gtk_widget_set_sensitive((GtkWidget*)directions_button,true);
            entry_from = entry_to = NULL;
            do_get_from = true;
            can_close = true;
            break;
        case GTK_RESPONSE_REJECT:
            // Re-enable the directions button once user exits directions function
            gtk_widget_set_sensitive((GtkWidget*)directions_button,true);
            entry_from = entry_to = NULL;
            do_get_from = true;
            can_close = true;
            break;
        default:
            break;
    }
    
    if(can_close) gtk_widget_destroy(GTK_WIDGET(dialog));
}

// Opens window to prompt user to clear the path directions
void press_directions_clear(ezgl::application *application)
{
    GtkWindow *ListWindow; // Parent window for the dialog
    GtkWidget *scrolling;
    GtkWidget *directions_list;
    GtkWidget *dist_label;
    
    // Create the window
    ListWindow = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(ListWindow,"Directions");
    gtk_window_set_default_size(ListWindow,350,600);
    gtk_widget_set_hexpand((GtkWidget*)ListWindow,true);
    gtk_window_set_modal(ListWindow,false); // Allow interaction with main app
    gtk_window_present(ListWindow);
    
    //Move the prompt window to the top left corner of the map window
    int windowx, windowy;
    GObject *window = application->get_object(application->get_main_window_id().c_str());
    gtk_window_get_position((GtkWindow*) window,&windowx,&windowy);
    gtk_window_move (ListWindow,windowx-360,windowy);
    
    // Set up instructions area
    scrolling = gtk_scrolled_window_new(NULL,NULL);
    directions_list = gtk_grid_new();
    
    InfoStreetSegment ss_info;
    bool was_straight = false;
    int instruction_count = 0;
    double distance = 0;
    std::string label,dist;
    
    // Show where the user is starting from
    // Show end point icon
    GtkWidget *start = gtk_image_new_from_file("./libstreetmap/resources/endpoint.png");
    gtk_grid_attach((GtkGrid*)directions_list,start,0,2*instruction_count,1,1);
    
    // Show initial intersection
    label = "Depart from " + getIntersectionName(from_intersection);
    GtkWidget *depart = gtk_label_new(label.c_str());
    gtk_label_set_line_wrap((GtkLabel*)depart,true);
    gtk_widget_set_size_request(depart,275,-1);
    gtk_label_set_xalign((GtkLabel*)depart,0);
    gtk_grid_attach((GtkGrid*)directions_list, depart,1,2*(instruction_count++),1,1);
    
    // Go through the directions to get to final destination
    for(unsigned i = 0; i < shortest_path.size()-1; i++){
        ss_info = getInfoStreetSegment(shortest_path[i+1]);
        TurnType tt = find_turn_type(shortest_path[i], shortest_path[i+1]);
        
        // Add onto distance until turn/straight
        distance += find_street_segment_length(shortest_path[i]);
        // Format distance based on how long it is
        if(distance<1000) dist = (boost::format("Travel %.0f m") %distance).str();
        else dist = str(boost::format("Travel %.1f km") %(round(distance/100)/10.0));
        dist_label = gtk_label_new(dist.c_str());
        
        if(tt == TurnType::LEFT){
            label = "Turn left onto " + getStreetName(ss_info.streetID);
            was_straight = false;
            
            // Show distance until next turn
            gtk_grid_attach((GtkGrid*)directions_list, dist_label,1,2*instruction_count-1,1,1);
            gtk_label_set_xalign((GtkLabel*)dist_label,0);
            // Reset distance for next turn
            distance = 0;
            
            // Show left turn icon
            GtkWidget *image = gtk_image_new_from_file("./libstreetmap/resources/left_turn.png");
            gtk_grid_attach((GtkGrid*)directions_list, image,0,2*instruction_count,1,1);
        } else if(tt == TurnType::RIGHT) {
            label = "Turn right onto " + getStreetName(ss_info.streetID);
            was_straight = false;
            
            // Update label to show distance until next turn
            gtk_grid_attach((GtkGrid*)directions_list, dist_label,1,2*instruction_count-1,1,1);
            gtk_label_set_xalign((GtkLabel*)dist_label,0);
            // Reset distance for next turn
            distance = 0;
            
            // Show right turn icon
            GtkWidget *image = gtk_image_new_from_file("./libstreetmap/resources/right_turn.png");
            gtk_grid_attach((GtkGrid*)directions_list, image,0,2*instruction_count,1,1);
        } else if(tt == TurnType::STRAIGHT) {
            if(!was_straight) was_straight = true;
        } else if(tt == TurnType::NONE) {
            was_straight = false;
        }
        
        GtkWidget *instruction = gtk_label_new(label.c_str());
        gtk_label_set_line_wrap((GtkLabel*)instruction,true);
        gtk_widget_set_size_request(instruction,275,-1);
        gtk_label_set_xalign((GtkLabel*)instruction,0);
        
        // Connect to dialog window
        if(!was_straight)
            gtk_grid_attach((GtkGrid*)directions_list, instruction,1,2*(instruction_count++),1,1);
    }
    // Get distance until final destination
    distance += find_street_segment_length(shortest_path.back());
    // Format distance based on how long it is
    if(distance<1000) dist = (boost::format("Travel %.0f m") %distance).str();
    else dist = str(boost::format("Travel %.1f km") %(round(distance/100)/10.0));
    dist_label = gtk_label_new(dist.c_str());
    
    // Update label to show distance until destination
    gtk_grid_attach((GtkGrid*)directions_list, dist_label,1,2*instruction_count-1,1,1);
    gtk_label_set_xalign((GtkLabel*)dist_label,0);
    // Show end point icon
    GtkWidget *end = gtk_image_new_from_file("./libstreetmap/resources/endpoint.png");
    gtk_grid_attach((GtkGrid*)directions_list,end,0,2*instruction_count,1,1);
    
    // Show final instruction
    label = "Arrive at " + getIntersectionName(to_intersection);
    GtkWidget *arrival = gtk_label_new(label.c_str());
    gtk_label_set_line_wrap((GtkLabel*)arrival,true);
    gtk_widget_set_size_request(arrival,275,-1);
    gtk_label_set_xalign((GtkLabel*)arrival,0);
    gtk_grid_attach((GtkGrid*)directions_list, arrival,1,2*(instruction_count++),1,1);
    
    // Put a little spacing between rows and columns so it isn't too cramped
    gtk_grid_set_row_spacing((GtkGrid*)directions_list,5);
    gtk_grid_set_column_spacing((GtkGrid*)directions_list,5);
    
    // Set up the window for display
    gtk_container_add(GTK_CONTAINER(scrolling),directions_list);
    gtk_container_add(GTK_CONTAINER(ListWindow),scrolling);
    gtk_widget_show_all((GtkWidget*)ListWindow);
    
    PathIntersectionData* entry = new PathIntersectionData(NULL, NULL, application);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        (GtkWidget*)ListWindow,
        "delete-event",
        G_CALLBACK(on_prompt_response_directions_clear),
        entry
    );
}

//Actions to be done based on different user responses to the clear prompt
void on_prompt_response_directions_clear(GtkDialog *dialog, gint, PathIntersectionData *entry) {
    
    //Pressing anything will clear the directions path
    do_draw_directions = false;
    do_get_from = do_get_to = true; // Allow for the updating of intersections
    entry_from = entry_to = NULL;
    // Set FROM intersection to intersection closest to current mouse position
    LatLon pos = LatLon(lat_from_y(mouse_y), lon_from_x(mouse_x));
    from_intersection = find_closest_intersection(pos);
    name_from = name_to = getIntersectionName(from_intersection);
    
    entry->application->refresh_drawing();
    
    // Re-enable the directions button once user exits directions function
    GObject *directions_button = entry->application->get_object("DirectionsButton");
    gtk_widget_set_sensitive((GtkWidget*)directions_button,true);
    
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

//Returns the other intersection at the street segment
unsigned get_other_intersection(unsigned street_segment_id, unsigned from_or_to){
    InfoStreetSegment ss_info = getInfoStreetSegment(street_segment_id);
    
    if (static_cast<unsigned>(ss_info.from) == from_or_to) return ss_info.to;
    else return ss_info.from;
}

//Check if the street segment can be legally traveled (in case of one way)
bool is_street_segment_valid(int street_segment_id, int from_or_to){
    InfoStreetSegment ss_info = getInfoStreetSegment(street_segment_id);
    
    // A street segment is valid if
    // a) it is a two way street, or
    // b) travel is starting from the FROM endpoint
    return (!ss_info.oneWay) || (ss_info.from==from_or_to);
}

// Update the entry_from EntryBpx
void update_from_entry(std::string text){
    name_from = text;
    if(entry_from != NULL) gtk_entry_set_text((GtkEntry*)entry_from,name_from.c_str());
}

// Update the entry_to EntryBpx
void update_to_entry(std::string text){
    name_to = text;
    if(entry_to != NULL) gtk_entry_set_text((GtkEntry*)entry_to,name_to.c_str());
}

// Split the user input into partial street names
int split (std::string name){
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::string street_1,street_2;
    
    // List of valid separators
    std::string separator[3] = {"&"," and ",","};
    
    int found = -1;
    // Checks whether the string contains any separator words
    for(int i=0; found==-1 && i<3; i++) {
        found = name.find(separator[i]);
        if(found != -1) { // Separator found, split name
            street_1 = name.substr(0,found);
            street_2 = name.substr(found+separator[i].length());
        }
    }
    if(found == -1) return 4; //invalid intersection
    
    // Remove initial and trailing spaces
    boost::trim(street_1);
    boost::trim(street_2);
    
    // Further parsing to actually find intersections
    return find_intersection_directions(street_1,street_2);
}

// Find the intersection corresponding to the partial street names
int find_intersection_directions (std::string street_1, std::string street_2){
    //find street ids from partial street name
    std::vector<unsigned> street_id_1 = find_street_ids_from_partial_street_name(street_1);
    std::vector<unsigned> street_id_2 = find_street_ids_from_partial_street_name(street_2);
    
    if(street_id_1.size() == 0 || street_id_2.size() == 0) return 3; // not valid
    
    reset_highlighted_intersection_ID();
    std::transform(street_1.begin(), street_1.end(), street_1.begin(), ::tolower);
    std::transform(street_2.begin(), street_2.end(), street_2.begin(), ::tolower);
    
    std::vector<std::string> common_names;
    
    for(unsigned i=0; i<street_id_1.size(); i++) {
        for(unsigned j=0; j<street_id_2.size(); j++) {
            std::vector<unsigned> common_intersections = 
                find_intersection_ids_from_street_ids(street_id_1[i], street_id_2[j]);
            for(unsigned int k=0; k < common_intersections.size(); k++){
                information.push_back(common_intersections[k]);
                // Split the name into parts, arrange them alphabetically, and
                // store in common_names, the common_names will then later be used
                // to check whether the intersection is unique
                // std::set will ensure that the names are in order when inserted
                std::set<std::string> name_parts;
                std::string inter_name = getIntersectionName(common_intersections[k]);
                std::transform(inter_name.begin(), inter_name.end(), inter_name.begin(), ::tolower);
                // Keep splitting the name as long as there is a '&'
                int found = inter_name.find("&");
                while(found!=-1) {
                    std::string a = inter_name.substr(0,found);
                    inter_name = inter_name.substr(found+1);

                    // Trim leading and trailing spaces
                    boost::trim(a); boost::trim(inter_name);

                    name_parts.insert(a);
                    found = inter_name.find("&");
                }
                name_parts.insert(inter_name); // Insert the leftover name
                
                // Assemble the sorted name
                std::string sorted_name = "";
                std::set<std::string>::iterator it;
                for (it = name_parts.begin(); it != name_parts.end(); ++it) {
                    sorted_name = sorted_name + *it;
                }
                
                //Put the sorted name into common_names
                common_names.push_back(sorted_name);
            }
        }
    }
    
    // Compare names and if they differ, it is most likely a non unique intersection
    for(unsigned i=1; i<common_names.size(); i++) {
        if(common_names[i]!=common_names[i-1]) return 0; //Not unique
    }
    
    if(information.size()==0) return 1; // no intersections
    return 2; //found
}
