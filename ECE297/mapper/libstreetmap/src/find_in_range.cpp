#include "find_in_range.h"

std::vector<POIData> POIs; // Holds all the POIs within a radius
GtkWindow *ListWindow; // Window for the table of POIs
int find_poi_range = 0; // Initialize to 0
int table_pos_x,table_pos_y;
// Some kind of workaround to allow press_poi_ID to function
ezgl::application *workaround;

// Opens window to prompt user for range
void press_pois_in_range(GtkWidget *, ezgl::application *application)
{
    // For some reason seems to be the only way for press_poi_ID to function
    workaround = application;
    // Set the POI range's center to the current marker position
    set_poi_range_center();
    
    GObject *window; // Parent window for the dialog
    GtkWidget *content_area;
    GtkWidget *prompt;
    GtkWidget *entry_box; // Entry box for the user to input a radius
    GtkWidget *prompt_dialog; // Prompt dialog box

    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Reset table's x and y positions to just be somewhat to the right of window
    gtk_window_get_position((GtkWindow*)window,&table_pos_x,&table_pos_y);
    table_pos_x += 300;
    table_pos_y += 200;
    
    // Create the prompt
    prompt_dialog = gtk_dialog_new_with_buttons(
        "Find POIs in Range",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL, // Do not allow interaction with main window
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );
    
    // Prompt the user to enter a range to list the POIs in a textbox
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(prompt_dialog));
    prompt = gtk_label_new("Please enter the range (1-1000m):");
    entry_box = gtk_entry_new();
    gtk_entry_set_text((GtkEntry*)entry_box,"500"); // Default range
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), prompt);
    gtk_container_add(GTK_CONTAINER(content_area), entry_box);
    gtk_widget_show_all(prompt_dialog);
    
    POIListData* entry = new POIListData((GtkEntry*)entry_box,application);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        GTK_DIALOG(prompt_dialog),
        "response",
        G_CALLBACK(on_prompt_response),
        entry
    );
}

// Response to the prompt to enter a radius
void on_prompt_response(GtkDialog *dialog, gint response_id, POIListData *entry)
{
    // If there is a value and OK is pressed, open the window of new POIs
    // Keep the dialog open until the user enters a valid value
    bool can_close = false;
    int input_range;
    
    // Parse range from input
    std::stringstream input(gtk_entry_get_text(entry->entry));
    input >> input_range;
    
    // Check for user input
    switch(response_id) {
        case GTK_RESPONSE_ACCEPT:
            // Keep the prompt open if input is out of range
            if(input_range>=1 && input_range<=1000) {
                can_close = true;
                find_poi_range = input_range;
                generate_poi_table(entry->application);
            } else {
                popup_message((GtkWindow*)dialog,"ERROR",
                              "Please enter a value between 1 and 1000m");
            }
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            can_close = true;
            break;
        case GTK_RESPONSE_REJECT:
            can_close = true;
            break;
        default: break;
    }
    
    // Destroy widget window
    if(can_close) gtk_widget_destroy(GTK_WIDGET (dialog));
}

// Generates and displays a list of POIs within a range
void generate_poi_table(ezgl::application * application)
{
    // Find POIs within range
    LatLon position(lat_from_y(mouse_y),lon_from_x(mouse_x));
    POIs = find_poi_in_range(position);
    
    // Let the user know if there are no POIs in the vincinity
    if(POIs.size()==0) {
        GObject* window = application->get_object(application->get_main_window_id().c_str());
        std::string message = "There are no POIs within "+std::to_string(find_poi_range)+"m.";
        
        popup_message((GtkWindow*)window,"No POIs Found", message.c_str());
    } else {
        // Set zoom to the vicinity of the function, keep the zoom in the ratio 4:3
        ezgl::canvas * canvas = application->get_canvas("MainCanvas");
        double x_offset = 0.00000024*find_poi_range;
        double y_offset = 0.00000018*find_poi_range;
        ezgl::rectangle new_world({mouse_x-x_offset, mouse_y-y_offset}, 
                                  {mouse_x+x_offset, mouse_y+y_offset});
        canvas->get_camera().set_world(new_world);
        
        update_and_show_table();
        application->refresh_drawing(); // Draw the range indicator
    }
}

