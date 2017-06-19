/* The first example is a simple gtk+ program to get us started.
   gtk+ is first initialised, a gtkwindow widget created, and shown.
   gtk_main is called to begin the processing loop */

#include <gtk/gtk.h>

int main(int argc, char* argv[]) {
    GtkWidget* window;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);
    gtk_main();
    return 0;
}
