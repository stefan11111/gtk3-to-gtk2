#include <gdk/gdkscreen.h>

/**
 * gdk_screen_get_monitor_workarea:
 * @screen: a #GdkScreen
 * @monitor_num: the monitor number
 * @dest: (out) (allow-none): a #GdkRectangle to be filled with
 *     the monitor workarea
 *
 * Retrieves the #GdkRectangle representing the size and position of
 * the “work area” on a monitor within the entire screen area. The returned
 * geometry is in ”application pixels”, not in ”device pixels” (see
 * gdk_screen_get_monitor_scale_factor()).
 *
 * The work area should be considered when positioning menus and
 * similar popups, to avoid placing them below panels, docks or other
 * desktop components.
 *
 * Note that not all backends may have a concept of workarea. This
 * function will return the monitor geometry if a workarea is not
 * available, or does not apply.
 *
 * Monitor numbers start at 0. To obtain the number of monitors of
 * @screen, use gdk_screen_get_n_monitors().
 *
 * Since: 3.4
 *
 * Deprecated: 3.22: Use gdk_monitor_get_workarea() instead
 */
void
gdk_screen_get_monitor_workarea (GdkScreen    *screen,
                                 gint          monitor_num,
                                 GdkRectangle *dest)
{
  gdk_screen_get_monitor_geometry (screen, monitor_num, dest);
}
