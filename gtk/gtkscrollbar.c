#include <gtk/gtkvscrollbar.h>
#include <gtk/gtkhscrollbar.h>

/**
 * gtk_scrollbar_new:
 * @orientation: the scrollbar’s orientation.
 * @adjustment: (allow-none): the #GtkAdjustment to use, or %NULL to create a new adjustment.
 *
 * Creates a new scrollbar with the given orientation.
 *
 * Returns:  the new #GtkScrollbar.
 *
 * Since: 3.0
 **/
GtkWidget *
gtk_scrollbar_new (GtkOrientation  orientation,
                   GtkAdjustment  *adjustment)
{
#if 1
return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vscrollbar_new(adjustment) : gtk_hscrollbar_new (adjustment);
#else
  return gtk_scrollbar_new (orientation, adjustment);
#endif
}
