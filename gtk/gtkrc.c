#include <gtk/gtk.h>

/* XXX everything in here is copied from gtk2, but is also here because these symobls are not exported... XXX */

#ifndef GTK_DATA_PREFIX
#define GTK_DATA_PREFIX "/usr"
#endif

#ifndef GTK_SYSCONFDIR
#define GTK_SYSCONFDIR "/usr/share"
#endif

typedef struct _GtkRcSet    GtkRcSet;
typedef struct _GtkRcNode   GtkRcNode;
typedef struct _GtkRcFile   GtkRcFile;

enum
{
  PATH_ELT_PSPEC,
  PATH_ELT_UNRESOLVED,
  PATH_ELT_TYPE
};

typedef struct
{
  gint type;
  union
  {
    GType         class_type;
    gchar        *class_name;
    GPatternSpec *pspec;
  } elt;
} PathElt;

struct _GtkRcSet
{
  GtkPathType   type;

  GPatternSpec *pspec;
  GSList       *path;

  GtkRcStyle   *rc_style;
  gint          priority;
};

struct _GtkRcFile
{
  time_t mtime;
  gchar *name;
  gchar *canonical_name;
  gchar *directory;
  guint  reload    : 1;
  guint  is_string : 1; /* If TRUE, name is a string to parse with gtk_rc_parse_string() */
};


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

typedef struct _GtkRcStylePrivate GtkRcStylePrivate;

struct _GtkRcStylePrivate
{
  GSList *color_hashes;
};

#if 0 /* unused */
static GHashTable *realized_style_ht = NULL;
#endif

#if 0 /* unused */
static gchar *im_module_file = NULL;
#endif

static gint    max_default_files = 0;
static gchar **gtk_rc_default_files = NULL;

/* A stack of information of RC files we are parsing currently.
 * The directories for these files are implicitely added to the end of
 * PIXMAP_PATHS.
 */
#if 0 /* unused */
static GSList *current_files_stack = NULL;
#endif

/* RC files and strings that are parsed for every context
 */
#if 0 /* unused */
static GSList *global_rc_files = NULL;
#endif

/* Keep list of all current RC contexts for convenience
 */
static GSList *rc_contexts;

static void
gtk_rc_add_initial_default_files (void)
{
  static gint init = FALSE;
  const gchar *var;
  gchar *str;
  gchar **files;
  gint i;

  if (init)
    return;

  gtk_rc_default_files = g_new (gchar*, 10);
  max_default_files = 10;

  gtk_rc_default_files[0] = NULL;
  init = TRUE;

  var = g_getenv ("GTK2_RC_FILES");

  if (var)
    {
      files = g_strsplit (var, G_SEARCHPATH_SEPARATOR_S, -1);
      i=0;
      while (files[i])
        {
          gtk_rc_add_default_file (files[i]);
          i++;
        }
      g_strfreev (files);
    }
  else
    {
      const gchar *home;
      const gchar * const *config_dirs;
      const gchar *config_dir;

      str = g_build_filename (GTK_DATA_PREFIX, "share", "gtk-2.0", "gtkrc", NULL);
      gtk_rc_add_default_file (str);
      g_free (str);

      config_dirs = g_get_system_config_dirs ();
      for (config_dir = *config_dirs; *config_dirs != NULL; config_dirs++)
        {
          str = g_build_filename (config_dir, "gtk-2.0", "gtkrc", NULL);
          gtk_rc_add_default_file (str);
          g_free (str);
        }

      str = g_build_filename (GTK_SYSCONFDIR, "gtk-2.0", "gtkrc", NULL);
      gtk_rc_add_default_file (str);
      g_free (str);

      home = g_get_home_dir ();
      if (home)
        {
          str = g_build_filename (home, ".gtkrc-2.0", NULL);
          gtk_rc_add_default_file (str);
          g_free (str);
        }
    }
}

