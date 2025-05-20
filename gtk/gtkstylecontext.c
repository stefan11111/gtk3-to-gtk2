#include <gtk/gtk.h>
#include <string.h>
#include "util.h"
#include "gtkstyleproviderprivate.h"
#include "gtkstylecascadeprivate.h"
#include "gtksettingsprivate.h"

/* TODO: maybe remove */
#define IMPLEMENT_STYLE_CASCADE

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



/* later */

const PangoFontDescription *
gtk_style_context_get_font (GtkStyleContext *context,
                            GtkStateFlags    state)
{
  return context->font_desc;
}

void
gtk_style_context_get_color (GtkStyleContext *context,
                             GtkStateFlags    state,
                             GdkRGBA         *color)
{
  *color = GdkRGBA_from_GdkColor(&context->base[GtkStateType_from_GtkStateFlags(state)]);
}
