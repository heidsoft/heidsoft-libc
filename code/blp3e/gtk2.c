/* This example demonstrates the principles of gtk+ events and callback
   functions using a GtkButton. The "clicked" signal emitted by the GtkButton
   is connected to the button_clicked function using gtk_signal_connect */

#include <gtk/gtk.h>
#include <stdio.h>

static int count = 0;

/* This is the callback function called when the button is clicked */
void button_clicked(GtkWidget* button, gpointer data) {
    printf("%s pressed %d time(s) \n", (char*) data, ++count);
}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* button;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* We create the button and add it to the window */
    button = gtk_button_new_with_label("Hello World!");
    gtk_container_add(GTK_CONTAINER(window), button);
    /* This is the important bit where the callback function is connected
       to the widget signal. The "Button 1" string is passed as the arbitrary user data */
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(button_clicked),
                       "Button 1");
    /* We make both widgets visible on screen. Alternatively, we could write
       just gtk_widget_show_all(window); */
    gtk_widget_show(window);
    gtk_widget_show(button);
    gtk_main();
    return 0;
}
