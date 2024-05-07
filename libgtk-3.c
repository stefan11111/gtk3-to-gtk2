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

void
gtk_render_frame (GtkStyleContext *context,
                  cairo_t         *cr,
                  gdouble          x,
                  gdouble          y,
                  gdouble          width,
                  gdouble          height)
{
}

typedef struct _GdkFrameClock GdkFrameClock;

GdkFrameClock*
gtk_widget_get_frame_clock (GtkWidget *widget)
{
  return NULL;
}

typedef enum
{
  GTK_STATE_FLAG_NORMAL       = 0,
  GTK_STATE_FLAG_ACTIVE       = 1 << 0,
  GTK_STATE_FLAG_PRELIGHT     = 1 << 1,
  GTK_STATE_FLAG_SELECTED     = 1 << 2,
  GTK_STATE_FLAG_INSENSITIVE  = 1 << 3,
  GTK_STATE_FLAG_INCONSISTENT = 1 << 4,
  GTK_STATE_FLAG_FOCUSED      = 1 << 5,
  GTK_STATE_FLAG_BACKDROP     = 1 << 6,
  GTK_STATE_FLAG_DIR_LTR      = 1 << 7,
  GTK_STATE_FLAG_DIR_RTL      = 1 << 8,
  GTK_STATE_FLAG_LINK         = 1 << 9,
  GTK_STATE_FLAG_VISITED      = 1 << 10,
  GTK_STATE_FLAG_CHECKED      = 1 << 11,
  GTK_STATE_FLAG_DROP_ACTIVE  = 1 << 12
} GtkStateFlags;

GtkStateFlags
gtk_style_context_get_state (GtkStyleContext *context)
{
  return 0;
}

void
gtk_style_context_set_parent (GtkStyleContext *context,
                              GtkStyleContext *parent)
{
}

GType gtk_header_bar_get_type (void)
{
  return 0;
}

void
gtk_color_selection_get_current_rgba (GtkColorSelection *colorsel,
                                      GdkRGBA           *rgba)
{
  GdkColor color;

  gtk_color_selection_get_current_color (colorsel, &color);

  rgba->red = (gdouble)color.red/65535;
  rgba->green = (gdouble)color.green/65535;
  rgba->blue = (gdouble)color.blue/65535;
  rgba->alpha = (gdouble)gtk_color_selection_get_current_alpha (colorsel)/65535;
}

void
gtk_color_selection_set_current_rgba (GtkColorSelection *colorsel,
                                      const GdkRGBA     *rgba)
{
  GdkColor color;

  color.pixel = 0;
  color.red = (guint16)(rgba->red * 65535);
  color.green = (guint16)(rgba->green * 65535);
  color.blue = (guint16)(rgba->green * 65535);

  gtk_color_selection_set_current_color (colorsel, &color);
  gtk_color_selection_set_current_alpha (colorsel, (guint16)(rgba->alpha * 65535));
}

void
gtk_entry_set_max_width_chars (GtkEntry *entry,
                               gint      n_chars)
{
  gtk_entry_set_max_length(entry, n_chars);
}

void
gtk_style_context_get_margin (GtkStyleContext *context,
                              GtkStateFlags    state,
                              GtkBorder       *margin)
{
}

void
gtk_style_context_clear_property_cache (GtkStyleContext *context)
{
}

typedef struct _GtkStyleProviderPrivate GtkStyleProviderPrivate;

GtkStyleProviderPrivate *
gtk_style_context_get_style_provider (GtkStyleContext *context)
{
  return NULL;
}

typedef struct _GtkCssStyle GtkCssStyle;

GtkCssStyle *
gtk_style_context_lookup_style (GtkStyleContext *context)
{
  return NULL;
}

typedef struct _GtkCssNode GtkCssNode;

GtkCssNode*
gtk_style_context_get_node (GtkStyleContext *context)
{
  return NULL;
}

GtkStyleContext *
gtk_style_context_new (void)
{
  return NULL;
}

