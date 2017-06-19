/* This demonstrates the GtkListView widget, useful for displaying simple
   lists of data. A GtkListStore is used to hold the data and a GtkTreeView to
   display the list */

#include <gtk/gtk.h>

/* This enum is useful to label the columns */
enum {
    FIRST_COLUMN,
    SECOND_COLUMN,
    N_COLUMNS
};

void closeApp(GtkWidget* window, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkListStore* store;
    GtkWidget* view;
    GtkTreeIter iter;
    GtkCellRenderer* renderer;
    int i;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(closeApp), NULL);
    /* We create the list store, passing in the number and type of each column*/
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_INT);

    /* Here we fill the list with data, first appending a new row, then
       setting the row's data */
    for (i = 0; i < 5; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           FIRST_COLUMN, "This is a row",
                           SECOND_COLUMN, i,
                           -1);
    }

    /* A GtkTreeView is the view component responsible for painting the list. A GtkListStore
       is a child class of GtkTreeModel, so it's ok to cast */
    view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    /* Finally we set the column datasource and titles */
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            0,
            "Title", renderer,
            "text", FIRST_COLUMN, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view),
            1,
            "text", renderer,
            "text", SECOND_COLUMN,
            NULL);
    gtk_container_add(GTK_CONTAINER(window), view);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
