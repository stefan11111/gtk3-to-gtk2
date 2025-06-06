#include <gtk/gtk.h>
#include <string.h>
#include "util.h"
#include "gtkstyleproviderprivate.h"
#include "gtkstylecascadeprivate.h"
#include "gtksettingsprivate.h"
#include "gtkstylecontextprivate.h"

/* TODO: maybe when gdkframeclock is implemented */
#define IMPLEMENT_STYLE_CASCADE
typedef struct _GdkFrameClock GdkFrameClock;

#define GDK_COLORMAP_GET_SCREEN(cmap) ((GdkScreen **) cmap->windowing_data)

static GtkStyleClass saved_state;

typedef GtkStyle GtkStyleContext;
typedef GtkStyleClass GtkStyleContextClass;

GType
gtk_style_context_get_type (void)
{
#if 0
  return gtk_style_get_type ();
#else
  return GTK_TYPE_STYLE;
#endif
}

GtkStyleContext *
gtk_style_context_new (void)
{
  return gtk_style_new ();
}

#ifdef IMPLEMENT_STYLE_CASCADE
static GtkStyleCascade *
gdk_screen_get_style_cascade (GdkScreen *screen)
{
#if 0 /* wasted computation, the function returns the same thing */
  return _gtk_settings_get_style_cascade (gtk_settings_get_for_screen (screen), 1);
#else
  return _gtk_settings_get_style_cascade (NULL, 1);
#endif
}
#endif

/**
 * gtk_style_context_add_provider_for_screen:
 * @screen: a #GdkScreen
 * @provider: a #GtkStyleProvider
 * @priority: the priority of the style provider. The lower
 *            it is, the earlier it will be used in the style
 *            construction. Typically this will be in the range
 *            between %GTK_STYLE_PROVIDER_PRIORITY_FALLBACK and
 *            %GTK_STYLE_PROVIDER_PRIORITY_USER
 *
 * Adds a global style provider to @screen, which will be used
 * in style construction for all #GtkStyleContexts under @screen.
 *
 * GTK+ uses this to make styling information from #GtkSettings
 * available.
 *
 * Note: If both priorities are the same, A #GtkStyleProvider
 * added through gtk_style_context_add_provider() takes precedence
 * over another added through this function.
 *
 * Since: 3.0
 **/
void
gtk_style_context_add_provider_for_screen (GdkScreen        *screen,
                                           GtkStyleProvider *provider,
                                           guint             priority)
{
#ifdef IMPLEMENT_STYLE_CASCADE
  GtkStyleCascade *cascade;

  g_return_if_fail (GDK_IS_SCREEN (screen));
  g_return_if_fail (GTK_IS_STYLE_PROVIDER (provider));
  g_return_if_fail (!GTK_IS_SETTINGS (provider) || _gtk_settings_get_screen (GTK_SETTINGS (provider)) == screen);

  cascade = gdk_screen_get_style_cascade (screen);
  _gtk_style_cascade_add_provider (cascade, provider, priority);
#endif
}

/**
 * gtk_style_context_remove_provider_for_screen:
 * @screen: a #GdkScreen
 * @provider: a #GtkStyleProvider
 *
 * Removes @provider from the global style providers list in @screen.
 *
 * Since: 3.0
 **/
void
gtk_style_context_remove_provider_for_screen (GdkScreen        *screen,
                                              GtkStyleProvider *provider)
{
#ifdef IMPLEMENT_STYLE_CASCADE
  GtkStyleCascade *cascade;

  g_return_if_fail (GDK_IS_SCREEN (screen));
  g_return_if_fail (GTK_IS_STYLE_PROVIDER (provider));
  g_return_if_fail (!GTK_IS_SETTINGS (provider));

  cascade = gdk_screen_get_style_cascade (screen);
  _gtk_style_cascade_remove_provider (cascade, provider);
#endif
}

#ifdef IMPLEMENT_STYLE_CASCADE
static GtkStyleCascade *
gtk_style_contex_get_cascade (GtkStyleContext  *context)
{
#if 0 /* wasted computation, the function returns the same thing */
  return _gtk_settings_get_style_cascade (gtk_settings_get_for_screen (gdk_colormap_get_screen (context->colormap)), 1);
#else
  return _gtk_settings_get_style_cascade (NULL, 1);
#endif
}
#endif

/**
 * gtk_style_context_add_provider:
 * @context: a #GtkStyleContext
 * @provider: a #GtkStyleProvider
 * @priority: the priority of the style provider. The lower
 *            it is, the earlier it will be used in the style
 *            construction. Typically this will be in the range
 *            between %GTK_STYLE_PROVIDER_PRIORITY_FALLBACK and
 *            %GTK_STYLE_PROVIDER_PRIORITY_USER
 *
 * Adds a style provider to @context, to be used in style construction.
 * Note that a style provider added by this function only affects
 * the style of the widget to which @context belongs. If you want
 * to affect the style of all widgets, use
 * gtk_style_context_add_provider_for_screen().
 *
 * Note: If both priorities are the same, a #GtkStyleProvider
 * added through this function takes precedence over another added
 * through gtk_style_context_add_provider_for_screen().
 *
 * Since: 3.0
 **/
void
gtk_style_context_add_provider (GtkStyleContext  *context,
                                GtkStyleProvider *provider,
                                guint             priority)
{
#ifdef IMPLEMENT_STYLE_CASCADE
  GtkStyleCascade *cascade;

  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  g_return_if_fail (GTK_IS_STYLE_PROVIDER (provider));

  cascade = gtk_style_contex_get_cascade (context);
  _gtk_style_cascade_add_provider (cascade, provider, priority);
#endif
}

/**
 * gtk_style_context_remove_provider:
 * @context: a #GtkStyleContext
 * @provider: a #GtkStyleProvider
 *
 * Removes @provider from the style providers list in @context.
 *
 * Since: 3.0
 **/
