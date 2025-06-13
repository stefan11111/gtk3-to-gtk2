#ifndef __INTERNAL_GDK_REGION_PRVATE_H__
#define __INTERNAL_GDK_REGION_PRVATE_H__

#include <gdk/gdkregion.h>

#include <cairo.h>

G_BEGIN_DECLS

cairo_region_t *cairo_region_from_GdkRegion (const GdkRegion* reg);
void GdkRegion_from_cairo_region (GdkRegion *dst, const cairo_region_t *src);

G_END_DECLS

#endif /* __INTERNAL_GDK_REGION_PRVATE_H__ */
