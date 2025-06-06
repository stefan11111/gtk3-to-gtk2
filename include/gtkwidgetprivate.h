#ifndef __INTERNAL_GTK_WIDGET_PRIVATE_H__
#define __INTERNAL_GTK_WIDGET_PRIVATE_H__

G_BEGIN_DECLS

#include <gtk/gtkwidget.h>

void              _gtk_widget_update_parent_muxer          (GtkWidget    *widget);
GtkActionMuxer *  _gtk_widget_get_action_muxer             (GtkWidget    *widget,
                                                            gboolean      create);

GtkActionMuxer * _gtk_widget_get_parent_muxer (GtkWidget *widget,
                                               gboolean   create);


G_END_DECLS

#endif /* __INTERNAL_GTK_WIDGET_PRIVATE_H__ */
