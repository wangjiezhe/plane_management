
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "PriorityQueue.h"
#include "data.h"
//#include "gtkitems.h"

PPriorityQueue waiting;
GtkWidget *status_bar;
GtkWidget *entry1;
GtkWidget *entry2;
GTimer *dialog_timer;
GtkWidget *text1;
GtkWidget *text2;
GtkWidget *text3;
GtkTextBuffer *buffer1;
GtkTextIter iter1;

GtkWidget *make_menu_item (GtkWidget *menu, gchar *item_text,
		GCallback callbackfunc, gpointer data) {
	GtkWidget *menu_item;
	menu_item = gtk_menu_item_new_with_label (item_text);
	gtk_menu_shell_append (GTK_MENU_SHELL(menu), menu_item);
	g_signal_connect (G_OBJECT(menu_item), "activate",
			callbackfunc, G_OBJECT(data));
	gtk_widget_show (menu_item);
	return menu_item;
}

void about_called (GtkWidget *widget, gpointer data) {
	GtkWidget *about_dialog;
	const gchar *authors[] = {
		"Wang Jiezhe"
	};
	about_dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name (
			GTK_ABOUT_DIALOG(about_dialog), "PLANE MANAGEMENT");
	gtk_about_dialog_set_version (
			GTK_ABOUT_DIALOG(about_dialog), "0.0.1");
	gtk_about_dialog_set_license (
			GTK_ABOUT_DIALOG(about_dialog), "GPLv3");
	gtk_about_dialog_set_wrap_license (
			GTK_ABOUT_DIALOG(about_dialog), TRUE);
	gtk_about_dialog_set_authors (
			GTK_ABOUT_DIALOG(about_dialog), authors);
//	gtk_dialog_run (GTK_DIALOG(about_dialog));
	g_signal_connect (G_OBJECT(about_dialog), "response",
			G_CALLBACK(gtk_widget_destroy), about_dialog);
	gtk_widget_show (about_dialog);
	return ;
}

GtkWidget *make_table_with_button (gchar *label, GCallback callbackfunc) {
	GtkWidget *table;
	GtkWidget *button;
	table = gtk_table_new (1, 3, TRUE);
	button = gtk_button_new_with_label (label);
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(callbackfunc), NULL);
	gtk_table_attach_defaults (GTK_TABLE(table), button, 1, 2, 0, 1);
	gtk_widget_show(button);
	gtk_widget_show (table);
	return table;
}

void dialog_ok_clicked (GtkWidget *widget, gpointer data) {
	gchar oil_text[7];
	DataType_pq flight;
	gdouble remaining_time;
	gdouble dialog_time;
	guint16 len1, len2;
	const gchar *entry1_text = g_utf8_strup (
			g_locale_to_utf8(gtk_entry_get_text(GTK_ENTRY(entry1)), -1, NULL, NULL, NULL), -1);
	const gchar *entry2_text = g_locale_to_utf8 (
			gtk_entry_get_text(GTK_ENTRY(entry2)), -1, NULL, NULL, NULL);
	strncpy (flight.flight_name, entry1_text, len1);
	strncpy (oil_text, entry2_text, len2);
	remaining_time = atoi(oil_text) / OPH;
	dialog_time = g_timer_elapsed (dialog_timer, NULL);
	flight.landing_deadline = time(NULL) + (time_t)(remaining_time * 3600) - (time_t)dialog_time;
	g_timer_stop (dialog_timer);
	g_timer_destroy (dialog_timer);
	add_heap (waiting, flight);
	gtk_widget_destroy (GTK_WIDGET(data));
	gtk_text_buffer_insert (buffer1, &iter1, "\nNew plane added into waiting queue.", -1);
	gtk_text_buffer_insert (buffer1, &iter1, "\nFlight No.: ", -1);
	gtk_text_buffer_insert (buffer1, &iter1, entry1_text, -1);
	gtk_text_buffer_insert (buffer1, &iter1, "\nRemaining oil(/L): ", -1);
	gtk_text_buffer_insert (buffer1, &iter1, entry2_text, -1);
//	g_print("%s, %d\n", entry1_text, len1);
//	g_print("%s, %d\n", entry2_text, len2);
	return;
}

void dialog_cancel_clicked (GtkWidget *widget, gpointer data) {
	g_timer_stop (dialog_timer);
	g_timer_destroy (dialog_timer);
	gtk_widget_destroy (GTK_WIDGET(data));
}

GtkWidget *make_entryhbox_with_label (GtkWidget *entry, gchar *label_text) {
	GtkWidget *hbox;
	GtkWidget *label;
	hbox = gtk_hbox_new (TRUE, 5);
	label = gtk_label_new (label_text);
	gtk_box_pack_start (GTK_BOX(hbox), label, TRUE, TRUE, 2);
	gtk_box_pack_end (GTK_BOX(hbox), entry, TRUE, TRUE, 2);
	gtk_widget_show (label);
	gtk_widget_show (entry);
	gtk_widget_show (hbox);
	return hbox;
}

