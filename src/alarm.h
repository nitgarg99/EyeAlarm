#ifndef ALARM_H_
#define ALARM_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <Python.h>

struct window_text;
struct face_data;
GdkPixbuf *create_pixbuf(const gchar *filename);
void print_msg();
gboolean timer_msg();
gboolean alarm_msg(gpointer window);
void enter_callback(GtkWidget *widget, struct window_text *wt);
gboolean face_callback(struct face_data *face);

#endif
