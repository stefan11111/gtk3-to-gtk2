#include "gtkcssproviderprivate.h"

/* XXX Not Implemented XXX */

static void
gtk_css_provider_init (GtkCssProvider *css_provider)
{
}

static void
gtk_css_provider_class_init (GtkCssProviderClass *klass)
{
}

G_DEFINE_TYPE (GtkCssProvider, gtk_css_provider, G_TYPE_OBJECT);

GQuark
gtk_css_provider_error_quark (void)
{
  return g_quark_from_static_string ("gtk-css-provider-error-quark");
}

/**
 * gtk_css_provider_new:
 *
 * Returns a newly created #GtkCssProvider.
 *
 * Returns: A new #GtkCssProvider
 **/
GtkCssProvider *
gtk_css_provider_new (void)
{
  return g_object_new (GTK_TYPE_CSS_PROVIDER, NULL);
}

/**
 * gtk_css_provider_to_string:
 * @provider: the provider to write to a string
 *
 * Converts the @provider into a string representation in CSS
 * format.
 *
 * Using gtk_css_provider_load_from_data() with the return value
 * from this function on a new provider created with
 * gtk_css_provider_new() will basically create a duplicate of
 * this @provider.
 *
 * Returns: a new string representing the @provider.
 *
 * Since: 3.2
 **/
char *
gtk_css_provider_to_string (GtkCssProvider *provider)
{
  return NULL;
}

/**
 * gtk_css_provider_load_from_data:
 * @css_provider: a #GtkCssProvider
 * @data: (array length=length) (element-type guint8): CSS data loaded in memory
 * @length: the length of @data in bytes, or -1 for NUL terminated strings. If
 *   @length is not -1, the code will assume it is not NUL terminated and will
 *   potentially do a copy.
 * @error: (out) (allow-none): return location for a #GError, or %NULL
 *
 * Loads @data into @css_provider, and by doing so clears any previously loaded
 * information.
 *
 * Returns: %TRUE. The return value is deprecated and %FALSE will only be
 *     returned for backwards compatibility reasons if an @error is not
 *     %NULL and a loading error occurred. To track errors while loading
 *     CSS, connect to the #GtkCssProvider::parsing-error signal.
 **/
gboolean
gtk_css_provider_load_from_data (GtkCssProvider  *css_provider,
                                 const gchar     *data,
                                 gssize           length,
                                 GError         **error)
{
  g_set_error (error, 0, 0, "Not Implemented");
  return FALSE;
}

/**
 * gtk_css_provider_load_from_file:
 * @css_provider: a #GtkCssProvider
 * @file: #GFile pointing to a file to load
 * @error: (out) (allow-none): return location for a #GError, or %NULL
 *
 * Loads the data contained in @file into @css_provider, making it
 * clear any previously loaded information.
 *
 * Returns: %TRUE. The return value is deprecated and %FALSE will only be
 *     returned for backwards compatibility reasons if an @error is not
 *     %NULL and a loading error occurred. To track errors while loading
 *     CSS, connect to the #GtkCssProvider::parsing-error signal.
 **/
gboolean
gtk_css_provider_load_from_file (GtkCssProvider  *css_provider,
                                 GFile           *file,
                                 GError         **error)
{
  g_set_error (error, 0, 0, "Not Implemented");
  return FALSE;
}

/**
 * gtk_css_provider_load_from_path:
 * @css_provider: a #GtkCssProvider
 * @path: the path of a filename to load, in the GLib filename encoding
 * @error: (out) (allow-none): return location for a #GError, or %NULL
 *
 * Loads the data contained in @path into @css_provider, making it clear
 * any previously loaded information.
 *
 * Returns: %TRUE. The return value is deprecated and %FALSE will only be
 *     returned for backwards compatibility reasons if an @error is not
 *     %NULL and a loading error occurred. To track errors while loading
 *     CSS, connect to the #GtkCssProvider::parsing-error signal.
 **/
gboolean
gtk_css_provider_load_from_path (GtkCssProvider  *css_provider,
                                 const gchar     *path,
                                 GError         **error)
{
  g_set_error (error, 0, 0, "Not Implemented");
  return FALSE;
}

/**
 * gtk_css_provider_load_from_resource:
 * @css_provider: a #GtkCssProvider
 * @resource_path: a #GResource resource path
 *
 * Loads the data contained in the resource at @resource_path into
 * the #GtkCssProvider, clearing any previously loaded information.
 *
 * To track errors while loading CSS, connect to the
 * #GtkCssProvider::parsing-error signal.
 *
 * Since: 3.16
 */
void
gtk_css_provider_load_from_resource (GtkCssProvider *css_provider,
                                     const gchar    *resource_path)
{
}

/**
 * gtk_css_provider_get_default:
 *
 * Returns the provider containing the style settings used as a
 * fallback for all widgets.
 *
 * Returns: (transfer none): The provider used for fallback styling.
 *          This memory is owned by GTK+, and you must not free it.
 *
 * Deprecated: 3.24: Use gtk_css_provider_new() instead.
 **/
GtkCssProvider *
gtk_css_provider_get_default (void)
{
  static GtkCssProvider *provider;

  if (G_UNLIKELY (!provider))
    {
      provider = gtk_css_provider_new ();
    }

  return provider;
}

/**
 * gtk_css_provider_get_named:
 * @name: A theme name
 * @variant: (allow-none): variant to load, for example, "dark", or
 *     %NULL for the default
 *
 * Loads a theme from the usual theme paths
 *
 * Returns: (transfer none): a #GtkCssProvider with the theme loaded.
 *     This memory is owned by GTK+, and you must not free it.
 */
GtkCssProvider *
gtk_css_provider_get_named (const gchar *name,
                            const gchar *variant)
{
  return gtk_css_provider_get_default ();
}

/* XXX ^^^ public ^^^ XXX */

/* XXX vvv private vvv XXX */

gchar *
_gtk_get_theme_dir (void)
{
  const gchar *var;

  var = g_getenv ("GTK_DATA_PREFIX");
  if (var == NULL) {
    var = "usr";
  }
  return g_build_filename (var, "share", "themes", NULL);
}

/* Return the path that this providers gtk.css was loaded from,
 * if it is part of a theme, otherwise NULL.
 */
const gchar *
_gtk_css_provider_get_theme_dir (GtkCssProvider *provider)
{
  return NULL;
}

/**
 * _gtk_css_provider_load_named:
 * @provider: a #GtkCssProvider
 * @name: A theme name
 * @variant: (allow-none): variant to load, for example, "dark", or
 *     %NULL for the default
 *
 * Loads a theme from the usual theme paths. The actual process of
 * finding the theme might change between releases, but it is
 * guaranteed that this function uses the same mechanism to load the
 * theme than GTK uses for loading its own theme.
 **/
void
_gtk_css_provider_load_named (GtkCssProvider *provider,
                              const gchar    *name,
                              const gchar    *variant)
{
}

/* This is exported privately for use in GtkInspector.
 * It is the callers responsibility to reparse the current theme.
 */
void
gtk_css_provider_set_keep_css_sections (void)
{
}
