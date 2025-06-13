#include <gtk/gtkwindow.h>
#include <gtk/gtkwidget.h>
#include <gtk/gtkactionmuxer.h>
#include <gtk/gtkicontheme.h>
#include <gtk/gtkprivate.h>
#include <gtk/gtkmain.h>

#include <gdk/gdk.h> /* gdk_threads_add_idle_full */ 

#include "gtkintl.h"
#include "gtkwidgetprivate.h"

#define default_icon_list (*gtk2_default_icon_list ())
#define default_icon_name (*gtk2_default_icon_name ())

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

static guint delete_event_handler = 0;

typedef struct
{
  GList     *icon_list;
  GdkPixmap *icon_pixmap;
  GdkPixmap *icon_mask;
  gchar     *icon_name;
  guint      realized : 1;
  guint      using_default_icon : 1;
  guint      using_parent_icon : 1;
  guint      using_themed_icon : 1;
} GtkWindowIconInfo;

typedef struct {
  GdkGeometry    geometry; /* Last set of geometry hints we set */
  GdkWindowHints flags;
  GdkRectangle   configure_request;
} GtkWindowLastGeometryInfo;

struct _GtkWindowGeometryInfo
{
  /* Properties that the app has set on the window
   */
  GdkGeometry    geometry;      /* Geometry hints */
  GdkWindowHints mask;
  GtkWidget     *widget;        /* subwidget to which hints apply */
  /* from last gtk_window_resize () - if > 0, indicates that
   * we should resize to this size.
   */
  gint           resize_width;
  gint           resize_height;

  /* From last gtk_window_move () prior to mapping -
   * only used if initial_pos_set
   */
  gint           initial_x;
  gint           initial_y;

  /* Default size - used only the FIRST time we map a window,
   * only if > 0.
   */
  gint           default_width;
  gint           default_height;
  /* whether to use initial_x, initial_y */
  guint          initial_pos_set : 1;
  /* CENTER_ALWAYS or other position constraint changed since
   * we sent the last configure request.
   */
  guint          position_constraints_changed : 1;

  /* if true, default_width, height come from gtk_window_parse_geometry,
   * and thus should be multiplied by the increments and affect the
   * geometry widget only
   */
  guint          default_is_geometry : 1;

  GtkWindowLastGeometryInfo last;
};


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

static GdkPixbuf *
icon_from_list (GList *list,
                gint   size)
{
  GdkPixbuf *best;
  GdkPixbuf *pixbuf;
  GList *l;

  best = NULL;
  for (l = list; l; l = l->next)
    {
      pixbuf = list->data;
      if (gdk_pixbuf_get_width (pixbuf) <= size &&
          gdk_pixbuf_get_height (pixbuf) <= size)
        {
          best = g_object_ref (pixbuf);
          break;
        }
    }

  if (best == NULL)
    best = gdk_pixbuf_scale_simple (GDK_PIXBUF (list->data), size, size, GDK_INTERP_BILINEAR);

  return best;
}

static GdkPixbuf *
icon_from_name (const gchar *name,
                gint         size)
{
  return gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
                                   name, size,
                                   GTK_ICON_LOOKUP_FORCE_SIZE, NULL);
}

GdkPixbuf *
gtk_window_get_icon_for_size (GtkWindow *window,
                              gint       size)
{
  GtkWindowIconInfo *info;
  const gchar *name;

  info = gtk2_ensure_icon_info (window);

  if (info->icon_list != NULL)
    return icon_from_list (info->icon_list, size);

  name = gtk_window_get_icon_name (window);
  if (name != NULL)
    return icon_from_name (name, size);

  if (window->transient_parent != NULL)
    {
      info = gtk2_ensure_icon_info (window->transient_parent);
      if (info->icon_list)
        return icon_from_list (info->icon_list, size);
    }

  if (default_icon_list != NULL)
    return icon_from_list (default_icon_list, size);

  if (default_icon_name != NULL)
    return icon_from_name (default_icon_name, size);

  return NULL;
}

void
_gtk_window_toggle_maximized (GtkWindow *window)
{

  if (window->maximize_initially)
    gtk_window_unmaximize (window);
  else
    gtk_window_maximize (window);
}

