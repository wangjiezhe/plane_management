#ifndef GTK_ITEMS_H_
#define	GTK_ITEMS_H_

extern GtkWidget *BuildMenuItem(gchar *menutext,
		                        gchar acceleratorkey,
								GCallback signalhandler,
								GtkWidget *menu,
								GtkAccelGroup *accelgroup);

extern GtkWidget *gtk_window_create_default(gchar *title, GtkWindowPosition position,
		                                    gint x, gint y, gint width);

#endif	/* gtkitems.h */
