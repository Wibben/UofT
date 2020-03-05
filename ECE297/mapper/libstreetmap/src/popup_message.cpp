#include "popup_message.h"

// Display an popup message to the user 
void popup_message(GtkWindow* parent, const char* title, const char* message)
{
    GtkWidget *content_area;
    GtkWidget *error_message;
    GtkWidget *popup_dialog; // Prompt dialog box
    
    // Create the prompt
    popup_dialog = gtk_dialog_new_with_buttons(
        title,
        parent,
        GTK_DIALOG_MODAL, // Do not allow interaction with main window
        ("OK"),
        GTK_RESPONSE_ACCEPT,
        NULL
    );
    
    // Create the error message
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(popup_dialog));
    error_message = gtk_label_new(message);
    // Set up the window and display it
    gtk_container_add(GTK_CONTAINER(content_area), error_message);
    gtk_widget_show_all(popup_dialog);
    
    // Connecting the "response" signal from the user to the associated callback
    g_signal_connect(
        GTK_DIALOG(popup_dialog),
        "response",
        G_CALLBACK(on_popup_response),
        NULL
    );
}

// Close the popup message
void on_popup_response(GtkDialog *dialog, gint, gpointer)
{
    gtk_widget_destroy(GTK_WIDGET (dialog));
}
