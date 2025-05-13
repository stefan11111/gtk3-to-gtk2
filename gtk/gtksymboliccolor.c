#include <gtk/gtk.h>

typedef struct _GtkCssValue GtkCssValue;
typedef struct _GtkCssParser GtkCssParser;


/* XXX Not Implemented XXX*/

/* Allocation is implemented, but is disabled by default*/
/* To enable, define ENABLE_GTK_SYMBOLIC_COLOR_ALLOCATION */
/* The allocation is useless, it just increments/decrements a refcount */

/**
 * SECTION:gtksymboliccolor
 * @Short_description: Symbolic colors
 * @Title: GtkSymbolicColor
 *
 * GtkSymbolicColor is a boxed type that represents a symbolic color.
 * It is the result of parsing a
 * [color expression][gtkcssprovider-symbolic-colors].
 * To obtain the color represented by a GtkSymbolicColor, it has to
 * be resolved with gtk_symbolic_color_resolve(), which replaces all
 * symbolic color references by the colors they refer to (in a given
 * context) and evaluates mix, shade and other expressions, resulting
 * in a #GdkRGBA value.
 *
 * It is not normally necessary to deal directly with #GtkSymbolicColors,
 * since they are mostly used behind the scenes by #GtkStyleContext and
 * #GtkCssProvider.
 *
 * #GtkSymbolicColor is deprecated. Symbolic colors are considered an
 * implementation detail of GTK+.
 */

G_DEFINE_BOXED_TYPE (GtkSymbolicColor, gtk_symbolic_color,
                     gtk_symbolic_color_ref, gtk_symbolic_color_unref)

struct _GtkSymbolicColor
{
#ifdef ENABLE_GTK_SYMBOLIC_COLOR_ALLOCATION
  gint ref_count;
#endif
};

static GtkSymbolicColor *
gtk_symbolic_color_new (GtkCssValue *value)
{
#ifdef ENABLE_GTK_SYMBOLIC_COLOR_ALLOCATION
  GtkSymbolicColor *symbolic;

  /* despite what the docs say, g_slice_new{,0} can return NULL */
  /* https://gitlab.gnome.org/GNOME/glib/-/issues/3688 */
  symbolic = g_slice_new0 (GtkSymbolicColor);
  symbolic->ref_count = 1;

  return symbolic;
#else
  return NULL;
#endif
}

/**
 * gtk_symbolic_color_new_literal:
 * @color: a #GdkRGBA
 *
 * Creates a symbolic color pointing to a literal color.
 *
 * Returns: a newly created #GtkSymbolicColor
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_new_literal (const GdkRGBA *color)
{
  return gtk_symbolic_color_new(NULL);
}

/**
 * gtk_symbolic_color_new_name:
 * @name: color name
 *
 * Creates a symbolic color pointing to an unresolved named
 * color. See gtk_style_context_lookup_color() and
 * gtk_style_properties_lookup_color().
 *
 * Returns: a newly created #GtkSymbolicColor
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_new_name (const gchar *name)
{
  return gtk_symbolic_color_new(NULL);
}

/**
 * gtk_symbolic_color_new_shade: (constructor)
 * @color: another #GtkSymbolicColor
 * @factor: shading factor to apply to @color
 *
 * Creates a symbolic color defined as a shade of
 * another color. A factor > 1.0 would resolve to
 * a brighter color, while < 1.0 would resolve to
 * a darker color.
 *
 * Returns: A newly created #GtkSymbolicColor
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_new_shade (GtkSymbolicColor *color,
                              gdouble           factor)
{
  return gtk_symbolic_color_new (NULL);
}

/**
 * gtk_symbolic_color_new_alpha: (constructor)
 * @color: another #GtkSymbolicColor
 * @factor: factor to apply to @color alpha
 *
 * Creates a symbolic color by modifying the relative alpha
 * value of @color. A factor < 1.0 would resolve to a more
 * transparent color, while > 1.0 would resolve to a more
 * opaque color.
 *
 * Returns: A newly created #GtkSymbolicColor
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_new_alpha (GtkSymbolicColor *color,
                              gdouble           factor)
{
  return gtk_symbolic_color_new (NULL);
}

/**
 * gtk_symbolic_color_new_mix: (constructor)
 * @color1: color to mix
 * @color2: another color to mix
 * @factor: mix factor
 *
 * Creates a symbolic color defined as a mix of another
 * two colors. a mix factor of 0 would resolve to @color1,
 * while a factor of 1 would resolve to @color2.
 *
 * Returns: A newly created #GtkSymbolicColor
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_new_mix (GtkSymbolicColor *color1,
                            GtkSymbolicColor *color2,
                            gdouble           factor)
{
  return gtk_symbolic_color_new (NULL);
}

/**
 * gtk_symbolic_color_new_win32: (constructor)
 * @theme_class: The theme class to pull color from
 * @id: The color id
 *
 * Creates a symbolic color based on the current win32
 * theme.
 *
 * Note that while this call is available on all platforms
 * the actual value returned is not reliable on non-win32
 * platforms.
 *
 * Returns: A newly created #GtkSymbolicColor
 *
 * Since: 3.4
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 */
GtkSymbolicColor *
gtk_symbolic_color_new_win32 (const gchar *theme_class,
                              gint         id)
{
  return gtk_symbolic_color_new (NULL);
}

