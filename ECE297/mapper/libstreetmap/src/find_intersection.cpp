#include "find_intersection.h"
#include <iostream>
#include <cstring>

std::vector<int> info;

gboolean press_find (GtkWidget *, gpointer data)
{
    auto application = static_cast<ezgl::application *>(data);

    GObject *window; // the parent window over which to add the dialog
    GtkWidget *content_area; // the content area of the dialog
    GtkWidget *prompt1;
    GtkWidget *entry1; // Entry box for the user to input a radius
    GtkWidget *prompt2;
    GtkWidget *entry2; // Entry box for the user to input a radius
    GtkWidget *dialog;
    
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the dialog window.
    // Modal windows prevent interaction with other windows in the same application
    dialog = gtk_dialog_new_with_buttons(
        "Find Intersection",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL,
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );

    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    prompt1 = gtk_label_new("Enter first street name:");
    entry1 = gtk_entry_new();
    gtk_entry_set_text((GtkEntry*)entry1,""); // Default range
    prompt2 = gtk_label_new("Enter second street name:");
    entry2 = gtk_entry_new();
    gtk_entry_set_text((GtkEntry*)entry2,""); // Default range
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), prompt1);
    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    gtk_container_add(GTK_CONTAINER(content_area), prompt2);
    gtk_container_add(GTK_CONTAINER(content_area), entry2);

    // The main purpose of this is to show dialog’s child widget, label
    gtk_widget_show_all(dialog);
    
    EntryData* entry = new EntryData(entry1,entry2,application);
    
    g_signal_connect(
        GTK_DIALOG(dialog),
        "response",
        G_CALLBACK(on_dialog_response),
        entry
    );
    
    return TRUE;
}
  
void on_dialog_response(GtkDialog *dialog, gint response_id, EntryData* entry)
{
    GtkEntry * entry1 = (GtkEntry*)entry->a;
    GtkEntry * entry2 = (GtkEntry*)entry->b;
    info.clear();
    
    bool can_close = false;
    std::string street_1= gtk_entry_get_text(entry1);
    std::string street_2 = gtk_entry_get_text(entry2);
    int found = find_intersection(street_1,street_2);
    
    switch(response_id) {
        case GTK_RESPONSE_ACCEPT:
            // Keep the prompt open if input is out of range
            if(street_1 == "" || street_2 == "" || found == 3) 
                popup_message((GtkWindow*)dialog,"ERROR","Please enter valid street names");
            else if(found == 0) 
                popup_message((GtkWindow*)dialog,"ERROR","Please enter unique street names");
            else if(found == 1) 
                popup_message((GtkWindow*)dialog,"ERROR","No common intersections");
            else{
                display_intersections(entry->app);
                can_close = true;
            }
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            can_close = true;
            break;
        case GTK_RESPONSE_REJECT:
            can_close = true;
            break;
        default:break;
    }
    // This will cause the dialog to be destroyed and close
    // without this line the dialog remains open unless the
    // response_id is GTK_RESPONSE_DELETE_EVENT which
    // automatically closes the dialog without the following line.
    if(can_close) gtk_widget_destroy(GTK_WIDGET (dialog));
}

int find_intersection (std::string street_1, std::string street_2){
    info.clear();
    //find street ids from partial street name
    std::vector<unsigned> street_id_1 = find_street_ids_from_partial_street_name(street_1);
    std::vector<unsigned> street_id_2 = find_street_ids_from_partial_street_name(street_2);
    
    if(street_id_1.size() == 0 || street_id_2.size() == 0) return 3; // not valid
    
    reset_highlighted_intersection_ID();
    
    for(unsigned b = 0; b < street_id_1.size(); b++){
        for(unsigned d =0; d < street_id_2.size(); d++){
            std::vector<unsigned> common_intersections = find_intersection_ids_from_street_ids
                                                   (street_id_1[b], street_id_2[d]);
            for(unsigned int i=0; i < common_intersections.size(); i++){
                info.push_back(common_intersections[i]);
            }
        }
    }
    
    //check validity
    if(info.size() == 0) return 1;
    //remove duplicates
    auto end = info.end();
    for(auto it = info.begin(); it != end; ++it){
        end = std::remove(it+1,end,*it);
    }
    info.erase(end, info.end());
    //set highlighted
    for(unsigned j =0; j< info.size(); j++)
        set_highlighted_intersection_ID(info[j]);
    return 2; //found
}

void display_intersections(ezgl::application *application)
{
    // Update the highlighted intersections
    application->refresh_drawing();
    
    GObject *window; // the parent window over which to add the dialog
    GtkWidget *content_area; // the content area of the dialog
    GtkWidget *dialog;
    
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the dialog window.
    // Modal windows prevent interaction with other windows in the same application
    dialog = gtk_dialog_new_with_buttons(
        "Common Intersections",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL,
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );
    gtk_widget_set_tooltip_text(dialog,
            "Click on a intersection to show its\nlocation on the map");

    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Add all the found intersections to the dialog
    for(unsigned i=0; i<info.size(); i++) {
        // Show intersection as a button
        std::string label = std::to_string(i+1)+ ". " +getIntersectionName(info[i]);
        GtkWidget *intersection_button = gtk_button_new_with_label(label.c_str());
        gtk_button_set_relief((GtkButton*)intersection_button,GTK_RELIEF_NONE);
        
        // Connect to dialog window
        gtk_container_add(GTK_CONTAINER(content_area), intersection_button);
        g_signal_connect(
            GTK_BUTTON(intersection_button),
            "clicked",
            G_CALLBACK(press_intersection),
            application
        );
    }

    // The main purpose of this is to show dialog’s child widget, label
    gtk_widget_show_all(dialog);
    
    // Just close the dialog if user hits OK or CANCEL
    g_signal_connect(
        GTK_DIALOG(dialog),
        "response",
        G_CALLBACK(on_popup_response), // Respond as if it was a popup
        NULL
    );
}

gboolean press_intersection(GtkWidget *button, gpointer data)
{
    auto application = static_cast<ezgl::application *>(data);
    
    // Get the label id on the intersection, which is the index of
    // the intersection id in info+1
    std::stringstream label(gtk_button_get_label((GtkButton*)button));
    IntersectionIndex label_id;
    label >> label_id;
    
    // Get the position of intersection associated with the button
    LatLon intersection_position = getIntersectionPosition(info[label_id-1]);
    application->update_message(getIntersectionName(info[label_id-1]));
    
    // Set the marker to the intersection's position
    set_marker_points(x_from_lon(intersection_position.lon()),
                      y_from_lat(intersection_position.lat()),
                      application->get_canvas(application->get_main_canvas_id()),
                      true); // Reset find POI range
    
    // Set zoom to center around intersection, keep zoom in 4:3 ratio
    ezgl::canvas * canvas = application->get_canvas("MainCanvas");
    ezgl::rectangle new_world(
        {x_from_lon(intersection_position.lon())-0.00012,
         y_from_lat(intersection_position.lat())-0.00009}, 
        {x_from_lon(intersection_position.lon())+0.00012,
         y_from_lat(intersection_position.lat())+0.00009}
    );
    canvas->get_camera().set_world(new_world);
    application->refresh_drawing();
    
    return TRUE;
}
