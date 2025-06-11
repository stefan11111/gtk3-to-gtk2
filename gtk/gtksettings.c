#include <gtk/gtksettings.h>
#include <gdk/gdkscreen.h>
#include "gtkstylecascadeprivate.h"

GdkScreen *
_gtk_settings_get_screen (GtkSettings *settings)
{
  /* breaks GSEAL */
  return settings->screen;
}

GtkStyleCascade *
_gtk_settings_get_style_cascade (GtkSettings *settings,
                                 gint         scale)
{
  static GtkStyleCascade *ret;
  if (!ret) { /* might 'leak' memory for one GtkStyleCascade */
    ret = _gtk_style_cascade_new ();
  }
  return ret;
}
