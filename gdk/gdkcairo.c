#include "gdk.h"

/**
 * gdk_cairo_set_source_rgba:
 * @cr: a cairo context
 * @rgba: a #GdkRGBA
 *
 * Sets the specified #GdkRGBA as the source color of @cr.
 *
 * Since: 3.0
 */
void
gdk_cairo_set_source_rgba (cairo_t       *cr,
                           const GdkRGBA *rgba)
{
  g_return_if_fail (cr != NULL);
  g_return_if_fail (rgba != NULL);

  cairo_set_source_rgba (cr,
                         rgba->red,
                         rgba->green,
                         rgba->blue,
                         rgba->alpha);
}
