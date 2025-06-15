#include <gtk/gtkstyleproperties.h>
#include <gtk/gtkthemingengine.h>

/* Property registration functions */

/**
 * gtk_theming_engine_register_property: (skip)
 * @name_space: namespace for the property name
 * @parse_func: (nullable): parsing function to use, or %NULL
 * @pspec: the #GParamSpec for the new property
 *
 * Registers a property so it can be used in the CSS file format,
 * on the CSS file the property will look like
 * "-${@name_space}-${property_name}". being
 * ${property_name} the given to @pspec. @name_space will usually
 * be the theme engine name.
 *
 * For any type a @parse_func may be provided, being this function
 * used for turning any property value (between “:” and “;”) in
 * CSS to the #GValue needed. For basic types there is already
 * builtin parsing support, so %NULL may be provided for these
 * cases.
 *
 * Engines must ensure property registration happens exactly once,
 * usually GTK+ deals with theming engines as singletons, so this
 * should be guaranteed to happen once, but bear this in mind
 * when creating #GtkThemeEngines yourself.
 *
 * In order to make use of the custom registered properties in
 * the CSS file, make sure the engine is loaded first by specifying
 * the engine property, either in a previous rule or within the same
 * one.
 * |[
 * * {
 *     engine: someengine;
 *     -SomeEngine-custom-property: 2;
 * }
 * ]|
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: Code should use the default properties provided by CSS.
 **/
void
gtk_theming_engine_register_property (const gchar            *name_space,
                                      GtkStylePropertyParser  parse_func,
                                      GParamSpec             *pspec)
{
  /* Not Implemented */
}

/**
 * gtk_style_properties_register_property: (skip)
 * @parse_func: (nullable): parsing function to use, or %NULL
 * @pspec: the #GParamSpec for the new property
 *
 * Registers a property so it can be used in the CSS file format.
 * This function is the low-level equivalent of
 * gtk_theming_engine_register_property(), if you are implementing
 * a theming engine, you want to use that function instead.
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: Code should use the default properties provided by CSS.
 **/
void
gtk_style_properties_register_property (GtkStylePropertyParser  parse_func,
                                        GParamSpec             *pspec)
{
  /* Not Implemented */
}

/**
 * gtk_style_properties_lookup_property: (skip)
 * @property_name: property name to look up
 * @parse_func: (out): return location for the parse function
 * @pspec: (out) (transfer none): return location for the #GParamSpec
 *
 * Returns %TRUE if a property has been registered, if @pspec or
 * @parse_func are not %NULL, the #GParamSpec and parsing function
 * will be respectively returned.
 *
 * Returns: %TRUE if the property is registered, %FALSE otherwise
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: This code could only look up custom properties and
 *     those are deprecated.
 **/
gboolean
gtk_style_properties_lookup_property (const gchar             *property_name,
                                      GtkStylePropertyParser  *parse_func,
                                      GParamSpec             **pspec)
{
  /* Not Implemented */
  return FALSE;
}
