#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "gtkstylecascadeprivate.h"

#define DEFAULT_KEY_THEME NULL

#define DEFAULT_TIMEOUT_INITIAL 200
#define DEFAULT_TIMEOUT_REPEAT   20
#define DEFAULT_TIMEOUT_EXPAND  500

typedef struct _GtkSettingsValuePrivate GtkSettingsValuePrivate;

typedef enum
{
  GTK_SETTINGS_SOURCE_DEFAULT,
  GTK_SETTINGS_SOURCE_RC_FILE,
  GTK_SETTINGS_SOURCE_XSETTING,
  GTK_SETTINGS_SOURCE_APPLICATION
} GtkSettingsSource;

struct _GtkSettingsValuePrivate
{
  GtkSettingsValue public;
  GtkSettingsSource source;
};

struct _GtkSettingsPropertyValue
{
  GValue value;
  GtkSettingsSource source;
};

enum {
  PROP_0,
  PROP_DOUBLE_CLICK_TIME,
  PROP_DOUBLE_CLICK_DISTANCE,
  PROP_CURSOR_BLINK,
  PROP_CURSOR_BLINK_TIME,
  PROP_CURSOR_BLINK_TIMEOUT,
  PROP_SPLIT_CURSOR,
  PROP_THEME_NAME,
  PROP_ICON_THEME_NAME,
  PROP_FALLBACK_ICON_THEME,
  PROP_KEY_THEME_NAME,
  PROP_MENU_BAR_ACCEL,
  PROP_DND_DRAG_THRESHOLD,
  PROP_FONT_NAME,
  PROP_ICON_SIZES,
  PROP_MODULES,
#ifdef GDK_WINDOWING_X11
  PROP_XFT_ANTIALIAS,
  PROP_XFT_HINTING,
  PROP_XFT_HINTSTYLE,
  PROP_XFT_RGBA,
  PROP_XFT_DPI,
  PROP_CURSOR_THEME_NAME,
  PROP_CURSOR_THEME_SIZE,
#endif
  PROP_ALTERNATIVE_BUTTON_ORDER,
  PROP_ALTERNATIVE_SORT_ARROWS,
  PROP_SHOW_INPUT_METHOD_MENU,
  PROP_SHOW_UNICODE_MENU,
  PROP_TIMEOUT_INITIAL,
  PROP_TIMEOUT_REPEAT,
  PROP_TIMEOUT_EXPAND,
  PROP_COLOR_SCHEME,
  PROP_ENABLE_ANIMATIONS,
  PROP_TOUCHSCREEN_MODE,
  PROP_TOOLTIP_TIMEOUT,
  PROP_TOOLTIP_BROWSE_TIMEOUT,
  PROP_TOOLTIP_BROWSE_MODE_TIMEOUT,
  PROP_KEYNAV_CURSOR_ONLY,
  PROP_KEYNAV_WRAP_AROUND,
  PROP_ERROR_BELL,
  PROP_COLOR_HASH,
  PROP_FILE_CHOOSER_BACKEND,
  PROP_PRINT_BACKENDS,
  PROP_PRINT_PREVIEW_COMMAND,
  PROP_ENABLE_MNEMONICS,
  PROP_ENABLE_ACCELS,
  PROP_RECENT_FILES_LIMIT,
  PROP_IM_MODULE,
  PROP_RECENT_FILES_MAX_AGE,
  PROP_FONTCONFIG_TIMESTAMP,
  PROP_SOUND_THEME_NAME,
  PROP_ENABLE_INPUT_FEEDBACK_SOUNDS,
  PROP_ENABLE_EVENT_SOUNDS,
  PROP_ENABLE_TOOLTIPS,
  PROP_TOOLBAR_STYLE,
  PROP_TOOLBAR_ICON_SIZE,
  PROP_AUTO_MNEMONICS,
  PROP_PRIMARY_BUTTON_WARPS_SLIDER,
  PROP_BUTTON_IMAGES,
  PROP_ENTRY_SELECT_ON_FOCUS,
  PROP_ENTRY_PASSWORD_HINT_TIMEOUT,
  PROP_MENU_IMAGES,
  PROP_MENU_BAR_POPUP_DELAY,
  PROP_SCROLLED_WINDOW_PLACEMENT,
  PROP_CAN_CHANGE_ACCELS,
  PROP_MENU_POPUP_DELAY,
  PROP_MENU_POPDOWN_DELAY,
  PROP_LABEL_SELECT_ON_FOCUS,
  PROP_COLOR_PALETTE,
  PROP_IM_PREEDIT_STYLE,
  PROP_IM_STATUS_STYLE
};

