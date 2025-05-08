#include <gtk/gtk.h>

typedef struct _GtkStyleContext GtkStyleContext;

void
gtk_render_layout (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   PangoLayout     *layout)
{
  g_return_if_fail (PANGO_IS_LAYOUT (layout));
  g_return_if_fail (cr);

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
