#ifndef ALARM_H_
#define ALARM_H_

#include <gtk/gtk.h>
#include <stdlib.h>

struct window_text;
GdkPixbuf *create_pixbuf(const gchar *filename);
void print_msg();
gboolean timer_msg();
gboolean alarm_msg(gpointer window);
void enter_callback(GtkWidget *widget, struct window_text *wt);

#endif