// Sort POIs by ID
void press_ID(GtkWidget *, gint, gpointer)
{
    // Store the current location of the window
    gtk_window_get_position(ListWindow,&table_pos_x,&table_pos_y);
    sort(POIs.begin(),POIs.end(),cmp_ID);
    gtk_widget_destroy(GTK_WIDGET(ListWindow)); // Close old window
    update_and_show_table();
}
// Sort POIs by name
void press_name(GtkWidget *, gint, gpointer)
{
    // Store the current location of the window
    gtk_window_get_position(ListWindow,&table_pos_x,&table_pos_y);
    sort(POIs.begin(),POIs.end(),cmp_name);
    gtk_widget_destroy(GTK_WIDGET(ListWindow)); // Close old window
    update_and_show_table();
}
// Sort POIs by type
void press_type(GtkWidget *, gint, gpointer)
{
    // Store the current location of the window
    gtk_window_get_position(ListWindow,&table_pos_x,&table_pos_y);
    sort(POIs.begin(),POIs.end(),cmp_type);
    gtk_widget_destroy(GTK_WIDGET(ListWindow)); // Close old window
    update_and_show_table();
}
// Sort POIs by distance
void press_dist(GtkWidget *, gint, gpointer)
{
    // Store the current location of the window
    gtk_window_get_position(ListWindow,&table_pos_x,&table_pos_y);
    sort(POIs.begin(),POIs.end(),cmp_dist);
    gtk_widget_destroy(GTK_WIDGET(ListWindow)); // Close old window
    update_and_show_table();
}

// Response for pressing the id of the POI to display
void press_poi_ID(GtkWidget *ID_button, gint, gpointer)
{
    // Parse POI ID from button label and get the POI's lat/lon position
    POIIndex poi_ID;
    std::stringstream ID_parser(gtk_button_get_label((GtkButton*)ID_button));
    ID_parser >> poi_ID;
    
    LatLon poi_latlon = getPointOfInterestPosition(poi_ID);
    
    // Set the marker to the POI's position
    set_marker_points(x_from_lon(poi_latlon.lon()),
                      y_from_lat(poi_latlon.lat()),
                      workaround->get_canvas(workaround->get_main_canvas_id()),
                      false);
    workaround->update_message(getPointOfInterestName(poi_ID));
    workaround->refresh_drawing();
}