/* callback function for button "New plane arrived" */
void new_plane_arrived (GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	GtkWidget *button;
	GtkWidget *entryhbox;

	dialog_timer = g_timer_new();
	g_timer_start(dialog_timer);

	dialog = gtk_dialog_new();
	gtk_window_set_title (GTK_WINDOW(dialog), "New Plane");
	gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
	button = gtk_button_new_with_label ("Ok");
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(dialog_ok_clicked), dialog);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
			button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label ("Cancel");
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(dialog_cancel_clicked), dialog);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
			button, TRUE, TRUE, 0);
	gtk_widget_show (button);

	entry1 = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY(entry1), 6);
	entryhbox = make_entryhbox_with_label (entry1, "Flight No. :");
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), entryhbox, TRUE, TRUE, 2);

	entry2 = gtk_entry_new ();
	entryhbox = make_entryhbox_with_label (entry2, "Oil (/L) :");
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), entryhbox, TRUE, TRUE, 2);

	gtk_widget_show (dialog);

	return;
}

/* callback function for button "Ok to land" */
void ok_to_land (GtkWidget *widget, gpointer data) {
	return;
}

/* callback function for button "Ok to take off" */
void ok_to_takeoff (GtkWidget *widget, gpointer data) {
	return;
}

gint main (gint argc, gchar *argv[]) {
	GtkWidget *window;
	GtkWidget *menu_bar;
	GtkWidget *menu;
	GtkWidget *menu_item;
	GtkWidget *root_menu;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *vboxin;
//	GtkWidget *button;
	GtkWidget *scrolled_window;
	GtkWidget *table;

	guint context_id;

	waiting = create_heap (MAXNUM);

	/* init the GTK */
	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "PLANE MANAGEMENT");
//	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	g_signal_connect (G_OBJECT (window), "delete_event",
			G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT (window), "destroy",
			G_CALLBACK (gtk_main_quit), NULL);


	/* a vbox to put all things in */
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(window), vbox);


	/* init the menu */
	menu = gtk_menu_new ();

	/* create menu-items under the root-menu */
	menu_item = make_menu_item (menu, "About", G_CALLBACK(about_called), window);
	menu_item = make_menu_item (menu, "Quit", G_CALLBACK(gtk_main_quit), window);
	/* this is the root-menu and will be displayed on the menu-bar */
	root_menu = gtk_menu_item_new_with_label("Menu");
	gtk_widget_show (root_menu);

	/* menu we created is the menu for the root-menu */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(root_menu), menu);
	gtk_widget_show (menu);

	/* create a menu_bar to hold the menus and add it to the main window */
	menu_bar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX(vbox), menu_bar, FALSE, TRUE, 2);
	gtk_widget_show (menu_bar);

	/* append the menu-item to the menu-bar */
	gtk_menu_shell_append (GTK_MENU_SHELL(menu_bar), root_menu);


	/* create a status bar (optional) */
	status_bar = gtk_statusbar_new ();
	gtk_box_pack_end (GTK_BOX(vbox), status_bar, FALSE, FALSE, 0);
	gtk_widget_show (status_bar);
	context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR(status_bar), "status");


	hbox = gtk_hbox_new (TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 2);




	/* module for arriving */
	vboxin = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);

	table = make_table_with_button ("New plane arrived", G_CALLBACK(new_plane_arrived));
	gtk_box_pack_start(GTK_BOX(vboxin), table, FALSE, FALSE, 2);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 5);

	text1 = gtk_text_view_new ();
	gtk_text_view_set_editable (GTK_TEXT_VIEW(text1), FALSE);
	gtk_container_add (GTK_CONTAINER(scrolled_window), text1);
	buffer1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text1));
	gtk_text_buffer_get_iter_at_offset (buffer1, &iter1, 0);
	gtk_widget_show (text1);

	gtk_widget_show(scrolled_window);
	gtk_widget_show (vboxin);



	/* module for landing */
	vboxin = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);

	table = make_table_with_button ("Ok to land", G_CALLBACK(ok_to_land));
	gtk_box_pack_start(GTK_BOX(vboxin), table, FALSE, FALSE, 2);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 2);


	gtk_widget_show(scrolled_window);
	gtk_widget_show (vboxin);



	/* module for taking off */
	vboxin = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);

	table = make_table_with_button ("Ok to take off", G_CALLBACK(ok_to_takeoff));
	gtk_box_pack_start(GTK_BOX(vboxin), table, FALSE, FALSE, 2);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 2);


	gtk_widget_show(scrolled_window);
	gtk_widget_show (vboxin);




	gtk_widget_show (hbox);

	gtk_widget_show (vbox);

	/* display the window */
	gtk_widget_show (window);

	gtk_main ();

	return 0;
}