typedef struct {
  GHashTable *color_hash;
  GHashTable *tables[GTK_SETTINGS_SOURCE_APPLICATION + 1];
  gchar *lastentry[GTK_SETTINGS_SOURCE_APPLICATION + 1];
} ColorSchemeData;

static GQuark            quark_property_parser = 0;

static void    settings_update_color_scheme      (GtkSettings *settings);

static gboolean
add_color_to_hash (gchar      *name,
                   GdkColor   *color,
                   GHashTable *target)
{
  GdkColor *old;

  old = g_hash_table_lookup (target, name);
  if (!old || !gdk_color_equal (old, color))
    {
      g_hash_table_insert (target, g_strdup (name), gdk_color_copy (color));

      return TRUE;
    }

  return FALSE;
}

static gboolean
add_colors_to_hash_from_string (GHashTable  *hash,
                                const gchar *colors)
{
  gchar *s, *p, *name;
  GdkColor color;
  gboolean changed = FALSE;
  gchar *copy;

  copy = g_strdup (colors);
  s = copy;
  while (s && *s)
    {
      name = s;
      p = strchr (s, ':');
      if (p)
        {
          *p = '\0';
          p++;
        }
      else
        break;

      while (*p == ' ')
        p++;

      s = p;
      while (*s)
        {
          if (*s == '\n' || *s == ';')
            {
              *s = '\0';
              s++;
              break;
            }
          s++;
        }

      if (gdk_color_parse (p, &color))
        changed |= add_color_to_hash (name, &color, hash);
    }

  g_free (copy);

  return changed;
}

static gboolean
update_color_hash (ColorSchemeData   *data,
                   const gchar       *str,
                   GtkSettingsSource  source)
{
  gboolean changed = FALSE;
  gint i;
  GHashTable *old_hash;
  GHashTableIter iter;
  gpointer name;
  gpointer color;

  if ((str == NULL || *str == '\0') &&
      (data->lastentry[source] == NULL || data->lastentry[source][0] == '\0'))
    return FALSE;

  if (str && data->lastentry[source] && strcmp (str, data->lastentry[source]) == 0)
    return FALSE;

  /* For the RC_FILE source we merge the values rather than over-writing
   * them, since multiple rc files might define independent sets of colors
   */
  if ((source != GTK_SETTINGS_SOURCE_RC_FILE) &&
      data->tables[source] && g_hash_table_size (data->tables[source]) > 0)
    {
      g_hash_table_unref (data->tables[source]);
      data->tables[source] = NULL;
      changed = TRUE; /* We can't rely on the code below since str might be "" */
    }

  if (data->tables[source] == NULL)
    data->tables[source] = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                  g_free,
                                                  (GDestroyNotify) gdk_color_free);

  g_free (data->lastentry[source]);
  data->lastentry[source] = g_strdup (str);

  changed |= add_colors_to_hash_from_string (data->tables[source], str);

  if (!changed)
    return FALSE;

  /* Rebuild the merged hash table. */
  if (data->color_hash)
    {
      old_hash = g_hash_table_new_full (g_str_hash, g_str_equal, g_free,
                                        (GDestroyNotify) gdk_color_free);

      g_hash_table_iter_init (&iter, data->color_hash);
      while (g_hash_table_iter_next (&iter, &name, &color))
        {
          g_hash_table_insert (old_hash, name, color);
          g_hash_table_iter_steal (&iter);
        }
    }
  else
    {
      old_hash = NULL;
    }

  for (i = 0; i <= GTK_SETTINGS_SOURCE_APPLICATION; i++)
    {
      if (data->tables[i])
        g_hash_table_foreach (data->tables[i], (GHFunc) add_color_to_hash,
                              data->color_hash);
    }

  if (old_hash)
    {
      /* now check if the merged hash has changed */
      changed = FALSE;
      if (g_hash_table_size (old_hash) != g_hash_table_size (data->color_hash))
        changed = TRUE;
      else
        {
          GHashTableIter iter;
          gpointer key, value, new_value;

          g_hash_table_iter_init (&iter, old_hash);
          while (g_hash_table_iter_next (&iter, &key, &value))
            {
              new_value = g_hash_table_lookup (data->color_hash, key);
              if (!new_value || !gdk_color_equal (value, new_value))
                {
                  changed = TRUE;
                  break;
                }
            }
        }

      g_hash_table_unref (old_hash);
    }
  else
    changed = TRUE;

  return changed;
}

