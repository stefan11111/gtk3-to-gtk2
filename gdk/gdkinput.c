#include <gdk/gdkinput.h>

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