GtkStyleContext *
gtk_style_context_new_for_node (GtkCssNode *node)
{
  return NULL;
}

void
gtk_style_context_add_provider ()
{
}

void
gtk_style_context_remove_provider ()
{
}

void
gtk_style_context_reset_widgets (GdkScreen *screen)
{
  GList *list, *toplevels;

  toplevels = gtk_window_list_toplevels ();
  g_list_foreach (toplevels, (GFunc) g_object_ref, NULL);

  for (list = toplevels; list; list = list->next)
    {
      g_object_unref (list->data);
    }

  g_list_free (toplevels);
}

void
gtk_style_context_add_provider_for_screen ()
{
}

void
gtk_style_context_remove_provider_for_screen ()
{
}

typedef struct _GtkCssSection GtkCssSection;

GtkCssSection *
gtk_style_context_get_section (GtkStyleContext *context,
                               const gchar     *property)
{
  return NULL;
}

void
gtk_style_context_get_property (GtkStyleContext *context,
                                const gchar     *property,
                                GtkStateFlags    state,
                                GValue          *value)
{
}

void
gtk_style_context_get_valist (GtkStyleContext *context,
                              GtkStateFlags    state,
                              va_list          args)
{
}

void
gtk_style_context_get (GtkStyleContext *context,
                       GtkStateFlags    state,
                       ...)
{
}

void
gtk_style_context_set_id (GtkStyleContext *context,
                          const char      *id)
{
}

const char *
gtk_style_context_get_id (GtkStyleContext *context)
{
  return NULL;
}

void
gtk_style_context_set_state (GtkStyleContext *context,
                             GtkStateFlags    flags)
{
}

void
gtk_style_context_set_scale (GtkStyleContext *context,
                             gint             scale)
{
}

gint
gtk_style_context_get_scale (GtkStyleContext *context)
{
  return 0;
}

gboolean
gtk_style_context_state_is_running (GtkStyleContext *context,
                                    GtkStateType     state,
                                    gdouble         *progress)
{
  return FALSE;
}

void
gtk_style_context_set_path (GtkStyleContext *context,
                            GtkWidgetPath   *path)
{
}

const GtkWidgetPath *
gtk_style_context_get_path (GtkStyleContext *context)
{
  return NULL;
}

GtkStyleContext *
gtk_style_context_get_parent (GtkStyleContext *context)
{
  return NULL;
}

void
gtk_style_context_save_to_node (GtkStyleContext *context,
                                GtkCssNode      *node)
{
}

void
gtk_style_context_save_named (GtkStyleContext *context,
                              const char      *name)
{
}

void
gtk_style_context_save (GtkStyleContext *context)
{
}

void
gtk_style_context_restore (GtkStyleContext *context)
{
}

void
gtk_style_context_add_class (GtkStyleContext *context,
                             const gchar     *class_name)
{
}

void
gtk_style_context_remove_class (GtkStyleContext *context,
                                const gchar     *class_name)
{
}

gboolean
gtk_style_context_has_class (GtkStyleContext *context,
                             const gchar     *class_name)
{
  return FALSE;
}

GList *
gtk_style_context_list_classes (GtkStyleContext *context)
{
  return NULL;
}

GList *
gtk_style_context_list_regions (GtkStyleContext *context)
{
  return NULL;
}

void
gtk_style_context_add_region ()
{
}

void
gtk_style_context_remove_region (GtkStyleContext *context,
                                 const gchar     *region_name)
{
}

gboolean
gtk_style_context_has_region ()
{
  return FALSE;
}

void
gtk_style_context_get_style_valist (GtkStyleContext *context,
                                    va_list          args)
{
}

void
gtk_style_context_get_style (GtkStyleContext *context,
                             ...)
{
}

void
gtk_style_context_set_screen (GtkStyleContext *context,
                              GdkScreen       *screen)
{
}

GdkScreen *
gtk_style_context_get_screen (GtkStyleContext *context)
{
  return NULL;
}

void
gtk_style_context_set_frame_clock (GtkStyleContext *context,
                                   GdkFrameClock   *frame_clock)
{
}

