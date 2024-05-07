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

void
gtk_style_context_set_junction_sides ()
{
}

struct _GtkWidgetPath
{
  guint ref_count;

  GArray *elems; /* First element contains the described widget */
};

typedef struct _GtkWidgetPath GtkWidgetPath;

gint
gtk_widget_path_length (const GtkWidgetPath *path)
{
#if 0 /* runtime cost */
  if (!path) {
    return 0;
  }
#endif
  return path->elems->len;
}

void
gtk_widget_shape_combine_region (GtkWidget *widget,
                                 cairo_region_t *region)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  /*  set_shape doesn't work on widgets without GDK window */
  g_return_if_fail (gtk_widget_get_has_window (widget));

  if (region == NULL)
    {
      if (widget->window)
        gdk_window_shape_combine_region (widget->window, NULL, 0, 0);

      g_object_set_qdata (G_OBJECT (widget), g_quark_from_static_string ("gtk-shape-info"), NULL);
    }
  else
    {
      g_object_set_qdata_full (G_OBJECT (widget), g_quark_from_static_string ("gtk-shape-info"),
                               cairo_region_copy (region),
                               (GDestroyNotify) cairo_region_destroy);

      /* set shape if widget has a GDK window already.
       * otherwise the shape is scheduled to be set by gtk_widget_realize().
       */
      if (widget->window)
        gdk_window_shape_combine_region (widget->window, (const GdkRegion *)((const int*)region + 2), 0, 0);
    }
}

GtkWidget *
gtk_button_box_new (GtkOrientation orientation)
{
  return orientation ? gtk_vbutton_box_new () : gtk_hbutton_box_new ();
}

/* not stubs, resize grips removed upstream */

void
gtk_window_set_has_resize_grip (GtkWindow *window,
                                gboolean   value)
{
}

gboolean
gtk_window_resize_grip_is_visible (GtkWindow *window)
{
  return FALSE;
}

gboolean
gtk_window_get_has_resize_grip (GtkWindow *window)

{
  return FALSE;
}

gboolean
gtk_window_get_resize_grip_area (GtkWindow    *window,
                                 GdkRectangle *rect)
{
  return FALSE;
}

#if 0
static void
gtk_widget_compute_size_for_orientation (GtkWidget        *widget,
                                         GtkOrientation    orientation,
                                         gint              for_size,
                                         gint             *minimum,
                                         gint             *natural,
                                         gint             *minimum_baseline,
                                         gint             *natural_baseline)
{
  if (minimum) {
    *minimum = 0;
  }
  if (natural) {
    *natural = 0;
  }
  if (minimum_baseline) {
    *minimum_baseline = -1;
  }
  if (natural_baseline) {
    *natural_baseline = -1;
  }
}
#endif
void
gtk_widget_get_preferred_width (GtkWidget *widget,
                                gint      *minimum_width,
                                gint      *natural_width)
{
  if (minimum_width) {
    *minimum_width = 0;
  }
  if (natural_width) {
    *natural_width = 0;
  }
}

void
gtk_widget_get_preferred_height (GtkWidget *widget,
                                 gint      *minimum_height,
                                 gint      *natural_height)
{
  if (minimum_height) {
    *minimum_height = 0;
  }
  if (natural_height) {
    *natural_height = 0;
  }
}

void
gtk_widget_get_preferred_width_for_height (GtkWidget *widget,
                                           gint       height,
                                           gint      *minimum_width,
                                           gint      *natural_width)
{
  if (minimum_width) {
    *minimum_width = 0;
  }
  if (natural_width) {
    *natural_width = 0;
  }
}

void
gtk_widget_get_preferred_height_for_width (GtkWidget *widget,
                                           gint       width,
                                           gint      *minimum_height,
                                           gint      *natural_height)
{
  if (minimum_height) {
    *minimum_height = 0;
  }
  if (natural_height) {
    *natural_height = 0;
  }
}

void
gtk_widget_get_preferred_height_and_baseline_for_width (GtkWidget *widget,
                                                        gint       width,
                                                        gint      *minimum_height,
                                                        gint      *natural_height,
                                                        gint      *minimum_baseline,
                                                        gint      *natural_baseline)
{
  if (minimum_height) {
    *minimum_height = 0;
  }
  if (natural_height) {
    *natural_height = 0;
  }
  if (minimum_baseline) {
    *minimum_baseline = -1;
  }
  if (natural_baseline) {
    *natural_baseline = -1;
  }
}

