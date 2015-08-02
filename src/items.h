#ifndef	ITEMS_H_
#define	ITEMS_H_ 0

PPriorityQueue waiting;
PSeqQueue reseting;
DataType_pq flight_landing;
DataType_q flight_takingoff;
gint runway_down, runway_up;

GtkWidget *status_bar;
GtkWidget *waiting_label;
gchar waiting_label_text[20];
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

extern GtkWidget *make_menu_item (GtkWidget *menu, gchar *item_text,
		GCallback callbackfunc, gpointer data);

extern void about_called (GtkWidget *widget, gpointer data) ;

extern GtkWidget *make_table_with_button (gchar *label, GCallback callbackfunc);

extern void dialog_ok_clicked (GtkWidget *widget, gpointer data);

extern void dialog_cancel_clicked (GtkWidget *widget, gpointer data);

extern GtkWidget *make_entryhbox_with_label (GtkWidget *entry, gchar *label_text);

extern void new_plane_arrived (GtkWidget *widget, gpointer data);

extern void alarm_handler ();

extern gint progress_timeout2 (gpointer data);

extern gint progress_timeout3 (gpointer data);

extern void new_plane_to_land (GtkWidget *widget, gpointer data);

extern void new_plane_to_take_off (GtkWidget *widget, gpointer data);

#endif	/* items.h */
