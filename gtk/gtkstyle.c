#include <gtk/gtk.h>

/* XXX everything in here is copied from gtk2, but is also here because these symobls are not exported... XXX */

typedef struct {
  GType       widget_type;
  GParamSpec *pspec;
  GValue      value;
} PropertyValue;

typedef struct _CursorInfo CursorInfo;

struct _CursorInfo
{
  GType for_type;
  GdkGC *primary_gc;
  GdkGC *secondary_gc;
};

#ifndef I_
#define I_(x) x
#endif

static gint
style_property_values_cmp (gconstpointer bsearch_node1,
                           gconstpointer bsearch_node2)
{
  const PropertyValue *val1 = bsearch_node1;
  const PropertyValue *val2 = bsearch_node2;

  if (val1->widget_type == val2->widget_type)
    return val1->pspec < val2->pspec ? -1 : val1->pspec == val2->pspec ? 0 : 1;
  else
    return val1->widget_type < val2->widget_type ? -1 : 1;
}

const GValue*
_gtk_style_peek_property_value (GtkStyle           *style,
                                GType               widget_type,
                                GParamSpec         *pspec,
                                GtkRcPropertyParser parser)
{
  PropertyValue *pcache, key = { 0, NULL, { 0, } };
  const GtkRcProperty *rcprop = NULL;
  guint i;

  g_return_val_if_fail (GTK_IS_STYLE (style), NULL);
  g_return_val_if_fail (G_IS_PARAM_SPEC (pspec), NULL);
  g_return_val_if_fail (g_type_is_a (pspec->owner_type, GTK_TYPE_WIDGET), NULL);
  g_return_val_if_fail (g_type_is_a (widget_type, pspec->owner_type), NULL);

  key.widget_type = widget_type;
  key.pspec = pspec;

  /* need value cache array */
  if (!style->property_cache)
    style->property_cache = g_array_new (FALSE, FALSE, sizeof (PropertyValue));
  else
    {
      pcache = bsearch (&key,
                        style->property_cache->data, style->property_cache->len,
                        sizeof (PropertyValue), style_property_values_cmp);
      if (pcache)
        return &pcache->value;
    }

  i = 0;
  while (i < style->property_cache->len &&
         style_property_values_cmp (&key, &g_array_index (style->property_cache, PropertyValue, i)) >= 0)
    i++;

  g_array_insert_val (style->property_cache, i, key);
  pcache = &g_array_index (style->property_cache, PropertyValue, i);

  /* cache miss, initialize value type, then set contents */
  g_param_spec_ref (pcache->pspec);
  g_value_init (&pcache->value, G_PARAM_SPEC_VALUE_TYPE (pspec));

  /* value provided by rc style? */
  if (style->rc_style)
    {
      GQuark prop_quark = g_quark_from_string (pspec->name);

      do
        {
          rcprop = _gtk_rc_style_lookup_rc_property (style->rc_style,
                                                     g_type_qname (widget_type),
                                                     prop_quark);
          if (rcprop)
            break;
          widget_type = g_type_parent (widget_type);
        }
      while (g_type_is_a (widget_type, pspec->owner_type));
    }

  /* when supplied by rc style, we need to convert */
  if (rcprop && !_gtk_settings_parse_convert (parser, &rcprop->value,
                                              pspec, &pcache->value))
    {
      gchar *contents = g_strdup_value_contents (&rcprop->value);

      g_message ("%s: failed to retrieve property `%s::%s' of type `%s' from rc file value \"%s\" of type `%s'",
                 rcprop->origin ? rcprop->origin : "(for origin information, set GTK_DEBUG)",
                 g_type_name (pspec->owner_type), pspec->name,
                 g_type_name (G_PARAM_SPEC_VALUE_TYPE (pspec)),
                 contents,
                 G_VALUE_TYPE_NAME (&rcprop->value));
      g_free (contents);
      rcprop = NULL; /* needs default */
    }

  /* not supplied by rc style (or conversion failed), revert to default */
  if (!rcprop)
    g_param_value_set_default (pspec, &pcache->value);

  return &pcache->value;
}