// Updates the table by drawing a brand new window
void update_and_show_table()
{
    GtkWidget *scrolling; // Setting up to allow for scrolling
    GtkWidget *table; // Table for the POIs
    GtkWidget *headings[4]; // Column headings
    
    // Create the window
    std::string title = "POIs within " + std::to_string(find_poi_range) + "m";
    ListWindow = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(ListWindow,title.c_str());
    gtk_window_set_default_size(ListWindow,1000,600);
    gtk_widget_set_hexpand((GtkWidget*)ListWindow,true);
    gtk_window_set_modal(ListWindow,true); // Do not allow interaction with main app
    gtk_window_activate_focus(ListWindow);
    
    scrolling = gtk_scrolled_window_new(NULL,NULL);
    gtk_widget_set_tooltip_text(scrolling, 
            "Click the ID of the POI to show its\nlocation on the map");
    
    // Set up the table with buttons as headings
    table = gtk_grid_new();
    headings[0] = gtk_button_new_with_label("ID");
    gtk_widget_set_tooltip_text(headings[0], "Sort by ID");
    headings[1] = gtk_button_new_with_label("Name");
    gtk_widget_set_tooltip_text(headings[1], "Sort by Name");
    headings[2] = gtk_button_new_with_label("Type");
    gtk_widget_set_tooltip_text(headings[2], "Sort by Type");
    headings[3] = gtk_button_new_with_label("Distance");
    gtk_widget_set_tooltip_text(headings[3], "Sort by Distance");
    gtk_grid_attach((GtkGrid*)table,headings[0],0,0,1,1);
    gtk_grid_attach((GtkGrid*)table,headings[1],1,0,1,1);
    gtk_grid_attach((GtkGrid*)table,headings[2],2,0,1,1);
    gtk_grid_attach((GtkGrid*)table,headings[3],3,0,1,1);
    
    // Input the values into the table as labels
    for(unsigned int i=0; i<POIs.size(); i++) {
        GtkWidget* col[4];
        // Convert everything to char*'s, strings can be converted directly
        // while ints and doubles must be casted to std::string first
        // Also only take the first 6 characters of distance
        col[0] = gtk_button_new_with_label(std::to_string(POIs[i].ID).c_str());
        gtk_widget_set_name(col[0],"list_of_pois_ID");
        gtk_button_set_relief((GtkButton*)col[0],GTK_RELIEF_NONE);
        g_signal_connect(col[0],"clicked",G_CALLBACK(press_poi_ID),NULL);
        col[1] = gtk_label_new(POIs[i].name.c_str());
        col[2] = gtk_label_new(POIs[i].type.c_str());
        col[3] = gtk_label_new(std::to_string(POIs[i].dist).substr(0,6).c_str());
        
        // Right align the IDs
        gtk_widget_set_halign(col[0],GTK_ALIGN_END);
        
        // Put the row into the table right justified
        for(int j=0; j<4; j++) {
            gtk_widget_set_hexpand(col[j],true);
            gtk_grid_attach((GtkGrid*)table,col[j],j,i+1,1,1);
        }
    }
    // Put a little spacing between columns so it isn't too cramped
    gtk_grid_set_column_spacing((GtkGrid*)table,5);
    
    // Set up the window for display
    gtk_container_add(GTK_CONTAINER(scrolling),table);
    gtk_container_add(GTK_CONTAINER(ListWindow),scrolling);
    gtk_window_move(ListWindow,table_pos_x,table_pos_y);
    gtk_widget_show_all((GtkWidget*)ListWindow);
    
    // Set up the callbacks for the buttons
    g_signal_connect(headings[0],"clicked",G_CALLBACK(press_ID),NULL);
    g_signal_connect(headings[1],"clicked",G_CALLBACK(press_name),NULL);
    g_signal_connect(headings[2],"clicked",G_CALLBACK(press_type),NULL);
    g_signal_connect(headings[3],"clicked",G_CALLBACK(press_dist),NULL);
}

// Returns all points of interests within range of a given position
// and their distance to the point, range is in meters
std::vector<POIData> find_poi_in_range(LatLon position)
{
    // Get the num of POIs, indexing starts from 0 to num_poi-1
    int num_poi = getNumPointsOfInterest();
    std::vector<POIData> POIs_in_range;
    
    double dist;
    
    // Go through every POI and insert the ones within range into the vector
    for(POIIndex i=0; i<num_poi; i++) {
        dist = find_distance_between_two_points(
                    position,getPointOfInterestPosition(i));
        
        if(dist<=find_poi_range) POIs_in_range.push_back( POIData(
                                i, // ID
                                getPointOfInterestName(i), // Name
                                getPointOfInterestType(i), // Type
                                dist // Distance
                        ));
    }
    
    return POIs_in_range;
}

// Comparison Functions, will sort from least to greatest
bool cmp_ID(const POIData &a, const POIData &b) { return a.ID < b.ID; }
bool cmp_name(const POIData &a, const POIData &b) 
{ 
    if(a.name==b.name && a.type==b.type) return a.dist < b.dist;
    else if(a.name==b.name) a.type < b.type;
    return a.name < b.name; 
}
bool cmp_type(const POIData &a, const POIData &b) 
{ 
    if(a.type==b.type && a.name==b.name) return a.dist < b.dist;
    else if(a.type==b.type) return a.name < b.name;
    return a.type < b.type; 
}
bool cmp_dist(const POIData &a, const POIData &b) { return a.dist < b.dist; }