void
gtk_style_context_remove_provider (GtkStyleContext  *context,
                                   GtkStyleProvider *provider)
{
#ifdef IMPLEMENT_STYLE_CASCADE
  GtkStyleCascade *cascade;

  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  g_return_if_fail (GTK_IS_STYLE_PROVIDER (provider));

  cascade = gtk_style_contex_get_cascade (context);
  _gtk_style_cascade_remove_provider (cascade, provider);
#endif
}

/**
 * gtk_style_context_save:
 * @context: a #GtkStyleContext
 *
 * Saves the @context state, so temporary modifications done through
 * gtk_style_context_add_class(), gtk_style_context_remove_class(),
 * gtk_style_context_set_state(), etc. can quickly be reverted
 * in one go through gtk_style_context_restore().
 *
 * The matching call to gtk_style_context_restore() must be done
 * before GTK returns to the main loop.
 *
 * Since: 3.0
 **/
void
gtk_style_context_save (GtkStyleContext *context)
{
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  memcpy(&saved_state, GTK_STYLE_GET_CLASS(context), sizeof(GtkStyleClass));
}

/**
 * gtk_style_context_restore:
 * @context: a #GtkStyleContext
 *
 * Restores @context state to a previous stage.
 * See gtk_style_context_save().
 *
 * Since: 3.0
 **/
void
gtk_style_context_restore (GtkStyleContext *context)
{
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  memcpy(GTK_STYLE_GET_CLASS(context), &saved_state, sizeof(GtkStyleClass));
}

/**
 * gtk_style_context_get_section:
 * @context: a #GtkStyleContext
 * @property: style property name
 *
 * Queries the location in the CSS where @property was defined for the
 * current @context. Note that the state to be queried is taken from
 * gtk_style_context_get_state().
 *
 * If the location is not available, %NULL will be returned. The
 * location might not be available for various reasons, such as the
 * property being overridden, @property not naming a supported CSS
 * property or tracking of definitions being disabled for performance
 * reasons.
 *
 * Shorthand CSS properties cannot be queried for a location and will
 * always return %NULL.
 *
 * Returns: (nullable) (transfer none): %NULL or the section where a value
 * for @property was defined
 **/
GtkCssSection *
gtk_style_context_get_section (GtkStyleContext *context,
                               const gchar     *property)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gtk_style_context_get_property:
 * @context: a #GtkStyleContext
 * @property: style property name
 * @state: state to retrieve the property value for
 * @value: (out) (transfer full):  return location for the style property value
 *
 * Gets a style property from @context for the given state.
 *
 * Note that not all CSS properties that are supported by GTK+ can be
 * retrieved in this way, since they may not be representable as #GValue.
 * GTK+ defines macros for a number of properties that can be used
 * with this function.
 *
 * Note that passing a state other than the current state of @context
 * is not recommended unless the style context has been saved with
 * gtk_style_context_save().
 *
 * When @value is no longer needed, g_value_unset() must be called
 * to free any allocated memory.
 *
 * Since: 3.0
 **/
void
gtk_style_context_get_property (GtkStyleContext *context,
                                const gchar     *property,
                                GtkStateFlags    state,
                                GValue          *value)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_valist:
 * @context: a #GtkStyleContext
 * @state: state to retrieve the property values for
 * @args: va_list of property name/return location pairs, followed by %NULL
 *
 * Retrieves several style property values from @context for a given state.
 *
 * See gtk_style_context_get_property() for details.
 *
 * Since: 3.0
 */
void
gtk_style_context_get_valist (GtkStyleContext *context,
                              GtkStateFlags    state,
                              va_list          args)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get:
 * @context: a #GtkStyleContext
 * @state: state to retrieve the property values for
 * @...: property name / return value pairs, followed by %NULL
 *
 * Retrieves several style property values from @context for a
 * given state.
 *
 * See gtk_style_context_get_property() for details.
 *
 * For the property name / return value pairs, it works similarly as
 * g_object_get(). Example:
 *
 * |[<!-- language="C" -->
 * GdkRGBA *background_color = NULL;
 * PangoFontDescription *font_desc = NULL;
 * gint border_radius = 0;
 *
 * gtk_style_context_get (style_context,
 *                        gtk_style_context_get_state (style_context),
 *                        GTK_STYLE_PROPERTY_BACKGROUND_COLOR, &background_color,
 *                        GTK_STYLE_PROPERTY_FONT, &font_desc,
 *                        GTK_STYLE_PROPERTY_BORDER_RADIUS, &border_radius,
 *                        NULL);
 *
 * // Do something with the property values.
 *
 * if (background_color != NULL)
 *   gdk_rgba_free (background_color);
 * if (font_desc != NULL)
 *   pango_font_description_free (font_desc);
 * ]|
 *
 * Since: 3.0
 */
void
gtk_style_context_get (GtkStyleContext *context,
                       GtkStateFlags    state,
                       ...)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_set_state:
 * @context: a #GtkStyleContext
 * @flags: state to represent
 *
 * Sets the state to be used for style matching.
 *
 * Since: 3.0
 **/
void
gtk_style_context_set_state (GtkStyleContext *context,
                             GtkStateFlags    flags)
{
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  GtkStyleContextClass *klass = GTK_STYLE_GET_CLASS (context);

  /* hack to not break abi compat with gtk2 */
  /* this should really be in GtkStyle, not GtkStyleClass */
  klass->flags = flags;
}

/**
 * gtk_style_context_get_state:
 * @context: a #GtkStyleContext
 *
 * Returns the state used for style matching.
 *
 * This method should only be used to retrieve the #GtkStateFlags
 * to pass to #GtkStyleContext methods, like gtk_style_context_get_padding().
 * If you need to retrieve the current state of a #GtkWidget, use
 * gtk_widget_get_state_flags().
 *
 * Returns: the state flags
 *
 * Since: 3.0
 **/
