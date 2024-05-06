#include <math.h>

#include <gtk/gtk.h>

guint
gtk_get_major_version (void)
{
  return 3;
}

guint
gtk_get_minor_version (void)
{
  return 24;
}

guint
gtk_get_micro_version (void)
{
  return 39;
}

GtkWidget *
gtk_grid_new (void)
{
  return NULL;
}

void
gtk_grid_attach ()
{
}

void
gtk_grid_set_column_spacing()
{
}

gboolean
gtk_widget_get_hexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_hexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

gboolean
gtk_widget_get_vexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_vexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

GtkWidget*
gtk_box_new (GtkOrientation orientation,
             gint           spacing)
{
  return orientation ? gtk_vbox_new(FALSE, spacing) : gtk_hbox_new(FALSE, spacing);
}

int
gtk_widget_get_allocated_width (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.width;
}

int
gtk_widget_get_allocated_height (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.height;
}

typedef struct _GtkStyleContext GtkStyleContext;

GtkStyleContext *
gtk_widget_get_style_context (GtkWidget *widget)
{
  return NULL;
}

const PangoFontDescription *
gtk_style_context_get_font ()
{
  return NULL;
}

void
gtk_style_context_get_color ()
{
}

void
gtk_render_layout (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   PangoLayout     *layout)
{
  g_return_if_fail (PANGO_IS_LAYOUT (layout));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);
  const PangoMatrix *matrix = pango_context_get_matrix (pango_layout_get_context (layout));

  cairo_move_to (cr, x, y);

  if (!matrix) {
    return;
  }

  cairo_matrix_t cairo_matrix;

  cairo_matrix_init (&cairo_matrix,
                     matrix->xx, matrix->yx,
                     matrix->xy, matrix->yy,
                     matrix->x0, matrix->y0);
  cairo_transform (cr, &cairo_matrix);

  pango_cairo_show_layout (cr, layout);
  cairo_restore (cr);
}

GtkWidget*
gtk_font_chooser_dialog_new (const gchar *title,
                             GtkWindow   *parent)
{
  return gtk_font_selection_dialog_new(title);
}

typedef struct _GtkFontChooser GtkFontChooser;

void
gtk_font_chooser_set_font (GtkFontChooser *fontchooser,
                           const gchar    *fontname)
{
  g_return_if_fail (fontname != NULL);

  g_object_set (fontchooser, "font", fontname, NULL);
}

gchar *
gtk_font_chooser_get_font (GtkFontChooser *fontchooser)
{
  gchar *fontname;

  g_object_get (fontchooser, "font", &fontname, NULL);


  return fontname;
}
#if 0
void
gtk_font_chooser_set_font (GtkFontChooser *fontchooser,
                           const gchar    *fontname)
{
}

gchar *
gtk_font_chooser_get_font (GtkFontChooser *fontchooser)
{
  return NULL;
}
#endif

void
gtk_widget_override_font (GtkWidget                  *widget,
                          const PangoFontDescription *font_desc)
{
  gtk_widget_modify_font (widget, (PangoFontDescription *)font_desc);
}

GtkWidget *
gtk_tree_view_column_get_button (GtkTreeViewColumn *tree_column)
{
  g_return_val_if_fail (GTK_IS_TREE_VIEW_COLUMN (tree_column), NULL);

  return tree_column->button;
}

struct _GdkRGBA
{
  gdouble red;
  gdouble green;
  gdouble blue;
  gdouble alpha;
};

typedef struct _GdkRGBA GdkRGBA;

#if 0
static GdkRGBA GdkRGBA_from_GdkColor (GdkColor *color)
{
  GdkRGBA rgba;
  rgba.red = (gdouble)color->red/65535;
  rgba.green = (gdouble)color->green/65535;
  rgba.blue = (gdouble)color->blue/65535;
  rgba.alpha = 1;

  return rgba;
}
#endif

struct _GtkColorButtonPrivate
{
  GtkWidget *draw_area; /* Widget where we draw the color sample */
  GtkWidget *cs_dialog; /* Color selection dialog */

  gchar *title;         /* Title for the color selection window */

  GdkColor color;
  guint16 alpha;

  guint use_alpha : 1;  /* Use alpha or not */
};

void
gtk_color_button_get_rgba (GtkColorButton *button,
                           GdkRGBA        *rgba)
{
  g_return_if_fail (GTK_IS_COLOR_BUTTON (button));
  g_return_if_fail (rgba != NULL);

  rgba->red = (gdouble)button->priv->color.red/65535;
  rgba->green = (gdouble)button->priv->color.green/65535;
  rgba->blue = (gdouble)button->priv->color.blue/65535;
  rgba->alpha = button->priv->use_alpha ? (gdouble)button->priv->alpha/65535 : 1;
}

gint
gtk_widget_get_margin_left (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.x;
}

void
gtk_widget_set_margin_left (GtkWidget *widget,
                            gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  if (widget->allocation.x == margin) {
    return;
  }

  widget->requisition.width -= margin - widget->allocation.x;
  widget->allocation.width -= margin - widget->allocation.x;
  widget->allocation.x = margin;
  gtk_widget_queue_resize (widget);
}

gint
gtk_widget_get_margin_right (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.x + widget->allocation.width;
}

void
gtk_widget_set_margin_right (GtkWidget *widget,
                             gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  if (widget->allocation.x + widget->allocation.width == margin) {
    return;
  }

  widget->requisition.width = margin - widget->allocation.x;
  widget->allocation.width = margin - widget->allocation.x;
  gtk_widget_queue_resize (widget);
}

gint
gtk_widget_get_margin_start (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? widget->allocation.x + widget->allocation.width :
                                                                   widget->allocation.x;
}

