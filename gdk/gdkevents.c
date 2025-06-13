#include <gdk/gdkevents.h>

#include "gdkinternals.h"

/**
 * gdk_event_get_button:
 * @event: a #GdkEvent
 * @button: (out): location to store mouse button number
 *
 * Extract the button number from an event.
 *
 * Returns: %TRUE if the event delivered a button number
 *
 * Since: 3.2
 **/
gboolean
gdk_event_get_button (const GdkEvent *event,
                      guint *button)
{
  gboolean fetched = TRUE;
  guint number = 0;

  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      number = event->button.button;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (button)
    *button = number;

  return fetched;
}

/**
 * gdk_event_get_click_count:
 * @event: a #GdkEvent
 * @click_count: (out): location to store click count
 *
 * Extracts the click count from an event.
 *
 * Returns: %TRUE if the event delivered a click count
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_click_count (const GdkEvent *event,
                           guint *click_count)
{
  gboolean fetched = TRUE;
  guint number = 0;

  g_return_val_if_fail (event != NULL, FALSE);

  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      number = 1;
      break;
    case GDK_2BUTTON_PRESS:
      number = 2;
      break;
    case GDK_3BUTTON_PRESS:
      number = 3;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (click_count)
    *click_count = number;

  return fetched;
}

/**
 * gdk_event_get_keyval:
 * @event: a #GdkEvent
 * @keyval: (out): location to store the keyval
 *
 * Extracts the keyval from an event.
 *
 * Returns: %TRUE if the event delivered a key symbol
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_keyval (const GdkEvent *event,
                      guint *keyval)
{
  gboolean fetched = TRUE;
  guint number = 0;

  switch (event->type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      number = event->key.keyval;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (keyval)
    *keyval = number;

  return fetched;
}

/**
 * gdk_event_get_keycode:
 * @event: a #GdkEvent
 * @keycode: (out): location to store the keycode
 *
 * Extracts the hardware keycode from an event.
 *
 * Also see gdk_event_get_scancode().
 *
 * Returns: %TRUE if the event delivered a hardware keycode
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_keycode (const GdkEvent *event,
                       guint16 *keycode)
{
  gboolean fetched = TRUE;
  guint16 number = 0;

  switch (event->type)
    {
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      number = event->key.hardware_keycode;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (keycode)
    *keycode = number;

  return fetched;
}

/**
 * gdk_event_get_scroll_direction:
 * @event: a #GdkEvent
 * @direction: (out): location to store the scroll direction
 *
 * Extracts the scroll direction from an event.
 *
 * If @event is not of type %GDK_SCROLL, the contents of @direction
 * are undefined.
 *
 * If you wish to handle both discrete and smooth scrolling, you
 * should check the return value of this function, or of
 * gdk_event_get_scroll_deltas(); for instance:
 *
 * |[<!-- language="C" -->
 *   GdkScrollDirection direction;
 *   double vscroll_factor = 0.0;
 *   double x_scroll, y_scroll;
 *
 *   if (gdk_event_get_scroll_direction (event, &direction))
 *     {
 *       // Handle discrete scrolling with a known constant delta;
 *       const double delta = 12.0;
 *
 *       switch (direction)
 *         {
 *         case GDK_SCROLL_UP:
 *           vscroll_factor = -delta;
 *           break;
 *         case GDK_SCROLL_DOWN:
 *           vscroll_factor = delta;
 *           break;
 *         default:
 *           // no scrolling
 *           break;
 *         }
 *     }
 *   else if (gdk_event_get_scroll_deltas (event, &x_scroll, &y_scroll))
 *     {
 *       // Handle smooth scrolling directly
 *       vscroll_factor = y_scroll;
 *     }
 * ]|
 *
 * Returns: %TRUE if the event delivered a scroll direction
 *   and %FALSE otherwise
 *
 * Since: 3.2
 */
gboolean
gdk_event_get_scroll_direction (const GdkEvent *event,
                                GdkScrollDirection *direction)
{
  gboolean fetched = TRUE;
  GdkScrollDirection dir = 0;

  switch (event->type)
    {
    case GDK_SCROLL:
      dir = event->scroll.direction;
      break;
    default:
      fetched = FALSE;
      break;
    }

  if (direction)
    *direction = dir;

  return fetched;
}