/**
 * _gtk_style_init_for_settings:
 * @style: a #GtkStyle
 * @settings: a #GtkSettings
 *
 * Initializes the font description in @style according to the default
 * font name of @settings. This is called for gtk_style_new() with
 * the settings for the default screen (if any); if we are creating
 * a style for a particular screen, we then call it again in a
 * location where we know the correct settings.
 * The reason for this is that gtk_rc_style_create_style() doesn't
 * take the screen for an argument.
 **/
void
_gtk_style_init_for_settings (GtkStyle    *style,
                              GtkSettings *settings)
{
  const gchar *font_name = _gtk_rc_context_get_default_font_name (settings);

  if (style->font_desc)
    pango_font_description_free (style->font_desc);

  style->font_desc = pango_font_description_from_string (font_name);

  if (!pango_font_description_get_family (style->font_desc))
    {
      g_warning ("Default font does not have a family set");
      pango_font_description_set_family (style->font_desc, "Sans");
    }
  if (pango_font_description_get_size (style->font_desc) <= 0)
    {
      g_warning ("Default font does not have a positive size");
      pango_font_description_set_size (style->font_desc, 10 * PANGO_SCALE);
    }
}

static void
rgb_to_hls (gdouble *r,
            gdouble *g,
            gdouble *b)
{
  gdouble min;
  gdouble max;
  gdouble red;
  gdouble green;
  gdouble blue;
  gdouble h, l, s;
  gdouble delta;

  red = *r;
  green = *g;
  blue = *b;

  if (red > green)
    {
      if (red > blue)
        max = red;
      else
        max = blue;

      if (green < blue)
        min = green;
      else
        min = blue;
    }
  else
    {
      if (green > blue)
        max = green;
      else
        max = blue;

      if (red < blue)
        min = red;
      else
        min = blue;
    }

  l = (max + min) / 2;
  s = 0;
  h = 0;

  if (max != min)
    {
      if (l <= 0.5)
        s = (max - min) / (max + min);
      else
        s = (max - min) / (2 - max - min);

      delta = max -min;
      if (red == max)
        h = (green - blue) / delta;
      else if (green == max)
        h = 2 + (blue - red) / delta;
      else if (blue == max)
        h = 4 + (red - green) / delta;

      h *= 60;
      if (h < 0.0)
        h += 360;
    }

  *r = h;
  *g = l;
  *b = s;
}

static void
hls_to_rgb (gdouble *h,
            gdouble *l,
            gdouble *s)
{
  gdouble hue;
  gdouble lightness;
  gdouble saturation;
  gdouble m1, m2;
  gdouble r, g, b;

  lightness = *l;
  saturation = *s;

  if (lightness <= 0.5)
    m2 = lightness * (1 + saturation);
  else
    m2 = lightness + saturation - lightness * saturation;
  m1 = 2 * lightness - m2;

  if (saturation == 0)
    {
      *h = lightness;
      *l = lightness;
      *s = lightness;
    }
  else
    {
      hue = *h + 120;
      while (hue > 360)
        hue -= 360;
      while (hue < 0)
        hue += 360;

      if (hue < 60)
        r = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
        r = m2;
      else if (hue < 240)
        r = m1 + (m2 - m1) * (240 - hue) / 60;
      else
        r = m1;

      hue = *h;
      while (hue > 360)
        hue -= 360;
      while (hue < 0)
        hue += 360;

      if (hue < 60)
        g = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
        g = m2;
      else if (hue < 240)
        g = m1 + (m2 - m1) * (240 - hue) / 60;
      else
        g = m1;

      hue = *h - 120;
      while (hue > 360)
        hue -= 360;
      while (hue < 0)
        hue += 360;

      if (hue < 60)
        b = m1 + (m2 - m1) * hue / 60;
      else if (hue < 180)
        b = m2;
      else if (hue < 240)
        b = m1 + (m2 - m1) * (240 - hue) / 60;
      else
        b = m1;

      *h = r;
      *l = g;
      *s = b;
    }
}

