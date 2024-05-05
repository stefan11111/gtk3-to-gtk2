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

gchar *
gdk_rgba_to_string (const GdkRGBA *rgba)
{
  if (rgba->alpha > 0.999)
    {
      return g_strdup_printf ("rgb(%d,%d,%d)",
                              (int)(0.5 + CLAMP (rgba->red, 0., 1.) * 255.),
                              (int)(0.5 + CLAMP (rgba->green, 0., 1.) * 255.),
                              (int)(0.5 + CLAMP (rgba->blue, 0., 1.) * 255.));
    }
  else
    {
      gchar alpha[G_ASCII_DTOSTR_BUF_SIZE];

      g_ascii_formatd (alpha, G_ASCII_DTOSTR_BUF_SIZE, "%g", CLAMP (rgba->alpha, 0, 1));

      return g_strdup_printf ("rgba(%d,%d,%d,%s)",
                              (int)(0.5 + CLAMP (rgba->red, 0., 1.) * 255.),
                              (int)(0.5 + CLAMP (rgba->green, 0., 1.) * 255.),
                              (int)(0.5 + CLAMP (rgba->blue, 0., 1.) * 255.),
                              alpha);
    }
}
