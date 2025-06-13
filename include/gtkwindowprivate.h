#ifndef __INTERNAL_GTK_WINDOW_PRIVATE_H__
#define __INTERNAL_GTK_WINDOW_PRIVATE_H__

#include <gdk/gdkpixbuf.h>

#include <gtk/gtkwindow.h>

G_BEGIN_DECLS

void            _gtk_window_toggle_maximized (GtkWindow *window);

GdkPixbuf *gtk_window_get_icon_for_size (GtkWindow *window,
                                         gint       size);


void            _gtk_window_set_allocation         (GtkWindow           *window,
                                                    const GtkAllocation *allocation,
                                                    GtkAllocation       *allocation_out);


gboolean        _gtk_window_titlebar_shows_app_menu (GtkWindow *window);

void            _gtk_window_get_shadow_width (GtkWindow *window,
                                              GtkBorder *border);

G_END_DECLS

#endif /* __INTERNAL_GTK_WINDOW_PRIVATE_H__ */
