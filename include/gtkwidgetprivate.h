#ifndef __INTERNAL_GTK_WIDGET_PRIVATE_H__
#define __INTERNAL_GTK_WIDGET_PRIVATE_H__

#include <gtk/gtkwidget.h>
#include <gtk/gtkactionmuxer.h>

G_BEGIN_DECLS

#define GTK_STATE_FLAGS_BITS 13

gboolean     gtk_widget_needs_allocate      (GtkWidget *widget);

void              _gtk_widget_update_parent_muxer          (GtkWidget    *widget);
GtkActionMuxer *  _gtk_widget_get_action_muxer             (GtkWidget    *widget,
                                                            gboolean      create);

GtkActionMuxer * _gtk_widget_get_parent_muxer (GtkWidget *widget,
                                               gboolean   create);


G_END_DECLS

#endif /* __INTERNAL_GTK_WIDGET_PRIVATE_H__ */
