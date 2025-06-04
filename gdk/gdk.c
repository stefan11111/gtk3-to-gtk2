#include <gdk/gdk.h>

#include <fribidi.h>

#include "util.h"

#define gdk_initialized (*gtk2_gdk_initialized())

/**
 * gdk_disable_multidevice:
 *
 * Disables multidevice support in GDK. This call must happen prior
 * to gdk_display_open(), gtk_init(), gtk_init_with_args() or
 * gtk_init_check() in order to take effect.
 *
 * Most common GTK+ applications won’t ever need to call this. Only
 * applications that do mixed GDK/Xlib calls could want to disable
 * multidevice support if such Xlib code deals with input devices in
 * any way and doesn’t observe the presence of XInput 2.
 *
 * Since: 3.0
 */
void
gdk_disable_multidevice (void)
{
  if (gdk_initialized)
    return;

  _gdk_disable_multidevice = TRUE;
}

PangoDirection
gdk_unichar_direction (gunichar ch)
{
  FriBidiCharType fribidi_ch_type;

  G_STATIC_ASSERT (sizeof (FriBidiChar) == sizeof (gunichar));

  fribidi_ch_type = fribidi_get_bidi_type (ch);

  if (!FRIBIDI_IS_STRONG (fribidi_ch_type))
    return PANGO_DIRECTION_NEUTRAL;
  else if (FRIBIDI_IS_RTL (fribidi_ch_type))
    return PANGO_DIRECTION_RTL;
  else
    return PANGO_DIRECTION_LTR;
}

#if defined (G_HAS_CONSTRUCTORS) && !defined (G_OS_WIN32)
#define GDK_USE_CONSTRUCTORS
#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(stash_startup_id)
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(stash_autostart_id)
#endif
G_DEFINE_CONSTRUCTOR(stash_startup_id)
G_DEFINE_CONSTRUCTOR(stash_autostart_id)
#endif

static char *desktop_startup_id = NULL;
static char *desktop_autostart_id = NULL;

static void
stash_startup_id (void)
{
  const char *startup_id = g_getenv ("DESKTOP_STARTUP_ID");

  if (startup_id == NULL || startup_id[0] == '\0')
    return;

  if (!g_utf8_validate (startup_id, -1, NULL))
    {
      g_warning ("DESKTOP_STARTUP_ID contains invalid UTF-8");
      return;
    }

  desktop_startup_id = g_strdup (startup_id);
}

static void
stash_autostart_id (void)
{
  const char *autostart_id = g_getenv ("DESKTOP_AUTOSTART_ID");
  desktop_autostart_id = g_strdup (autostart_id ? autostart_id : "");
}

const gchar *
gdk_get_desktop_startup_id (void)
{
  static gsize init = 0;

  if (g_once_init_enter (&init))
    {
#ifndef GDK_USE_CONSTRUCTORS
      stash_startup_id ();
#endif
      /* Clear the environment variable so it won't be inherited by
       * child processes and confuse things.
       */
      g_unsetenv ("DESKTOP_STARTUP_ID");

      g_once_init_leave (&init, 1);
    }

  return desktop_startup_id;
}

const gchar *
gdk_get_desktop_autostart_id (void)
{
  static gsize init = 0;

  if (g_once_init_enter (&init))
    {
#ifndef GDK_USE_CONSTRUCTORS
      stash_autostart_id ();
#endif
      /* Clear the environment variable so it won't be inherited by
       * child processes and confuse things.
       */
      g_unsetenv ("DESKTOP_AUTOSTART_ID");

      g_once_init_leave (&init, 1);
    }

  return desktop_autostart_id;
}
