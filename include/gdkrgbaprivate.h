#ifndef __INTERNAL_GDK_RGBA_H__
#define __INTERNAL_GDK_RGBA_H__

#include <gdk/gdkcolor.h>
#include <gdk/gdkrgba.h>

G_BEGIN_DECLS

GdkRGBA GdkRGBA_from_GdkColor (const GdkColor * const restrict color);
GdkColor GdkColor_from_GdkRGBA (const GdkRGBA * const restrict rgba);

G_END_DECLS

#endif /* __INTERNAL_GDK_RGBA_H__ */