/**
 * gtk_symbolic_color_ref:
 * @color: a #GtkSymbolicColor
 *
 * Increases the reference count of @color
 *
 * Returns: the same @color
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_symbolic_color_ref (GtkSymbolicColor *color)
{
#ifdef ENABLE_GTK_SYMBOLIC_COLOR_ALLOCATION
  g_return_val_if_fail (color != NULL, NULL);

  color->ref_count++;

  return color;
#else
  return NULL;
#endif
}

/**
 * gtk_symbolic_color_unref:
 * @color: a #GtkSymbolicColor
 *
 * Decreases the reference count of @color, freeing its memory if the
 * reference count reaches 0.
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
void
gtk_symbolic_color_unref (GtkSymbolicColor *color)
{
#ifdef ENABLE_GTK_SYMBOLIC_COLOR_ALLOCATION
  g_return_if_fail (color != NULL);

  if (--color->ref_count)
    return;

  g_slice_free (GtkSymbolicColor, color);
#endif
}

/**
 * gtk_symbolic_color_resolve:
 * @color: a #GtkSymbolicColor
 * @props: (allow-none): #GtkStyleProperties to use when resolving
 *    named colors, or %NULL
 * @resolved_color: (out): return location for the resolved color
 *
 * If @color is resolvable, @resolved_color will be filled in
 * with the resolved color, and %TRUE will be returned. Generally,
 * if @color can’t be resolved, it is due to it being defined on
 * top of a named color that doesn’t exist in @props.
 *
 * When @props is %NULL, resolving of named colors will fail, so if
 * your @color is or references such a color, this function will
 * return %FALSE.
 *
 * Returns: %TRUE if the color has been resolved
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
gboolean
gtk_symbolic_color_resolve (GtkSymbolicColor   *color,
			    GtkStyleProperties *props,
			    GdkRGBA            *resolved_color)
{
  return FALSE;
}

/**
 * gtk_symbolic_color_to_string:
 * @color: color to convert to a string
 *
 * Converts the given @color to a string representation. This is useful
 * both for debugging and for serialization of strings. The format of
 * the string may change between different versions of GTK, but it is
 * guaranteed that the GTK css parser is able to read the string and
 * create the same symbolic color from it.
 *
 * Returns: a new string representing @color
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
char *
gtk_symbolic_color_to_string (GtkSymbolicColor *color)
{
  return NULL;
}

GtkSymbolicColor *
_gtk_css_symbolic_value_new (GtkCssParser *parser)
{
  return gtk_symbolic_color_new (NULL);
}

GtkCssValue *
_gtk_symbolic_color_get_css_value (GtkSymbolicColor *symbolic)
{
  return NULL;
}
