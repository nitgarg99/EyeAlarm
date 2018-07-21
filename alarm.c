#include "alarm.h"

static guint _TIMER;

struct window_text {
    GtkWidget *window;
    GtkWidget *entry;
};

int main(int argc, char *argv[]) {

    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *vbox;
    GtkWidget *label;
    GdkPixbuf *icon;

    gtk_init(&argc, &argv);

    /* Window layout */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Alarm");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    /* Vertical Box (alignment tool in gtk) */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    /* Label */
    label = gtk_label_new("Enter alarm time in minutes then press enter.");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
    gtk_widget_show(label);

    /* Text entry box */
    entry = gtk_entry_new();
    struct window_text *wt = malloc(sizeof(struct window_text));
    wt->window = window;
    wt->entry = entry;
    gtk_entry_set_max_length(GTK_ENTRY(entry), 20);
    g_signal_connect(entry, "activate", G_CALLBACK(enter_callback), wt);
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show(entry);
    
    /* Button 
    button = gtk_button_new_with_mnemonic("_Button");
    gtk_widget_set_tooltip_text(button, "Activate onTime");
    g_signal_connect(button, "clicked",
            G_CALLBACK(print_msg), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
    gtk_widget_show(button);
    */

    gtk_widget_show(window);

    /* Set icon using icon creation function */
    icon = create_pixbuf("2.png");
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    /* For exiting the window */
    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    /* Timeout call */
    _TIMER = g_timeout_add(3600000, (GSourceFunc) alarm_msg, (gpointer) window);
    g_object_unref(icon);

    /* Event wait loop */
    gtk_main();

    free(wt);
    return 0;
}

/* Icon creation function */
GdkPixbuf *create_pixbuf(const gchar *filename) {

    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if (!pixbuf) {

        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

void print_msg() {

    printf("Button clicked\n");
}

gboolean alarm_msg(gpointer window) {

    /*
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Alarm Signaled!");
    gtk_window_set_title(GTK_WINDOW(dialog), "Signal!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    */
    system("notify-send 'Alarm!' 'Move around, you bum.'");
    return TRUE;
}

gboolean timer_msg() {
    printf("Alarm!\n");
    return TRUE;
}

void enter_callback(GtkWidget *widget, struct window_text *wt) {
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(wt->entry));
    g_source_remove(_TIMER);
    int tmp = atoi(entry_text);
    tmp = tmp * 60 * 1000; //convert input into ms
    _TIMER = g_timeout_add(tmp, (GSourceFunc) alarm_msg, (gpointer) wt->window);
}
