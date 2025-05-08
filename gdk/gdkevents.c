#include <gdk/gdk.h>
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

