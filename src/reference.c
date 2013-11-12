gulong g_signal_connect( gpointer *object,
                         const gchar *name,
                         GCallback func,
                         gpointer func_data );

void callback_func( GtkWidget *widget,
                    gpointer callback_data );


gulong g_signal_connect_swapped( gpointer *object,
                                 const gchar *name,
                                 GCallback func,
                                 gpointer *slot_object );

void callback_func( GtkObject *object );


gint callback_func( GtkWidget *widget,
                    GdkEvent *event,
                    gpointer callback_data );


void g_signal_handler_disconnect( gpointer object,
                                  gulong id );


void g_signal_handler_block( gpointer object,
                             gulong id );

void g_signal_handlers_block_by_func( gpointer object,
                                      GCallback func,
                                      gpointer data );

void g_signal_handler_unblock( gpointer object,
                               gulong id );

void g_signal_handlers_unblock_by_func( gpointer object,
                                        GCallback func,
                                        gpointer data );


void gtk_box_pack_start( GtkBox *box,
                         GtkWidget *child,
						 gboolean expand,
						 gboolean fill,
						 guint padding );


GtkWidget *gtk_hbox_new ( gboolean homogeneous,
		                  gint spacing );


GtkWidget *gtk_table_new( guint rows,
                          guint columns,
						  gboolean homogeneous );



