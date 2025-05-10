#ifndef __GDK_RGBA_H__
#define __GDK_RGBA_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
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

G_END_DECLS

#endif /* __GDK_RGBA_H__ */