void
_gtk_rc_init (void)
{
  static gboolean initialized = FALSE;

  if (!initialized)
    {
      initialized = TRUE;

      gtk_rc_add_initial_default_files ();
    }

  /* Default RC string */
  gtk_rc_parse_string ("style \"gtk-default-tooltips-style\" {\n"
                       "  bg[NORMAL] = \"#eee1b3\"\n"
                       "  fg[NORMAL] = \"#000000\"\n"
                       "}\n"
                       "\n"
                       "style \"gtk-default-progress-bar-style\" {\n"
                       "  bg[PRELIGHT] = \"#4b6983\"\n"
                       "  fg[PRELIGHT] = \"#ffffff\"\n"
                       "  bg[NORMAL]   = \"#c4c2bd\"\n"
                       "}\n"
                       "\n"
                       "style \"gtk-default-entry-style\" {\n"
                       "  bg[SELECTED] = \"#b7c3cd\"\n"
                       "  fg[SELECTED] = \"#000000\"\n"
                       "}\n"
                       "\n"
                       "style \"gtk-default-menu-bar-item-style\" {\n"
                       "  GtkMenuItem::horizontal_padding = 5\n"
                       "}\n"
                       "\n"
                       "style \"gtk-default-menu-item-style\" {\n"
                       "  bg[PRELIGHT] = \"#4b6983\"\n"
                       "  fg[PRELIGHT] = \"#ffffff\"\n"
                       "  base[PRELIGHT] = \"#4b6983\"\n"
                       "  text[PRELIGHT] = \"#ffffff\"\n"
                       "}\n"
                       "\n"
                       /* Work around clipping of accelerator underlines */
                       "style \"gtk-default-label-style\" {\n"
                       "  GtkWidget::draw-border = {0,0,0,1}\n"
                       "}\n"
                       "\n"
                       "class \"GtkProgressBar\" style : gtk \"gtk-default-progress-bar-style\"\n"
                       "class \"GtkEntry\" style : gtk \"gtk-default-entry-style\"\n"
                       "widget \"gtk-tooltip*\" style : gtk \"gtk-default-tooltips-style\"\n"
                       "widget_class \"*<GtkMenuItem>*\" style : gtk \"gtk-default-menu-item-style\"\n"
                       "widget_class \"*<GtkMenuBar>*<GtkMenuItem>\" style : gtk \"gtk-default-menu-bar-item-style\"\n"
                       "class \"GtkLabel\" style : gtk \"gtk-default-label-style\"\n"
      );
}

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
insert_rc_property (GtkRcStyle    *style,
                    GtkRcProperty *property,
                    gboolean       replace)
{
  guint i;
  GtkRcProperty *new_property = NULL;
  GtkRcProperty key = { 0, 0, NULL, { 0, }, };

  key.type_name = property->type_name;
  key.property_name = property->property_name;

  if (!style->rc_properties)
    style->rc_properties = g_array_new (FALSE, FALSE, sizeof (GtkRcProperty));

  i = 0;
  while (i < style->rc_properties->len)
    {
      gint cmp = gtk_rc_properties_cmp (&key, &g_array_index (style->rc_properties, GtkRcProperty, i));

      if (cmp == 0)
        {
          if (replace)
            {
              new_property = &g_array_index (style->rc_properties, GtkRcProperty, i);

              g_free (new_property->origin);
              g_value_unset (&new_property->value);

              *new_property = key;
              break;
            }
          else
            return;
        }
      else if (cmp < 0)
        break;

      i++;
    }

  if (!new_property)
    {
      g_array_insert_val (style->rc_properties, i, key);
      new_property = &g_array_index (style->rc_properties, GtkRcProperty, i);
    }

  new_property->origin = g_strdup (property->origin);
  g_value_init (&new_property->value, G_VALUE_TYPE (&property->value));
  g_value_copy (&property->value, &new_property->value);
}

void
_gtk_rc_style_set_rc_property (GtkRcStyle *rc_style,
                               GtkRcProperty *property)
{
  g_return_if_fail (GTK_IS_RC_STYLE (rc_style));
  g_return_if_fail (property != NULL);

  insert_rc_property (rc_style, property, TRUE);
}

void
_gtk_rc_style_unset_rc_property (GtkRcStyle *rc_style,
                                 GQuark      type_name,
                                 GQuark      property_name)
{
  GtkRcProperty *node;

  g_return_if_fail (GTK_IS_RC_STYLE (rc_style));

  node = (GtkRcProperty *) _gtk_rc_style_lookup_rc_property (rc_style,
                                                             type_name,
                                                             property_name);

  if (node != NULL)
    {
      guint index = node - (GtkRcProperty *) rc_style->rc_properties->data;
      g_value_unset (&node->value);
      g_free (node->origin);
      g_array_remove_index (rc_style->rc_properties, index);
    }
}

#if 0 /* goes too deep into gtkrc internals */
extern void* gtk_rc_style_get_instance_private (void*);
#define GTK_RC_STYLE_GET_PRIVATE(obj) ((GtkRcStylePrivate*)gtk_rc_style_get_instance_private ((GtkRcStyle*)obj))

GSList *
_gtk_rc_style_get_color_hashes (GtkRcStyle *rc_style)
{
  GtkRcStylePrivate *priv = GTK_RC_STYLE_GET_PRIVATE (rc_style);

  return priv->color_hashes;
}
#endif

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

