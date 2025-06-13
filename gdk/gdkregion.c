#include <gdk/gdkregion.h>
#include <gdk/gdktypes.h>

#include <pixman.h>
#include <cairo.h>

#include <string.h>

typedef GdkSegment GdkRegionBox;

/*
 *   clip region
 */

struct _GdkRegion
{
  long size;
  long numRects;
  GdkRegionBox *rects;
  GdkRegionBox extents;
};

typedef int cairo_reference_count_t;

struct _cairo_region {
    cairo_reference_count_t ref_count;
    cairo_status_t status;

    pixman_region32_t rgn;
};

/* creates a new cairo_region_t */
cairo_region_t *
cairo_region_from_GdkRegion (const GdkRegion* reg)
{
  cairo_region_t *ret;
  pixman_region32_t *pixman_reg;

  ret = cairo_region_create_rectangles ((cairo_rectangle_int_t*)reg->rects, reg->numRects);

  pixman_reg = &ret->rgn;
  memcpy(&pixman_reg->extents, &reg->extents, sizeof(pixman_box32_t));

  pixman_reg->data->size = reg->size;

  return ret;
}

/* dst must be already allocated */
void
GdkRegion_from_cairo_region (GdkRegion *dst, const cairo_region_t *src)
{
  /* dst is not NULL */
  const pixman_region32_t *pixman_reg;
  const pixman_region32_data_t *data;
  const long *rects;

  pixman_reg = &src->rgn;
  memcpy(&dst->extents, &pixman_reg->extents, sizeof(GdkRegionBox));

  data = pixman_reg->data;
  dst->size = data->size;
  dst->numRects = data->numRects;

  rects = &data->numRects + 1; /* base of the rects array */
  memcpy(dst->rects, rects, dst->numRects * sizeof(pixman_box32_t));
}