/**
 * gdk_event_get_scroll_deltas:
 * @event: a #GdkEvent
 * @delta_x: (out): return location for X delta
 * @delta_y: (out): return location for Y delta
 *
 * Retrieves the scroll deltas from a #GdkEvent
 *
 * See also: gdk_event_get_scroll_direction()
 *
 * Returns: %TRUE if the event contains smooth scroll information
 *   and %FALSE otherwise
 *
 * Since: 3.4
 **/
gboolean
gdk_event_get_scroll_deltas (const GdkEvent *event,
                             gdouble        *delta_x,
                             gdouble        *delta_y)
{
  /* Not Implemented */
  return FALSE;
}

/**
 * gdk_event_is_scroll_stop_event
 * @event: a #GdkEvent
 *
 * Check whether a scroll event is a stop scroll event. Scroll sequences
 * with smooth scroll information may provide a stop scroll event once the
 * interaction with the device finishes, e.g. by lifting a finger. This
 * stop scroll event is the signal that a widget may trigger kinetic
 * scrolling based on the current velocity.
 *
 * Stop scroll events always have a a delta of 0/0.
 *
 * Returns: %TRUE if the event is a scroll stop event
 *
 * Since: 3.20
 */
gboolean
gdk_event_is_scroll_stop_event (const GdkEvent *event)
{
  /* Not Implemented */
  return FALSE;
}

/**
 * gdk_event_get_device:
 * @event: a #GdkEvent.
 *
 * If the event contains a “device” field, this function will return
 * it, else it will return %NULL.
 *
 * Returns: (nullable) (transfer none): a #GdkDevice, or %NULL.
 *
 * Since: 3.0
 **/
GdkDevice *
gdk_event_get_device (const GdkEvent *event)
{
  g_return_val_if_fail (event != NULL, NULL);

  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
      return event->motion.device;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      return event->button.device;
    case GDK_SCROLL:
      return event->scroll.device;
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      return event->proximity.device;
    default:
      break;
    }

  /* Fallback if event has no device set */
  return NULL;
}

/**
 * gdk_event_set_device:
 * @event: a #GdkEvent
 * @device: a #GdkDevice
 *
 * Sets the device for @event to @device. The event must
 * have been allocated by GTK+, for instance, by
 * gdk_event_copy().
 *
 * Since: 3.0
 **/
void
gdk_event_set_device (GdkEvent  *event,
                      GdkDevice *device)
{
  g_return_if_fail (gtk2_gdk_event_is_allocated (event));

  switch (event->type)
    {
    case GDK_MOTION_NOTIFY:
      event->motion.device = device;
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      event->button.device = device;
      break;
    case GDK_SCROLL:
      event->scroll.device = device;
      break;
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      event->proximity.device = device;
      break;
    default:
      break;
    }
}

/**
 * gdk_event_set_source_device:
 * @event: a #GdkEvent
 * @device: a #GdkDevice
 *
 * Sets the slave device for @event to @device.
 *
 * The event must have been allocated by GTK+,
 * for instance by gdk_event_copy().
 *
 * Since: 3.0
 **/
void
gdk_event_set_source_device (GdkEvent  *event,
                             GdkDevice *device)
{
  gdk_event_set_device (event, device);
}

/**
 * gdk_event_get_source_device:
 * @event: a #GdkEvent
 *
 * This function returns the hardware (slave) #GdkDevice that has
 * triggered the event, falling back to the virtual (master) device
 * (as in gdk_event_get_device()) if the event wasn’t caused by
 * interaction with a hardware device. This may happen for example
 * in synthesized crossing events after a #GdkWindow updates its
 * geometry or a grab is acquired/released.
 *
 * If the event does not contain a device field, this function will
 * return %NULL.
 *
 * Returns: (nullable) (transfer none): a #GdkDevice, or %NULL.
 *
 * Since: 3.0
 **/
GdkDevice *
gdk_event_get_source_device (const GdkEvent *event)
{
  /* Fallback to event device */
  return gdk_event_get_device (event);
}

/**
 * gdk_event_get_window:
 * @event: a #GdkEvent
 *
 * Extracts the #GdkWindow associated with an event.
 *
 * Returns: (transfer none): The #GdkWindow associated with the event
 *
 * Since: 3.10
 */
GdkWindow *
gdk_event_get_window (const GdkEvent *event)
{
  g_return_val_if_fail (event != NULL, NULL);

  return event->any.window;
}

