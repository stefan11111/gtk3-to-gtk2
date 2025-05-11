#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "util.h"

static void
prepare_context_for_layout (cairo_t *cr,
                            gdouble x,
                            gdouble y,
                            PangoLayout *layout)
{
  const PangoMatrix *matrix;

  matrix = pango_context_get_matrix (pango_layout_get_context (layout));

  cairo_move_to (cr, x, y);

  if (matrix)
    {
      cairo_matrix_t cairo_matrix;

      cairo_matrix_init (&cairo_matrix,
                         matrix->xx, matrix->yx,
                         matrix->xy, matrix->yy,
                         matrix->x0, matrix->y0);

      cairo_transform (cr, &cairo_matrix);
    }
}

void
gtk_render_layout (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   PangoLayout     *layout)
{
  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  g_return_if_fail (PANGO_IS_LAYOUT (layout));
  g_return_if_fail (cr);

  GdkRGBA _fg_color;

  const GdkRGBA *fg_color;

  cairo_save (cr);

  _fg_color = GdkRGBA_from_GdkColor(&context->fg[GTK_STATE_NORMAL]);
  fg_color = &_fg_color;

  prepare_context_for_layout (cr, x, y, layout);

  gdk_cairo_set_source_rgba (cr, fg_color);
  pango_cairo_show_layout (cr, layout);

  cairo_restore (cr);

  context->fg[GTK_STATE_NORMAL] = GdkColor_from_GdkRGBA(fg_color);
}
