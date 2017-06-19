/* A simple demonstration of Gnome menus using GnomeUIInfo structs */

#include <gnome.h>

void closeApp(GtkWidget* window, gpointer data) {
    gtk_main_quit();
}

/* We define a callback for a menu item */
void item_clicked(GtkWidget* widget, gpointer user_data) {
    printf("Item Clicked!\n");
}

/* Defining the menuitems is done with an array of GnomeUIInfo structs */
static GnomeUIInfo submenu[] = {
    {GNOME_APP_UI_ITEM, "SubMenu", "SubMenu Hint", GTK_SIGNAL_FUNC(item_clicked), NULL, NULL, 0, NULL, 0, 0, NULL},
    {GNOME_APP_UI_ENDOFINFO, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, 0, NULL}
};

static GnomeUIInfo menu[] = {
    {GNOME_APP_UI_ITEM, "Menu Item 1", "Menu Hint", NULL, NULL, NULL, 0, NULL, 0, 0, NULL},
    {GNOME_APP_UI_SUBTREE, "Menu Item 2", "Menu Hint", submenu, NULL, NULL, 0, NULL, 0, 0, NULL},
    {GNOME_APP_UI_ENDOFINFO, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, 0, NULL}
};

static GnomeUIInfo menubar[] = {
    {GNOME_APP_UI_SUBTREE, "Toplevel Item", NULL, menu, NULL, NULL, 0, NULL, 0, 0, NULL},
    {GNOME_APP_UI_ENDOFINFO, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, 0, NULL}
};

int main(int argc, char* argv[]) {
    GtkWidget* app;
    /* We must initialise the Gnome libraries */
    gnome_program_init("gnome1", "0.1", LIBGNOMEUI_MODULE,
                       argc, argv,
                       GNOME_PARAM_NONE);
    /* A GnomeApp widget is a child of GtkWindow */
    app = gnome_app_new("gnome1", "Menus, menus, menus");
    gtk_window_set_default_size(GTK_WINDOW(app), 300, 200);
    gtk_signal_connect(GTK_OBJECT(app), "destroy",
                       GTK_SIGNAL_FUNC(closeApp), NULL);
    /* Create the menus and toolbar on the GnomeApp window */
    gnome_app_create_menus(GNOME_APP(app), menubar);
    gnome_app_create_toolbar(GNOME_APP(app), submenu);
    gtk_widget_show(app);
    gtk_main();
    return 0;
}
