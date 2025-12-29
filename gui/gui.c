#include <gtk/gtk.h>
#include <stdio.h>

#include "avl_tree.h"
#include "gtk_avl_tree.h"
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

        gtk_widget_queue_draw (app->drawing_area);
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
            gtk_widget_queue_draw (app->drawing_area);
        }
    }

    gtk_entry_set_text (GTK_ENTRY (app->remove_box), "");
}




static void
draw_node (cairo_t *cr, Tree* node, double x, double y, double x_offset) {
    if (!node) return;

    // Circle
    double r = 20; // radius
    cairo_arc (cr, x, y, r, 0, 2 * G_PI);
    cairo_stroke (cr);

    char buf[8];
    snprintf(buf, sizeof buf, "%d", node->data);

    cairo_move_to (cr, x - 6, y + 5);
    cairo_show_text (cr, buf);

    cairo_new_path (cr);

    double x_left  = x - x_offset;
    double x_right = x + x_offset;
    double y_height = 80;
    double offset  = x_offset / 2;

    if (node->left) {
        // line
        cairo_move_to (cr, x, y + r);
        cairo_line_to (cr, x_left, (y + y_height - r));
        cairo_stroke (cr);

        draw_node (cr, node->left, x_left, y + y_height, offset);
    }

    if (node->right) {
        // line
        cairo_move_to (cr, x, y + r);
        cairo_line_to (cr, x_right, (y + y_height - r));
        cairo_stroke (cr);

        draw_node (cr, node->right, x_right, y + y_height, offset);
    }
}



static gboolean
on_draw (GtkWidget* widget, cairo_t* cr, gpointer data)
{
    AppData* app = data;

    // Background
    cairo_set_source_rgb(cr, 0.6, 0.85, 0.8);
    cairo_paint(cr);

    if (!app || !app->root)
        return FALSE;


    int width  = gtk_widget_get_allocated_width (widget);
    int height = gtk_widget_get_allocated_height (widget);

    double x = width / 2.0;
    double y = 50;

    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_arc (cr, x, y, 20, 0, 2 * G_PI);
    cairo_stroke(cr);

    double start_offset = width / 4.0;
    draw_node(cr, app->root, x, y, start_offset);

    return FALSE;
}



static void
on_window_destroy (GtkWidget* widget, gpointer user_data) {
    AppData* app = user_data;

    free_tree(app->root);
    g_free(app);
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


    AppData* app_data = g_new0 (AppData, 1);
    app_data->root = NULL;
    app_data->insert_box = insert_box;
    app_data->remove_box = remove_box;
    app_data->drawing_area = drawing_area;

    g_signal_connect (drawing_area, "draw", G_CALLBACK (on_draw), app_data);

    g_signal_connect (insert_button, "clicked", G_CALLBACK (on_insert_click), app_data);
    g_signal_connect (remove_button, "clicked", G_CALLBACK (on_remove_click), app_data);

    g_signal_connect (window, "destroy", G_CALLBACK (on_window_destroy), app_data);

    gtk_widget_show_all (window);
}





int main(int argc, char** argv) {
    GtkApplication* app;
    int status;

    app = gtk_application_new ("com.avltree", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

