#ifndef __INTERNAL_GDK_CAIRO_H__
#define __INTERNAL_GDK_CAIRO_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdk.h> can be included directly."
#endif

#include "gdkrgba.h"

G_BEGIN_DECLS

void       gdk_cairo_set_source_rgba    (cairo_t              *cr,
                                         const GdkRGBA        *rgba);

G_END_DECLS

#endif /* __INTERNAL_GDK_CAIRO_H__ */
