#include <gtk/gtkcolorchooser.h>

#include "gdkrgba.h"

GType
gtk_color_chooser_get_type (void)
{
#if 0
  return gtk_color_selection_get_type ();
#else
  return GTK_TYPE_COLOR_SELECTION;
#endif
}

void
gtk_color_chooser_get_rgba (GtkColorChooser *chooser,
                            GdkRGBA         *color)
{
  GdkColor c;
  guint16 alpha;

  gtk_color_selection_get_current_color (chooser, &c);
  alpha = gtk_color_selection_get_current_alpha (chooser);

  *color = GdkRGBA_from_GdkColor(&c);
  color->alpha = (gdouble)alpha/65535;

  if (!GTK_COLOR_CHOOSER_GET_IFACE(chooser)->get_rgba) {
    GTK_COLOR_CHOOSER_GET_IFACE(chooser)->get_rgba = gtk_color_chooser_get_rgba;
  }
}

void
gtk_color_chooser_set_rgba (GtkColorChooser *chooser,
                            const GdkRGBA   *color)
{
  GdkColor c = GdkColor_from_GdkRGBA(color);

  gtk_color_selection_set_current_color (chooser, &c);
  gtk_color_selection_set_current_alpha (chooser, (guint16)(color->alpha * 65535));

  if (!GTK_COLOR_CHOOSER_GET_IFACE(chooser)->set_rgba) {
    GTK_COLOR_CHOOSER_GET_IFACE(chooser)->set_rgba = gtk_color_chooser_set_rgba;
  }
}

gboolean
gtk_color_chooser_get_use_alpha (GtkColorChooser *chooser)
{
  return gtk_color_selection_get_has_opacity_control (chooser);
}

void
gtk_color_chooser_set_use_alpha (GtkColorChooser *chooser,
                                 gboolean         use_alpha)
{
  gtk_color_selection_set_has_opacity_control (chooser, use_alpha);
}

void
gtk_color_chooser_add_palette (GtkColorChooser *chooser,
                               GtkOrientation   orientation,
                               gint             colors_per_line,
                               gint             n_colors,
                               GdkRGBA         *colors)
{
  /* Not Implemented */
  g_return_if_fail (GTK_IS_COLOR_CHOOSER (chooser));

  if (!GTK_COLOR_CHOOSER_GET_IFACE(chooser)->add_palette) {
    GTK_COLOR_CHOOSER_GET_IFACE(chooser)->add_palette = gtk_color_chooser_add_palette;
  }
}

cairo_pattern_t *
_gtk_color_chooser_get_checkered_pattern (void)
{
  /* need to respect pixman's stride being a multiple of 4 */
  static unsigned char data[8] = { 0xFF, 0x00, 0x00, 0x00,
                                   0x00, 0xFF, 0x00, 0x00 };
  static cairo_surface_t *checkered = NULL;
  cairo_pattern_t *pattern;

  if (checkered == NULL)
    checkered = cairo_image_surface_create_for_data (data,
                                                     CAIRO_FORMAT_A8,
                                                     2, 2, 4);

  pattern = cairo_pattern_create_for_surface (checkered);
  cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
  cairo_pattern_set_filter (pattern, CAIRO_FILTER_NEAREST);

  return pattern;
}