void
gtk_widget_get_preferred_size (GtkWidget      *widget,
                               GtkRequisition *minimum_size,
                               GtkRequisition *natural_size)
{
#if 0 /* 2 molv */
  if (minimum_size) {
    minimum_size->width = 0;
    minimum_size->height = 0;
  }
  if (natural_size) {
    natural_size->width = 0;
    natural_size->height = 0;
  }
#endif /* 1 movq */
  if (minimum_size) {
    memset(minimum_size, 0, sizeof(GtkRequisition));
  }
  if (natural_size) {
    memset(natural_size, 0, sizeof(GtkRequisition));
  }
}

PangoFontDescription *
gtk_font_chooser_get_font_desc (GtkFontChooser *fontchooser)
{
  PangoFontDescription *font_desc;

  g_return_val_if_fail (fontchooser, NULL);

  g_object_get (fontchooser, "font-desc", &font_desc, NULL);

  return font_desc;
}

void
gtk_font_chooser_set_font_desc (GtkFontChooser             *fontchooser,
                                const PangoFontDescription *font_desc)
{
  g_return_if_fail (fontchooser);
  g_return_if_fail (font_desc != NULL);

  g_object_set (fontchooser, "font-desc", font_desc, NULL);
}

gchar *
gtk_font_chooser_get_preview_text (GtkFontChooser *fontchooser)
{
  char *text;

  g_return_val_if_fail (fontchooser, NULL);

  g_object_get (fontchooser, "preview-text", &text, NULL);

  return text;
}

void
gtk_font_chooser_set_preview_text (GtkFontChooser *fontchooser,
                                   const gchar    *text)
{
  g_return_if_fail (fontchooser);
  g_return_if_fail (text != NULL);

  g_object_set (fontchooser, "preview-text", text, NULL);
}

gboolean
gtk_font_chooser_get_show_preview_entry (GtkFontChooser *fontchooser)
{
  gboolean show;

  g_return_val_if_fail (fontchooser, FALSE);

  g_object_get (fontchooser, "show-preview-entry", &show, NULL);

  return show;
}

void
gtk_font_chooser_set_show_preview_entry (GtkFontChooser *fontchooser,
                                         gboolean        show_preview_entry)
{
  g_return_if_fail (fontchooser);

  show_preview_entry = show_preview_entry != FALSE;
  g_object_set (fontchooser, "show-preview-entry", show_preview_entry, NULL);
}

PangoFontFamily *
gtk_font_chooser_get_font_family (GtkFontChooser *fontchooser)
{
  return NULL;
}

PangoFontFace *
gtk_font_chooser_get_font_face (GtkFontChooser *fontchooser)
{
  return NULL;
}

gint
gtk_font_chooser_get_font_size (GtkFontChooser *fontchooser)
{
  return -1;
}

void
gtk_font_chooser_set_filter_func ()
{
}

void
gtk_font_chooser_set_font_map (GtkFontChooser *fontchooser,
                               PangoFontMap   *fontmap)
{
}

PangoFontMap *
gtk_font_chooser_get_font_map (GtkFontChooser *fontchooser)
{
  return NULL;
}

typedef enum {
  GTK_FONT_CHOOSER_LEVEL_FAMILY     = 0,
  GTK_FONT_CHOOSER_LEVEL_STYLE      = 1 << 0,
  GTK_FONT_CHOOSER_LEVEL_SIZE       = 1 << 1,
  GTK_FONT_CHOOSER_LEVEL_VARIATIONS = 1 << 2,
  GTK_FONT_CHOOSER_LEVEL_FEATURES   = 1 << 3
} GtkFontChooserLevel;

void
gtk_font_chooser_set_level (GtkFontChooser      *fontchooser,
                            GtkFontChooserLevel  level)
{
  g_return_if_fail (fontchooser);

  g_object_set (fontchooser, "level", level, NULL);
}

GtkFontChooserLevel
gtk_font_chooser_get_level (GtkFontChooser *fontchooser)
{
  GtkFontChooserLevel level;

  g_return_val_if_fail (fontchooser, 0);

  g_object_get (fontchooser, "level", &level, NULL);

  return level;
}

char *
gtk_font_chooser_get_font_features (GtkFontChooser *fontchooser)
{
  char *text;

  g_return_val_if_fail (fontchooser, NULL);

  g_object_get (fontchooser, "font-features", &text, NULL);

  return text;
}

char *
gtk_font_chooser_get_language (GtkFontChooser *fontchooser)
{
  char *text;

  g_return_val_if_fail (fontchooser, NULL);

  g_object_get (fontchooser, "language", &text, NULL);

  return text;
}

void
gtk_font_chooser_set_language (GtkFontChooser *fontchooser,
                               const char     *language)
{
  g_return_if_fail (fontchooser);

  g_object_set (fontchooser, "language", language, NULL);
}

GtkWidget *
gtk_separator_new (GtkOrientation orientation)
{
  return g_object_new (GTK_TYPE_SEPARATOR,
                       "orientation", orientation,
                       NULL);
}