GSList *
_gtk_rc_parse_widget_class_path (const gchar *pattern)
{
  GSList *result;
  PathElt *path_elt;
  const gchar *current;
  const gchar *class_start;
  const gchar *class_end;
  const gchar *pattern_end;
  const gchar *pattern_start;
  gchar *sub_pattern;

  result = NULL;
  current = pattern;
  while ((class_start = strchr (current, '<')) &&
         (class_end = strchr (class_start, '>')))
    {
      /* Add patterns, but ignore single dots */
      if (!(class_start == current ||
            (class_start == current + 1 && current[0] == '.')))
        {
          pattern_end = class_start - 1;
          pattern_start = current;

          path_elt = g_new (PathElt, 1);

          sub_pattern = g_strndup (pattern_start, pattern_end - pattern_start + 1);
          path_elt->type = PATH_ELT_PSPEC;
          path_elt->elt.pspec = g_pattern_spec_new (sub_pattern);
          g_free (sub_pattern);

          result = g_slist_prepend (result, path_elt);
        }

      path_elt = g_new (PathElt, 1);

      /* The < > need to be removed from the string. */
      sub_pattern = g_strndup (class_start + 1, class_end - class_start - 1);

      path_elt->type = PATH_ELT_UNRESOLVED;
      path_elt->elt.class_name = sub_pattern;

      result = g_slist_prepend (result, path_elt);

      current = class_end + 1;
    }

  /* Add the rest, if anything is left */
  if (strlen (current) > 0)
    {
      path_elt = g_new (PathElt, 1);
      path_elt->type = PATH_ELT_PSPEC;
      path_elt->elt.pspec = g_pattern_spec_new (current);

      result = g_slist_prepend (result, path_elt);
    }

  return g_slist_reverse (result);
}

static void
free_path_elt (gpointer data,
               gpointer user_data)
{
  PathElt *path_elt = data;

  switch (path_elt->type)
    {
    case PATH_ELT_PSPEC:
      g_pattern_spec_free (path_elt->elt.pspec);
      break;
    case PATH_ELT_UNRESOLVED:
      g_free (path_elt->elt.class_name);
      break;
    case PATH_ELT_TYPE:
      break;
    default:
      g_assert_not_reached ();
    }

  g_free (path_elt);
}

void
_gtk_rc_free_widget_class_path (GSList *list)
{
  g_slist_foreach (list, free_path_elt, NULL);
  g_slist_free (list);
}

static gboolean
match_widget_class_recursive (GSList *list,
                              guint   length,
                              gchar  *path,
                              gchar  *path_reversed);

gboolean
_gtk_rc_match_widget_class (GSList  *list,
                            gint     length,
                            gchar   *path,
                            gchar   *path_reversed)
{
  return match_widget_class_recursive (list, length, path, path_reversed);
}

static gboolean
match_class (PathElt *path_elt,
             gchar   *type_name)
{
  GType type;

  if (path_elt->type == PATH_ELT_UNRESOLVED)
    {
      type = g_type_from_name (path_elt->elt.class_name);
      if (type != G_TYPE_INVALID)
        {
          g_free (path_elt->elt.class_name);
          path_elt->elt.class_type = type;
          path_elt->type = PATH_ELT_TYPE;
        }
      else
        return g_str_equal (type_name, path_elt->elt.class_name);
    }

  return g_type_is_a (g_type_from_name (type_name), path_elt->elt.class_type);
}

static gboolean
match_widget_class_recursive (GSList *list,
                              guint   length,
                              gchar  *path,
                              gchar  *path_reversed)
{
  PathElt *path_elt;

  /* break out if we cannot match anymore. */
  if (list == NULL)
    {
      if (length > 0)
        return FALSE;
      else
        return TRUE;
    }

  /* there are two possibilities:
   *  1. The next pattern should match the class.
   *  2. First normal matching, and then maybe a class */

  path_elt = list->data;

  if (path_elt->type != PATH_ELT_PSPEC)
    {
      gchar *class_start = path;
      gchar *class_end;

      /* ignore leading dot */
      if (class_start[0] == '.')
        class_start++;
      class_end = strchr (class_start, '.');

      if (class_end == NULL)
        {
          if (!match_class (path_elt, class_start))
            return FALSE;
          else
            return match_widget_class_recursive (list->next, 0, "", "");
        }
      else
        {
          class_end[0] = '\0';
          if (!match_class (path_elt, class_start))
            {
              class_end[0] = '.';
              return FALSE;
            }
          else
            {
              gboolean result;
              gint new_length = length - (class_end - path);
              gchar old_char = path_reversed[new_length];

              class_end[0] = '.';

              path_reversed[new_length] = '\0';
              result = match_widget_class_recursive (list->next, new_length, class_end, path_reversed);
              path_reversed[new_length] = old_char;

              return result;
            }
        }
    }
  else
    {
      PathElt *class_elt;
      gchar *class_start;
      gchar *class_end;
      gboolean result = FALSE;

      /* If there is nothing after this (ie. no class match),
       * just compare the pspec.
       */
      if (list->next == NULL)
        return g_pattern_spec_match (path_elt->elt.pspec, length, path, path_reversed);

      class_elt = (PathElt *)list->next->data;
      g_assert (class_elt->type != PATH_ELT_PSPEC);

      class_start = path;
      if (class_start[0] == '.')
        class_start++;

      while (TRUE)
        {
          class_end = strchr (class_start, '.');

          /* It should be cheaper to match the class first. (either the pattern
           * is simple, and will match most of the times, or it may be complex
           * and matching is slow)
           */
          if (class_end == NULL)
            {
              result = match_class (class_elt, class_start);
            }
          else
            {
              class_end[0] = '\0';
              result = match_class (class_elt, class_start);
              class_end[0] = '.';
            }

          if (result)
            {
              gchar old_char;
              result = FALSE;

              /* terminate the string in front of the class. It does not matter
               * that the class becomes unusable, because it is not needed
               * inside the recursion
               */
              old_char = class_start[0];
              class_start[0] = '\0';

              if (g_pattern_spec_match (path_elt->elt.pspec, class_start - path, path, path_reversed + length - (class_start - path)))
                {
                  if (class_end != NULL)
                    {
                      gint new_length = length - (class_end - path);
                      gchar path_reversed_char = path_reversed[new_length];

                      path_reversed[new_length] = '\0';

                      result = match_widget_class_recursive (list->next->next, new_length, class_end, path_reversed);

                      path_reversed[new_length] = path_reversed_char;
                    }
                  else
                    result = match_widget_class_recursive (list->next->next, 0, "", "");
                }

              class_start[0] = old_char;
            }

          if (result)
            return TRUE;

          /* get next class in path, or break out */
          if (class_end != NULL)
            class_start = class_end + 1;
          else
            return FALSE;
        }
    }
}

