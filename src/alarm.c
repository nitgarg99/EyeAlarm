#include "alarm.h"

#define PY_FUNC "detect"

static guint _TIMER;
static guint _FACETIMER;
static int _ALARMTIME;
static int _FACEDETECTS;

/* Used to pass both the window and the text entry objects */
struct window_text {
    GtkWidget *window;
    GtkWidget *entry;
};

struct face_data {
    PyObject *pFunc;
    GtkWidget *window;
};

int main(int argc, char *argv[]) {

    /* Python initialization code */
    PyObject *pName, *pModule, *pDict, *pFunc;
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
    
    /* Insert capture.py into python interpretor */
    pName = PyString_FromString("capture");
    pModule = PyImport_Import(pName);
    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, PY_FUNC);

    /* Set up gui objects */
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *time_label;
    GtkWidget *face_label;
    GdkPixbuf *icon;

    gtk_init(&argc, &argv);

    /* Window layout */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Alarm");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    /* Vertical Box */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    /* Label */
    label = gtk_label_new("Enter alarm time in minutes then press enter.");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0); //Puts the gtk object into a the 'box' for layout purposes
    gtk_widget_show(label);

    /* Text entry box */
    entry = gtk_entry_new();
    struct window_text *wt = malloc(sizeof(struct window_text));
    wt->window = window;
    wt->entry = entry;
    gtk_entry_set_max_length(GTK_ENTRY(entry), 20);
    g_signal_connect(entry, "activate", G_CALLBACK(enter_callback), wt); //Set callback function for when user presses 'enter'
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show(entry);
    

    gtk_widget_show(window);

    /* Set icon using icon creation function */
    icon = create_pixbuf("resources/icon.png");
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    /* For exiting the window */
    g_signal_connect(window, "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    /* Timeout call for alarm */
    _ALARMTIME = 3600000;
    _TIMER = g_timeout_add(_ALARMTIME, (GSourceFunc) alarm_msg, (gpointer) window);
    g_object_unref(icon);

    /* Timeout call of face detection */
    struct face_data *face = malloc(sizeof(struct face_data));
    face->window = window;
    face->pFunc = pFunc;
    _FACETIMER = g_timeout_add(6000, (GSourceFunc) face_callback, face);

    /* Event wait loop */
    gtk_main();

    g_source_remove(_TIMER);
    g_source_remove(_FACETIMER);
    free(wt);
    free(face);
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

gboolean alarm_msg(gpointer window __attribute__((unused))) {

    system("notify-send 'Alarm!' 'Move around, you bum.'");
    return TRUE;
}

gboolean face_callback(struct face_data *face) {
    PyObject *pValue = PyObject_CallObject(face->pFunc, NULL);
    if (PyInt_AsLong(pValue) > 0 )
    {
        _FACEDETECTS++;
    } else {
        _FACEDETECTS = 0;
    }

    if (_FACEDETECTS == 10)
    {
       g_source_remove(_TIMER);
       _TIMER = g_timeout_add(_ALARMTIME, (GSourceFunc) alarm_msg, (gpointer) face->window);
    }
    return TRUE;
}

       


void enter_callback(GtkWidget *widget __attribute__((unused)), struct window_text *wt) {
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY(wt->entry));
    _ALARMTIME = atoi(entry_text);
    gtk_entry_set_text(GTK_ENTRY(wt->entry), ""); //Clear the text upon hitting enter
    g_source_remove(_TIMER);
    _ALARMTIME = _ALARMTIME * 60 * 1000; //convert input into ms
    _TIMER = g_timeout_add(_ALARMTIME, (GSourceFunc) alarm_msg, (gpointer) wt->window);
}
