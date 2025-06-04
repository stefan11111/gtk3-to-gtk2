#include <gtk/gtk.h>

#include "util.h"

#include "gtkapplication.h"

/* XXX Only half-implemented  XXX */
/* XXX This deals with things like dbus, which I don't want to implement XXX */
/* XXX This also has some other things I might want to implement eventually XXX */


enum {
  WINDOW_ADDED,
  WINDOW_REMOVED,
  QUERY_END,
  LAST_SIGNAL
};

static guint gtk_application_signals[LAST_SIGNAL];

enum {
  PROP_ZERO,
  PROP_REGISTER_SESSION,
  PROP_SCREENSAVER_ACTIVE,
  PROP_APP_MENU,
  PROP_MENUBAR,
  PROP_ACTIVE_WINDOW,
  NUM_PROPERTIES
};

static GParamSpec *gtk_application_props[NUM_PROPERTIES];

/* XXX Opaque typedefs XXX */
typedef struct _GtkApplicationImpl GtkApplicationImpl;
typedef struct _GtkApplicationAccels GtkApplicationAccels;
typedef struct _GtkActionMuxer GtkActionMuxer;


struct _GtkApplicationPrivate
{
  GtkApplicationImpl *impl;
  GtkApplicationAccels *accels;

  GList *windows;

  GMenuModel      *app_menu;
  GMenuModel      *menubar;
  guint            last_window_id;

  gboolean         register_session;
  gboolean         screensaver_active;
  GtkActionMuxer  *muxer;
  GtkBuilder      *menus_builder;
  gchar           *help_overlay_path;
  guint            profiler_id;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkApplication, gtk_application, G_TYPE_APPLICATION)

static void
gtk_application_init (GtkApplication *application)
{
  application->priv = gtk_application_get_instance_private (application);

#if 0 /* TODO: fix later */
  application->priv->muxer = gtk_action_muxer_new ();

  application->priv->accels = gtk_application_accels_new ();
#else
  application->priv->muxer = NULL;
  application->priv->accels = NULL;
#endif
  /* getenv now at the latest */
  gdk_get_desktop_startup_id ();
}

static void
gtk_application_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GtkApplication *application = GTK_APPLICATION (object);

  switch (prop_id)
    {
    case PROP_REGISTER_SESSION:
      g_value_set_boolean (value, application->priv->register_session);
      break;

    case PROP_SCREENSAVER_ACTIVE:
      g_value_set_boolean (value, application->priv->screensaver_active);
      break;

    case PROP_APP_MENU:
      g_value_set_object (value, gtk_application_get_app_menu (application));
      break;

    case PROP_MENUBAR:
      g_value_set_object (value, gtk_application_get_menubar (application));
      break;

    case PROP_ACTIVE_WINDOW:
      g_value_set_object (value, gtk_application_get_active_window (application));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_application_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GtkApplication *application = GTK_APPLICATION (object);

  switch (prop_id)
    {
    case PROP_REGISTER_SESSION:
      application->priv->register_session = g_value_get_boolean (value);
      break;

    case PROP_APP_MENU:
      gtk_application_set_app_menu (application, g_value_get_object (value));
      break;

    case PROP_MENUBAR:
      gtk_application_set_menubar (application, g_value_get_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_application_finalize (GObject *object)
{
  GtkApplication *application = GTK_APPLICATION (object);

  g_clear_object (&application->priv->menus_builder);
  g_clear_object (&application->priv->app_menu);
  g_clear_object (&application->priv->menubar);
  g_clear_object (&application->priv->muxer);
  g_clear_object (&application->priv->accels);

  g_free (application->priv->help_overlay_path);

  G_OBJECT_CLASS (gtk_application_parent_class)->finalize (object);
}

static gboolean
gtk_application_local_command_line (GApplication   *application,
                                    gchar        ***arguments,
                                    gint           *exit_status)
{
  g_application_add_option_group (application, gtk_get_option_group (FALSE));

  return G_APPLICATION_CLASS (gtk_application_parent_class)->local_command_line (application, arguments, exit_status);
}

static void
gtk_application_add_platform_data (GApplication    *application,
                                   GVariantBuilder *builder)
{
  /* This is slightly evil.
   *
   * We don't have an impl here because we're remote so we can't figure
   * out what to do on a per-display-server basis.
   *
   * So we do all the things... which currently is just one thing.
   */
  const gchar *desktop_startup_id = gdk_get_desktop_startup_id ();
  if (desktop_startup_id)
    g_variant_builder_add (builder, "{sv}", "desktop-startup-id",
                           g_variant_new_string (desktop_startup_id));
}

static void
gtk_application_class_init (GtkApplicationClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GApplicationClass *application_class = G_APPLICATION_CLASS (class);

  object_class->get_property = gtk_application_get_property;
  object_class->set_property = gtk_application_set_property;
  object_class->finalize = gtk_application_finalize;

  application_class->local_command_line = gtk_application_local_command_line;
  application_class->add_platform_data = gtk_application_add_platform_data;
}

/**
 * gtk_application_new:
 * @application_id: (allow-none): The application ID.
 * @flags: the application flags
 *
 * Creates a new #GtkApplication instance.
 *
 * When using #GtkApplication, it is not necessary to call gtk_init()
 * manually. It is called as soon as the application gets registered as
 * the primary instance.
 *
 * Concretely, gtk_init() is called in the default handler for the
 * #GApplication::startup signal. Therefore, #GtkApplication subclasses should
 * chain up in their #GApplication::startup handler before using any GTK+ API.
 *
 * Note that commandline arguments are not passed to gtk_init().
 * All GTK+ functionality that is available via commandline arguments
 * can also be achieved by setting suitable environment variables
 * such as `G_DEBUG`, so this should not be a big
 * problem. If you absolutely must support GTK+ commandline arguments,
 * you can explicitly call gtk_init() before creating the application
 * instance.
 *
 * If non-%NULL, the application ID must be valid.  See
 * g_application_id_is_valid().
 *
 * If no application ID is given then some features (most notably application
 * uniqueness) will be disabled. A null application ID is only allowed with
 * GTK+ 3.6 or later.
 *
 * Returns: a new #GtkApplication instance
 *
 * Since: 3.0
 */
GtkApplication *
gtk_application_new (const gchar       *application_id,
                     GApplicationFlags  flags)
{
  g_return_val_if_fail (application_id == NULL || g_application_id_is_valid (application_id), NULL);

  return g_object_new (GTK_TYPE_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       NULL);
}
