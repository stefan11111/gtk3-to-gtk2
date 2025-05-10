#ifndef __INTERNAL_GDK_RGBA_H__
#define __INTERNAL_GDK_RGBA_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdk.h> can be included directly."
#endif

#include <gdk/gdk.h>

G_BEGIN_DECLS

typedef struct _GdkRGBA GdkRGBA;

struct _GdkRGBA
{
  gdouble red;
  gdouble green;
  gdouble blue;
  gdouble alpha;
};

GdkRGBA GdkRGBA_from_GdkColor (const GdkColor * const restrict color);
GdkColor GdkColor_from_GdkRGBA (const GdkRGBA * const restrict rgba);

G_END_DECLS

#endif /* __INTERNAL_GDK_RGBA_H__ */
