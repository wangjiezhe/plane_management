#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdlib.h>
#include <string.h>
// #include <libintl.h>
// #include <locale.h>
#include "PriorityQueue.h"
#include "Queue.h"
#include "data.h"
#include "items.h"

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
		N_("Wang Jiezhe")
	};
	about_dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name (
			GTK_ABOUT_DIALOG(about_dialog), _("PLANE MANAGEMENT"));
	gtk_about_dialog_set_version (
			GTK_ABOUT_DIALOG(about_dialog), "1.1.1");
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

	gtk_text_buffer_insert (buffer1, &iter1, _("\nNew plane added into waiting queue."), -1);
	gtk_text_buffer_insert (buffer1, &iter1, _("\nFlight No.: "), -1);
	gtk_text_buffer_insert_with_tags_by_name (buffer1, &iter1, entry1_text, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer1, &iter1, _("\nRemaining oil(/L): "), -1);
	gtk_text_buffer_insert_with_tags_by_name (buffer1, &iter1, entry2_text, -1, "bold", NULL);
	gtk_text_buffer_insert (buffer1, &iter1, "\n", -1);
	sprintf (waiting_label_text, _("Waiting: %d"), waiting->n);
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
	gtk_window_set_title (GTK_WINDOW(dialog), _("New Plane"));
	gtk_window_set_modal (GTK_WINDOW(dialog), TRUE);
	button = gtk_button_new_with_label (_("Ok"));
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(dialog_ok_clicked), dialog);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
			button, TRUE, TRUE, 0);
	gtk_widget_show (button);
	button = gtk_button_new_with_label (_("Cancel"));
	g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(dialog_cancel_clicked), dialog);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->action_area),
			button, TRUE, TRUE, 0);
	gtk_widget_show (button);

	entry1 = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY(entry1), 6);
	entryhbox = make_entryhbox_with_label (entry1, _("Flight No. :"));
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), entryhbox, TRUE, TRUE, 2);

	entry2 = gtk_entry_new ();
	entryhbox = make_entryhbox_with_label (entry2, _("Oil (/L) :"));
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), entryhbox, TRUE, TRUE, 2);

	gtk_widget_show (dialog);

	return ;
}

void alarm_handler () {
	DataType_q temp;
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, _("\nA new plane is ready to take off"), -1, "notify", NULL);
	gtk_text_buffer_insert_with_tags_by_name (
			buffer3, &iter3, _("\nFlight No.: "), -1, "notify", NULL);
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
//	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar))+ 0.5;	/* testing */
	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 1/((t_d+t_db)*60);
	if (new_val >= 1.0) {
		new_val = 0.0;
		g_source_remove (pdata->timer);
		DataType_q temp;
		strcpy (temp.flight_name, flight_landing.flight_name);
		temp.landing_time = time(NULL);
		enQueue(reseting, temp);
		runway_down = 0;
		gtk_text_buffer_insert_with_tags_by_name (
				buffer2, &iter2, _("\nSucceed!\n "), -1, "notify", NULL);
		gtk_text_buffer_move_mark (buffer2, mark2, &iter2);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text2), mark2, 0.0, TRUE, 0.0, 1.0);
		if (isOnlyOneInQueue(reseting))
//			alarm(5);	/* testing */
			alarm(t_r);
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
//	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 0.5;	/* testing */
	new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR(pdata->pbar)) + 1/((t_u+t_ub)*60);
	if (new_val >= 1.0) {
		new_val = 0.0;
		g_source_remove (pdata->timer);
		runway_up = 0;
		gtk_text_buffer_insert_with_tags_by_name (
				buffer3, &iter3, _("\nSucceed!\n "), -1, "notify", NULL);
		gtk_text_buffer_move_mark (buffer3, mark3, &iter3);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text3), mark3, 0.0, TRUE, 0.0, 1.0);
		if (!isEmptyQueue(reseting)) {
//			alarm (5);	/* testing */
			temp = frontQueue(reseting);
			alarm (t_r - (time(NULL) - temp.landing_time));
			}
		}
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(pdata->pbar), new_val);
	return TRUE;
}

/* callback function for button "Ok to land" */
void new_plane_to_land (GtkWidget *widget, gpointer data) {
	if (isEmpty_heap(waiting)) {
		gtk_text_buffer_insert_with_tags_by_name (
				buffer2, &iter2, _("\nNo plane is waiting for landing.\n"), -1, "error", NULL);
		gtk_text_buffer_move_mark (buffer2, mark2, &iter2);
		gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW(text2), mark2, 0.0, TRUE, 0.0, 1.0);
		return ;
	}
	runway_down = 1;
	flight_landing = waiting->pq[0];
	removeMin_heap (waiting);
	sprintf (waiting_label_text, _("Waiting: %d"), waiting->n);
	gtk_label_set_text(GTK_LABEL(waiting_label), waiting_label_text);
	gtk_text_buffer_insert (buffer2, &iter2, _("\nA new plane is landing..."), -1);
	gtk_text_buffer_insert (buffer2, &iter2, _("\nFlight No. :"), -1);
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
	gtk_text_buffer_insert (buffer3, &iter3, _("\nA new plane is taking off..."), -1);
	gtk_text_buffer_insert (buffer3, &iter3, _("\nFlight No.: "), -1);
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
