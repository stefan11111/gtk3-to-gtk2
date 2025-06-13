#ifndef __INTERNAL_GTK_WIDGET_PRIVATE_H__
#define __INTERNAL_GTK_WIDGET_PRIVATE_H__

#include <gtk/gtkwidget.h>
#include <gtk/gtkactionmuxer.h>

G_BEGIN_DECLS

#define GTK_STATE_FLAGS_BITS 13

gboolean     gtk_widget_needs_allocate      (GtkWidget *widget);

void         gtk_widget_draw_internal       (GtkWidget *widget,
                                             cairo_t   *cr,
                                             gboolean   do_clip);


void _gtk_widget_get_preferred_size_for_size   (GtkWidget         *widget,
                                                GtkOrientation     orientation,
                                                gint               size,
                                                gint              *minimum,
                                                gint              *natural,
                                                gint              *minimum_baseline,
                                                gint              *natural_baseline);
void _gtk_widget_get_preferred_size_and_baseline(GtkWidget        *widget,
                                                GtkRequisition    *minimum_size,
                                                GtkRequisition    *natural_size,
                                                gint              *minimum_baseline,
                                                gint              *natural_baseline);
gboolean _gtk_widget_has_baseline_support (GtkWidget *widget);


void              _gtk_widget_update_parent_muxer          (GtkWidget    *widget);
GtkActionMuxer *  _gtk_widget_get_action_muxer             (GtkWidget    *widget,
                                                            gboolean      create);

GtkActionMuxer * _gtk_widget_get_parent_muxer (GtkWidget *widget,
                                               gboolean   create);


gboolean          gtk_widget_has_tick_callback             (GtkWidget *widget);

G_END_DECLS

#endif /* __INTERNAL_GTK_WIDGET_PRIVATE_H__ */