void
gtk_widget_set_margin_start (GtkWidget *widget,
                             gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_set_margin_right (widget, margin) :
                                                            gtk_widget_set_margin_left (widget, margin);
}

gint
gtk_widget_get_margin_end (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? widget->allocation.x:
                                                                   widget->allocation.x +  widget->allocation.width;
}

void
gtk_widget_set_margin_end (GtkWidget *widget,
                           gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_set_margin_left (widget, margin) :
                                                            gtk_widget_set_margin_right (widget, margin);
}

gint
gtk_widget_get_margin_top (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.y;
}

void
gtk_widget_set_margin_top (GtkWidget *widget,
                           gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  if (widget->allocation.y == margin) {
    return;
  }

  widget->requisition.height -= margin - widget->allocation.y;
  widget->allocation.height -= margin - widget->allocation.y;
  widget->allocation.y = margin;
  gtk_widget_queue_resize (widget);
}

gint
gtk_widget_get_margin_bottom (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.y - widget->allocation.height;
}

void
gtk_widget_set_margin_bottom (GtkWidget *widget,
                              gint       margin)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  if (widget->allocation.y - widget->allocation.height == margin) {
    return;
  }

  widget->requisition.height = margin - widget->allocation.y;
  widget->allocation.height = margin - widget->allocation.y;
  gtk_widget_queue_resize (widget);
}

void
gtk_render_background (GtkStyleContext *context,
                       cairo_t         *cr,
                       gdouble          x,
                       gdouble          y,
                       gdouble          width,
                       gdouble          height)
{
}

void
gtk_revealer_set_transition_duration ()
{
}

void
gtk_style_context_get_padding ()
{
}

typedef struct _GtkScrollable GtkScrollable;

GtkAdjustment *
gtk_scrollable_get_hadjustment (GtkScrollable *scrollable)
{
  GtkAdjustment *adj = NULL;

  g_return_val_if_fail (scrollable, NULL);

  g_object_get (scrollable, "hadjustment", &adj, NULL);

  /* Horrid hack; g_object_get() returns a new reference but
   * that contradicts the memory management conventions
   * for accessors.
   */
  if (adj)
    g_object_unref (adj);

  return adj;
}

void
gtk_scrollable_set_hadjustment (GtkScrollable *scrollable,
                                GtkAdjustment *hadjustment)
{
  g_return_if_fail (scrollable);
  g_return_if_fail (hadjustment == NULL || GTK_IS_ADJUSTMENT (hadjustment));

  g_object_set (scrollable, "hadjustment", hadjustment, NULL);
}

GtkAdjustment *
gtk_scrollable_get_vadjustment (GtkScrollable *scrollable)
{
  GtkAdjustment *adj = NULL;

  g_return_val_if_fail (scrollable, NULL);

  g_object_get (scrollable, "vadjustment", &adj, NULL);

  /* Horrid hack; g_object_get() returns a new reference but
   * that contradicts the memory management conventions
   * for accessors.
   */
  if (adj)
    g_object_unref (adj);

  return adj;
}

void
gtk_scrollable_set_vadjustment (GtkScrollable *scrollable,
                                GtkAdjustment *vadjustment)
{
  g_return_if_fail (scrollable);
  g_return_if_fail (vadjustment == NULL || GTK_IS_ADJUSTMENT (vadjustment));

  g_object_set (scrollable, "vadjustment", vadjustment, NULL);
}

GType gtk_scrollable_get_type (void)
{
  return 0;
}

void
gtk_render_option (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   gdouble          width,
                   gdouble          height)
{
  g_return_if_fail (cr);

  if (width <= 0 || height <= 0)
    return;

  cairo_matrix_t matrix;

  cairo_get_matrix (cr, &matrix);

  cairo_translate (cr, x, y);

  cairo_set_matrix (cr, &matrix);
}

void
gtk_widget_path_iter_set_name ()
{
}

void
gtk_style_context_get_style_property (GtkStyleContext *context,
                                      const gchar     *property_name,
                                      GValue          *value)
{
}

void
gtk_style_context_get_border_color ()
{
}

void
gtk_widget_set_clip (GtkWidget           *widget,
                     const GtkAllocation *clip)
{
}

GtkWidget *
gtk_paned_new (GtkOrientation orientation)
{
  return g_object_new (GTK_TYPE_PANED,
                       "orientation", orientation,
                       NULL);
}

GtkWidget *
gtk_scale_new (GtkOrientation  orientation,
               GtkAdjustment  *adjustment)
{
  g_return_val_if_fail (adjustment == NULL || GTK_IS_ADJUSTMENT (adjustment),
                        NULL);

  return g_object_new (GTK_TYPE_SCALE,
                       "orientation", orientation,
                       "adjustment",  adjustment,
                       NULL);
}

GtkWidget *
gtk_scale_new_with_range (GtkOrientation orientation,
                          gdouble        min,
                          gdouble        max,
                          gdouble        step)
{
  GtkObject *adj;
  gint digits;

  g_return_val_if_fail (min < max, NULL);
  g_return_val_if_fail (step != 0.0, NULL);

  adj = gtk_adjustment_new (min, min, max, step, 10 * step, 0);

  if (fabs (step) >= 1.0 || step == 0.0)
    {
      digits = 0;
    }
  else
    {
      digits = abs ((gint) floor (log10 (fabs (step))));
      if (digits > 5)
        digits = 5;
    }

  return g_object_new (GTK_TYPE_SCALE,
                       "orientation", orientation,
                       "adjustment",  adj,
                       "digits",      digits,
                       NULL);
}

GtkIconSet *
gtk_style_context_lookup_icon_set (GtkStyleContext *context,
                                   const gchar     *stock_id)
{
  g_return_val_if_fail (stock_id, NULL);

  return gtk_icon_factory_lookup_default (stock_id);
}
