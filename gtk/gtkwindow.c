#include <gtk/gtkwindow.h>
#include <gtk/gtkwidget.h>
#include <gtk/gtkactionmuxer.h>

#include "util.h"

#ifndef P_
#define P_(x) x
#endif

enum {
  PROP_0,

  /* Construct */
  PROP_TYPE,

  /* Normal Props */
  PROP_TITLE,
  PROP_ROLE,
  PROP_ALLOW_SHRINK,
  PROP_ALLOW_GROW,
  PROP_RESIZABLE,
  PROP_MODAL,
  PROP_WIN_POS,
  PROP_DEFAULT_WIDTH,
  PROP_DEFAULT_HEIGHT,
  PROP_DESTROY_WITH_PARENT,
  PROP_ICON,
  PROP_ICON_NAME,
  PROP_SCREEN,
  PROP_TYPE_HINT,
  PROP_SKIP_TASKBAR_HINT,
  PROP_SKIP_PAGER_HINT,
  PROP_URGENCY_HINT,
  PROP_ACCEPT_FOCUS,
  PROP_FOCUS_ON_MAP,
  PROP_DECORATED,
  PROP_DELETABLE,
  PROP_GRAVITY,
  PROP_TRANSIENT_FOR,
  PROP_OPACITY,

  /* Readonly properties */
  PROP_IS_ACTIVE,
  PROP_HAS_TOPLEVEL_FOCUS,

  /* Writeonly properties */
  PROP_STARTUP_ID,

  PROP_MNEMONICS_VISIBLE,

  /* XXX ^^^ gtk2 props ^^^ XXX */

  /* XXX vvv extra props from gtk3 XXX */

  PROP_APPLICATION,

  LAST_ARG
};

static GParamSpec* prop_application = 0;

/**
 * gtk_window_get_application:
 * @window: a #GtkWindow
 *
 * Gets the #GtkApplication associated with the window (if any).
 *
 * Returns: (nullable) (transfer none): a #GtkApplication, or %NULL
 *
 * Since: 3.0
 **/
GtkApplication *
gtk_window_get_application (GtkWindow *window)
{
  g_return_val_if_fail (GTK_IS_WINDOW (window), NULL);

  /* hack to keep abi compat with gtk2 */
  GtkWindowClass *klass = GTK_WINDOW_GET_CLASS (window);

  return klass->application;
}

static void
gtk_window_release_application (GtkWindow *window)
{
  /* GTK_IS_WINDOW (window) already checked */

  /* hack to keep abi compat with gtk2 */
  GtkWindowClass *klass = GTK_WINDOW_GET_CLASS (window);

  if (klass->application)
    {
      GtkApplication *application;

      /* steal reference into temp variable */
      application = klass->application;
      klass->application = NULL;

      gtk_application_remove_window (application, window);
      g_object_unref (application);
    }
}


void
gtk_window_set_application (GtkWindow      *window,
                            GtkApplication *application)
{
  GtkWindowClass *klass;

  g_return_if_fail (GTK_IS_WINDOW (window));

  /* hack to keep abi compat with gtk2 */
  klass = GTK_WINDOW_GET_CLASS (window);

  if (klass->application != application)
    {
      gtk_window_release_application (window);

      klass->application = application;

      if (klass->application != NULL)
        {
          g_object_ref (klass->application);

          gtk_application_add_window (klass->application, window);
        }

      _gtk_widget_update_parent_muxer (GTK_WIDGET (window));

      gtk2_gtk_window_notify_keys_changed (window);

      if (!prop_application) {
        prop_application = g_param_spec_object ("application",
                                                P_("GtkApplication"),
                                                P_("The GtkApplication for the window"),
                                                GTK_TYPE_APPLICATION,
                                                GTK_PARAM_READWRITE|G_PARAM_STATIC_STRINGS|G_PARAM_EXPLICIT_NOTIFY);
      }

      /* install this even if we already installed it */
      g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_APPLICATION, prop_application);

      g_object_notify_by_pspec (G_OBJECT (window), prop_application);
    }
}
