/* GTK - The GIMP Toolkit
 * Copyright (C) 2010 Carlos Garnacho <carlosg@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtkstyleproperties.h>

/**
 * SECTION:gtkstyleproperties
 * @Short_description: Store for style property information
 * @Title: GtkStyleProperties
 *
 * GtkStyleProperties provides the storage for style information
 * that is used by #GtkStyleContext and other #GtkStyleProvider
 * implementations.
 *
 * Before style properties can be stored in GtkStyleProperties, they
 * must be registered with gtk_style_properties_register_property().
 *
 * Unless you are writing a #GtkStyleProvider implementation, you
 * are unlikely to use this API directly, as gtk_style_context_get()
 * and its variants are the preferred way to access styling information
 * from widget implementations and theming engine implementations
 * should use the APIs provided by #GtkThemingEngine instead.
 *
 * #GtkStyleProperties has been deprecated in GTK 3.16. The CSS
 * machinery does not use it anymore and all users of this object
 * have been deprecated.
 */

/* XXX Nothing in here is implemented XXX */

G_DEFINE_TYPE (GtkStyleProperties, gtk_style_properties, G_TYPE_OBJECT);


static void
gtk_style_properties_class_init (GtkStylePropertiesClass *klass)
{
}

static void
gtk_style_properties_init (GtkStyleProperties *props)
{
}

/* GtkStyleProperties methods */

/**
 * gtk_style_properties_new:
 *
 * Returns a newly created #GtkStyleProperties
 *
 * Returns: a new #GtkStyleProperties
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
GtkStyleProperties *
gtk_style_properties_new (void)
{
  return g_object_new (GTK_TYPE_STYLE_PROPERTIES, NULL);
}

/**
 * gtk_style_properties_map_color:
 * @props: a #GtkStyleProperties
 * @name: color name
 * @color: #GtkSymbolicColor to map @name to
 *
 * Maps @color so it can be referenced by @name. See
 * gtk_style_properties_lookup_color()
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
void
gtk_style_properties_map_color (GtkStyleProperties *props,
                                const gchar        *name,
                                GtkSymbolicColor   *color)
{
}

/**
 * gtk_style_properties_lookup_color:
 * @props: a #GtkStyleProperties
 * @name: color name to lookup
 *
 * Returns the symbolic color that is mapped
 * to @name.
 *
 * Returns: (transfer none): The mapped color
 *
 * Since: 3.0
 *
 * Deprecated: 3.8: #GtkSymbolicColor is deprecated.
 **/
GtkSymbolicColor *
gtk_style_properties_lookup_color (GtkStyleProperties *props,
                                   const gchar        *name)
{
  return NULL;
}

#if 0 /* Not Needed */
void
_gtk_style_properties_set_property_by_property (GtkStyleProperties  *props,
                                                GtkCssStyleProperty *style_prop,
                                                GtkStateFlags        state,
                                                GtkCssValue         *value)
{
}
#endif

/**
 * gtk_style_properties_set_property:
 * @props: a #GtkStyleProperties
 * @property: styling property to set
 * @state: state to set the value for
 * @value: new value for the property
 *
 * Sets a styling property in @props.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_set_property (GtkStyleProperties *props,
                                   const gchar        *property,
                                   GtkStateFlags       state,
                                   const GValue       *value)
{
}

/**
 * gtk_style_properties_set_valist:
 * @props: a #GtkStyleProperties
 * @state: state to set the values for
 * @args: va_list of property name/value pairs, followed by %NULL
 *
 * Sets several style properties on @props.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_set_valist (GtkStyleProperties *props,
                                 GtkStateFlags       state,
                                 va_list             args)
{
}

/**
 * gtk_style_properties_set:
 * @props: a #GtkStyleProperties
 * @state: state to set the values for
 * @...: property name/value pairs, followed by %NULL
 *
 * Sets several style properties on @props.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_set (GtkStyleProperties *props,
                          GtkStateFlags       state,
                          ...)
{
}

/**
 * gtk_style_properties_get_property:
 * @props: a #GtkStyleProperties
 * @property: style property name
 * @state: state to retrieve the property value for
 * @value: (out) (transfer full):  return location for the style property value.
 *
 * Gets a style property from @props for the given state. When done with @value,
 * g_value_unset() needs to be called to free any allocated memory.
 *
 * Returns: %TRUE if the property exists in @props, %FALSE otherwise
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
gboolean
gtk_style_properties_get_property (GtkStyleProperties *props,
                                   const gchar        *property,
                                   GtkStateFlags       state,
                                   GValue             *value)
{
  return FALSE;
}

/**
 * gtk_style_properties_get_valist:
 * @props: a #GtkStyleProperties
 * @state: state to retrieve the property values for
 * @args: va_list of property name/return location pairs, followed by %NULL
 *
 * Retrieves several style property values from @props for a given state.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_get_valist (GtkStyleProperties *props,
                                 GtkStateFlags       state,
                                 va_list             args)
{
}

/**
 * gtk_style_properties_get:
 * @props: a #GtkStyleProperties
 * @state: state to retrieve the property values for
 * @...: property name /return value pairs, followed by %NULL
 *
 * Retrieves several style property values from @props for a
 * given state.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_get (GtkStyleProperties *props,
                          GtkStateFlags       state,
                          ...)
{
}

/**
 * gtk_style_properties_unset_property:
 * @props: a #GtkStyleProperties
 * @property: property to unset
 * @state: state to unset
 *
 * Unsets a style property in @props.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_unset_property (GtkStyleProperties *props,
                                     const gchar        *property,
                                     GtkStateFlags       state)
{
}

/**
 * gtk_style_properties_clear:
 * @props: a #GtkStyleProperties
 *
 * Clears all style information from @props.
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_clear (GtkStyleProperties *props)
{
}

/**
 * gtk_style_properties_merge:
 * @props: a #GtkStyleProperties
 * @props_to_merge: a second #GtkStyleProperties
 * @replace: whether to replace values or not
 *
 * Merges into @props all the style information contained
 * in @props_to_merge. If @replace is %TRUE, the values
 * will be overwritten, if it is %FALSE, the older values
 * will prevail.
 *
 * Since: 3.0
 *
 * Deprecated: 3.16: #GtkStyleProperties are deprecated.
 **/
void
gtk_style_properties_merge (GtkStyleProperties       *props,
                            const GtkStyleProperties *props_to_merge,
                            gboolean                  replace)
{
}
