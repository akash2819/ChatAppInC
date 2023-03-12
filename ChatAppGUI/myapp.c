#include <gtk/gtk.h>

void button_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry *entry = GTK_ENTRY(data);
    const char *text = gtk_entry_get_text(entry);
    g_print("Text entered: %s\n", text);
}

static void load_css(){
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;

	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_path (provider,"style.css",NULL);

}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);


    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *btn = gtk_button_new_with_label("SEND");
    GtkWidget *inputText = gtk_entry_new(); 
	
    //Load CSS file	
    load_css();	

    //Set the window position
    //gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);	

    // Set the window title
    gtk_window_set_title(GTK_WINDOW(window), "My Chat App");


    // Set the window size
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    //Set text in input widget
    gtk_entry_set_text(GTK_ENTRY(inputText), "Enter your message...");
    g_signal_connect(btn,"clicked",G_CALLBACK(button_clicked), inputText);

    //To add widgets in window
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_box_pack_start(GTK_BOX(box), inputText, TRUE, TRUE, 0);	
    gtk_box_pack_start(GTK_BOX(box), btn, TRUE, TRUE, 0);
	
    gtk_widget_set_name(btn, "my-btn");
    gtk_widget_set_name (box, "my-box");	

    //To close the window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   
    // Show the window
    gtk_widget_show_all(window);

    // Run the main loop
    gtk_main();

    return 0;
}
	
