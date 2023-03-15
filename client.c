#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include<gtk/gtk.h>
#include<string.h>

#define PORT 8080

void *input_handler(void *);
void *receive_handler(void *);
void load_css();
void button_clicked(GtkWidget *widget, gpointer user_data) ;

typedef struct {
    GtkWidget *entry_widget;
    GtkWidget *box;
} Widgets;

int main(int argc, char *argv[])
{

	
    	// Initialize GTK
   	 gtk_init(&argc, &argv);


    	// Create a new window
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	GtkWidget *grid = gtk_grid_new();
    	GtkWidget *grid2 = gtk_grid_new();
    	GtkWidget *btn = gtk_button_new_with_label("SEND");
    	GtkWidget *inputText = gtk_entry_new(); 

    	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);	

    	//create a scrolled window to hold the messages
    	GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    	gtk_widget_set_size_request(scroll, -1, 400); // set minimum height to 400 pixels
    	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    

    	// Create a new GtkBox object with vertical orientation
    	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    	gtk_container_add(GTK_CONTAINER(window), vbox);

    	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    	gtk_container_add(GTK_CONTAINER(hbox), inputText);
    	gtk_container_add(GTK_CONTAINER(hbox), btn);
 
    	//Load CSS file	
    	load_css();	

    	// Set the window title
    	gtk_window_set_title(GTK_WINDOW(window), "My Chat App");

    	// Set the window size
    	gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    	//Set text in input widget
    	gtk_entry_set_text(GTK_ENTRY(inputText), "Enter your message...");
    	

    	gtk_widget_set_name (grid, "my-grid");
    	gtk_widget_set_name(scroll, "my-scroll");
    	gtk_widget_set_name(btn, "my-btn");
    	gtk_style_context_add_class(gtk_widget_get_style_context(inputText), "input");	    
 
        // set the full width of the window
		gtk_widget_set_hexpand(GTK_WIDGET(grid), TRUE);

		// set the btm to take up the full width of the grid
		gtk_widget_set_hexpand(btn, TRUE);
		gtk_widget_set_halign(btn, GTK_ALIGN_FILL);

		// set the inputText to take up the full width of the grid
		gtk_widget_set_hexpand(inputText, TRUE);
		gtk_widget_set_halign(inputText, GTK_ALIGN_FILL);


		// set the scroll to take up the full width of the grid
		gtk_widget_set_hexpand(scroll, TRUE);
		gtk_widget_set_halign(scroll, GTK_ALIGN_FILL);

		//Box to add labels	
		GtkWidget *boxl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);


		gtk_grid_attach(GTK_GRID(grid2), scroll, 0, 0, 1, 1);
    	gtk_grid_attach(GTK_GRID(grid), hbox, 0, 1, 1, 1);
        gtk_container_add(GTK_CONTAINER(scroll), boxl);		

		//Since only one argument can be passed to g_signal_connect, we need a structure of pointers
    	Widgets widgets = { inputText, boxl };	
		g_signal_connect(btn, "clicked",  G_CALLBACK(button_clicked), &widgets);

    	//gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
    	//gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);    
    	gtk_box_pack_end(GTK_BOX(vbox), grid, FALSE, FALSE, 0);
    	gtk_box_pack_end(GTK_BOX(vbox), grid2, FALSE, FALSE, 0);


		gtk_container_set_border_width(GTK_CONTAINER(btn), 10);		
	
		// set the border width to 10 pixels
		gtk_entry_set_has_frame(GTK_ENTRY(inputText), TRUE);
		gtk_widget_set_margin_top(inputText, 10);
		gtk_widget_set_margin_bottom(inputText, 10);
		gtk_widget_set_margin_start(inputText, 10);
		gtk_widget_set_margin_end(inputText, 10);

    	//To close the window
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
   
    	// Show the window
    	gtk_widget_show_all(window);

    	// Run the main loop
        gtk_main();



     int sock = 0, valread;
     struct sockaddr_in serv_addr;

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect the socket to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
printf("_________ .__    .__  __            _________ .__            __   \n");
    printf("\\_   ___ \\|  |__ |__|/  |_          \\_   ___ \\|  |__ _____ _/  |_ \n");
    printf("/    \\  \\/|  |  \\|  \\   __\\  ______ /    \\  \\/|  |  \\\\__  \\\\   __\\\n");
    printf("\\     \\___|   Y  \\  ||  |   /_____/ \\     \\___|   Y  \\/ __ \\|  |  \n");
    printf(" \\______  /___|  /__||__|            \\______  /___|  (____  /__|  \n");
    printf("        \\/     \\/                           \\/     \\/     \\/      \n");
    printf("Connected to server\n");

    // Create threads to handle input and receive
    pthread_t input_thread_id, receive_thread_id;

    if (pthread_create(&input_thread_id, NULL, input_handler, (void*) &sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    if (pthread_create(&receive_thread_id, NULL, receive_handler, (void*) &sock) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    // Wait for threads to complete
    pthread_join(input_thread_id, NULL);
    pthread_join(receive_thread_id, NULL);

    return 0;
}

void *input_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    char message[1024] = {0};

    while(1) {
        printf("Enter message: ");
        fgets(message, 1024, stdin);
        send(sock , message , strlen(message) , 0 );
    }

    return NULL;
}

void *receive_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    int valread;
    char buffer[1024] = {0};

    while(1) {
        valread = read( sock , buffer, 1024);
        printf("%s\n", buffer);
    }

    return NULL;
}


void button_clicked(GtkWidget *widget, gpointer user_data) {
     	Widgets *widgets = (Widgets*)user_data;
    
    	// Access the two pointers stored in the structure
    	GtkEntry *entry = GTK_ENTRY(widgets->entry_widget);
    	GtkWidget *boxl = widgets->box;
    	const char *text = gtk_entry_get_text(entry);
    	//g_print("Text entered: %s\n", text);


   	if(strlen(text) > 0) {
		GtkWidget *label = gtk_label_new(text);
		gtk_label_set_xalign(GTK_LABEL(label), 0.0);
		gtk_box_pack_start(GTK_BOX(boxl), label, FALSE, FALSE, 0);
		gtk_widget_show(label);	
    		gtk_entry_set_text(GTK_ENTRY(entry), "");
   	}
}

void load_css(){
    	GtkCssProvider *provider;
    	GdkDisplay *display;
    	GdkScreen *screen;

    	provider = gtk_css_provider_new ();
    	display = gdk_display_get_default ();
    	screen = gdk_display_get_default_screen (display);
    	gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    	const gchar *myCssFile = "style.css";
    	GError *error = 0;

    	gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    	g_object_unref (provider);
}
