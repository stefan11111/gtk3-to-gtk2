#include <gdk/gdkrectangle.h>

/**
 * gdk_rectangle_equal:
 * @rect1: a #GdkRectangle
 * @rect2: a #GdkRectangle
 *
 * Checks if the two given rectangles are equal.
 *
 * Returns: %TRUE if the rectangles are equal.
 *
 * Since: 3.20
 */
gboolean
gdk_rectangle_equal (const GdkRectangle *rect1,
                     const GdkRectangle *rect2)
{
  return rect1->x == rect2->x
      && rect1->y == rect2->y
      && rect1->width == rect2->width
      && rect1->height == rect2->height;
}
