#include <gtk/gtk.h>

/* XXX everything in here is copied from gtk2, but is also here because these symobls are not exported... XXX */

static GSList *rc_contexts;

struct _GtkRcContext
{
  GHashTable *rc_style_ht;
  GtkSettings *settings;
  GSList *rc_sets_widget;
  GSList *rc_sets_widget_class;
  GSList *rc_sets_class;

  /* The files we have parsed, to reread later if necessary */
  GSList *rc_files;

  gchar *theme_name;
  gchar *key_theme_name;
  gchar *font_name;

  gchar **pixmap_path;

  gint default_priority;
  GtkStyle *default_style;

  GHashTable *color_hash;

  guint reloading : 1;
};

static gint
gtk_rc_properties_cmp (gconstpointer bsearch_node1,
                       gconstpointer bsearch_node2)
{
  const GtkRcProperty *prop1 = bsearch_node1;
  const GtkRcProperty *prop2 = bsearch_node2;

  if (prop1->type_name == prop2->type_name)
    return prop1->property_name < prop2->property_name ? -1 : prop1->property_name == prop2->property_name ? 0 : 1;
  else
    return prop1->type_name < prop2->type_name ? -1 : 1;
}

const GtkRcProperty*
_gtk_rc_style_lookup_rc_property (GtkRcStyle *rc_style,
                                  GQuark      type_name,
                                  GQuark      property_name)
{
  GtkRcProperty *node = NULL;

  g_return_val_if_fail (GTK_IS_RC_STYLE (rc_style), NULL);

  if (rc_style->rc_properties)
    {
      GtkRcProperty key;

      key.type_name = type_name;
      key.property_name = property_name;

      node = bsearch (&key,
                      rc_style->rc_properties->data, rc_style->rc_properties->len,
                      sizeof (GtkRcProperty), gtk_rc_properties_cmp);
    }

  return node;
}

static void
gtk_rc_color_hash_changed (GtkSettings  *settings,
                           GParamSpec   *pspec,
                           GtkRcContext *context)
{
  GHashTable *old_hash;

  old_hash = context->color_hash;

  g_object_get (settings, "color-hash", &context->color_hash, NULL);

  if (old_hash)
    g_hash_table_unref (old_hash);

  gtk_rc_reparse_all_for_settings (settings, TRUE);
}

static void
gtk_rc_font_name_changed (GtkSettings  *settings,
                          GParamSpec   *pspec,
                          GtkRcContext *context)
{
  if (!context->reloading)
    _gtk_rc_context_get_default_font_name (settings);
}

static void
gtk_rc_settings_changed (GtkSettings  *settings,
                         GParamSpec   *pspec,
                         GtkRcContext *context)
{
  gchar *new_theme_name;
  gchar *new_key_theme_name;

  if (context->reloading)
    return;

  g_object_get (settings,
                "gtk-theme-name", &new_theme_name,
                "gtk-key-theme-name", &new_key_theme_name,
                NULL);

  if ((new_theme_name != context->theme_name &&
       !(new_theme_name && context->theme_name && strcmp (new_theme_name, context->theme_name) == 0)) ||
      (new_key_theme_name != context->key_theme_name &&
       !(new_key_theme_name && context->key_theme_name && strcmp (new_key_theme_name, context->key_theme_name) == 0)))
    {
      gtk_rc_reparse_all_for_settings (settings, TRUE);
    }

  g_free (new_theme_name);
  g_free (new_key_theme_name);
}

static GtkRcContext *
gtk_rc_context_get (GtkSettings *settings)
{
  if (!settings->rc_context)
    {
      GtkRcContext *context = settings->rc_context = g_new (GtkRcContext, 1);

      context->settings = settings;
      context->rc_style_ht = NULL;
      context->rc_sets_widget = NULL;
      context->rc_sets_widget_class = NULL;
      context->rc_sets_class = NULL;
      context->rc_files = NULL;
      context->default_style = NULL;
      context->reloading = FALSE;

      g_object_get (settings,
                    "gtk-theme-name", &context->theme_name,
                    "gtk-key-theme-name", &context->key_theme_name,
                    "gtk-font-name", &context->font_name,
                    "color-hash", &context->color_hash,
                    NULL);

      g_signal_connect (settings,
                        "notify::gtk-theme-name",
                        G_CALLBACK (gtk_rc_settings_changed),
                        context);
      g_signal_connect (settings,
                        "notify::gtk-key-theme-name",
                        G_CALLBACK (gtk_rc_settings_changed),
                        context);
      g_signal_connect (settings,
                        "notify::gtk-font-name",
                        G_CALLBACK (gtk_rc_font_name_changed),
                        context);
      g_signal_connect (settings,
                        "notify::color-hash",
                        G_CALLBACK (gtk_rc_color_hash_changed),
                        context);

      context->pixmap_path = NULL;

      context->default_priority = GTK_PATH_PRIO_RC;

      rc_contexts = g_slist_prepend (rc_contexts, settings->rc_context);
    }

  return settings->rc_context;
}

const gchar*
_gtk_rc_context_get_default_font_name (GtkSettings *settings)
{
  GtkRcContext *context;
  gchar *new_font_name;

  g_return_val_if_fail (GTK_IS_SETTINGS (settings), NULL);

  context = gtk_rc_context_get (settings);

  g_object_get (context->settings,
                "gtk-font-name", &new_font_name,
                NULL);

  if (new_font_name != context->font_name && !(new_font_name && strcmp (context->font_name, new_font_name) == 0))
    {
       g_free (context->font_name);
       context->font_name = g_strdup (new_font_name);

       gtk_rc_reset_styles (settings);
    }

  g_free (new_font_name);

  return context->font_name;
}