GtkStateFlags
gtk_style_context_get_state (GtkStyleContext *context)
{
  g_return_val_if_fail (GTK_IS_STYLE_CONTEXT (context), GTK_STATE_FLAG_NORMAL);

  /* hack to not break abi compat with gtk2 */
  /* this should really be in GtkStyle, not GtkStyleClass */
  return GTK_STYLE_GET_CLASS (context)->flags;
  /* No need to check if this was set, even if it wasn't, it just returns 0, which is GTK_STATE_FLAG_NORMAL */
}

/**
 * gtk_style_context_set_scale:
 * @context: a #GtkStyleContext
 * @scale: scale
 *
 * Sets the scale to use when getting image assets for the style.
 *
 * Since: 3.10
 **/
void
gtk_style_context_set_scale (GtkStyleContext *context,
                             gint             scale)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_scale:
 * @context: a #GtkStyleContext
 *
 * Returns the scale used for assets.
 *
 * Returns: the scale
 *
 * Since: 3.10
 **/
gint
gtk_style_context_get_scale (GtkStyleContext *context)
{
  /* Not Implemented */
  return 1;
}

/**
 * gtk_style_context_state_is_running:
 * @context: a #GtkStyleContext
 * @state: a widget state
 * @progress: (out): return location for the transition progress
 *
 * Returns %TRUE if there is a transition animation running for the
 * current region (see gtk_style_context_push_animatable_region()).
 *
 * If @progress is not %NULL, the animation progress will be returned
 * there, 0.0 means the state is closest to being unset, while 1.0 means
 * it’s closest to being set. This means transition animation will
 * run from 0 to 1 when @state is being set and from 1 to 0 when
 * it’s being unset.
 *
 * Returns: %TRUE if there is a running transition animation for @state.
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function always returns %FALSE
 **/
gboolean
gtk_style_context_state_is_running (GtkStyleContext *context,
                                    GtkStateType     state,
                                    gdouble         *progress)
{
  return FALSE;
}

/**
 * gtk_style_context_set_path:
 * @context: a #GtkStyleContext
 * @path: a #GtkWidgetPath
 *
 * Sets the #GtkWidgetPath used for style matching. As a
 * consequence, the style will be regenerated to match
 * the new given path.
 *
 * If you are using a #GtkStyleContext returned from
 * gtk_widget_get_style_context(), you do not need to call
 * this yourself.
 *
 * Since: 3.0
 **/
void
gtk_style_context_set_path (GtkStyleContext *context,
                            GtkWidgetPath   *path)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_path:
 * @context: a #GtkStyleContext
 *
 * Returns the widget path used for style matching.
 *
 * Returns: (transfer none): A #GtkWidgetPath
 *
 * Since: 3.0
 **/
const GtkWidgetPath *
gtk_style_context_get_path (GtkStyleContext *context)
{
  /* Not Implemented */
  static const GtkWidgetPath *path;
  if (!path) {
    path = gtk_widget_path_new();
  }
  return path;
}

/**
 * gtk_style_context_set_parent:
 * @context: a #GtkStyleContext
 * @parent: (allow-none): the new parent or %NULL
 *
 * Sets the parent style context for @context. The parent style
 * context is used to implement
 * [inheritance](http://www.w3.org/TR/css3-cascade/#inheritance)
 * of properties.
 *
 * If you are using a #GtkStyleContext returned from
 * gtk_widget_get_style_context(), the parent will be set for you.
 *
 * Since: 3.4
 **/
void
gtk_style_context_set_parent (GtkStyleContext *context,
                              GtkStyleContext *parent)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_parent:
 * @context: a #GtkStyleContext
 *
 * Gets the parent context set via gtk_style_context_set_parent().
 * See that function for details.
 *
 * Returns: (nullable) (transfer none): the parent context or %NULL
 *
 * Since: 3.4
 **/
