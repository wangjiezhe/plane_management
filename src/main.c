#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include <libintl.h>
// #include <locale.h>
#include "PriorityQueue.h"
#include "Queue.h"
#include "data.h"
#include "items.h"

gint main (gint argc, gchar *argv[]) {
	GtkWidget *window;
	GtkWidget *menu_bar;
	GtkWidget *menu;
	GtkWidget *menu_item;
	GtkWidget *root_menu;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *vboxin;
	GtkWidget *scrolled_window;
	GtkWidget *align;

	guint context_id;

	waiting = create_heap (MPD);
	reseting = createEmptyQueue (APRONS+1);
	pdata2 = g_malloc (sizeof(ProgressData));
	pdata3 = g_malloc (sizeof(ProgressData));

	signal (SIGALRM, alarm_handler);

	// setlocale("LC_ALL", "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	bindtextdomain(PACKAGE, "UTF-8");
	textdomain(PACKAGE);

//	if(!g_thread_get_initialized()) g_thread_init(NULL);
//	g_thread_init();
	gdk_threads_init ();

	gdk_threads_enter ();

	/* init the GTK */
	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), _("PLANE MANAGEMENT"));
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);
	gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	g_signal_connect (G_OBJECT (window), "delete_event",
			G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT (window), "destroy",
			G_CALLBACK (gtk_main_quit), NULL);


	/* a vbox to put all things in */
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER(window), vbox);
	gtk_widget_show (vbox);

	/* init the menu */
	menu = gtk_menu_new ();

	/* create menu-items under the root-menu */
	menu_item = make_menu_item (menu, _("About"), G_CALLBACK(about_called), window);
	menu_item = make_menu_item (menu, _("Quit"), G_CALLBACK(gtk_main_quit), window);
	/* this is the root-menu and will be displayed on the menu-bar */
	root_menu = gtk_menu_item_new_with_label(_("Menu"));
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


	hbox = gtk_hbox_new (TRUE, 10);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 2);
	gtk_widget_show (hbox);



	/* module for arriving */
	vboxin = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);
	gtk_widget_show (vboxin);

	table1 = make_table_with_button (_("New plane arrived"), G_CALLBACK(new_plane_arrived));
	gtk_box_pack_start(GTK_BOX(vboxin), table1, FALSE, FALSE, 2);

	sprintf(waiting_label_text, _("Waiting: 0"));
	waiting_label = gtk_label_new (waiting_label_text);
	gtk_box_pack_start (GTK_BOX(vboxin), waiting_label, FALSE, FALSE, 2);
	gtk_widget_show (waiting_label);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 5);
	gtk_widget_show(scrolled_window);

	text1 = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER(scrolled_window), text1);
	gtk_widget_show (text1);

	gtk_text_view_set_editable (GTK_TEXT_VIEW(text1), FALSE);
	buffer1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text1));
	gtk_text_buffer_get_iter_at_offset (buffer1, &iter1, 0);
	mark1 = gtk_text_buffer_create_mark (buffer1, NULL, &iter1, TRUE);
	gtk_text_buffer_create_tag (
			buffer1, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);




	/* module for landing */
	vboxin = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);
	gtk_widget_show (vboxin);

	table2 = make_table_with_button (_("New plane to land"), G_CALLBACK(new_plane_to_land));
	gtk_box_pack_start (GTK_BOX(vboxin), table2, FALSE, FALSE, 2);

	align = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_box_pack_start (GTK_BOX(vboxin), align, FALSE, FALSE, 2);
	gtk_widget_show (align);

	pdata2->pbar = gtk_progress_bar_new ();
	gtk_container_add (GTK_CONTAINER(align), pdata2->pbar);
	gtk_widget_show (pdata2->pbar);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 5);
	gtk_widget_show(scrolled_window);

	text2 = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER(scrolled_window), text2);
	gtk_widget_show (text2);

	gtk_text_view_set_editable (GTK_TEXT_VIEW(text2), FALSE);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(text2), TRUE);
	buffer2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text2));
	gtk_text_buffer_get_iter_at_offset (buffer2, &iter2, 0);
	mark2 = gtk_text_buffer_create_mark (buffer2, NULL, &iter2, TRUE);
	gtk_text_buffer_create_tag (
			buffer2, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag (
			buffer2, "error", "foreground", "red", "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag (
			buffer2, "notify", "foreground", "green", "weight", PANGO_WEIGHT_BOLD, NULL);




	/* module for taking off */
	vboxin = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);
	gtk_widget_show (vboxin);

	table3 = make_table_with_button (_("New plane to take off"), G_CALLBACK(new_plane_to_take_off));
	gtk_widget_set_sensitive (table3, FALSE);
	gtk_box_pack_start(GTK_BOX(vboxin), table3, FALSE, FALSE, 2);

	align = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_box_pack_start (GTK_BOX(vboxin), align, FALSE, FALSE, 2);
	gtk_widget_show (align);

	pdata3->pbar = gtk_progress_bar_new ();
	gtk_container_add (GTK_CONTAINER(align), pdata3->pbar);
	gtk_widget_show (pdata3->pbar);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_window),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX(vboxin), scrolled_window, TRUE, TRUE, 5);
	gtk_widget_show(scrolled_window);

	text3 = gtk_text_view_new ();
	gtk_container_add (GTK_CONTAINER(scrolled_window), text3);
	gtk_widget_show (text3);

	gtk_text_view_set_editable (GTK_TEXT_VIEW(text3), FALSE);
	buffer3 = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text3));
	gtk_text_buffer_get_iter_at_offset (buffer3, &iter3, 0);
	mark3 = gtk_text_buffer_create_mark (buffer3, NULL, &iter3, TRUE);
	gtk_text_buffer_create_tag (
			buffer3, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag (
			buffer3, "notify", "foreground", "green", "weight", PANGO_WEIGHT_BOLD, NULL);



	/* display the window */
	gtk_widget_show (window);

	gtk_main ();
	gdk_threads_leave ();

	return 0;
}
