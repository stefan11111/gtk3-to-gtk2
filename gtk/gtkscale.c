#include <gtk/gtkhscale.h>
#include <gtk/gtkvscale.h>

/**
 * gtk_scale_new:
 * @orientation: the scale’s orientation.
 * @adjustment: (nullable): the #GtkAdjustment which sets the range
 *              of the scale, or %NULL to create a new adjustment.
 *
 * Creates a new #GtkScale.
 *
 * Returns: a new #GtkScale
 *
 * Since: 3.0
 **/
GtkWidget *
gtk_scale_new (GtkOrientation  orientation,
               GtkAdjustment  *adjustment)
{
#if 1
  return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vscale_new(adjustment) : gtk_hscale_new(adjustment);
#else
  return g_object_new (GTK_TYPE_SCALE,
                       "orientation", orientation,
                       "adjustment",  adjustment,
                       NULL);
#endif
}

/**
 * gtk_scale_new_with_range:
 * @orientation: the scale’s orientation.
 * @min: minimum value
 * @max: maximum value
 * @step: step increment (tick size) used with keyboard shortcuts
 *
 * Creates a new scale widget with the given orientation that lets the
 * user input a number between @min and @max (including @min and @max)
 * with the increment @step.  @step must be nonzero; it’s the distance
 * the slider moves when using the arrow keys to adjust the scale
 * value.
 *
 * Note that the way in which the precision is derived works best if @step
 * is a power of ten. If the resulting precision is not suitable for your
 * needs, use gtk_scale_set_digits() to correct it.
 *
 * Returns: a new #GtkScale
 *
 * Since: 3.0
 */
GtkWidget *
gtk_scale_new_with_range (GtkOrientation orientation,
                          gdouble        min,
                          gdouble        max,
                          gdouble        step)
{
  return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vscale_new_with_range(min, max, step) :
                                                    gtk_hscale_new_with_range(min, max, step);
}

/**
 * gtk_scale_set_has_origin:
 * @scale: a #GtkScale
 * @has_origin: %TRUE if the scale has an origin
 *
 * If #GtkScale:has-origin is set to %TRUE (the default), the scale will
 * highlight the part of the trough between the origin (bottom or left side)
 * and the current value.
 *
 * Since: 3.4
 */
void
gtk_scale_set_has_origin (GtkScale *scale,
                          gboolean  has_origin)
{
  /* Not Implemented */
}

/**
 * gtk_scale_get_has_origin:
 * @scale: a #GtkScale
 *
 * Returns whether the scale has an origin.
 *
 * Returns: %TRUE if the scale has an origin.
 *
 * Since: 3.4
 */
gboolean
gtk_scale_get_has_origin (GtkScale *scale)
{
  /* Not Implemented */
  return FALSE;
}