static void
color_scheme_data_free (ColorSchemeData *data)
{
  gint i;

  g_hash_table_unref (data->color_hash);

  for (i = 0; i <= GTK_SETTINGS_SOURCE_APPLICATION; i++)
    {
      if (data->tables[i])
        g_hash_table_unref (data->tables[i]);
      g_free (data->lastentry[i]);
    }

  g_slice_free (ColorSchemeData, data);
}

static void
merge_color_scheme (GtkSettings       *settings,
                    const GValue      *value,
                    GtkSettingsSource  source)
{
  ColorSchemeData *data;
  const gchar *colors;

  g_object_freeze_notify (G_OBJECT (settings));

  colors = g_value_get_string (value);

  settings_update_color_scheme (settings);

  data = (ColorSchemeData *) g_object_get_data (G_OBJECT (settings),
                                                "gtk-color-scheme");

  if (update_color_hash (data, colors, source))
    g_object_notify (G_OBJECT (settings), "color-hash");

  g_object_thaw_notify (G_OBJECT (settings));
}

static void
settings_update_color_scheme (GtkSettings *settings)
{
  if (!g_object_get_data (G_OBJECT (settings), "gtk-color-scheme"))
    {
      ColorSchemeData *data;
      GValue value = { 0, };

      data = g_slice_new0 (ColorSchemeData);
      data->color_hash = g_hash_table_new_full (g_str_hash, g_str_equal, g_free,
                                                (GDestroyNotify) gdk_color_free);
      g_object_set_data_full (G_OBJECT (settings), "gtk-color-scheme",
                              data, (GDestroyNotify) color_scheme_data_free);

      g_value_init (&value, G_TYPE_STRING);
      if (gdk_screen_get_setting (settings->screen, "gtk-color-scheme", &value))
        {
          merge_color_scheme (settings, &value, GTK_SETTINGS_SOURCE_XSETTING);
          g_value_unset (&value);
        }
   }
}

static void
apply_queued_setting (GtkSettings             *data,
                      GParamSpec              *pspec,
                      GtkSettingsValuePrivate *qvalue)
{
  GValue tmp_value = { 0, };

  if (!quark_property_parser) {
    quark_property_parser = g_quark_from_static_string ("gtk-rc-property-parser");
  }

  GtkRcPropertyParser parser = (GtkRcPropertyParser) g_param_spec_get_qdata (pspec, quark_property_parser);

  g_value_init (&tmp_value, G_PARAM_SPEC_VALUE_TYPE (pspec));
  if (_gtk_settings_parse_convert (parser, &qvalue->public.value,
                                   pspec, &tmp_value))
    {
      if (pspec->param_id == PROP_COLOR_SCHEME)
        merge_color_scheme (data, &tmp_value, qvalue->source);

      if (data->property_values[pspec->param_id - 1].source <= qvalue->source)
        {
          g_value_copy (&tmp_value, &data->property_values[pspec->param_id - 1].value);
          data->property_values[pspec->param_id - 1].source = qvalue->source;
          g_object_notify (G_OBJECT (data), g_param_spec_get_name (pspec));
        }

    }
  else
    {
      gchar *debug = g_strdup_value_contents (&qvalue->public.value);

      g_message ("%s: failed to retrieve property `%s' of type `%s' from rc file value \"%s\" of type `%s'",
                 qvalue->public.origin ? qvalue->public.origin : "(for origin information, set GTK_DEBUG)",
                 pspec->name,
                 g_type_name (G_PARAM_SPEC_VALUE_TYPE (pspec)),
                 debug,
                 G_VALUE_TYPE_NAME (&tmp_value));
      g_free (debug);
    }
  g_value_unset (&tmp_value);
}

