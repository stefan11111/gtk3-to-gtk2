#include <gtk/gtk.h>

struct _GtkBuilderPrivate
{
  gchar *domain;
  GHashTable *objects;
  GSList *delayed_properties;
  GSList *signals;
  gchar *filename;
};

/**
 * gtk_builder_add_from_resource:
 * @builder: a #GtkBuilder
 * @resource_path: the path of the resource file to parse
 * @error: (allow-none): return location for an error, or %NULL
 *
 * Parses a resource file containing a [GtkBuilder UI definition][BUILDER-UI]
 * and merges it with the current contents of @builder.
 *
 * Most users will probably want to use gtk_builder_new_from_resource().
 *
 * If an error occurs, 0 will be returned and @error will be assigned a
 * #GError from the #GTK_BUILDER_ERROR, #G_MARKUP_ERROR or #G_RESOURCE_ERROR
 * domain.
 *
 * It’s not really reasonable to attempt to handle failures of this
 * call.  The only reasonable thing to do when an error is detected is
 * to call g_error().
 *
 * Returns: A positive value on success, 0 if an error occurred
 *
 * Since: 3.4
 **/
guint
gtk_builder_add_from_resource (GtkBuilder   *builder,
                               const gchar  *resource_path,
                               GError      **error)
{
  GError *tmp_error;
  GBytes *data;
  char *filename_for_errors;

  g_return_val_if_fail (GTK_IS_BUILDER (builder), 0);
  g_return_val_if_fail (resource_path != NULL, 0);
  g_return_val_if_fail (error == NULL || *error == NULL, 0);

  tmp_error = NULL;

  data = g_resources_lookup_data (resource_path, 0, &tmp_error);
  if (data == NULL)
    {
      g_propagate_error (error, tmp_error);
      return 0;
    }

  g_free (builder->priv->filename);

  builder->priv->filename = g_strdup (".");

  filename_for_errors = g_strconcat ("<resource>", resource_path, NULL);

  gtk2_gtk_builder_parser_parse_buffer (builder, filename_for_errors,
                                    g_bytes_get_data (data, NULL), g_bytes_get_size (data),
                                    NULL,
                                    &tmp_error);

  g_free (filename_for_errors);
  g_bytes_unref (data);

  if (tmp_error != NULL)
    {
      g_propagate_error (error, tmp_error);
      return 0;
    }

  return 1;
}

/**
 * gtk_builder_new_from_resource:
 * @resource_path: a #GResource resource path
 *
 * Builds the [GtkBuilder UI definition][BUILDER-UI]
 * at @resource_path.
 *
 * If there is an error locating the resource or parsing the
 * description, then the program will be aborted.
 *
 * Returns: a #GtkBuilder containing the described interface
 *
 * Since: 3.10
 **/
GtkBuilder *
gtk_builder_new_from_resource (const gchar *resource_path)
{
  GError *error = NULL;
  GtkBuilder *builder;

  builder = gtk_builder_new ();
  if (!gtk_builder_add_from_resource (builder, resource_path, &error))
    g_error ("failed to add UI: %s", error->message);

  return builder;
}

