#include <gdk/gdkdisplay.h>

/**
 * gdk_display_get_default_seat:
 * @display: a #GdkDisplay
 *
 * Returns the default #GdkSeat for this display.
 *
 * Returns: (transfer none): the default seat.
 *
 * Since: 3.20
 **/
GdkSeat *
gdk_display_get_default_seat (GdkDisplay *display)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gdk_display_list_seats:
 * @display: a #GdkDisplay
 *
 * Returns the list of seats known to @display.
 *
 * Returns: (transfer container) (element-type GdkSeat): the
 *          list of seats known to the #GdkDisplay
 *
 * Since: 3.20
 **/
GList *
gdk_display_list_seats (GdkDisplay *display)
{
  /* Not Implemented */
  return NULL;
}
