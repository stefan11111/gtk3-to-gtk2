#include <gtk/gtk.h>

struct _GtkIconThemePrivate
{
  guint custom_theme        : 1;
  guint is_screen_singleton : 1;
  guint pixbuf_supports_svg : 1;
  guint themes_valid        : 1;
  guint check_reload        : 1;
  guint loading_themes      : 1;

  char *current_theme;
  char *fallback_theme;
  char **search_path;
  int search_path_len;

  /* A list of all the themes needed to look up icons.
   * In search order, without duplicates
   */
  GList *themes;
  GHashTable *unthemed_icons;

  /* Note: The keys of this hashtable are owned by the
   * themedir and unthemed hashtables.
   */
  GHashTable *all_icons;

  /* GdkScreen for the icon theme (may be NULL)
   */
  GdkScreen *screen;

  /* time when we last stat:ed for theme changes */
  guint64 last_stat_time;
  GList *dir_mtimes;

  gulong reset_styles_idle;
};


/**
 * gtk_icon_theme_add_resource_path:
 * @icon_theme: a #GtkIconTheme
 * @path: a resource path
 *
 * Adds a resource path that will be looked at when looking
 * for icons, similar to search paths.
 *
 * This function should be used to make application-specific icons
 * available as part of the icon theme.
 *
 * The resources are considered as part of the hicolor icon theme
 * and must be located in subdirectories that are defined in the
 * hicolor icon theme, such as `@path/16x16/actions/run.png`.
 * Icons that are directly placed in the resource path instead
 * of a subdirectory are also considered as ultimate fallback.
 *
 * Since: 3.14
 */
void
gtk_icon_theme_add_resource_path (GtkIconTheme *icon_theme,
                                  const gchar  *path)
{
  GtkIconThemePrivate *priv;
  gchar **new_path;

  g_return_if_fail (GTK_IS_ICON_THEME (icon_theme));
  g_return_if_fail (path != NULL);

  priv = icon_theme->priv;
  /* XXX shit implementation for a dynamic array, but this is what is used in gtk2 XXX */
  /* gtk3 and gtk4 implementations are somehow even worse */
  /* The proper way to implement this is to exponentially increase capacity every time the array is full */
  /* and keep track of both capacity and length */
  new_path = g_realloc(priv->search_path, (priv->search_path_len + 1) * sizeof(gchar*));
  /* no NULL check, glib aborts on allocation failure */
  priv->search_path = new_path;
  new_path[priv->search_path_len] = g_strdup (path);
  priv->search_path_len++;

  /* could do without, but would be a lot slower using only public api */
  gtk2_do_theme_change (icon_theme);
}