static void
free_value (gpointer data)
{
  GtkSettingsValuePrivate *qvalue = data;

  g_value_unset (&qvalue->public.value);
  g_free (qvalue->public.origin);
  g_slice_free (GtkSettingsValuePrivate, qvalue);
}

static void
gtk_settings_set_property_value_internal (GtkSettings            *settings,
                                          const gchar            *prop_name,
                                          const GtkSettingsValue *new_value,
                                          GtkSettingsSource       source)
{
  GtkSettingsValuePrivate *qvalue;
  GParamSpec *pspec;
  gchar *name;
  GQuark name_quark;

  if (!G_VALUE_HOLDS_LONG (&new_value->value) &&
      !G_VALUE_HOLDS_DOUBLE (&new_value->value) &&
      !G_VALUE_HOLDS_STRING (&new_value->value) &&
      !G_VALUE_HOLDS (&new_value->value, G_TYPE_GSTRING))
    {
      g_warning (G_STRLOC ": value type invalid");
      return;
    }

  name = g_strdup (prop_name);
  g_strcanon (name, G_CSET_DIGITS "-" G_CSET_a_2_z G_CSET_A_2_Z, '-');
  name_quark = g_quark_from_string (name);
  g_free (name);

  qvalue = g_datalist_id_get_data (&settings->queued_settings, name_quark);
  if (!qvalue)
    {
      qvalue = g_slice_new0 (GtkSettingsValuePrivate);
      g_datalist_id_set_data_full (&settings->queued_settings, name_quark, qvalue, free_value);
    }
  else
    {
      g_free (qvalue->public.origin);
      g_value_unset (&qvalue->public.value);
    }
  qvalue->public.origin = g_strdup (new_value->origin);
  g_value_init (&qvalue->public.value, G_VALUE_TYPE (&new_value->value));
  g_value_copy (&new_value->value, &qvalue->public.value);
  qvalue->source = source;
  pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), g_quark_to_string (name_quark));
  if (pspec)
    apply_queued_setting (settings, pspec, qvalue);
}

static void
reset_rc_values_foreach (GQuark    key_id,
                         gpointer  data,
                         gpointer  user_data)
{
  GtkSettingsValuePrivate *qvalue = data;
  GSList **to_reset = user_data;

  if (qvalue->source == GTK_SETTINGS_SOURCE_RC_FILE)
    *to_reset = g_slist_prepend (*to_reset, GUINT_TO_POINTER (key_id));
}

/* XXX everything above is copied from gtk2, but is also here because these symobls are not exported... XXX */
/* XXX so is almost everything bellow... XXX */

void
_gtk_settings_set_property_value_from_rc (GtkSettings            *settings,
                                          const gchar            *prop_name,
                                          const GtkSettingsValue *new_value)
{
  g_return_if_fail (GTK_SETTINGS (settings));
  g_return_if_fail (prop_name != NULL);
  g_return_if_fail (new_value != NULL);

  gtk_settings_set_property_value_internal (settings, prop_name, new_value,
                                            GTK_SETTINGS_SOURCE_RC_FILE);

}

void
_gtk_settings_reset_rc_values (GtkSettings *settings)
{
  GSList *to_reset = NULL;
  GSList *tmp_list;
  GParamSpec **pspecs, **p;
  gint i;

  /* Remove any queued settings
   */
  g_datalist_foreach (&settings->queued_settings,
                      reset_rc_values_foreach,
                      &to_reset);

  for (tmp_list = to_reset; tmp_list; tmp_list = tmp_list->next)
    {
      GQuark key_id = GPOINTER_TO_UINT (tmp_list->data);
      g_datalist_id_remove_data (&settings->queued_settings, key_id);
    }

   g_slist_free (to_reset);

  /* Now reset the active settings
   */
  pspecs = g_object_class_list_properties (G_OBJECT_GET_CLASS (settings), NULL);
  i = 0;

  g_object_freeze_notify (G_OBJECT (settings));
  for (p = pspecs; *p; p++)
    {
      if (settings->property_values[i].source == GTK_SETTINGS_SOURCE_RC_FILE)
        {
          GParamSpec *pspec = *p;

          g_param_value_set_default (pspec, &settings->property_values[i].value);
          g_object_notify (G_OBJECT (settings), pspec->name);
        }
      i++;
    }
  g_object_thaw_notify (G_OBJECT (settings));
  g_free (pspecs);
}

