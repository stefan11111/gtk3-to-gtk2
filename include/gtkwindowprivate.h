#ifndef __INTERNAL_GTK_WINDOW_PRIVATE_H__
#define __INTERNAL_GTK_WINDOW_PRIVATE_H__

#include <gdk/gdkpixbuf.h>

#include <gtk/gtkwindow.h>

G_BEGIN_DECLS

void            _gtk_window_toggle_maximized (GtkWindow *window);

GdkPixbuf *gtk_window_get_icon_for_size (GtkWindow *window,
                                         gint       size);

G_END_DECLS

#endif /* __INTERNAL_GTK_WINDOW_PRIVATE_H__ */
