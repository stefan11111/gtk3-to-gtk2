#include <gdk/gdkdisplay.h>
#include <gdk/gdkscreen.h>

#include "gdkmonitorprivate.h"

#ifdef X11
#include "gdkscreen-x11.h"
#endif

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

#ifdef X11
static GdkMonitor *
gdk_x11_display_get_primary_monitor (GdkDisplay *display)
{
  static GdkMonitor *monitor;
  if (monitor) {
    return monitor;
  }

  GdkScreen *screen = gdk_display_get_screen (display, /* */ 0 /* */);
  GdkScreenX11 *x11_screen = GDK_SCREEN_X11 (screen);
  GdkX11Monitor *x11_monitor;

  if (0 <= x11_screen->primary_monitor && x11_screen->primary_monitor < x11_screen->n_monitors) {
    monitor = gdk_monitor_new (display);
    x11_monitor = &x11_screen->monitors[x11_screen->primary_monitor];
    monitor->geometry = x11_monitor->geometry;
    gdk_monitor_set_size (monitor, x11_monitor->geometry.width, x11_monitor->geometry.height);
    gdk_monitor_set_manufacturer (monitor, x11_monitor->manufacturer);
    gdk_monitor_set_physical_size (monitor, x11_monitor->width_mm, x11_monitor->height_mm);
  }
  return monitor;
}
#endif

/**
 * gdk_display_get_primary_monitor:
 * @display: a #GdkDisplay
 *
 * Gets the primary monitor for the display.
 *
 * The primary monitor is considered the monitor where the “main desktop”
 * lives. While normal application windows typically allow the window
 * manager to place the windows, specialized desktop applications
 * such as panels should place themselves on the primary monitor.
 *
 * Returns: (nullable) (transfer none): the primary monitor, or %NULL if no primary
 *     monitor is configured by the user
 * Since: 3.22
 */
GdkMonitor *
gdk_display_get_primary_monitor (GdkDisplay *display)
{
#ifdef X11
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);
  return gdk_x11_display_get_primary_monitor (display);
#else
  return NULL;
#endif
}
