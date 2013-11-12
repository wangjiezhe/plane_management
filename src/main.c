
#include <gtk/gtk.h>
//#include "gtkitems.h"

GtkWidget *status_bar;

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
	return;
}

void dialog_cancel_clicked (GtkWidget *widget, gpointer data) {
	gtk_widget_destroy(GTK_WIDGET(data));
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
	GtkWidget *entry1;
	GtkWidget *entry2;
	GtkWidget *entryhbox;

	dialog = gtk_dialog_new();
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

	/* init the GTK */
	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "PLANE MANAGEMENT");
//	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	g_signal_connect (G_OBJECT (window), "delete_event",
			G_CALLBACK (gtk_main_quit), NULL);
//	g_signal_connect (G_OBJECT (window), "destory",
//			G_CALLBACK (gtk_main_quit), NULL);


	/* a vbox to put all things in */
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(window), vbox);


	/* init the menu */
	menu = gtk_menu_new ();

	/* create menu-items under the root-menu */
	menu_item = gtk_menu_item_new_with_label ("Quit");
	gtk_menu_shell_append (GTK_MENU_SHELL(menu), menu_item);
	g_signal_connect (G_OBJECT(menu_item), "activate",
			G_CALLBACK(gtk_main_quit), G_OBJECT(window));
	gtk_widget_show (menu_item);

	/* this is the root-menu and will be displayed on the menu-bar */
	root_menu = gtk_menu_item_new_with_label("Menu");
	gtk_widget_show (root_menu);

	/* menu we created is the menu for the root-menu */
	gtk_menu_item_set_submenu (GTK_MENU_ITEM(root_menu), menu);
	gtk_widget_show (menu);

	/* create a menu_bar to hold the menus and add it to the main window */
	menu_bar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX(vbox), menu_bar, FALSE, FALSE, 2);
	gtk_widget_show (menu_bar);

	/* append the menu-item to the menu-bar */
	gtk_menu_shell_append (GTK_MENU_SHELL(menu_bar), root_menu);


	/* create a status bar (optional) */
	status_bar = gtk_statusbar_new ();
	gtk_box_pack_end (GTK_BOX(vbox), status_bar, FALSE, FALSE, 0);
	gtk_widget_show (status_bar);
	context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR(status_bar), "status");


	hbox = gtk_hbox_new (TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, FALSE, 2);




	/* module for arriving */
	vboxin = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);

	table = make_table_with_button ("New plane arrived", G_CALLBACK(new_plane_arrived));
	gtk_box_pack_start(GTK_BOX(vboxin), table, FALSE, FALSE, 2);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 2);


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
