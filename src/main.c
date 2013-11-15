
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "PriorityQueue.h"
#include "Queue.h"
#include "data.h"
//#include "gtkitems.h"

PPriorityQueue waiting;
PSeqQueue reseting;
DataType_pq flight_landing;
DataType_q flight_takingoff;
gint runway_down, runway_up;

GtkWidget *status_bar;
GtkWidget *waiting_label;
gchar waiting_label_text[20];
//GtkWidget *button1, *button2, *button3;
GtkWidget *table1, *table2, *table3;
GtkWidget *entry1, *entry2;
GTimer *dialog_timer;
GtkWidget *text1, *text2, *text3;
GtkTextBuffer *buffer1, *buffer2, *buffer3;
GtkTextIter iter1, iter2, iter3;
GtkTextMark *mark1, *mark2, *mark3;

typedef struct _ProgressData {
	GtkWidget *pbar;
	int timer;
} ProgressData;

ProgressData *pdata2, *pdata3;

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
			GTK_ABOUT_DIALOG(about_dialog), "0.1.0");
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
	table = gtk_table_new (1, 5, TRUE);
	button = gtk_button_new_with_label (label);
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(callbackfunc), NULL);
	gtk_table_attach_defaults (GTK_TABLE(table), button, 1, 4, 0, 1);
	gtk_widget_show(button);
	gtk_widget_show (table);
	return table;
}

void dialog_ok_clicked (GtkWidget *widget, gpointer data) {
	gchar oil_text[7];
	DataType_pq flight;
	gdouble remaining_time;
	gdouble dialog_time;
//	gchar num[4];
	const gchar *entry1_text = g_utf8_strup (
			g_locale_to_utf8(gtk_entry_get_text(GTK_ENTRY(entry1)), -1, NULL, NULL, NULL), -1);
	const gchar *entry2_text =
			g_locale_to_utf8 (gtk_entry_get_text(GTK_ENTRY(entry2)), -1, NULL, NULL, NULL);
	if (strlen(entry1_text) == 0 || strlen(entry2_text) == 0) {
//		dialog_input_error();
		return ;
	}
	gtk_widget_destroy (GTK_WIDGET(data));

	strcpy (flight.flight_name, entry1_text);
	strcpy (oil_text, entry2_text);
	remaining_time = atoi(oil_text) / OPH;
	dialog_time = g_timer_elapsed (dialog_timer, NULL);
	flight.landing_deadline =
		time(NULL) + (time_t)(remaining_time * 3600) - (time_t)dialog_time;
	g_timer_stop (dialog_timer);
	g_timer_destroy (dialog_timer);
	add_heap (waiting, flight);

	gtk_text_buffer_insert (buffer1, &iter1, "\nNew plane added into waiting queue.", -1);
	gtk_text_buffer_insert (buffer1, &iter1, "\nFlight No.: ", -1);
	gtk_text_buffer_insert_with_tags_by_name (buffer1, &iter1, entry1_text, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer1, &iter1, "\nRemaining oil(/L): ", -1);
	gtk_text_buffer_insert_with_tags_by_name (buffer1, &iter1, entry2_text, -1, "bold", NULL);
//	gtk_text_buffer_insert (buffer1, &iter1, "\nPlanes waiting to land: ", -1);
//	sprintf(num, "%d", waiting->n);
//	gtk_text_buffer_insert_with_tags_by_name (buffer1, &iter1, num, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer1, &iter1, "\n", -1);
	sprintf (waiting_label_text, "Waiting: %d", waiting->n);
	gtk_label_set_text(GTK_LABEL(waiting_label), waiting_label_text);
	gtk_text_buffer_move_mark (buffer1, mark1, &iter1);
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text1), mark1, 0.0, TRUE, 0.0, 1.0);

	return ;
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

	return ;
}

void alarm_handler () {
	DataType_q temp;
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, "\nA new plane is ready to take off", -1, "notify", NULL);
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, "\nFlight No.: ", -1, "notify", NULL);
	temp = frontQueue(reseting);
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, temp.flight_name, -1, "notify", NULL);
	gtk_text_buffer_insert (buffer3, &iter3, "\n", -1);
	gtk_text_buffer_move_mark (buffer3, mark3, &iter3);
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text3), mark3, 0.0, TRUE, 0.0, 1.0);
	if (runway_up == 0 && !isEmptyQueue(reseting))
		gtk_widget_set_sensitive (table3, TRUE);
}

gint progress_timeout2 (gpointer data) {
	ProgressData *pdata = (ProgressData *)data;
	gdouble new_val;
	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar))+ 0.5;	/* testing */
//	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 1/((t_d+t_db)*60);
	if (new_val >= 1.0) {
		new_val = 0.0;
		g_source_remove (pdata->timer);
		DataType_q temp;
		strcpy (temp.flight_name, flight_landing.flight_name);
		temp.landing_time = time(NULL);
		enQueue(reseting, temp);
		runway_down = 0;
		gtk_text_buffer_insert_with_tags_by_name (
				buffer2, &iter2, "\nSucceed!\n ", -1, "notify", NULL);
//		gtk_text_buffer_insert_with_tags_by_name (
//				buffer2, &iter2, temp.flight_name, -1, "notify", NULL);
//		gtk_text_buffer_insert_with_tags_by_name (
//				buffer2, &iter2, " has successfully landed.\n", -1, "notify", NULL);
		gtk_text_buffer_move_mark (buffer2, mark2, &iter2);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text2), mark2, 0.0, TRUE, 0.0, 1.0);
		if (isOnlyOneInQueue(reseting))
			alarm(5);	/* testing */
