/* This is an example of gtk+ button widgets - checkbutton,
   togglebutton, radiobutton and simple gtkbutton */

#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget* checkbutton;
GtkWidget* togglebutton;
GtkWidget* radiobutton1, *radiobutton2;

void closeApp(GtkWidget* window, gpointer data) {
    gtk_main_quit();
}

/* This is a convenience function that adds a widget with a label
   of specified text to the given container widget */

void add_widget_with_label(GtkContainer* box, gchar* caption, GtkWidget* widget) {
    GtkWidget* label = gtk_label_new(caption);
    GtkWidget* hbox = gtk_hbox_new(TRUE, 4);
    gtk_container_add(GTK_CONTAINER(hbox), label);
    gtk_container_add(GTK_CONTAINER(hbox), widget);
    gtk_container_add(box, hbox);
}

/* This function prints the state of the given button with identifying
   name */
void print_active(char* name, GtkToggleButton* button) {
    gboolean active = gtk_toggle_button_get_active(button);
    printf("%s is currently %s\n", name, active ? "active" : "not active");
}

/* Here's the callback function that we connect to the "clicked" signal of the
   'ok' gtkbutton. It prints the status (i.e. on or off)
    of each of the buttons */
void button_clicked(GtkWidget* button, gpointer data) {
    print_active("Checkbutton", GTK_TOGGLE_BUTTON(checkbutton));
    print_active("Togglebutton", GTK_TOGGLE_BUTTON(togglebutton));
    print_active("Radiobutton1", GTK_TOGGLE_BUTTON(radiobutton1));
    print_active("Radiobutton2", GTK_TOGGLE_BUTTON(radiobutton2));
    printf("\n");
}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* button;
    GtkWidget* vbox;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(closeApp), NULL);
    /* Here we create the buttons, note that we pass the radiobutton1 pointer
       when we create radiobutton2 so they are in the same group, and only one
       can be selected at a time */
    button = gtk_button_new_with_label("Ok");
    checkbutton = gtk_check_button_new();
    togglebutton = gtk_toggle_button_new_with_label("Toggle");
    radiobutton1 = gtk_radio_button_new(NULL);
    radiobutton2 = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radiobutton1));
    vbox = gtk_vbox_new(TRUE, 4);
    /* We use our helper function to populate the window with our buttons */
    add_widget_with_label(GTK_CONTAINER(vbox), "CheckButton:", checkbutton);
    add_widget_with_label(GTK_CONTAINER(vbox), "ToggleButton:", togglebutton);
    add_widget_with_label(GTK_CONTAINER(vbox), "Radio 1:", radiobutton1);
    add_widget_with_label(GTK_CONTAINER(vbox), "Radio 2:", radiobutton2);
    add_widget_with_label(GTK_CONTAINER(vbox), "Button:", button);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(button_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