void
_gtk_style_shade (const GdkColor *a,
                  GdkColor       *b,
                  gdouble         k)
{
  gdouble red;
  gdouble green;
  gdouble blue;

  red = (gdouble) a->red / 65535.0;
  green = (gdouble) a->green / 65535.0;
  blue = (gdouble) a->blue / 65535.0;

  rgb_to_hls (&red, &green, &blue);

  green *= k;
  if (green > 1.0)
    green = 1.0;
  else if (green < 0.0)
    green = 0.0;

  blue *= k;
  if (blue > 1.0)
    blue = 1.0;
  else if (blue < 0.0)
    blue = 0.0;

  hls_to_rgb (&red, &green, &blue);

  b->red = red * 65535.0;
  b->green = green * 65535.0;
  b->blue = blue * 65535.0;
}

static GdkGC *
make_cursor_gc (GtkWidget      *widget,
                const gchar    *property_name,
                const GdkColor *fallback)
{
  GdkGCValues gc_values;
  GdkGCValuesMask gc_values_mask;
  GdkColor *cursor_color;

  gtk_widget_style_get (widget, property_name, &cursor_color, NULL);

  gc_values_mask = GDK_GC_FOREGROUND;
  if (cursor_color)
    {
      gc_values.foreground = *cursor_color;
      gdk_color_free (cursor_color);
    }
  else
    gc_values.foreground = *fallback;

  gdk_rgb_find_color (widget->style->colormap, &gc_values.foreground);
  return gtk_gc_get (widget->style->depth, widget->style->colormap, &gc_values, gc_values_mask);
}

static GdkGC *
get_insertion_cursor_gc (GtkWidget *widget,
                         gboolean   is_primary)
{
  CursorInfo *cursor_info;

  cursor_info = g_object_get_data (G_OBJECT (widget->style), "gtk-style-cursor-info");
  if (!cursor_info)
    {
      cursor_info = g_new (CursorInfo, 1);
      g_object_set_data (G_OBJECT (widget->style), I_("gtk-style-cursor-info"), cursor_info);
      cursor_info->primary_gc = NULL;
      cursor_info->secondary_gc = NULL;
      cursor_info->for_type = G_TYPE_INVALID;
    }

  /* We have to keep track of the type because gtk_widget_style_get()
   * can return different results when called on the same property and
   * same style but for different widgets. :-(. That is,
   * GtkEntry::cursor-color = "red" in a style will modify the cursor
   * color for entries but not for text view.
   */
  if (cursor_info->for_type != G_OBJECT_TYPE (widget))
    {
      cursor_info->for_type = G_OBJECT_TYPE (widget);
      if (cursor_info->primary_gc)
        {
          gtk_gc_release (cursor_info->primary_gc);
          cursor_info->primary_gc = NULL;
        }
      if (cursor_info->secondary_gc)
        {
          gtk_gc_release (cursor_info->secondary_gc);
          cursor_info->secondary_gc = NULL;
        }
    }

  /* Cursors in text widgets are drawn only in NORMAL state,
   * so we can use text[GTK_STATE_NORMAL] as text color here */
  if (is_primary)
    {
      if (!cursor_info->primary_gc)
        cursor_info->primary_gc = make_cursor_gc (widget,
                                                  "cursor-color",
                                                  &widget->style->text[GTK_STATE_NORMAL]);

      return cursor_info->primary_gc;
    }
  else
    {
      if (!cursor_info->secondary_gc)
        cursor_info->secondary_gc = make_cursor_gc (widget,
                                                    "secondary-cursor-color",
                                                    /* text_aa is the average of text and base colors,
                                                     * in usual black-on-white case it's grey. */
                                                    &widget->style->text_aa[GTK_STATE_NORMAL]);

      return cursor_info->secondary_gc;
    }
}

GdkGC *
_gtk_widget_get_cursor_gc (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);
  g_return_val_if_fail (gtk_widget_get_realized (widget), NULL);
  return get_insertion_cursor_gc (widget, TRUE);
}

void
_gtk_widget_get_cursor_color (GtkWidget *widget,
                              GdkColor  *color)
{
  GdkColor *style_color;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (color != NULL);

  gtk_widget_style_get (widget, "cursor-color", &style_color, NULL);

  if (style_color)
    {
      *color = *style_color;
      gdk_color_free (style_color);
    }
  else
    *color = widget->style->text[GTK_STATE_NORMAL];
}