//			alarm(t_r);
		if (!isFullQueue(reseting))
			gtk_widget_set_sensitive (table2, TRUE);
	}
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(pdata->pbar), new_val);
	return TRUE;
}

gint progress_timeout3 (gpointer data) {
	ProgressData *pdata = (ProgressData *)data;
	gdouble new_val;
	DataType_q temp;
	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 0.5;	/* testing */
//	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 1/((t_u+t_ub)*60);
	if (new_val >= 1.0) {
		new_val = 0.0;
		g_source_remove (pdata->timer);
		runway_up = 0;
		gtk_text_buffer_insert_with_tags_by_name (
				buffer3, &iter3, "\nSucceed!\n ", -1, "notify", NULL);
//		gtk_text_buffer_insert_with_tags_by_name (
//				buffer3, &iter3, temp.flight_name, -1, "notify", NULL);
//		gtk_text_buffer_insert_with_tags_by_name (
//				buffer3, &iter3, " has successfully taken off.\n", -1, "notify", NULL);
		gtk_text_buffer_move_mark (buffer3, mark3, &iter3);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text3), mark3, 0.0, TRUE, 0.0, 1.0);
		if (!isEmptyQueue(reseting)) {
			alarm (5);	/* testing */
//			temp = frontQueue(reseting);
//			alarm (t_r - (time(NULL) - temp.landing_time);
			}
		}
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(pdata->pbar), new_val);
	return TRUE;
}

/* callback function for button "Ok to land" */
void new_plane_to_land (GtkWidget *widget, gpointer data) {
//	way_down->status = 1;
//	way_down->last_time = time(NULL);
	if (isEmpty_heap(waiting)) {
		gtk_text_buffer_insert_with_tags_by_name (
				buffer2, &iter2, "\nNo plane is waiting for landing.\n", -1, "error", NULL);
		gtk_text_buffer_move_mark (buffer2, mark2, &iter2);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text2), mark2, 0.0, TRUE, 0.0, 1.0);
		return ;
	}
	runway_down = 1;
	flight_landing = waiting->pq[0];
	removeMin_heap (waiting);
	sprintf (waiting_label_text, "Waiting: %d", waiting->n);
	gtk_label_set_text(GTK_LABEL(waiting_label), waiting_label_text);
	gtk_text_buffer_insert (buffer2, &iter2, "\nA new plane is landing...", -1);
	gtk_text_buffer_insert (buffer2, &iter2, "\nFlight No. :", -1);
	gtk_text_buffer_insert_with_tags_by_name (
			buffer2, &iter2, flight_landing.flight_name, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer2, &iter2, "\n", -1);
	gtk_text_buffer_move_mark (buffer2, mark2, &iter2);
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text2), mark2, 0.0, TRUE, 0.0, 1.0);
	pdata2->timer = g_timeout_add (1000, progress_timeout2, pdata2);
	gtk_widget_set_sensitive (table2, FALSE);
	return ;
}

/* callback function for button "Ok to take off" */
void new_plane_to_take_off (GtkWidget *widget, gpointer data) {
	runway_up = 1;
	flight_takingoff = frontQueue (reseting);
	deQueue (reseting);
	gtk_text_buffer_insert (buffer3, &iter3, "\nA new plane is taking off...", -1);
	gtk_text_buffer_insert (buffer3, &iter3, "\nFlight No.: ", -1);
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, flight_takingoff.flight_name, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer3, &iter3, "\n", -1);
	gtk_text_buffer_move_mark (buffer3, mark3, &iter3);
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text3), mark3, 0.0, TRUE, 0.0, 1.0);
	pdata3->timer = g_timeout_add (1000, progress_timeout3, pdata3);
	gtk_widget_set_sensitive (table3, FALSE);
	if (runway_down == 0)
		gtk_widget_set_sensitive (table2, TRUE);
	return ;
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
//	GtkWidget *table;
	GtkWidget *align;

	guint context_id;

	waiting = create_heap (MPD);
	reseting = createEmptyQueue (APRONS+1);
	pdata2 = g_malloc (sizeof(ProgressData));
	pdata3 = g_malloc (sizeof(ProgressData));

	signal (SIGALRM, alarm_handler);

//	if(!g_thread_get_initialized()) g_thread_init(NULL);
//	g_thread_init();
	gdk_threads_init ();

	gdk_threads_enter ();

	/* init the GTK */
	gtk_init (&argc, &argv);

	/* create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "PLANE MANAGEMENT");
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


	hbox = gtk_hbox_new (TRUE, 10);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, TRUE, TRUE, 2);




	/* module for arriving */
	vboxin = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX(hbox), vboxin, TRUE, TRUE, 2);
	gtk_widget_show (vboxin);

	table1 = make_table_with_button ("New plane arrived", G_CALLBACK(new_plane_arrived));
	gtk_box_pack_start(GTK_BOX(vboxin), table1, FALSE, FALSE, 2);

	sprintf(waiting_label_text, "Waiting: 0");
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

	table2 = make_table_with_button ("New plane to land", G_CALLBACK(new_plane_to_land));
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

	table3 = make_table_with_button ("New plane to take off", G_CALLBACK(new_plane_to_take_off));
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






	gtk_widget_show (hbox);

	gtk_widget_show (vbox);

	/* display the window */
	gtk_widget_show (window);

	gtk_main ();
	gdk_threads_leave ();

	return 0;
}
