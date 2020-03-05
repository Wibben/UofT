#include "load_new_map.h"

// Opens a dialog to select a new map
void press_load_map(GtkWidget *, ezgl::application *application)
{
    GObject *window; // Parent window for the dialog
    GtkWidget *content_area;
    GtkWidget *prompt;
    GtkWidget *combo_box; // Entry box for the user to input a radius
    GtkWidget *load_map_dialog; // Prompt dialog box

    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the prompt
    load_map_dialog = gtk_dialog_new_with_buttons(
        "Load Map",
        (GtkWindow*) window,
        GTK_DIALOG_MODAL, // Do not allow interaction with main window
        ("LOAD"),
        GTK_RESPONSE_ACCEPT,
        ("CANCEL"),
        GTK_RESPONSE_REJECT,
        NULL
    );
    
    // Prompt the user to choose from a list of maps
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(load_map_dialog));
    prompt = gtk_label_new("Please select a map to load");
    combo_box = gtk_combo_box_text_new();
    // Add all of the maps to the ComboBox
    for(int i=0; i<19; i++) {
        gtk_combo_box_text_append((GtkComboBoxText*)combo_box, 
                                  NULL,
                                  map_list[i].name);
    }
    gtk_combo_box_set_active((GtkComboBox*)combo_box,0); // Preset to an item
    
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), prompt);
    gtk_container_add(GTK_CONTAINER(content_area), combo_box);
    gtk_widget_show_all(load_map_dialog);
    
    // Wrapper to pass in combo_box and application to on_load_response
    LoadMapData* entry = new LoadMapData((GtkComboBox*)combo_box,application);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        GTK_DIALOG(load_map_dialog),
        "response",
        G_CALLBACK(on_load_response),
        entry
    );
}

// Response to the prompt to select a map to load
void on_load_response(GtkDialog *dialog, gint response_id, LoadMapData * entry)
{
    // Check if the user has chosen to load a map
    if(response_id==GTK_RESPONSE_ACCEPT) {
        // Base directory of maps
        std::string map_path = "/cad2/ece297s/public/maps/";
        std::string map_name = "Loaded map of ";
    
        // Current ComboBox selection
        int map_index = gtk_combo_box_get_active(entry->combo_box);

        // Add the path of the selected item to map_path if valid
        map_path = map_path + map_list[map_index].file_path;
        map_name = map_name + map_list[map_index].name;
        
        // Close the current map and open a new one
        close_map();
        load_map(map_path);
        
        // Set the new canvas limits first and then redraw
        ezgl::canvas * canvas = entry->application->get_canvas("MainCanvas");
        ezgl::rectangle new_world({x_from_lon(min_lon), y_from_lat(min_lat)}, 
                                  {x_from_lon(max_lon), y_from_lat(max_lat)});
        canvas->get_camera().set_world(new_world);
        canvas->get_camera().set_initial_world(new_world); // For zoom fit
        entry->application->update_message(map_name.c_str());
        entry->application->refresh_drawing();
    }
    
    // Destroy widget window
    gtk_widget_destroy(GTK_WIDGET (dialog));
}
