/* This example shows container widgets in action. We lay out three label widgets
   using nested GtkHbox and GtkVbox widgets. */
#include <gtk/gtk.h>

/* closeApp is a callback function that is called when we close the window. It
   ensures gtk+ quits when the window is closed */

void closeApp(GtkWidget* window, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* label1, *label2, *label3;
    GtkWidget* hbox;
    GtkWidget* vbox;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /* These next functions set the window title and size and position on screen */
    gtk_window_set_title(GTK_WINDOW(window), "The Window Title");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    /* The "destroy" signal is emitted when the window is closed  */
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(closeApp), NULL);
    /* We create our label and container widgets in readyness */
    label1 = gtk_label_new("Label 1");
    label2 = gtk_label_new("Label 2");
    label3 = gtk_label_new("Label 3");
    hbox = gtk_hbox_new(TRUE, 5);
    vbox = gtk_vbox_new(FALSE, 10);
    /* First, we pack label1 and label2 into the vertical container widget, vbox */
    gtk_box_pack_start(GTK_BOX(vbox), label1, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), label2, TRUE, FALSE, 5);
    /* Next, we nest vbox inside the horizontal container widget, hbox and also pack
       label3 */
    gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), label3, FALSE, FALSE, 5);
    /* Finally we add hbox to the window, and show all widgets */
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