GtkStyleContext *
gtk_style_context_get_parent (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gtk_style_context_list_classes:
 * @context: a #GtkStyleContext
 *
 * Returns the list of classes currently defined in @context.
 *
 * Returns: (transfer container) (element-type utf8): a #GList of
 *          strings with the currently defined classes. The contents
 *          of the list are owned by GTK+, but you must free the list
 *          itself with g_list_free() when you are done with it.
 *
 * Since: 3.0
 **/
GList *
gtk_style_context_list_classes (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gtk_style_context_add_class:
 * @context: a #GtkStyleContext
 * @class_name: class name to use in styling
 *
 * Adds a style class to @context, so posterior calls to
 * gtk_style_context_get() or any of the gtk_render_*()
 * functions will make use of this new class for styling.
 *
 * In the CSS file format, a #GtkEntry defining a “search”
 * class, would be matched by:
 *
 * |[ <!-- language="CSS" -->
 * entry.search { ... }
 * ]|
 *
 * While any widget defining a “search” class would be
 * matched by:
 * |[ <!-- language="CSS" -->
 * .search { ... }
 * ]|
 *
 * Since: 3.0
 **/
void
gtk_style_context_add_class (GtkStyleContext *context,
                             const gchar     *class_name)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_remove_class:
 * @context: a #GtkStyleContext
 * @class_name: class name to remove
 *
 * Removes @class_name from @context.
 *
 * Since: 3.0
 **/
void
gtk_style_context_remove_class (GtkStyleContext *context,
                                const gchar     *class_name)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_has_class:
 * @context: a #GtkStyleContext
 * @class_name: a class name
 *
 * Returns %TRUE if @context currently has defined the
 * given class name.
 *
 * Returns: %TRUE if @context has @class_name defined
 *
 * Since: 3.0
 **/
gboolean
gtk_style_context_has_class (GtkStyleContext *context,
                             const gchar     *class_name)
{
  /* Not Implemented */
  return FALSE;
}

/**
 * gtk_style_context_list_regions:
 * @context: a #GtkStyleContext
 *
 * Returns the list of regions currently defined in @context.
 *
 * Returns: (transfer container) (element-type utf8): a #GList of
 *          strings with the currently defined regions. The contents
 *          of the list are owned by GTK+, but you must free the list
 *          itself with g_list_free() when you are done with it.
 *
 * Since: 3.0
 *
 * Deprecated: 3.14
 **/
GList *
gtk_style_context_list_regions (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gtk_style_context_add_region:
 * @context: a #GtkStyleContext
 * @region_name: region name to use in styling
 * @flags: flags that apply to the region
 *
 * Adds a region to @context, so posterior calls to
 * gtk_style_context_get() or any of the gtk_render_*()
 * functions will make use of this new region for styling.
 *
 * In the CSS file format, a #GtkTreeView defining a “row”
 * region, would be matched by:
 *
 * |[ <!-- language="CSS" -->
 * treeview row { ... }
 * ]|
 *
 * Pseudo-classes are used for matching @flags, so the two
 * following rules:
 * |[ <!-- language="CSS" -->
 * treeview row:nth-child(even) { ... }
 * treeview row:nth-child(odd) { ... }
 * ]|
 *
 * would apply to even and odd rows, respectively.
 *
 * Region names must only contain lowercase letters
 * and “-”, starting always with a lowercase letter.
 *
 * Since: 3.0
 *
 * Deprecated: 3.14
 **/
void
gtk_style_context_add_region (GtkStyleContext *context,
                              const gchar     *region_name,
                              GtkRegionFlags   flags)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_remove_region:
 * @context: a #GtkStyleContext
 * @region_name: region name to unset
 *
 * Removes a region from @context.
 *
 * Since: 3.0
 *
 * Deprecated: 3.14
 **/
void
gtk_style_context_remove_region (GtkStyleContext *context,
                                 const gchar     *region_name)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_has_region:
 * @context: a #GtkStyleContext
 * @region_name: a region name
 * @flags_return: (out) (allow-none): return location for region flags
 *
 * Returns %TRUE if @context has the region defined.
 * If @flags_return is not %NULL, it is set to the flags
 * affecting the region.
 *
 * Returns: %TRUE if region is defined
 *
 * Since: 3.0
 *
 * Deprecated: 3.14
 **/
gboolean
gtk_style_context_has_region (GtkStyleContext *context,
                              const gchar     *region_name,
                              GtkRegionFlags  *flags_return)
{
  /* Not Implemented */
  return FALSE;
}

/**
 * gtk_style_context_get_style_property:
 * @context: a #GtkStyleContext
 * @property_name: the name of the widget style property
 * @value: Return location for the property value
 *
 * Gets the value for a widget style property.
 *
 * When @value is no longer needed, g_value_unset() must be called
 * to free any allocated memory.
 **/
void
gtk_style_context_get_style_property (GtkStyleContext *context,
                                      const gchar     *property_name,
                                      GValue          *value)
{
  gtk_style_get_style_property (context, GTK_TYPE_WIDGET, property_name, value);
}

/**
 * gtk_style_context_get_style_valist:
 * @context: a #GtkStyleContext
 * @args: va_list of property name/return location pairs, followed by %NULL
 *
 * Retrieves several widget style properties from @context according to the
 * current style.
 *
 * Since: 3.0
 **/
void
gtk_style_context_get_style_valist (GtkStyleContext *context,
                                    va_list          args)
{
  const gchar *first_property_name = va_arg (args, const gchar *);
  gtk_style_get_valist (context, GTK_TYPE_WIDGET, first_property_name, args);
}

/**
 * gtk_style_context_get_style:
 * @context: a #GtkStyleContext
 * @...: property name /return value pairs, followed by %NULL
 *
 * Retrieves several widget style properties from @context according to the
 * current style.
 *
 * Since: 3.0
 **/
void
gtk_style_context_get_style (GtkStyleContext *context,
                             ...)
{
  va_list args;

  va_start (args, context);
  gtk_style_context_get_style_valist (context, args);
  va_end (args);
}

/**
 * gtk_style_context_lookup_icon_set:
 * @context: a #GtkStyleContext
 * @stock_id: an icon name
 *
 * Looks up @stock_id in the icon factories associated to @context and
 * the default icon factory, returning an icon set if found, otherwise
 * %NULL.
 *
 * Returns: (nullable) (transfer none): The looked up %GtkIconSet, or %NULL
 *
 * Deprecated: 3.10: Use gtk_icon_theme_lookup_icon() instead.
 **/
GtkIconSet *
gtk_style_context_lookup_icon_set (GtkStyleContext *context,
                                   const gchar     *stock_id)
{
  return gtk_style_lookup_icon_set (context, stock_id);
}

/**
 * gtk_icon_set_render_icon_pixbuf:
 * @icon_set: a #GtkIconSet
 * @context: a #GtkStyleContext
 * @size: (type int): icon size (#GtkIconSize). A size of `(GtkIconSize)-1`
 *        means render at the size of the source and don’t scale.
 *
 * Renders an icon using gtk_render_icon_pixbuf(). In most cases,
 * gtk_widget_render_icon_pixbuf() is better, since it automatically provides
 * most of the arguments from the current widget settings.  This
 * function never returns %NULL; if the icon can’t be rendered
 * (perhaps because an image file fails to load), a default "missing
 * image" icon will be returned instead.
 *
 * Returns: (transfer full): a #GdkPixbuf to be displayed
 *
 * Since: 3.0
 *
 * Deprecated: 3.10: Use #GtkIconTheme instead.
 */
GdkPixbuf *
gtk_icon_set_render_icon_pixbuf (GtkIconSet        *icon_set,
                                 GtkStyleContext   *context,
                                 GtkIconSize        size)
{
  GtkStateType state;
  GtkTextDirection direction;

  state = gtk_style_context_get_state (context);
  direction = gtk_style_context_get_direction (context);
  return gtk_icon_set_render_icon (icon_set, context, direction, state, size, (GtkWidget*)NULL, NULL);
}

/**
 * gtk_icon_set_render_icon_surface:
 * @icon_set: a #GtkIconSet
 * @context: a #GtkStyleContext
 * @size: (type int): icon size (#GtkIconSize). A size of `(GtkIconSize)-1`
 *        means render at the size of the source and don’t scale.
 * @scale: the window scale to render for
 * @for_window: (allow-none): #GdkWindow to optimize drawing for, or %NULL
 *
 * Renders an icon using gtk_render_icon_pixbuf() and converts it to a
 * cairo surface.
 *
 * This function never returns %NULL; if the icon can’t be rendered
 * (perhaps because an image file fails to load), a default "missing
 * image" icon will be returned instead.
 *
 * Returns: (transfer full): a #cairo_surface_t to be displayed
 *
 * Since: 3.10
 *
 * Deprecated: 3.10: Use #GtkIconTheme instead.
 */
cairo_surface_t *
gtk_icon_set_render_icon_surface  (GtkIconSet      *icon_set,
                                   GtkStyleContext *context,
                                   GtkIconSize      size,
                                   gint             scale,
                                   GdkWindow       *for_window)
{
  GdkPixbuf *pixbuf;
  cairo_surface_t *surface;

  pixbuf = gtk_icon_set_render_icon_pixbuf (icon_set, context, size);

  surface = gdk_cairo_surface_create_from_pixbuf (pixbuf, 1, for_window);
  g_object_unref (pixbuf);

  return surface;
}

/**
 * gtk_style_context_set_screen:
 * @context: a #GtkStyleContext
 * @screen: a #GdkScreen
 *
 * Attaches @context to the given screen.
 *
 * The screen is used to add style information from “global” style
 * providers, such as the screen’s #GtkSettings instance.
 *
 * If you are using a #GtkStyleContext returned from
 * gtk_widget_get_style_context(), you do not need to
 * call this yourself.
 *
 * Since: 3.0
 **/
void
gtk_style_context_set_screen (GtkStyleContext *context,
                              GdkScreen       *screen)
{
#ifdef X11
  *GDK_COLORMAP_GET_SCREEN(context->colormap) = screen;
#endif
}

/**
 * gtk_style_context_get_screen:
 * @context: a #GtkStyleContext
 *
 * Returns the #GdkScreen to which @context is attached.
 *
 * Returns: (transfer none): a #GdkScreen.
 **/
GdkScreen *
gtk_style_context_get_screen (GtkStyleContext *context)
{
  return gdk_colormap_get_screen (context->colormap);
}

/**
 * gtk_style_context_set_frame_clock:
 * @context: a #GdkFrameClock
 * @frame_clock: a #GdkFrameClock
 *
 * Attaches @context to the given frame clock.
 *
 * The frame clock is used for the timing of animations.
 *
 * If you are using a #GtkStyleContext returned from
 * gtk_widget_get_style_context(), you do not need to
 * call this yourself.
 *
 * Since: 3.8
 **/
void
gtk_style_context_set_frame_clock (GtkStyleContext *context,
                                   GdkFrameClock   *frame_clock)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_frame_clock:
 * @context: a #GtkStyleContext
 *
 * Returns the #GdkFrameClock to which @context is attached.
 *
 * Returns: (nullable) (transfer none): a #GdkFrameClock, or %NULL
 *  if @context does not have an attached frame clock.
 *
 * Since: 3.8
 **/
GdkFrameClock *
gtk_style_context_get_frame_clock (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gtk_style_context_set_direction:
 * @context: a #GtkStyleContext
 * @direction: the new direction.
 *
 * Sets the reading direction for rendering purposes.
 *
 * If you are using a #GtkStyleContext returned from
 * gtk_widget_get_style_context(), you do not need to
 * call this yourself.
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: Use gtk_style_context_set_state() with
 *   #GTK_STATE_FLAG_DIR_LTR and #GTK_STATE_FLAG_DIR_RTL
 *   instead.
 **/
void
gtk_style_context_set_direction (GtkStyleContext  *context,
                                 GtkTextDirection  direction)
{
  GtkStateFlags state;

  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  state = gtk_style_context_get_state (context);
  state &= ~(GTK_STATE_FLAG_DIR_LTR | GTK_STATE_FLAG_DIR_RTL);

  switch (direction)
    {
    case GTK_TEXT_DIR_LTR:
      state |= GTK_STATE_FLAG_DIR_LTR;
      break;

    case GTK_TEXT_DIR_RTL:
      state |= GTK_STATE_FLAG_DIR_RTL;
      break;

    case GTK_TEXT_DIR_NONE:
    default:
      break;
    }

  gtk_style_context_set_state (context, state);
}

/**
 * gtk_style_context_get_direction:
 * @context: a #GtkStyleContext
 *
 * Returns the widget direction used for rendering.
 *
 * Returns: the widget direction
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: Use gtk_style_context_get_state() and
 *   check for #GTK_STATE_FLAG_DIR_LTR and
 *   #GTK_STATE_FLAG_DIR_RTL instead.
 **/
GtkTextDirection
gtk_style_context_get_direction (GtkStyleContext *context)
{
  GtkStateFlags state;

  g_return_val_if_fail (GTK_IS_STYLE_CONTEXT (context), GTK_TEXT_DIR_LTR);

  state = gtk_style_context_get_state (context);

  if (state & GTK_STATE_FLAG_DIR_LTR) {
    return GTK_TEXT_DIR_LTR;
  }
  if (state & GTK_STATE_FLAG_DIR_RTL) {
    return GTK_TEXT_DIR_RTL;
  }
  return GTK_TEXT_DIR_NONE;
}

/**
 * gtk_style_context_set_junction_sides:
 * @context: a #GtkStyleContext
 * @sides: sides where rendered elements are visually connected to
 *     other elements
 *
 * Sets the sides where rendered elements (mostly through
 * gtk_render_frame()) will visually connect with other visual elements.
 *
 * This is merely a hint that may or may not be honored
 * by themes.
 *
 * Container widgets are expected to set junction hints as appropriate
 * for their children, so it should not normally be necessary to call
 * this function manually.
 *
 * Since: 3.0
 **/
void
gtk_style_context_set_junction_sides (GtkStyleContext  *context,
                                      GtkJunctionSides  sides)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_junction_sides:
 * @context: a #GtkStyleContext
 *
 * Returns the sides where rendered elements connect visually with others.
 *
 * Returns: the junction sides
 *
 * Since: 3.0
 **/
GtkJunctionSides
gtk_style_context_get_junction_sides (GtkStyleContext *context)
{
  /* Not Implemented */
  return GTK_JUNCTION_NONE;
}

/**
 * gtk_style_context_lookup_color:
 * @context: a #GtkStyleContext
 * @color_name: color name to lookup
 * @color: (out): Return location for the looked up color
 *
 * Looks up and resolves a color name in the @context color map.
 *
 * Returns: %TRUE if @color_name was found and resolved, %FALSE otherwise
 **/
gboolean
gtk_style_context_lookup_color (GtkStyleContext *context,
                                const gchar     *color_name,
                                GdkRGBA         *color)
{
  GdkColor c;
  int ret;
  ret = gtk_style_lookup_color (context, color_name, &c);

  if (!ret) {
    return FALSE;
  }

  *color = GdkRGBA_from_GdkColor (&c);
  return TRUE;
}

/**
 * gtk_style_context_notify_state_change:
 * @context: a #GtkStyleContext
 * @window: a #GdkWindow
 * @region_id: (allow-none): animatable region to notify on, or %NULL.
 *     See gtk_style_context_push_animatable_region()
 * @state: state to trigger transition for
 * @state_value: %TRUE if @state is the state we are changing to,
 *     %FALSE if we are changing away from it
 *
 * Notifies a state change on @context, so if the current style makes use
 * of transition animations, one will be started so all rendered elements
 * under @region_id are animated for state @state being set to value
 * @state_value.
 *
 * The @window parameter is used in order to invalidate the rendered area
 * as the animation runs, so make sure it is the same window that is being
 * rendered on by the gtk_render_*() functions.
 *
 * If @region_id is %NULL, all rendered elements using @context will be
 * affected by this state transition.
 *
 * As a practical example, a #GtkButton notifying a state transition on
 * the prelight state:
 * |[ <!-- language="C" -->
 * gtk_style_context_notify_state_change (context,
 *                                        gtk_widget_get_window (widget),
 *                                        NULL,
 *                                        GTK_STATE_PRELIGHT,
 *                                        button->in_button);
 * ]|
 *
 * Can be handled in the CSS file like this:
 * |[ <!-- language="CSS" -->
 * button {
 *     background-color: #f00
 * }
 *
 * button:hover {
 *     background-color: #fff;
 *     transition: 200ms linear
 * }
 * ]|
 *
 * This combination will animate the button background from red to white
 * if a pointer enters the button, and back to red if the pointer leaves
 * the button.
 *
 * Note that @state is used when finding the transition parameters, which
 * is why the style places the transition under the :hover pseudo-class.
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function does nothing.
 **/
void
gtk_style_context_notify_state_change (GtkStyleContext *context,
                                       GdkWindow       *window,
                                       gpointer         region_id,
                                       GtkStateType     state,
                                       gboolean         state_value)
{
}

/**
 * gtk_style_context_cancel_animations:
 * @context: a #GtkStyleContext
 * @region_id: (allow-none): animatable region to stop, or %NULL.
 *     See gtk_style_context_push_animatable_region()
 *
 * Stops all running animations for @region_id and all animatable
 * regions underneath.
 *
 * A %NULL @region_id will stop all ongoing animations in @context,
 * when dealing with a #GtkStyleContext obtained through
 * gtk_widget_get_style_context(), this is normally done for you
 * in all circumstances you would expect all widget to be stopped,
 * so this should be only used in complex widgets with different
 * animatable regions.
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function does nothing.
 **/
void
gtk_style_context_cancel_animations (GtkStyleContext *context,
                                     gpointer         region_id)
{
}

/**
 * gtk_style_context_scroll_animations:
 * @context: a #GtkStyleContext
 * @window: a #GdkWindow used previously in
 *          gtk_style_context_notify_state_change()
 * @dx: Amount to scroll in the X axis
 * @dy: Amount to scroll in the Y axis
 *
 * This function is analogous to gdk_window_scroll(), and
 * should be called together with it so the invalidation
 * areas for any ongoing animation are scrolled together
 * with it.
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function does nothing.
 **/
void
gtk_style_context_scroll_animations (GtkStyleContext *context,
                                     GdkWindow       *window,
                                     gint             dx,
                                     gint             dy)
{
}

/**
 * gtk_style_context_push_animatable_region:
 * @context: a #GtkStyleContext
 * @region_id: unique identifier for the animatable region
 *
 * Pushes an animatable region, so all further gtk_render_*() calls between
 * this call and the following gtk_style_context_pop_animatable_region()
 * will potentially show transition animations for this region if
 * gtk_style_context_notify_state_change() is called for a given state,
 * and the current theme/style defines transition animations for state
 * changes.
 *
 * The @region_id used must be unique in @context so the themes
 * can uniquely identify rendered elements subject to a state transition.
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function does nothing.
 **/
void
gtk_style_context_push_animatable_region (GtkStyleContext *context,
                                          gpointer         region_id)
{
}

/**
 * gtk_style_context_pop_animatable_region:
 * @context: a #GtkStyleContext
 *
 * Pops an animatable region from @context.
 * See gtk_style_context_push_animatable_region().
 *
 * Since: 3.0
 *
 * Deprecated: 3.6: This function does nothing.
 **/
void
gtk_style_context_pop_animatable_region (GtkStyleContext *context)
{
}

void
gtk_style_context_get_color (GtkStyleContext *context,
                             GtkStateFlags    state,
                             GdkRGBA         *color)
{
  g_return_if_fail (color != NULL);
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  *color = GdkRGBA_from_GdkColor(&context->base[GtkStateType_from_GtkStateFlags(state)]);
}

void
gtk_style_context_get_background_color (GtkStyleContext *context,
                             GtkStateFlags    state,
                             GdkRGBA         *color)
{
  g_return_if_fail (color != NULL);
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  *color = GdkRGBA_from_GdkColor(&context->bg[GtkStateType_from_GtkStateFlags(state)]);
}

/**
 * gtk_style_context_get_border_color:
 * @context: a #GtkStyleContext
 * @state: state to retrieve the color for
 * @color: (out): return value for the border color
 *
 * Gets the border color for a given state.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: Use gtk_render_frame() instead.
 **/
void
gtk_style_context_get_border_color (GtkStyleContext *context,
                                    GtkStateFlags    state,
                                    GdkRGBA         *color)
{
  g_return_if_fail (color != NULL);
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  /* Not implemented in gtk2 */
  /* use the dark color for the border color */
  *color = GdkRGBA_from_GdkColor(&context->dark[GtkStateType_from_GtkStateFlags(state)]);
}

const PangoFontDescription *
gtk_style_context_get_font (GtkStyleContext *context,
                            GtkStateFlags    state)
{
  return context->font_desc;
}

void
gtk_style_context_get_border (GtkStyleContext *context,
                              GtkStateFlags    state,
                              GtkBorder       *border)
{
  /* Not Implemented */
  g_return_if_fail (border != NULL);
  *border = (GtkBorder){0};
}

void
gtk_style_context_get_padding (GtkStyleContext *context,
                               GtkStateFlags    state,
                               GtkBorder       *padding)
{
  /* Not Implemented */
  g_return_if_fail (padding != NULL);
  *padding = (GtkBorder){0};
}

/**
 * gtk_style_context_get_margin:
 * @context: a #GtkStyleContext
 * @state: state to retrieve the border for
 * @margin: (out): return value for the margin settings
 *
 * Gets the margin for a given state as a #GtkBorder.
 * See gtk_style_property_get() and #GTK_STYLE_PROPERTY_MARGIN
 * for details.
 *
 * Since: 3.0
 **/
void
gtk_style_context_get_margin (GtkStyleContext *context,
                              GtkStateFlags    state,
                              GtkBorder       *margin)
{
  /* Not Implemented */
  g_return_if_fail (margin != NULL);
  *margin = (GtkBorder){0};
}

/**
 * gtk_style_context_invalidate:
 * @context: a #GtkStyleContext.
 *
 * Invalidates @context style information, so it will be reconstructed
 * again. It is useful if you modify the @context and need the new
 * information immediately.
 *
 * Since: 3.0
 *
 * Deprecated: 3.12: Style contexts are invalidated automatically.
 **/
void
gtk_style_context_invalidate (GtkStyleContext *context)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_reset_widgets:
 * @screen: a #GdkScreen
 *
 * This function recomputes the styles for all widgets under a particular
 * #GdkScreen. This is useful when some global parameter has changed that
 * affects the appearance of all widgets, because when a widget gets a new
 * style, it will both redraw and recompute any cached information about
 * its appearance. As an example, it is used when the color scheme changes
 * in the related #GtkSettings object.
 *
 * Since: 3.0
 **/
void
gtk_style_context_reset_widgets (GdkScreen *screen)
{
  GList *list, *toplevels;

  toplevels = gtk_window_list_toplevels ();
  g_list_foreach (toplevels, (GFunc) g_object_ref, NULL);

  for (list = toplevels; list; list = list->next)
    {
      if (gtk_widget_get_screen (list->data) == screen)
        gtk_widget_reset_style (list->data);

      g_object_unref (list->data);
    }

  g_list_free (toplevels);
}

/**
 * gtk_style_context_set_background:
 * @context: a #GtkStyleContext
 * @window: a #GdkWindow
 *
 * Sets the background of @window to the background pattern or
 * color specified in @context for its current state.
 *
 * Since: 3.0
 *
 * Deprecated: 3.18: Use gtk_render_background() instead.
 *   Note that clients still using this function are now responsible
 *   for calling this function again whenever @context is invalidated.
 **/
void
gtk_style_context_set_background (GtkStyleContext *context,
                                  GdkWindow       *window)
{
  GtkStateType state;

  state = GtkStateType_from_GtkStateFlags(gtk_style_context_get_state(context));
  gdk_window_set_background (window, &context->bg[state]);
}

#if 0 /* implemented in gtkrender.c */
void        gtk_render_insertion_cursor
                                   (GtkStyleContext     *context,
                                    cairo_t             *cr,
                                    gdouble              x,
                                    gdouble              y,
                                    PangoLayout         *layout,
                                    int                  index,
                                    PangoDirection       direction);

void   gtk_draw_insertion_cursor    (GtkWidget          *widget,
                                     cairo_t            *cr,
                                     const GdkRectangle *location,
                                     gboolean            is_primary,
                                     GtkTextDirection    direction,
                                     gboolean            draw_arrow);
#endif

/**
 * GtkStyleContextPrintFlags:
 * @GTK_STYLE_CONTEXT_PRINT_RECURSE: Print the entire tree of
 *     CSS nodes starting at the style context's node
 * @GTK_STYLE_CONTEXT_PRINT_SHOW_STYLE: Show the values of the
 *     CSS properties for each node
 *
 * Flags that modify the behavior of gtk_style_context_to_string().
 * New values may be added to this enumeration.
 */

/**
 * gtk_style_context_to_string:
 * @context: a #GtkStyleContext
 * @flags: Flags that determine what to print
 *
 * Converts the style context into a string representation.
 *
 * The string representation always includes information about
 * the name, state, id, visibility and style classes of the CSS
 * node that is backing @context. Depending on the flags, more
 * information may be included.
 *
 * This function is intended for testing and debugging of the
 * CSS implementation in GTK+. There are no guarantees about
 * the format of the returned string, it may change.
 *
 * Returns: a newly allocated string representing @context
 *
 * Since: 3.20
 */
char *
gtk_style_context_to_string (GtkStyleContext           *context,
                             GtkStyleContextPrintFlags  flags)
{
  /* Not Implemented */
  return NULL;
}

/* XXX ^^^ PUBLIC ^^^ XXX */

/* XXX vvv PRIVATE vvv XXX */

GtkStyleContext *
gtk_style_context_new_for_node (GtkCssNode *node)
{
  /* Not Implemented */
  return gtk_style_context_new ();
}

GtkCssNode*
gtk_style_context_get_node (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

/*
 * gtk_style_context_set_id:
 * @context: a #GtkStyleContext
 * @id: (allow-none): the id to use or %NULL for none.
 *
 * Sets the CSS ID to be used when obtaining style information.
 **/
void
gtk_style_context_set_id (GtkStyleContext *context,
                          const char      *id)
{
  /* Not Implemented */
}

/*
 * gtk_style_context_get_id:
 * @context: a #GtkStyleContext
 *
 * Returns the CSS ID used when obtaining style information.
 *
 * Returns: (nullable): the ID or %NULL if no ID is set.
 **/
const char *
gtk_style_context_get_id (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

GtkStyleProviderPrivate *
gtk_style_context_get_style_provider (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

void
gtk_style_context_save_named (GtkStyleContext *context,
                              const char      *name)
{
  /* Not Implemented */
}

/*
 * gtk_style_context_save_to_node:
 * @context: a #GtkStyleContext
 * @node: the node to save to
 *
 * Saves the @context state, so temporary modifications done through
 * gtk_style_context_add_class(), gtk_style_context_remove_class(),
 * gtk_style_context_set_state(), etc. and rendering using
 * gtk_render_background() or similar functions are done using the
 * given @node.
 *
 * To undo, call gtk_style_context_restore().
 *
 * The matching call to gtk_style_context_restore() must be done
 * before GTK returns to the main loop.
 **/
void
gtk_style_context_save_to_node (GtkStyleContext *context,
                                GtkCssNode      *node)
{
  /* Not Implemented */
}

/**
 * gtk_style_context_get_change:
 * @context: the context to query
 *
 * Queries the context for the changes for the currently executing
 * GtkStyleContext::invalidate signal. If no signal is currently
 * emitted or the signal has not been triggered by a CssNode
 * invalidation, this function returns %NULL.
 *
 * FIXME 4.0: Make this part of the signal.
 *
 * Returns: %NULL or the currently invalidating changes
 **/

GtkCssStyleChange *
gtk_style_context_get_change (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

GtkCssStyle *
gtk_style_context_lookup_style (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

GtkCssValue *
_gtk_style_context_peek_property (GtkStyleContext *context,
                                  guint            property_id)
{
  /* Not Implemented */
  return NULL;
}

void
gtk_style_context_validate (GtkStyleContext  *context,
                            GtkCssStyleChange *change)
{
  /* Not Implemented */
}

typedef struct {
  GType       widget_type;
  GParamSpec *pspec;
  GValue      value;
} PropertyValue;

void
gtk_style_context_clear_property_cache (GtkStyleContext *style)
{
  if (style->property_cache)
    {
      guint i;

      for (i = 0; i < style->property_cache->len; i++)
        {
          PropertyValue *node = &g_array_index (style->property_cache, PropertyValue, i);

          g_param_spec_unref (node->pspec);
          g_value_unset (&node->value);
        }
      g_array_free (style->property_cache, TRUE);
      style->property_cache = NULL;
    }
}

gboolean
_gtk_style_context_check_region_name (const gchar *str)
{
  g_return_val_if_fail (str != NULL, FALSE);

  if (!g_ascii_islower (str[0]))
    return FALSE;

  while (*str)
    {
      if (*str != '-' &&
          !g_ascii_islower (*str))
        return FALSE;

      str++;
    }

  return TRUE;
}

gboolean
_gtk_style_context_resolve_color (GtkStyleContext    *context,
                                  GtkCssValue        *color,
                                  GdkRGBA            *result)
{
  /* Not Implemented */
  /* XXX Maybe actually return a color XXX */
  return FALSE;
}

void
_gtk_style_context_get_cursor_color (GtkStyleContext *context,
                                     GdkRGBA         *primary_color,
                                     GdkRGBA         *secondary_color)
{
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));

  /* XXX Cursor color is hardcoded as black XXX */
  GdkRGBA color = GdkRGBA_from_GdkColor (&context->black);

  if (primary_color) {
    *primary_color = color;
  }

  if (secondary_color) {
    *secondary_color = color;
  }
}

void
_gtk_style_context_get_icon_extents (GtkStyleContext *context,
                                     GdkRectangle    *extents,
                                     gint             x,
                                     gint             y,
                                     gint             width,
                                     gint             height)
{
  g_return_if_fail (extents != NULL);

  *extents = (GdkRectangle){ .x = x,
                             .y = y,
                             .width = width,
                             .height = height };
}

PangoAttrList *
_gtk_style_context_get_pango_attributes (GtkStyleContext *context)
{
  /* Not Implemented */
  return NULL;
}

#if 0 /* Not Implemented and not needed */
const GValue *
_gtk_style_context_peek_style_property (GtkStyleContext *context,
                                        GType            widget_type,
                                        GParamSpec      *pspec);
#endif
