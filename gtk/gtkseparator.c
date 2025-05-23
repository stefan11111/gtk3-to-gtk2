#include <gtk/gtkhseparator.h>
#include <gtk/gtkvseparator.h>

/**
 * gtk_separator_new:
 * @orientation: the separator's orientation.
 *
 * Creates a new #GtkSeparator with the given orientation.
 *
 * Return value: a new #GtkSeparator.
 *
 * Since: 2.16
 **/
GtkWidget *
gtk_separator_new (GtkOrientation orientation)
{
#if 1
  return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vseparator_new() : gtk_hseparator_new();
#else
  return g_object_new (GTK_TYPE_SEPARATOR,
                       "orientation", orientation,
                       NULL);
#endif
}