void
_gtk_settings_handle_event (GdkEventSetting *event)
{
  GtkSettings *settings;
  GParamSpec *pspec;
  guint property_id;

  settings = gtk_settings_get_for_screen (gdk_window_get_screen (event->window));
  pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), event->name);

  if (pspec)
    {
      property_id = pspec->param_id;

      if (property_id == PROP_COLOR_SCHEME)
        {
          GValue value = { 0, };

          g_value_init (&value, G_TYPE_STRING);
          if (!gdk_screen_get_setting (settings->screen, pspec->name, &value))
            g_value_set_static_string (&value, "");
          merge_color_scheme (settings, &value, GTK_SETTINGS_SOURCE_XSETTING);
          g_value_unset (&value);
        }
      g_object_notify (G_OBJECT (settings), pspec->name);
   }
}

GtkRcPropertyParser
_gtk_rc_property_parser_from_type (GType type)
{
  if (type == GDK_TYPE_COLOR)
    return gtk_rc_property_parse_color;
  else if (type == GTK_TYPE_REQUISITION)
    return gtk_rc_property_parse_requisition;
  else if (type == GTK_TYPE_BORDER)
    return gtk_rc_property_parse_border;
  else if (G_TYPE_FUNDAMENTAL (type) == G_TYPE_ENUM && G_TYPE_IS_DERIVED (type))
    return gtk_rc_property_parse_enum;
  else if (G_TYPE_FUNDAMENTAL (type) == G_TYPE_FLAGS && G_TYPE_IS_DERIVED (type))
    return gtk_rc_property_parse_flags;
  else
    return NULL;
}

gboolean
_gtk_settings_parse_convert (GtkRcPropertyParser parser,
                             const GValue       *src_value,
                             GParamSpec         *pspec,
                             GValue             *dest_value)
{
  gboolean success = FALSE;

  g_return_val_if_fail (G_VALUE_HOLDS (dest_value, G_PARAM_SPEC_VALUE_TYPE (pspec)), FALSE);

  if (parser)
    {
      GString *gstring;
      gboolean free_gstring = TRUE;

      if (G_VALUE_HOLDS (src_value, G_TYPE_GSTRING))
        {
          gstring = g_value_get_boxed (src_value);
          free_gstring = FALSE;
        }
      else if (G_VALUE_HOLDS_LONG (src_value))
        {
          gstring = g_string_new (NULL);
          g_string_append_printf (gstring, "%ld", g_value_get_long (src_value));
        }
      else if (G_VALUE_HOLDS_DOUBLE (src_value))
        {
          gstring = g_string_new (NULL);
          g_string_append_printf (gstring, "%f", g_value_get_double (src_value));
        }
      else if (G_VALUE_HOLDS_STRING (src_value))
        {
          gchar *tstr = g_strescape (g_value_get_string (src_value), NULL);

          gstring = g_string_new ("\"");
          g_string_append (gstring, tstr);
          g_string_append_c (gstring, '\"');
          g_free (tstr);
        }
      else
        {
          g_return_val_if_fail (G_VALUE_HOLDS (src_value, G_TYPE_GSTRING), FALSE);
          gstring = NULL; /* silence compiler */
        }

      success = (parser (pspec, gstring, dest_value) &&
                 !g_param_value_validate (pspec, dest_value));

      if (free_gstring)
        g_string_free (gstring, TRUE);
    }
  else if (G_VALUE_HOLDS (src_value, G_TYPE_GSTRING))
    {
      if (G_VALUE_HOLDS (dest_value, G_TYPE_STRING))
        {
          GString *gstring = g_value_get_boxed (src_value);

          g_value_set_string (dest_value, gstring ? gstring->str : NULL);
          success = !g_param_value_validate (pspec, dest_value);
        }
    }
  else if (g_value_type_transformable (G_VALUE_TYPE (src_value), G_VALUE_TYPE (dest_value)))
    success = g_param_value_convert (pspec, src_value, dest_value, TRUE);

  return success;
}