/**
 * gtk_window_is_maximized:
 * @window: a #GtkWindow
 *
 * Retrieves the current maximized state of @window.
 *
 * Note that since maximization is ultimately handled by the window
 * manager and happens asynchronously to an application request, you
 * shouldn’t assume the return value of this function changing
 * immediately (or at all), as an effect of calling
 * gtk_window_maximize() or gtk_window_unmaximize().
 *
 * Returns: whether the window has a maximized state.
 *
 * Since: 3.12
 */
gboolean
gtk_window_is_maximized (GtkWindow *window)
{
  g_return_val_if_fail (GTK_IS_WINDOW (window), FALSE);

  return window->maximize_initially;
}

static gboolean
send_delete_event (gpointer data)
{
  GtkWidget *window = data;
  GdkWindow *gdk_window;

  delete_event_handler = 0;

  gdk_window = gtk_widget_get_window (window);
  if (gdk_window)
    {
      GdkEvent *event;

      event = gdk_event_new (GDK_DELETE);
      event->any.window = g_object_ref (gdk_window);
      event->any.send_event = TRUE;

      gtk_main_do_event (event);

      gdk_event_free (event);
    }

  return G_SOURCE_REMOVE;
}

/**
 * gtk_window_close:
 * @window: a #GtkWindow
 *
 * Requests that the window is closed, similar to what happens
 * when a window manager close button is clicked.
 *
 * This function can be used with close buttons in custom
 * titlebars.
 *
 * Since: 3.10
 */
void
gtk_window_close (GtkWindow *window)
{
  if (!gtk_widget_get_realized (GTK_WIDGET (window)))
    return;

  delete_event_handler = gdk_threads_add_idle_full (G_PRIORITY_DEFAULT, send_delete_event, window, NULL);
  g_source_set_name_by_id (delete_event_handler, "[gtk+] send_delete_event");
}

void
_gtk_window_get_shadow_width (GtkWindow *window,
                              GtkBorder *border)
{
  /* Not Implemented */
  g_return_if_fail (border != NULL);
  *border = (GtkBorder){0};
}

/**
 * gtk_window_set_titlebar:
 * @window: a #GtkWindow
 * @titlebar: (allow-none): the widget to use as titlebar
 *
 * Sets a custom titlebar for @window.
 *
 * A typical widget used here is #GtkHeaderBar, as it provides various features
 * expected of a titlebar while allowing the addition of child widgets to it.
 *
 * If you set a custom titlebar, GTK+ will do its best to convince
 * the window manager not to put its own titlebar on the window.
 * Depending on the system, this function may not work for a window
 * that is already visible, so you set the titlebar before calling
 * gtk_widget_show().
 *
 * Since: 3.10
 */
void
gtk_window_set_titlebar (GtkWindow *window,
                         GtkWidget *titlebar)
{
  /* Not Implemented */
}

/**
 * gtk_window_get_titlebar:
 * @window: a #GtkWindow
 *
 * Returns the custom titlebar that has been set with
 * gtk_window_set_titlebar().
 *
 * Returns: (nullable) (transfer none): the custom titlebar, or %NULL
 *
 * Since: 3.16
 */
GtkWidget *
gtk_window_get_titlebar (GtkWindow *window)
{
  /* Not Implemented */
  return NULL;
}

gboolean
_gtk_window_titlebar_shows_app_menu (GtkWindow *window)
{
  /* Not Implemented */
  return FALSE;
}

/* _gtk_window_set_allocation:
 * @window: a #GtkWindow
 * @allocation: the original allocation for the window
 * @allocation_out: @allocation taking decorations into
 * consideration
 *
 * This function is like gtk_widget_set_allocation()
 * but does the necessary extra work to update
 * the resize grip positioning, etc.
 *
 * Call this instead of gtk_widget_set_allocation()
 * when overriding ::size_allocate in a GtkWindow
 * subclass without chaining up.
 *
 * The @allocation parameter will be adjusted to
 * reflect any internal decorations that the window
 * may have. That revised allocation will then be
 * returned in the @allocation_out parameter.
 */
void
_gtk_window_set_allocation (GtkWindow           *window,
                            const GtkAllocation *allocation,
                            GtkAllocation       *allocation_out)
{
  g_assert (allocation != NULL);
  g_assert (allocation_out != NULL);

  gtk_widget_set_allocation ((GtkWidget*)window, allocation);
  *allocation_out = *allocation;
}
