#ifndef __GTK_RENDER_H__
#define __GTK_RENDER_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <cairo.h>
#include <pango/pango.h>
#include <gdk/gdk.h>

#include <gtk/gtkenums.h>
#include <gtk/gtkstylecontext.h>

G_BEGIN_DECLS

void        gtk_render_layout      (GtkStyleContext     *context,
                                    cairo_t             *cr,
                                    gdouble              x,
                                    gdouble              y,
                                    PangoLayout         *layout);
G_END_DECLS

#endif /* __GTK_RENDER_H__ */