GdkScreen *
_gtk_settings_get_screen (GtkSettings *settings)
{
  /* breaks GSEAL */
  return settings->screen;
}

GtkStyleCascade *
_gtk_settings_get_style_cascade (GtkSettings *settings,
                                 gint         scale)
{
  static GtkStyleCascade *ret;
  if (!ret) { /* might 'leak' memory for one GtkStyleCascade */
    ret = _gtk_style_cascade_new ();
  }
  return ret;
}

GtkSettingsSource
_gtk_settings_get_setting_source (GtkSettings *settings,
                                  const gchar *name)
{
  GtkSettingsValuePrivate *qvalue;

  qvalue = g_datalist_get_data (&settings->queued_settings, name);
  return qvalue ? qvalue->source : GTK_SETTINGS_SOURCE_DEFAULT;
}

static gboolean
settings_update_xsetting (GtkSettings *settings,
                          GParamSpec  *pspec,
                          gboolean     force)
{
  GType value_type;
  GType fundamental_type;
  gboolean retval = FALSE;

  if (settings->property_values[pspec->param_id - 1].source == GTK_SETTINGS_SOURCE_APPLICATION)
    return FALSE;

  if (settings->property_values[pspec->param_id - 1].source == GTK_SETTINGS_SOURCE_XSETTING && !force)
    return FALSE;

  value_type = G_PARAM_SPEC_VALUE_TYPE (pspec);
  fundamental_type = G_TYPE_FUNDAMENTAL (value_type);

  if ((g_value_type_transformable (G_TYPE_INT, value_type) &&
       !(fundamental_type == G_TYPE_ENUM || fundamental_type == G_TYPE_FLAGS)) ||
      g_value_type_transformable (G_TYPE_STRING, value_type) ||
      g_value_type_transformable (GDK_TYPE_RGBA, value_type))
    {
      GValue val = G_VALUE_INIT;

      g_value_init (&val, value_type);

      if (!gdk_screen_get_setting (settings->screen, pspec->name, &val))
        return FALSE;

      g_param_value_validate (pspec, &val);
      g_value_copy (&val, &settings->property_values[pspec->param_id - 1].value);
      settings->property_values[pspec->param_id - 1].source = GTK_SETTINGS_SOURCE_XSETTING;

      g_value_unset (&val);

      retval = TRUE;
    }
  else
    {
      GValue tmp_value = G_VALUE_INIT;
      GValue gstring_value = G_VALUE_INIT;
      GValue val = G_VALUE_INIT;
      GtkRcPropertyParser parser = (GtkRcPropertyParser) g_param_spec_get_qdata (pspec, quark_property_parser);

      g_value_init (&val, G_TYPE_STRING);

      if (!gdk_screen_get_setting (settings->screen, pspec->name, &val))
        return FALSE;

      g_value_init (&gstring_value, G_TYPE_GSTRING);
      g_value_take_boxed (&gstring_value, g_string_new (g_value_get_string (&val)));

      g_value_init (&tmp_value, value_type);
      if (parser && _gtk_settings_parse_convert (parser, &gstring_value,
                                                 pspec, &tmp_value))
        {
          g_param_value_validate (pspec, &tmp_value);
          g_value_copy (&tmp_value, &settings->property_values[pspec->param_id - 1].value);
          settings->property_values[pspec->param_id - 1].source = GTK_SETTINGS_SOURCE_XSETTING;
          retval = TRUE;
        }

      g_value_unset (&gstring_value);
      g_value_unset (&tmp_value);

      g_value_unset (&val);
    }

  return retval;
}


