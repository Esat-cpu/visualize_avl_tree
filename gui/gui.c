#include <gtk/gtk.h>
#include <string.h>

#include "avl_tree.h"
#include "str_to_int.h"


typedef struct {
    Tree* root;
    GtkWidget* drawing_area;
    GtkWidget* insert_box;
    GtkWidget* remove_box;
} AppData;



static void on_insert_click (GtkButton* button, gpointer user_data) {
    AppData* app = (AppData*) user_data;

    const char* text = gtk_entry_get_text (GTK_ENTRY (app->insert_box));
    int val;

    int fl = str_to_int(text, &val);

    if (fl == -1) {
        g_print("Geçersiz Giriş!\n");
    }

    else if (fl == 0) {
        Tree* node = init_tree_node(val);
        app->root = avl_insert(app->root, node);

        // TODO: Draw the new tree
    }

    gtk_entry_set_text (GTK_ENTRY (app->insert_box), "");
}



static void on_remove_click (GtkButton* button, gpointer user_data) {
    AppData* app = (AppData*) user_data;

    const char* text = gtk_entry_get_text (GTK_ENTRY (app->remove_box));
    int val;

    int fl = str_to_int(text, &val);

    if (fl == -1) {
        g_print("Geçersiz Giriş!\n");
    }

    else if (fl == 0) {
        int found = 0;
        app->root = avl_remove(app->root, val, &found);

        if (found) {
            // TODO: Draw the new tree
        }
    }

    gtk_entry_set_text (GTK_ENTRY (app->remove_box), "");
}



static gboolean
on_draw (GtkWidget* widget, cairo_t* cr, gpointer data)
{
    cairo_set_source_rgb(cr, 0.6, 0.85, 0.8);
    cairo_paint(cr);
    return FALSE;
}



static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* vbox;

    // top bar
    GtkWidget* top_bar;

    GtkWidget* insert_button;
    GtkWidget* insert_box;

    GtkWidget* remove_button;
    GtkWidget* remove_box;

    // Drawing Area
    GtkWidget* drawing_area;

    // window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "AVL Tree Thing");
    gtk_window_set_default_size (GTK_WINDOW (window), 1280, 800);


    // Vertical Box
    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    // Top bar (horizonal)
    top_bar = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start (GTK_BOX (vbox), top_bar, FALSE, FALSE, 0);


    // Colorize
    gtk_widget_set_name (top_bar, "top_bar");
    GtkCssProvider* provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider,
            "#top_bar { background-color: #bbbbff; }", -1, NULL);
    GtkStyleContext* context = gtk_widget_get_style_context (top_bar);
    gtk_style_context_add_provider (context,
            GTK_STYLE_PROVIDER (provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (provider);


    // Entries and buttons
    insert_box = gtk_entry_new ();
    insert_button = gtk_button_new_with_label ("Ekle");
    remove_box = gtk_entry_new ();
    remove_button = gtk_button_new_with_label ("Sil");

    gtk_box_pack_start (GTK_BOX (top_bar), insert_box, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX (top_bar), insert_button, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX (top_bar), remove_box, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX (top_bar), remove_button, FALSE, FALSE, 5);


    // Drawing Area
    drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (drawing_area, 400, 300);
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

    g_signal_connect (drawing_area, "draw", G_CALLBACK (on_draw), NULL);

    AppData* app_data = g_new0 (AppData, 1);
    app_data->root = NULL;
    app_data->insert_box = insert_box;
    app_data->remove_box = remove_box;
    app_data->drawing_area = drawing_area;

    g_signal_connect (insert_button, "clicked", G_CALLBACK (on_insert_click), app_data);
    g_signal_connect (remove_button, "clicked", G_CALLBACK (on_remove_click), app_data);


    gtk_widget_show_all (window);
}





int main(int argc, char** argv) {
    GtkApplication* app;
    int status;

    Tree* root = NULL;

    app = gtk_application_new ("com.avltree", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

