#include <gdk/gdkinput.h>
#include <gdk/gdkwindow.h>

/**
 * gdk_device_get_seat:
 * @device: A #GdkDevice
 *
 * Returns the #GdkSeat the device belongs to.
 *
 * Returns: (transfer none): A #GdkSeat. This memory is owned by GTK+ and
 *          must not be freed.
 *
 * Since: 3.20
 **/
GdkSeat *
gdk_device_get_seat (GdkDevice *device)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gdk_device_get_window_at_position:
 * @device: pointer #GdkDevice to query info to.
 * @win_x: (out) (allow-none): return location for the X coordinate of the device location,
 *         relative to the window origin, or %NULL.
 * @win_y: (out) (allow-none): return location for the Y coordinate of the device location,
 *         relative to the window origin, or %NULL.
 *
 * Obtains the window underneath @device, returning the location of the device in @win_x and @win_y. Returns
 * %NULL if the window tree under @device is not known to GDK (for example, belongs to another application).
 *
 * As a slave device coordinates are those of its master pointer, This
 * function may not be called on devices of type %GDK_DEVICE_TYPE_SLAVE,
 * unless there is an ongoing grab on them, see gdk_device_grab().
 *
 * Returns: (nullable) (transfer none): the #GdkWindow under the
 * device position, or %NULL.
 *
 * Since: 3.0
 **/
GdkWindow *
gdk_device_get_window_at_position (GdkDevice  *device,
                                   gint       *win_x,
                                   gint       *win_y)
{
  return gdk_window_at_pointer (win_x, win_y);
}

/**
 * gdk_device_get_window_at_position_double:
 * @device: pointer #GdkDevice to query info to.
 * @win_x: (out) (allow-none): return location for the X coordinate of the device location,
 *         relative to the window origin, or %NULL.
 * @win_y: (out) (allow-none): return location for the Y coordinate of the device location,
 *         relative to the window origin, or %NULL.
 *
 * Obtains the window underneath @device, returning the location of the device in @win_x and @win_y in
 * double precision. Returns %NULL if the window tree under @device is not known to GDK (for example,
 * belongs to another application).
 *
 * As a slave device coordinates are those of its master pointer, This
 * function may not be called on devices of type %GDK_DEVICE_TYPE_SLAVE,
 * unless there is an ongoing grab on them, see gdk_device_grab().
 *
 * Returns: (nullable) (transfer none): the #GdkWindow under the
 *   device position, or %NULL.
 *
 * Since: 3.0
 **/
GdkWindow *
gdk_device_get_window_at_position_double (GdkDevice  *device,
                                          gdouble    *win_x,
                                          gdouble    *win_y)
{
  GdkWindow *window;
  gint x, y;

  window = gdk_device_get_window_at_position (device, &x, &y);

  if (win_x) {
    *win_x = x;
  }

  if (win_y) {
    *win_y = y;
  }

  return window;
}

