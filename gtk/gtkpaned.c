#include <gtk/gtkvpaned.h>
#include <gtk/gtkhpaned.h>

/**
 * gtk_paned_new:
 * @orientation: the paned’s orientation.
 *
 * Creates a new #GtkPaned widget.
 *
 * Returns: a new #GtkPaned.
 *
 * Since: 3.0
 **/
GtkWidget *
gtk_paned_new (GtkOrientation orientation)
{
#if 0
  return g_object_new (GTK_TYPE_PANED,
                       "orientation", orientation,
                       NULL);
#else
  return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vpaned_new() : gtk_hpaned_new();
#endif
}

/**
 * gtk_paned_set_wide_handle:
 * @paned: a #GtkPaned
 * @wide: the new value for the #GtkPaned:wide-handle property
 *
 * Sets the #GtkPaned:wide-handle property.
 *
 * Since: 3.16
 */
void
gtk_paned_set_wide_handle (GtkPaned *paned,
                           gboolean  wide)
{
  /* Not Implemented */
}

/**
 * gtk_paned_get_wide_handle:
 * @paned: a #GtkPaned
 *
 * Gets the #GtkPaned:wide-handle property.
 *
 * Returns: %TRUE if the paned should have a wide handle
 *
 * Since: 3.16
 */
gboolean
gtk_paned_get_wide_handle (GtkPaned *paned)
{
  /* Not Implemented */
  return FALSE;
}