GdkFrameClock *
gtk_style_context_get_frame_clock (GtkStyleContext *context)
{
  return NULL;
}

void
gtk_style_context_set_direction (GtkStyleContext  *context,
                                 GtkTextDirection  direction)
{
}

GtkTextDirection
gtk_style_context_get_direction (GtkStyleContext *context)
{
  return GTK_TEXT_DIR_LTR;
}

typedef enum {
  GTK_JUNCTION_NONE   = 0,
  GTK_JUNCTION_CORNER_TOPLEFT = 1 << 0,
  GTK_JUNCTION_CORNER_TOPRIGHT = 1 << 1,
  GTK_JUNCTION_CORNER_BOTTOMLEFT = 1 << 2,
  GTK_JUNCTION_CORNER_BOTTOMRIGHT = 1 << 3,
  GTK_JUNCTION_TOP    = (GTK_JUNCTION_CORNER_TOPLEFT | GTK_JUNCTION_CORNER_TOPRIGHT),
  GTK_JUNCTION_BOTTOM = (GTK_JUNCTION_CORNER_BOTTOMLEFT | GTK_JUNCTION_CORNER_BOTTOMRIGHT),
  GTK_JUNCTION_LEFT   = (GTK_JUNCTION_CORNER_TOPLEFT | GTK_JUNCTION_CORNER_BOTTOMLEFT),
  GTK_JUNCTION_RIGHT  = (GTK_JUNCTION_CORNER_TOPRIGHT | GTK_JUNCTION_CORNER_BOTTOMRIGHT)
} GtkJunctionSides;

GtkJunctionSides
gtk_style_context_get_junction_sides (GtkStyleContext *context)
{
  return 0;
}

gboolean
gtk_style_context_lookup_color (GtkStyleContext *context,
                                const gchar     *color_name,
                                GdkRGBA         *color)
{
  return FALSE;
}

void
gtk_style_context_notify_state_change (GtkStyleContext *context,
                                       GdkWindow       *window,
                                       gpointer         region_id,
                                       GtkStateType     state,
                                       gboolean         state_value)
{
}

void
gtk_style_context_cancel_animations (GtkStyleContext *context,
                                     gpointer         region_id)
{
}

void
gtk_style_context_scroll_animations (GtkStyleContext *context,
                                     GdkWindow       *window,
                                     gint             dx,
                                     gint             dy)
{
}

void
gtk_style_context_push_animatable_region (GtkStyleContext *context,
                                          gpointer         region_id)
{
}

void
gtk_style_context_pop_animatable_region (GtkStyleContext *context)
{
}

void
gtk_style_context_validate ()
{
}

void
gtk_style_context_invalidate (GtkStyleContext *context)
{
}

void
gtk_style_context_set_background (GtkStyleContext *context,
                                  GdkWindow       *window)
{
}

void
gtk_style_context_get_background_color (GtkStyleContext *context,
                                        GtkStateFlags    state,
                                        GdkRGBA         *color)
{
}

typedef struct _GtkCssStyleChange GtkCssStyleChange;

GtkCssStyleChange *
gtk_style_context_get_change (GtkStyleContext *context)
{
  return NULL;
}

char *
gtk_style_context_to_string ()
{
  return g_string_free(g_string_new (""), FALSE);
}

void
gtk_style_context_get_border (GtkStyleContext *context,
                              GtkStateFlags    state,
                              GtkBorder       *border)
{
}

void
gtk_color_button_set_rgba (GtkColorButton *button,
                           const GdkRGBA  *rgba)
{
  GdkColor color;

  color.pixel = 0;
  color.red = (guint16)(rgba->red * 65535);
  color.green = (guint16)(rgba->green * 65535);
  color.blue = (guint16)(rgba->blue * 65535);

  gtk_color_button_set_color (button, &color);

  gtk_color_button_set_use_alpha (button, TRUE);

  gtk_color_button_set_alpha (button, (guint16)(rgba->alpha * 65535));
}
