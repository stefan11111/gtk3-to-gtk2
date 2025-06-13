#ifndef __INTERNAL_GTK_GLOBALS_H__
#define __INTERNAL_GTK_GLOBALS_H__

#include <gtk/gtktypes.h>
#include <gtk/gtkenums.h>
#include <gdk/gdktypes.h>

G_BEGIN_DECLS

/* variables that mark if we initialized a lib or not */
extern gsize gtk2_loaded;
extern gsize glib_g_object_loaded;

/* init functions */
void gtk2_init(void);
void glib_g_object_init (void);

/* gtk2 symbols */
extern void (*gtk2_gtk_draw_insertion_cursor)(GtkWidget          *widget,
                                              GdkDrawable        *drawable,
                                              const GdkRectangle *area,
                                              const GdkRectangle *location,
                                              gboolean            is_primary,
                                              GtkTextDirection    direction,
                                              gboolean            draw_arrow);

extern void (*gtk2_gtk_widget_draw) (GtkWidget          *widget,
                                     const GdkRectangle *area);


/* glib-gobject symbols */
extern gpointer (*glib_g_object_ref) (gpointer object);
extern void (*glib_g_object_unref) (gpointer object);

G_END_DECLS

#endif /* __INTERNAL_GTK_GLOBALS_H__ */