static void
gtk_rc_clear_rc_files (GtkRcContext *context)
{
  GSList *list;

  list = context->rc_files;
  while (list)
    {
      GtkRcFile *rc_file = list->data;

      if (rc_file->canonical_name != rc_file->name)
        g_free (rc_file->canonical_name);
      g_free (rc_file->directory);
      g_free (rc_file->name);
      g_free (rc_file);

      list = list->next;
    }

  g_slist_free (context->rc_files);
  context->rc_files = NULL;
}

static void
gtk_rc_set_free (GtkRcSet *rc_set)
{
  if (rc_set->pspec)
    g_pattern_spec_free (rc_set->pspec);

  _gtk_rc_free_widget_class_path (rc_set->path);

  g_free (rc_set);
}

static void
gtk_rc_free_rc_sets (GSList *slist)
{
  while (slist)
    {
      GtkRcSet *rc_set;

      rc_set = slist->data;
      gtk_rc_set_free (rc_set);

      slist = slist->next;
    }
}

static void
gtk_rc_clear_hash_node (gpointer key,
                        gpointer data,
                        gpointer user_data)
{
  g_object_unref (data);
}

static void
gtk_rc_clear_styles (GtkRcContext *context)
{
  /* Clear out all old rc_styles */

  if (context->rc_style_ht)
    {
      g_hash_table_foreach (context->rc_style_ht, gtk_rc_clear_hash_node, NULL);
      g_hash_table_destroy (context->rc_style_ht);
      context->rc_style_ht = NULL;
    }

  gtk_rc_free_rc_sets (context->rc_sets_widget);
  g_slist_free (context->rc_sets_widget);
  context->rc_sets_widget = NULL;

  gtk_rc_free_rc_sets (context->rc_sets_widget_class);
  g_slist_free (context->rc_sets_widget_class);
  context->rc_sets_widget_class = NULL;

  gtk_rc_free_rc_sets (context->rc_sets_class);
  g_slist_free (context->rc_sets_class);
  context->rc_sets_class = NULL;
}

void
_gtk_rc_context_destroy (GtkSettings *settings)
{
  GtkRcContext *context;

  g_return_if_fail (GTK_IS_SETTINGS (settings));

  context = settings->rc_context;
  if (!context)
    return;

  _gtk_settings_reset_rc_values (context->settings);
  gtk_rc_clear_styles (context);
  gtk_rc_clear_rc_files (context);

  if (context->default_style)
    g_object_unref (context->default_style);

  g_strfreev (context->pixmap_path);

  g_free (context->theme_name);
  g_free (context->key_theme_name);
  g_free (context->font_name);

  if (context->color_hash)
    g_hash_table_unref (context->color_hash);

  g_signal_handlers_disconnect_by_func (settings,
                                        gtk_rc_settings_changed, context);
  g_signal_handlers_disconnect_by_func (settings,
                                        gtk_rc_font_name_changed, context);
  g_signal_handlers_disconnect_by_func (settings,
                                        gtk_rc_color_hash_changed, context);

  rc_contexts = g_slist_remove (rc_contexts, context);

  g_free (context);

  settings->rc_context = NULL;
}