gboolean
gtk_settings_get_enable_animations (GtkSettings *settings)
{
  GtkSettingsPropertyValue *svalue = &settings->property_values[PROP_ENABLE_ANIMATIONS - 1];

  if (svalue->source < GTK_SETTINGS_SOURCE_XSETTING)
    {
      GParamSpec *pspec;

      pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), "gtk-enable-animations");
      if (settings_update_xsetting (settings, pspec, FALSE))
        g_object_notify_by_pspec (G_OBJECT (settings), pspec);
    }

  return g_value_get_boolean (&svalue->value);
}

gint
gtk_settings_get_dnd_drag_threshold (GtkSettings *settings)
{
  GtkSettingsPropertyValue *svalue = &settings->property_values[PROP_DND_DRAG_THRESHOLD - 1];

  if (svalue->source < GTK_SETTINGS_SOURCE_XSETTING)
    {
      GParamSpec *pspec;

      pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), "gtk-dnd-drag-threshold");
      if (settings_update_xsetting (settings, pspec, FALSE))
        g_object_notify_by_pspec (G_OBJECT (settings), pspec);
    }

  return g_value_get_int (&svalue->value);
}

const gchar *
gtk_settings_get_font_name (GtkSettings *settings)
{
  GtkSettingsPropertyValue *svalue = &settings->property_values[PROP_FONT_NAME - 1];

  if (svalue->source < GTK_SETTINGS_SOURCE_XSETTING)
    {
      GParamSpec *pspec;

      pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), "gtk-font-name");
      if (settings_update_xsetting (settings, pspec, FALSE))
        g_object_notify_by_pspec (G_OBJECT (settings), pspec);
    }

  return g_value_get_string (&svalue->value);
}


const gchar *
gtk_settings_get_font_family (GtkSettings *settings)
{
  PangoFontDescription *desc;
  const gchar *font_name;

  font_name = gtk_settings_get_font_name (settings);
  desc = pango_font_description_from_string (font_name);
  return desc ? pango_font_description_get_family (desc) : "Sans";
}

gint
gtk_settings_get_font_size (GtkSettings *settings)
{
  PangoFontDescription *desc;
  const gchar *font_name;

  font_name = gtk_settings_get_font_name (settings);
  desc = pango_font_description_from_string (font_name);
  return desc ? pango_font_description_get_size (desc) : 10 * PANGO_SCALE;
}

gboolean
gtk_settings_get_font_size_is_absolute (GtkSettings *settings)
{
  PangoFontDescription *desc;
  const gchar *font_name;

  font_name = gtk_settings_get_font_name (settings);
  desc = pango_font_description_from_string (font_name);
  return desc ? pango_font_description_get_size_is_absolute (desc) : FALSE;
}

/* This function is public */
/* everything else in here is private */

/**
 * gtk_settings_reset_property:
 * @settings: a #GtkSettings object
 * @name: the name of the setting to reset
 *
 * Undoes the effect of calling g_object_set() to install an
 * application-specific value for a setting. After this call,
 * the setting will again follow the session-wide value for
 * this setting.
 *
 * Since: 3.20
 */
void
gtk_settings_reset_property (GtkSettings *settings,
                             const gchar *name)
{
  GParamSpec *pspec;
  GtkRcPropertyParser parser;
  GValue *value;
  GValue tmp_value = G_VALUE_INIT;

  pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (settings), name);

  g_return_if_fail (pspec != NULL);

  parser = (GtkRcPropertyParser) g_param_spec_get_qdata (pspec, quark_property_parser);
  value = g_param_spec_get_qdata (pspec, g_quark_from_string (name));

  g_value_init (&tmp_value, G_PARAM_SPEC_VALUE_TYPE (pspec));
  if (value && _gtk_settings_parse_convert (parser, value, pspec, &tmp_value))
    g_value_copy (&tmp_value, &settings->property_values[pspec->param_id - 1].value);
  else
    g_param_value_set_default (pspec, &settings->property_values[pspec->param_id - 1].value);

  settings->property_values[pspec->param_id - 1].source = GTK_SETTINGS_SOURCE_DEFAULT;
  g_object_notify_by_pspec (G_OBJECT (settings), pspec);
}

