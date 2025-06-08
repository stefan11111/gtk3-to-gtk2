#ifndef __GDK_RGBA_H__
#define __GDK_RGBA_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gdk/gdktypes.h>

G_BEGIN_DECLS

struct _GdkRGBA
{
  gdouble red;
  gdouble green;
  gdouble blue;
  gdouble alpha;
};

#define GDK_TYPE_RGBA (gdk_rgba_get_type ())

GType     gdk_rgba_get_type  (void) G_GNUC_CONST;


GdkRGBA * gdk_rgba_copy      (const GdkRGBA *rgba);

void      gdk_rgba_free      (GdkRGBA       *rgba);


guint     gdk_rgba_hash      (gconstpointer  p);

gboolean  gdk_rgba_equal     (gconstpointer  p1,
                              gconstpointer  p2);


gboolean  gdk_rgba_parse     (GdkRGBA       *rgba,
                              const gchar   *spec);

gchar *   gdk_rgba_to_string (const GdkRGBA *rgba);

G_END_DECLS

#endif /* __GDK_RGBA_H__ */
