#include <glib.h>
#include <cairo.h>

/**
 * gdk_x11_get_parent_relative_pattern:
 *
 * Used with gdk_window_set_background_pattern() to inherit background from
 * parent window. Useful for imitating transparency when compositing is not
 * available. Otherwise behaves like a transparent pattern.
 *
 * Since: 3.24.2
 *
 * Deprecated: 3.24: Don't use this function
 **/
cairo_pattern_t *
gdk_x11_get_parent_relative_pattern (void)
{
  static cairo_pattern_t *parent_relative_pattern = NULL;

  if (G_UNLIKELY (parent_relative_pattern == NULL))
    parent_relative_pattern = cairo_pattern_create_rgba (0.0, 0.0, 0.0, 0.0);

  return parent_relative_pattern;
}
