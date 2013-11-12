#include <gtk/gtk.h>

GtkWidget * BuildMenuItem (gchar *menutext,
		                   gchar acceleratorkey,
						   GCallback signalhandler,
						   GtkWidget *menu,
						   GtkAccelGroup * accelgroup)
{
	GtkWidget *menuitem;
	if (menutext != NULL)
		menuitem = gtk_menu_item_new_with_label(menutext);
	else
		menuitem = gtk_menu_item_new();
	if (signalhandler != NULL)
		g_signal_connect(G_OBJECT(menuitem),
				           "activate", signalhandler, NULL);
	if (menu != NULL)
		gtk_menu_append(GTK_MENU(menu), menuitem);
	gtk_widget_show(menuitem);
	if (accelgroup != NULL && (guint)acceleratorkey != 0)
		gtk_accel_group_add(accelgroup, (guint)acceleratorkey,
				            GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE,
							GTK_OBJECT(menuitem), "activate");
	return menuitem;
}

GtkWidget *gtk_window_create_default (gchar *title, GtkWindowPosition position,
		                              gint x, gint y, gint w) {
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	gtk_window_set_default_size(GTK_WINDOW_TOPLEVEL);
	if (title != NULL)
		gtk_window_set_title(GTK_WINDOW(window), title);
	if (position >= 0 && position < 5 )
		gtk_window_set_position(GTK_WINDOW(window), position);
	if (x != 0 && y != 0)
		gtk_window_set_default_size(GTK_WINDOW(window), x, y);
	if (w != 0)
		gtk_container_set_border_width(GTK_CONTAINER(window), w);
	g_signal_connect(G_OBJECT(window), "delete_event",
			G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(window), "destory",
			G_CALLBACK(gtk_main_quit), NULL);
	return window;
}
