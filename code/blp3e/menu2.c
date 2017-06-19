/* An improved demonstration of Gnome menus and toolbar using
   Gnome-defined macros to provide the GnomeUIInfo structs */

#include <gnome.h>

void closeApp(GtkWidget* window, gpointer data) {
    gtk_main_quit();
}

/* The macros provide stock icons and keyboard shortcuts */
static GnomeUIInfo filemenu[] = {
    GNOMEUIINFO_MENU_NEW_ITEM("New", "Menu Hint", NULL, NULL),
    GNOMEUIINFO_MENU_OPEN_ITEM(NULL, NULL),
    GNOMEUIINFO_MENU_SAVE_AS_ITEM(NULL, NULL),
    GNOMEUIINFO_SEPARATOR,
    GNOMEUIINFO_MENU_EXIT_ITEM(NULL, NULL),
    GNOMEUIINFO_END
};

static GnomeUIInfo editmenu[] = {
    GNOMEUIINFO_MENU_FIND_ITEM(NULL, NULL),
    GNOMEUIINFO_END
};

static GnomeUIInfo menubar[] = {
    GNOMEUIINFO_MENU_FILE_TREE(filemenu),
    GNOMEUIINFO_MENU_EDIT_TREE(editmenu),
    GNOMEUIINFO_END
};

int main(int argc, char* argv[]) {
    GtkWidget* app, *toolbar;
    gnome_program_init("gnome1", "0.1", LIBGNOMEUI_MODULE,
                       argc, argv,
                       GNOME_PARAM_NONE);
    app = gnome_app_new("gnome1", "Menus, menus, menus");
    gtk_window_set_default_size(GTK_WINDOW(app), 300, 200);
    gtk_signal_connect(GTK_OBJECT(app), "destroy",
                       GTK_SIGNAL_FUNC(closeApp), NULL);
    gnome_app_create_menus(GNOME_APP(app), menubar);
    toolbar = gtk_toolbar_new();
    gnome_app_add_toolbar(GNOME_APP(app), GTK_TOOLBAR(toolbar), "toolbar",
                          BONOBO_DOCK_ITEM_BEH_EXCLUSIVE,
                          BONOBO_DOCK_TOP, 1, 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 1);
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),
                             "gtk-add",
                             "Add new",
                             NULL, NULL, NULL, -1);
    gtk_toolbar_insert_space(GTK_TOOLBAR(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),
                             "gtk-quit",
                             "Quit the Application",
                             NULL, NULL, NULL, -1);
    gtk_widget_show(app);
    gtk_main();
    return 0;
}
