#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "util.h"
#include <math.h>
#include <dlfcn.h>

/* XXX The code is this file should be FAST XXX */

#define LIGHTNESS_MULT  1.3
#define DARKNESS_MULT   0.7

static const GtkRequisition default_option_indicator_size = { 7, 13 };
static const GtkBorder default_option_indicator_spacing = { 7, 5, 2, 2 };

typedef struct _CursorInfo CursorInfo;

struct _CursorInfo
{
  GType for_type;
  GdkColor primary;
  GdkColor secondary;
};

#ifndef I_
#define I_(x) x
#endif

#define GTK_GRAY		0xdcdc, 0xdada, 0xd5d5
#define GTK_DARK_GRAY		0xc4c4, 0xc2c2, 0xbdbd
#define GTK_LIGHT_GRAY		0xeeee, 0xebeb, 0xe7e7
#define GTK_WHITE		0xffff, 0xffff, 0xffff
#define GTK_BLUE		0x4b4b, 0x6969, 0x8383
#define GTK_VERY_DARK_GRAY	0x9c9c, 0x9a9a, 0x9494
#define GTK_BLACK		0x0000, 0x0000, 0x0000
#define GTK_WEAK_GRAY		0x7530, 0x7530, 0x7530

static inline GtkStateType
gtk_style_context_get_state_type (GtkStyleContext* context)
{
  return GtkStateType_from_GtkStateFlags (gtk_style_context_get_state (context));
}
static void 
gtk_default_draw_check (GtkStyle      *style,
			cairo_t       *cr,
			GtkStateType   state_type,
			GtkShadowType  shadow_type,
			GtkWidget     *widget, /* not used */
			const gchar   *detail,
			gint           x,
			gint           y,
			gint           width,
			gint           height)
{
  enum { BUTTON, MENU, CELL } type = BUTTON;
  int exterior_size;
  int interior_size;
  int pad;
  
  if (detail)
    {
      if (strcmp (detail, "cellcheck") == 0)
	type = CELL;
      else if (strcmp (detail, "check") == 0)
	type = MENU;
    }
      
  exterior_size = MIN (width, height);
  if (exterior_size % 2 == 0) /* Ensure odd */
    exterior_size -= 1;

  pad = style->xthickness + MAX (1, (exterior_size - 2 * style->xthickness) / 9);
  interior_size = MAX (1, exterior_size - 2 * pad);

  if (interior_size < 7)
    {
      interior_size = 7;
      pad = MAX (0, (exterior_size - interior_size) / 2);
    }

  x -= (1 + exterior_size - width) / 2;
  y -= (1 + exterior_size - height) / 2;

  switch (type)
    {
    case BUTTON:
    case CELL:
      if (type == BUTTON)
	gdk_cairo_set_source_color (cr, &style->fg[state_type]);
      else
	gdk_cairo_set_source_color (cr, &style->text[state_type]);
	
      cairo_set_line_width (cr, 1.0);
      cairo_rectangle (cr, x + 0.5, y + 0.5, exterior_size - 1, exterior_size - 1);
      cairo_stroke (cr);

      gdk_cairo_set_source_color (cr, &style->base[state_type]);
      cairo_rectangle (cr, x + 1, y + 1, exterior_size - 2, exterior_size - 2);
      cairo_fill (cr);
      break;

    case MENU:
      break;
    }
      
  switch (type)
    {
    case BUTTON:
    case CELL:
      gdk_cairo_set_source_color (cr, &style->text[state_type]);
      break;
    case MENU:
      gdk_cairo_set_source_color (cr, &style->fg[state_type]);
      break;
    }

  switch (shadow_type) {
  case GTK_SHADOW_IN:
    cairo_translate (cr,
                     x + pad, y + pad);

    cairo_scale (cr, interior_size / 7., interior_size / 7.);

    cairo_move_to  (cr, 7.0, 0.0);
    cairo_line_to  (cr, 7.5, 1.0);
    cairo_curve_to (cr, 5.3, 2.0,
                    4.3, 4.0,
                    3.5, 7.0);
    cairo_curve_to (cr, 3.0, 5.7,
                    1.3, 4.7,
                    0.0, 4.7);
    cairo_line_to  (cr, 0.2, 3.5);
    cairo_curve_to (cr, 1.1, 3.5,
                    2.3, 4.3,
                    3.0, 5.0);
    cairo_curve_to (cr, 1.0, 3.9,
                    2.4, 4.1,
                    3.2, 4.9);
    cairo_curve_to (cr, 3.5, 3.1,
                    5.2, 2.0,
                    7.0, 0.0);

    cairo_fill (cr);
  break;
  case GTK_SHADOW_ETCHED_IN: /* inconsistent */
    int line_thickness = MAX (1, (3 + interior_size * 2) / 7);

    cairo_rectangle (cr,
                    x + pad,
                    y + pad + (1 + interior_size - line_thickness) / 2,
                    interior_size,
                    line_thickness);
    cairo_fill (cr);
  break;
  default: /* silence gcc */
  break;
  }
}

/**
 * gtk_cairo_paint_check:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: the type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the rectangle to draw the check in
 * @y: y origin of the rectangle to draw the check in
 * @width: the width of the rectangle to draw the check in
 * @height: the height of the rectangle to draw the check in
 * 
 * Draws a check button indicator in the given rectangle on @cr with 
 * the given parameters.
 */
void
gtk_cairo_paint_check (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkShadowType       shadow_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_check (style, cr, state_type, shadow_type, widget, detail, x, y, width, height);

  cairo_restore (cr);
}

void
gtk_render_check (GtkStyleContext *context,
                  cairo_t         *cr,
                  gdouble          x,
                  gdouble          y,
                  gdouble          width,
                  gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_check (context, cr, state_type, GTK_SHADOW_NONE, NULL, NULL, x, y, width, height);
}

static void 
gtk_default_draw_option (GtkStyle      *style,
			 cairo_t       *cr,
			 GtkStateType   state_type,
			 GtkShadowType  shadow_type,
			 GtkWidget     *widget, /* not used */
			 const gchar   *detail,
			 gint           x,
			 gint           y,
			 gint           width,
			 gint           height)
{
  enum { BUTTON, MENU, CELL } type = BUTTON;
  int exterior_size;
  
  if (detail)
    {
      if (strcmp (detail, "radio") == 0)
	type = CELL;
      else if (strcmp (detail, "option") == 0)
	type = MENU;
    }
      
  exterior_size = MIN (width, height);
  if (exterior_size % 2 == 0) /* Ensure odd */
    exterior_size -= 1;
  
  x -= (1 + exterior_size - width) / 2;
  y -= (1 + exterior_size - height) / 2;

  switch (type)
    {
    case BUTTON:
    case CELL:
      gdk_cairo_set_source_color (cr, &style->base[state_type]);
      
      cairo_arc (cr,
		 x + exterior_size / 2.,
		 y + exterior_size / 2.,
		 (exterior_size - 1) / 2.,
		 0, 2 * G_PI);

      cairo_fill_preserve (cr);

      if (type == BUTTON)
	gdk_cairo_set_source_color (cr, &style->fg[state_type]);
      else
	gdk_cairo_set_source_color (cr, &style->text[state_type]);
	
      cairo_set_line_width (cr, 1.);
      cairo_stroke (cr);
      break;

    case MENU:
      break;
    }
      
  switch (type)
    {
    case BUTTON:
      gdk_cairo_set_source_color (cr, &style->text[state_type]);
      break;
    case CELL:
      break;
    case MENU:
      gdk_cairo_set_source_color (cr, &style->fg[state_type]);
      break;
    }

  if (shadow_type == GTK_SHADOW_IN)
    {
      int pad = style->xthickness + MAX (1, 2 * (exterior_size - 2 * style->xthickness) / 9);
      int interior_size = MAX (1, exterior_size - 2 * pad);

      if (interior_size < 5)
	{
	  interior_size = 7;
	  pad = MAX (0, (exterior_size - interior_size) / 2);
	}

      cairo_arc (cr,
		 x + pad + interior_size / 2.,
		 y + pad + interior_size / 2.,
		 interior_size / 2.,
		 0, 2 * G_PI);
      cairo_fill (cr);
    }
  else if (shadow_type == GTK_SHADOW_ETCHED_IN) /* inconsistent */
    {
      int pad = style->xthickness + MAX (1, (exterior_size - 2 * style->xthickness) / 9);
      int interior_size = MAX (1, exterior_size - 2 * pad);
      int line_thickness;

      if (interior_size < 7)
	{
	  interior_size = 7;
	  pad = MAX (0, (exterior_size - interior_size) / 2);
	}

      line_thickness = MAX (1, (3 + interior_size * 2) / 7);

      cairo_rectangle (cr,
		       x + pad,
		       y + pad + (interior_size - line_thickness) / 2.,
		       interior_size,
		       line_thickness);
      cairo_fill (cr);
    }
}

/**
 * gtk_cairo_paint_option:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: the type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the rectangle to draw the option in
 * @y: y origin of the rectangle to draw the option in
 * @width: the width of the rectangle to draw the option in
 * @height: the height of the rectangle to draw the option in
 *
 * Draws a radio button indicator in the given rectangle on @cr with 
 * the given parameters.
 */
void
gtk_cairo_paint_option (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_option (style, cr, state_type, shadow_type, widget, detail, x, y, width, height);

  cairo_restore (cr);
}

void
gtk_render_option (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   gdouble          width,
                   gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_option (context, cr, state_type, GTK_SHADOW_NONE, NULL, NULL, x, y, width, height);
}

static void
draw_arrow (cairo_t       *cr,
	    GdkColor      *color,
	    GtkArrowType   arrow_type,
	    gint           x,
	    gint           y,
	    gint           width,
	    gint           height)
{
  gdk_cairo_set_source_color (cr, color);
  cairo_save (cr);

  switch (arrow_type & 0x3) {
  case GTK_ARROW_DOWN:
    cairo_move_to (cr, x,              y);
    cairo_line_to (cr, x + width,      y);
    cairo_line_to (cr, x + width / 2., y + height);
  break;
  case GTK_ARROW_UP:
    cairo_move_to (cr, x,              y + height);
    cairo_line_to (cr, x + width / 2., y);
    cairo_line_to (cr, x + width,      y + height);
  break;
  case GTK_ARROW_LEFT:
    cairo_move_to (cr, x + width,      y);
    cairo_line_to (cr, x + width,      y + height);
    cairo_line_to (cr, x,              y + height / 2.);
  break;
  case GTK_ARROW_RIGHT:
    cairo_move_to (cr, x,              y);
    cairo_line_to (cr, x + width,      y + height / 2.);
    cairo_line_to (cr, x,              y + height);
  break;
  }

  cairo_close_path (cr);
  cairo_fill (cr);

  cairo_restore (cr);
}

static void
calculate_arrow_geometry (GtkArrowType  arrow_type,
			  gint         *x,
			  gint         *y,
			  gint         *width,
			  gint         *height)
{
  gint w = *width;
  gint h = *height;
  
  switch (arrow_type)
    {
    case GTK_ARROW_UP:
    case GTK_ARROW_DOWN:
      w += (w % 2) - 1;
      h = (w / 2 + 1);
      
      if (h > *height)
	{
	  h = *height;
	  w = 2 * h - 1;
	}
      
      if (arrow_type == GTK_ARROW_DOWN)
	{
	  if (*height % 2 == 1 || h % 2 == 0)
	    *height += 1;
	}
      else
	{
	  if (*height % 2 == 0 || h % 2 == 0)
	    *height -= 1;
	}
      break;

    case GTK_ARROW_RIGHT:
    case GTK_ARROW_LEFT:
      h += (h % 2) - 1;
      w = (h / 2 + 1);
      
      if (w > *width)
	{
	  w = *width;
	  h = 2 * w - 1;
	}
      
      if (arrow_type == GTK_ARROW_RIGHT)
	{
	  if (*width % 2 == 1 || w % 2 == 0)
	    *width += 1;
	}
      else
	{
	  if (*width % 2 == 0 || w % 2 == 0)
	    *width -= 1;
	}
      break;
      
    default:
      /* should not be reached */
      break;
    }

  *x += (*width - w) / 2;
  *y += (*height - h) / 2;
  *height = h;
  *width = w;
}

static void
gtk_default_draw_arrow (GtkStyle      *style,
			cairo_t       *cr,
			GtkStateType   state,
			GtkShadowType  shadow,
			GtkWidget     *widget, /* not used */
			const gchar   *detail,
			GtkArrowType   arrow_type,
			gboolean       fill, /* not used */
			gint           x,
			gint           y,
			gint           width,
			gint           height)
{
  calculate_arrow_geometry (arrow_type, &x, &y, &width, &height);

  if (detail && strcmp (detail, "menu_scroll_arrow_up") == 0)
    y++;

  if (state == GTK_STATE_INSENSITIVE)
    draw_arrow (cr, &style->white, arrow_type,
		x + 1, y + 1, width, height);
  draw_arrow (cr, &style->fg[state], arrow_type,
	      x, y, width, height);
}

/**
 * gtk_cairo_paint_arrow:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: the type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @arrow_type: the type of arrow to draw
 * @fill: %TRUE if the arrow tip should be filled
 * @x: x origin of the rectangle to draw the arrow in
 * @y: y origin of the rectangle to draw the arrow in
 * @width: width of the rectangle to draw the arrow in
 * @height: height of the rectangle to draw the arrow in
 * 
 * Draws an arrow in the given rectangle on @cr using the given 
 * parameters. @arrow_type determines the direction of the arrow.
 */
void
gtk_cairo_paint_arrow (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkShadowType       shadow_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       GtkArrowType        arrow_type,
                       gboolean            fill,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_arrow (style, cr, state_type, shadow_type, widget, detail, arrow_type, fill, x, y, width, height);

  cairo_restore (cr);
}

/**
 * gtk_render_arrow:
 * @context: a #GtkStyleContext
 * @cr: a #cairo_t
 * @angle: arrow angle from 0 to 2 * %G_PI, being 0 the arrow pointing to the north
 * @x: X origin of the render area
 * @y: Y origin of the render area
 * @size: square side for render area
 *
 * Renders an arrow pointing to @angle.
 *
 * Typical arrow rendering at 0, 1⁄2 π;, π; and 3⁄2 π:
 *
 * ![](arrows.png)
 *
 * Since: 3.0
 **/
void
gtk_render_arrow (GtkStyleContext *context,
                  cairo_t         *cr,
                  gdouble          angle,
                  gdouble          x,
                  gdouble          y,
                  gdouble          size)
{
  GtkStateType state_type;
  GtkArrowType arrow_type;

  /* map [0, 2 * pi] to [0, 4] */
  switch ((int)(2 * angle / G_PI + 0.5) & 0x3)
  {
  case 0: /* 4 & 3 == 0 */
    arrow_type = GTK_ARROW_UP;
    break;
  case 1:
    arrow_type = GTK_ARROW_RIGHT;
    break;
  case 2:
    arrow_type = GTK_ARROW_DOWN;
    break;
  case 3:
    arrow_type = GTK_ARROW_LEFT;
    break;
  default:
    /* not reached */
    break;
  }

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_arrow (context, cr, state_type, GTK_SHADOW_NONE, NULL, NULL, arrow_type, TRUE, x, y, size, size);
}

void
_gtk_style_apply_default_background (GtkStyle          *style,
                                     cairo_t           *cr,
                                     GdkWindow         *window,
                                     GtkStateType       state_type,
                                     gint               x,
                                     gint               y,
                                     gint               width,
                                     gint               height)
{
  cairo_save (cr);

  if (style->bg_pixmap[state_type] == (GdkPixmap*) GDK_PARENT_RELATIVE)
    {
      GdkWindow *parent = gdk_window_get_parent (window);
      int x_offset, y_offset;

      if (parent)
        {
          gdk_window_get_position (window, &x_offset, &y_offset);
          cairo_translate (cr, -x_offset, -y_offset);
          _gtk_style_apply_default_background (style, cr,
                                               parent, state_type,
                                               x + x_offset, y + y_offset,
                                               width, height);
          goto out;
        }
      else
        gdk_cairo_set_source_color (cr, &style->bg[state_type]);
    }
  else if (style->bg_pixmap[state_type])
    {
      gdk_cairo_set_source_pixmap (cr, style->bg_pixmap[state_type], 0, 0);
      cairo_pattern_set_extend (cairo_get_source (cr), CAIRO_EXTEND_REPEAT);
    }
  else
    gdk_cairo_set_source_color (cr, &style->bg[state_type]);

  cairo_rectangle (cr, x, y, width, height);
  cairo_fill (cr);

out:
  cairo_restore (cr);
}



void
gtk_render_background (GtkStyleContext *context,
                       cairo_t         *cr,
                       gdouble          x,
                       gdouble          y,
                       gdouble          width,
                       gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);

  /* already saves/restores cr */
  _gtk_style_apply_default_background (context, cr, (GdkWindow*)NULL, state_type, x, y, width, height);
}

/**
 * gtk_render_background_get_clip:
 * @context: a #GtkStyleContext
 * @x: X origin of the rectangle
 * @y: Y origin of the rectangle
 * @width: rectangle width
 * @height: rectangle height
 * @out_clip: (out): return location for the clip
 *
 * Returns the area that will be affected (i.e. drawn to) when
 * calling gtk_render_background() for the given @context and
 * rectangle.
 *
 * Since: 3.20
 */
void
gtk_render_background_get_clip (GtkStyleContext *context,
                                gdouble          x,
                                gdouble          y,
                                gdouble          width,
                                gdouble          height,
                                GdkRectangle    *out_clip)
{
  out_clip->x = x;
  out_clip->y = y;
  out_clip->width = width;
  out_clip->height = height;
}

static void
_cairo_draw_line (cairo_t  *cr,
                  GdkColor *color,
                  gint      x1,
                  gint      y1,
                  gint      x2,
                  gint      y2)
{
  cairo_save (cr);

  gdk_cairo_set_source_color (cr, color);
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_SQUARE);

  cairo_move_to (cr, x1 + 0.5, y1 + 0.5);
  cairo_line_to (cr, x2 + 0.5, y2 + 0.5);
  cairo_stroke (cr);

  cairo_restore (cr);
}

static void
_cairo_draw_rectangle (cairo_t *cr,
                       GdkColor *color,
                       gboolean filled,
                       gint x,
                       gint y,
                       gint width,
                       gint height)
{
  gdk_cairo_set_source_color (cr, color);

  if (filled)
    {
      cairo_rectangle (cr, x, y, width, height);
      cairo_fill (cr);
    }
  else
    {
      cairo_rectangle (cr, x + 0.5, y + 0.5, width, height);
      cairo_stroke (cr);
    }
}

static void
_cairo_draw_point (cairo_t *cr,
                   GdkColor *color,
                   gint x,
                   gint y)
{
  gdk_cairo_set_source_color (cr, color);
  cairo_rectangle (cr, x, y, 1, 1);
  cairo_fill (cr);
}

static void
draw_thin_shadow (GtkStyle      *style,
		  cairo_t       *cr,
		  GtkStateType   state,
		  gint           x,
		  gint           y,
		  gint           width,
		  gint           height)
{
  GdkColor *gc1, *gc2;

  gc1 = &style->light[state];
  gc2 = &style->dark[state];

  _cairo_draw_line (cr, gc1,
                    x, y + height - 1, x + width - 1, y + height - 1);
  _cairo_draw_line (cr, gc1,
                    x + width - 1, y,  x + width - 1, y + height - 1);
      
  _cairo_draw_line (cr, gc2,
                    x, y, x + width - 2, y);
  _cairo_draw_line (cr, gc2,
                    x, y, x, y + height - 2);
}

static void
draw_spinbutton_shadow (GtkStyle        *style,
			cairo_t         *cr,
			GtkStateType     state,
			GtkTextDirection direction,
			gint             x,
			gint             y,
			gint             width,
			gint             height)
{

  if (direction == GTK_TEXT_DIR_LTR)
    {
      _cairo_draw_line (cr, &style->dark[state],
                        x, y, x + width - 1, y);
      _cairo_draw_line (cr, &style->black,
                        x, y + 1, x + width - 2, y + 1);
      _cairo_draw_line (cr, &style->black,
                        x + width - 2, y + 2, x + width - 2, y + height - 3);
      _cairo_draw_line (cr, &style->light[state],
                        x + width - 1, y + 1, x + width - 1, y + height - 2);
      _cairo_draw_line (cr, &style->light[state],
                        x, y + height - 1, x + width - 1, y + height - 1);
      _cairo_draw_line (cr, &style->bg[state],
                        x, y + height - 2, x + width - 2, y + height - 2);
      _cairo_draw_line (cr, &style->black,
                        x, y + 2, x, y + height - 3);
    }
  else
    {
      _cairo_draw_line (cr, &style->dark[state],
                        x, y, x + width - 1, y);
      _cairo_draw_line (cr, &style->dark[state],
                        x, y + 1, x, y + height - 1);
      _cairo_draw_line (cr, &style->black,
                        x + 1, y + 1, x + width - 1, y + 1);
      _cairo_draw_line (cr, &style->black,
                        x + 1, y + 2, x + 1, y + height - 2);
      _cairo_draw_line (cr, &style->black,
                        x + width - 1, y + 2, x + width - 1, y + height - 3);
      _cairo_draw_line (cr, &style->light[state],
                        x + 1, y + height - 1, x + width - 1, y + height - 1);
      _cairo_draw_line (cr, &style->bg[state],
                        x + 2, y + height - 2, x + width - 1, y + height - 2);
    }
}

static void
draw_menu_shadow (GtkStyle        *style,
		  cairo_t         *cr,
		  GtkStateType     state,
		  gint             x,
		  gint             y,
		  gint             width,
		  gint             height)
{
  if (style->ythickness > 0)
    {
      if (style->ythickness > 1)
	{
	  _cairo_draw_line (cr, &style->dark[state],
                            x + 1, y + height - 2,
                            x + width - 2, y + height - 2);
	  _cairo_draw_line (cr, &style->black,
                            x, y + height - 1, x + width - 1, y + height - 1);
	}
      else
	{
	  _cairo_draw_line (cr, &style->dark[state],
                            x + 1, y + height - 1, x + width - 1, y + height - 1);
	}
    }
  
  if (style->xthickness > 0)
    {
      if (style->xthickness > 1)
	{
	  _cairo_draw_line (cr, &style->dark[state],
                            x + width - 2, y + 1,
                            x + width - 2, y + height - 2);

	  _cairo_draw_line (cr, &style->black,
                            x + width - 1, y, x + width - 1, y + height - 1);
	}
      else
	{
	  _cairo_draw_line (cr, &style->dark[state],
                            x + width - 1, y + 1, x + width - 1, y + height - 1);
	}
    }
  
  /* Light around top and left */
  
  if (style->ythickness > 0)
    _cairo_draw_line (cr, &style->black,
		   x, y, x + width - 2, y);
  if (style->xthickness > 0)
    _cairo_draw_line (cr, &style->black,
                      x, y, x, y + height - 2);
  
  if (style->ythickness > 1)
    _cairo_draw_line (cr, &style->light[state],
                      x + 1, y + 1, x + width - 3, y + 1);
  if (style->xthickness > 1)
    _cairo_draw_line (cr, &style->light[state],
                      x + 1, y + 1, x + 1, y + height - 3);
}

static GtkTextDirection
get_direction (GtkWidget *widget)
{
  GtkTextDirection dir;
  
  if (widget)
    dir = gtk_widget_get_direction (widget);
  else
    dir = GTK_TEXT_DIR_LTR;
  
  return dir;
}

static void
gtk_default_draw_shadow (GtkStyle      *style,
                         cairo_t       *cr,
                         GtkStateType   state_type,
                         GtkShadowType  shadow_type,
                         GtkWidget     *widget,
                         const gchar   *detail,
                         gint           x,
                         gint           y,
                         gint           width,
                         gint           height)
{
  GdkColor *gc1 = NULL;
  GdkColor *gc2 = NULL;
  gint thickness_light;
  gint thickness_dark;
  gint i;

  cairo_set_line_width (cr, 1.0);

  if (shadow_type == GTK_SHADOW_IN)
    {
      if (detail && strcmp (detail, "buttondefault") == 0)
	{
          _cairo_draw_rectangle (cr, &style->black, FALSE,
                                 x, y, width - 1, height - 1);

	  return;
	}
      if (detail && strcmp (detail, "trough") == 0)
	{
          draw_thin_shadow (style, cr, state_type,
                            x, y, width, height);

	  return;
	}
      if (GTK_IS_SPIN_BUTTON (widget) &&
         detail && strcmp (detail, "spinbutton") == 0)
	{
	  draw_spinbutton_shadow (style, cr, state_type, 
				  get_direction (widget), x, y, width, height);
	  
	  return;
	}
    }

  if (shadow_type == GTK_SHADOW_OUT && detail && strcmp (detail, "menu") == 0)
    {
      draw_menu_shadow (style, cr, state_type, x, y, width, height);
      return;
    }
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
    case GTK_SHADOW_ETCHED_IN:
      gc1 = &style->light[state_type];
      gc2 = &style->dark[state_type];
      break;
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = &style->dark[state_type];
      gc2 = &style->light[state_type];
      break;
    }
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;
      
    case GTK_SHADOW_IN:
      /* Light around right and bottom edge */

      if (style->ythickness > 0)
        _cairo_draw_line (cr, gc1,
                          x, y + height - 1, x + width - 1, y + height - 1);
      if (style->xthickness > 0)
        _cairo_draw_line (cr, gc1,
                          x + width - 1, y, x + width - 1, y + height - 1);

      if (style->ythickness > 1)
        _cairo_draw_line (cr, &style->bg[state_type],
                          x + 1, y + height - 2, x + width - 2, y + height - 2);
      if (style->xthickness > 1)
        _cairo_draw_line (cr, &style->bg[state_type],
                          x + width - 2, y + 1, x + width - 2, y + height - 2);

      /* Dark around left and top */

      if (style->ythickness > 1)
        _cairo_draw_line (cr, &style->black,
                          x + 1, y + 1, x + width - 2, y + 1);
      if (style->xthickness > 1)
        _cairo_draw_line (cr, &style->black,
                          x + 1, y + 1, x + 1, y + height - 2);

      if (style->ythickness > 0)
        _cairo_draw_line (cr, gc2,
                          x, y, x + width - 1, y);
      if (style->xthickness > 0)
        _cairo_draw_line (cr, gc2,
                          x, y, x, y + height - 1);
      break;
      
    case GTK_SHADOW_OUT:
      /* Dark around right and bottom edge */

      if (style->ythickness > 0)
        {
          if (style->ythickness > 1)
            {
              _cairo_draw_line (cr, gc1,
                                x + 1, y + height - 2, x + width - 2, y + height - 2);
              _cairo_draw_line (cr, &style->black,
                                x, y + height - 1, x + width - 1, y + height - 1);
            }
          else
            {
              _cairo_draw_line (cr, gc1,
                                x + 1, y + height - 1, x + width - 1, y + height - 1);
            }
        }

      if (style->xthickness > 0)
        {
          if (style->xthickness > 1)
            {
              _cairo_draw_line (cr, gc1,
                                x + width - 2, y + 1, x + width - 2, y + height - 2);
              
              _cairo_draw_line (cr, &style->black,
                                x + width - 1, y, x + width - 1, y + height - 1);
            }
          else
            {
              _cairo_draw_line (cr, gc1,
                                x + width - 1, y + 1, x + width - 1, y + height - 1);
            }
        }
      
      /* Light around top and left */

      if (style->ythickness > 0)
        _cairo_draw_line (cr, gc2,
                          x, y, x + width - 2, y);
      if (style->xthickness > 0)
        _cairo_draw_line (cr, gc2,
                          x, y, x, y + height - 2);

      if (style->ythickness > 1)
        _cairo_draw_line (cr, &style->bg[state_type],
                          x + 1, y + 1, x + width - 3, y + 1);
      if (style->xthickness > 1)
        _cairo_draw_line (cr, &style->bg[state_type],
                          x + 1, y + 1, x + 1, y + height - 3);
      break;
      
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      if (style->xthickness > 0)
        {
          if (style->xthickness > 1)
            {
              thickness_light = 1;
              thickness_dark = 1;
      
              for (i = 0; i < thickness_dark; i++)
                {
                  _cairo_draw_line (cr, gc1,
                                    x + width - i - 1,
                                    y + i,
                                    x + width - i - 1,
                                    y + height - i - 1);
                  _cairo_draw_line (cr, gc2,
                                    x + i,
                                    y + i,
                                    x + i,
                                    y + height - i - 2);
                }
      
              for (i = 0; i < thickness_light; i++)
                {
                  _cairo_draw_line (cr, gc1,
                                    x + thickness_dark + i,
                                    y + thickness_dark + i,
                                    x + thickness_dark + i,
                                    y + height - thickness_dark - i - 1);
                  _cairo_draw_line (cr, gc2,
                                    x + width - thickness_light - i - 1,
                                    y + thickness_dark + i,
                                    x + width - thickness_light - i - 1,
                                    y + height - thickness_light - 1);
                }
            }
          else
            {
              _cairo_draw_line (cr,
                                &style->dark[state_type],
                                x, y, x, y + height);
              _cairo_draw_line (cr,
                                &style->dark[state_type],
                                x + width, y, x + width, y + height);
            }
        }

      if (style->ythickness > 0)
        {
          if (style->ythickness > 1)
            {
              thickness_light = 1;
              thickness_dark = 1;
      
              for (i = 0; i < thickness_dark; i++)
                {
                  _cairo_draw_line (cr, gc1,
                                    x + i,
                                    y + height - i - 1,
                                    x + width - i - 1,
                                    y + height - i - 1);
          
                  _cairo_draw_line (cr, gc2,
                                    x + i,
                                    y + i,
                                    x + width - i - 2,
                                    y + i);
                }
      
              for (i = 0; i < thickness_light; i++)
                {
                  _cairo_draw_line (cr, gc1,
                                    x + thickness_dark + i,
                                    y + thickness_dark + i,
                                    x + width - thickness_dark - i - 2,
                                    y + thickness_dark + i);
          
                  _cairo_draw_line (cr, gc2,
                                    x + thickness_dark + i,
                                    y + height - thickness_light - i - 1,
                                    x + width - thickness_light - 1,
                                    y + height - thickness_light - i - 1);
                }
            }
          else
            {
              _cairo_draw_line (cr,
                                &style->dark[state_type],
                                x, y, x + width, y);
              _cairo_draw_line (cr,
                                &style->dark[state_type],
                                x, y + height, x + width, y + height);
            }
        }
      
      break;
    }

  if (shadow_type == GTK_SHADOW_IN &&
      GTK_IS_SPIN_BUTTON (widget) &&
      detail && strcmp (detail, "entry") == 0)
    {
      if (get_direction (widget) == GTK_TEXT_DIR_LTR)
	{
          _cairo_draw_line (cr,
                            &style->base[state_type],
                            x + width - 1, y + 2,
                            x + width - 1, y + height - 3);
          _cairo_draw_line (cr,
                            &style->base[state_type],
                            x + width - 2, y + 2,
                            x + width - 2, y + height - 3);
          /* draw point */
          _cairo_draw_point (cr,
                             &style->black,
                             x + width - 1, y + 1);
          _cairo_draw_point (cr,
                             &style->bg[state_type],
                             x + width - 1, y + height - 2);
	}
      else
	{
          _cairo_draw_line (cr,
                            &style->base[state_type],
                            x, y + 2,
                            x, y + height - 3);
          _cairo_draw_line (cr,
                            &style->base[state_type],
                            x + 1, y + 2,
                            x + 1, y + height - 3);

          _cairo_draw_point (cr,
                             &style->black,
                             x, y + 1);

          _cairo_draw_line (cr,
                            &style->bg[state_type],
                            x, y + height - 2,
                            x + 1, y + height - 2);
          _cairo_draw_point (cr,
                             &style->light[state_type],
                             x, y + height - 1);
	}
    }
}

/**
 * gtk_cairo_paint_shadow:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the rectangle
 * @y: y origin of the rectangle
 * @width: width of the rectangle
 * @height: width of the rectangle
 *
 * Draws a shadow around the given rectangle in @cr 
 * using the given style and state and shadow type.
 */
void
gtk_cairo_paint_shadow (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_shadow (style, cr, state_type, shadow_type, widget, detail, x, y, width, height);

  cairo_restore (cr);
}

void
gtk_render_frame (GtkStyleContext *context,
                  cairo_t         *cr,
                  gdouble          x,
                  gdouble          y,
                  gdouble          width,
                  gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);

  /* This hardcodes the type of shadow to draw */
  /* as is, this does nothing */
  /* TODO: parhaps change to GTK_SHADOW_IN */
  gtk_cairo_paint_shadow (context, cr, state_type, GTK_SHADOW_NONE, (GtkWidget*)NULL, NULL, x, y, width, height);
}

static void
gtk_default_draw_expander (GtkStyle        *style,
                           cairo_t         *cr,
                           GtkStateType     state_type,
                           GtkWidget       *widget,
                           const gchar     *detail,
                           gint             x,
                           gint             y,
			   GtkExpanderStyle expander_style)
{
#define DEFAULT_EXPANDER_SIZE 12

  gint expander_size;
  gint line_width;
  double vertical_overshoot;
  int diameter;
  double radius;
  double interp;		/* interpolation factor for center position */
  double x_double_horz, y_double_horz;
  double x_double_vert, y_double_vert;
  double x_double, y_double;
  gint degrees = 0;

  if (widget &&
      gtk_widget_class_find_style_property (GTK_WIDGET_GET_CLASS (widget),
					    "expander-size"))
    {
      gtk_widget_style_get (widget,
			    "expander-size", &expander_size,
			    NULL);
    }
  else
    expander_size = DEFAULT_EXPANDER_SIZE;
    
  line_width = MAX (1, expander_size/9);

  switch (expander_style)
    {
    case GTK_EXPANDER_COLLAPSED:
      degrees = (get_direction (widget) == GTK_TEXT_DIR_RTL) ? 180 : 0;
      interp = 0.0;
      break;
    case GTK_EXPANDER_SEMI_COLLAPSED:
      degrees = (get_direction (widget) == GTK_TEXT_DIR_RTL) ? 150 : 30;
      interp = 0.25;
      break;
    case GTK_EXPANDER_SEMI_EXPANDED:
      degrees = (get_direction (widget) == GTK_TEXT_DIR_RTL) ? 120 : 60;
      interp = 0.75;
      break;
    case GTK_EXPANDER_EXPANDED:
      degrees = 90;
      interp = 1.0;
      break;
    default:
      g_assert_not_reached ();
    }

  /* Compute distance that the stroke extends beyonds the end
   * of the triangle we draw.
   */
  vertical_overshoot = line_width / 2.0 * (1. / tan (G_PI / 8));

  /* For odd line widths, we end the vertical line of the triangle
   * at a half pixel, so we round differently.
   */
  if (line_width % 2 == 1)
    vertical_overshoot = ceil (0.5 + vertical_overshoot) - 0.5;
  else
    vertical_overshoot = ceil (vertical_overshoot);

  /* Adjust the size of the triangle we draw so that the entire stroke fits
   */
  diameter = MAX (3, expander_size - 2 * vertical_overshoot);

  /* If the line width is odd, we want the diameter to be even,
   * and vice versa, so force the sum to be odd. This relationship
   * makes the point of the triangle look right.
   */
  diameter -= (1 - (diameter + line_width) % 2);
  
  radius = diameter / 2.;

  /* Adjust the center so that the stroke is properly aligned with
   * the pixel grid. The center adjustment is different for the
   * horizontal and vertical orientations. For intermediate positions
   * we interpolate between the two.
   */
  x_double_vert = floor (x - (radius + line_width) / 2.) + (radius + line_width) / 2.;
  y_double_vert = y - 0.5;

  x_double_horz = x - 0.5;
  y_double_horz = floor (y - (radius + line_width) / 2.) + (radius + line_width) / 2.;

  x_double = x_double_vert * (1 - interp) + x_double_horz * interp;
  y_double = y_double_vert * (1 - interp) + y_double_horz * interp;
  
  cairo_translate (cr, x_double, y_double);
  cairo_rotate (cr, degrees * G_PI / 180);

  cairo_move_to (cr, - radius / 2., - radius);
  cairo_line_to (cr,   radius / 2.,   0);
  cairo_line_to (cr, - radius / 2.,   radius);
  cairo_close_path (cr);
  
  cairo_set_line_width (cr, line_width);

  switch (state_type) {
  case GTK_STATE_PRELIGHT:
    gdk_cairo_set_source_color (cr,
				&style->fg[GTK_STATE_PRELIGHT]);
  break;
  case GTK_STATE_ACTIVE:
    gdk_cairo_set_source_color (cr,
				&style->light[GTK_STATE_ACTIVE]);
  break;
  default:
    gdk_cairo_set_source_color (cr,
				&style->base[GTK_STATE_NORMAL]);
  break;
  }

  cairo_fill_preserve (cr);

  gdk_cairo_set_source_color (cr, &style->fg[state_type]);
  cairo_stroke (cr);
}

/**
 * gtk_cairo_paint_expander:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: the x position to draw the expander at
 * @y: the y position to draw the expander at
 * @expander_style: the style to draw the expander in; determines
 *   whether the expander is collapsed, expanded, or in an
 *   intermediate state.
 * 
 * Draws an expander as used in #GtkTreeView. @x and @y specify the
 * center the expander. The size of the expander is determined by the
 * "expander-size" style property of @widget.  (If widget is not
 * specified or doesn't have an "expander-size" property, an
 * unspecified default size will be used, since the caller doesn't
 * have sufficient information to position the expander, this is
 * likely not useful.) The expander is expander_size pixels tall
 * in the collapsed position and expander_size pixels wide in the
 * expanded position.
 **/
void
gtk_cairo_paint_expander (GtkStyle           *style,
                          cairo_t            *cr,
                          GtkStateType        state_type,
                          GtkWidget          *widget,
                          const gchar        *detail,
                          gint                x,
                          gint                y,
                          GtkExpanderStyle    expander_style)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_expander (style, cr, state_type, widget, detail, x, y, expander_style);

  cairo_restore (cr);
}

void
gtk_render_expander (GtkStyleContext *context,
                     cairo_t         *cr,
                     gdouble          x,
                     gdouble          y,
                     gdouble          width,
                     gdouble          height)
{
  GtkExpanderStyle expander_style;
  GtkStateType state_type;

  expander_style = (width >= 0 || height >= 0) ? GTK_EXPANDER_EXPANDED : GTK_EXPANDER_COLLAPSED;
  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_expander (context, cr, state_type, (GtkWidget*)NULL, NULL, x, y, expander_style);
}

static void 
gtk_default_draw_focus (GtkStyle      *style,
			cairo_t       *cr,
			GtkStateType   state_type,
			GtkWidget     *widget,
			const gchar   *detail,
			gint           x,
			gint           y,
			gint           width,
			gint           height)
{
  gboolean free_dash_list = FALSE;
  gint line_width = 1;
  gint8 *dash_list = (gint8 *) "\1\1";

  if (widget)
    {
      gtk_widget_style_get (widget,
			    "focus-line-width", &line_width,
			    "focus-line-pattern", (gchar *)&dash_list,
			    NULL);

      free_dash_list = TRUE;
  }

  if (detail && !strcmp (detail, "add-mode"))
    {
      if (free_dash_list)
	g_free (dash_list);

      dash_list = (gint8 *) "\4\4";
      free_dash_list = FALSE;
    }

  if (detail && !strcmp (detail, "colorwheel_light"))
    cairo_set_source_rgb (cr, 0., 0., 0.);
  else if (detail && !strcmp (detail, "colorwheel_dark"))
    cairo_set_source_rgb (cr, 1., 1., 1.);
  else
    gdk_cairo_set_source_color (cr, &style->fg[state_type]);

  cairo_set_line_width (cr, line_width);

  if (dash_list[0])
    {
      gint n_dashes = strlen ((const gchar *) dash_list);
      gdouble *dashes = g_new (gdouble, n_dashes);
      gdouble total_length = 0;
      gdouble dash_offset;
      gint i;

      for (i = 0; i < n_dashes; i++)
	{
	  dashes[i] = dash_list[i];
	  total_length += dash_list[i];
	}

      /* The dash offset here aligns the pattern to integer pixels
       * by starting the dash at the right side of the left border
       * Negative dash offsets in cairo don't work
       * (https://bugs.freedesktop.org/show_bug.cgi?id=2729)
       */
      dash_offset = - line_width / 2.;
      while (dash_offset < 0)
	dash_offset += total_length;
      
      cairo_set_dash (cr, dashes, n_dashes, dash_offset);
      g_free (dashes);
    }

  cairo_rectangle (cr,
		   x + line_width / 2.,
		   y + line_width / 2.,
		   width - line_width,
		   height - line_width);
  cairo_stroke (cr);

  if (free_dash_list)
    g_free (dash_list);
}

/**
 * gtk_cairo_paint_focus:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: the x origin of the rectangle around which to draw a focus indicator
 * @y: the y origin of the rectangle around which to draw a focus indicator
 * @width: the width of the rectangle around which to draw a focus indicator
 * @height: the height of the rectangle around which to draw a focus indicator
 *
 * Draws a focus indicator around the given rectangle on @cr using the
 * given style.
 */
void
gtk_cairo_paint_focus (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_focus (style, cr, state_type, widget, detail, x, y, width, height);

  cairo_restore (cr);
}

void
gtk_render_focus (GtkStyleContext *context,
                  cairo_t         *cr,
                  gdouble          x,
                  gdouble          y,
                  gdouble          width,
                  gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_focus (context, cr, state_type, (GtkWidget*)NULL, NULL, x, y, width, height);
}

static void
gtk_default_draw_layout (GtkStyle        *style,
                         cairo_t         *cr,
                         GtkStateType     state_type,
			 gboolean         use_text,
                         GtkWidget       *widget, /* not used */
                         const gchar     *detail,
                         gint             x,
                         gint             y,
                         PangoLayout     *layout)
{
  GdkColor *gc;
  const PangoMatrix *matrix;

  matrix = pango_context_get_matrix (pango_layout_get_context (layout));
  if (matrix)
    {
      cairo_matrix_t cairo_matrix;
      PangoRectangle rect;

      cairo_matrix_init (&cairo_matrix,
                         matrix->xx, matrix->yx,
                         matrix->xy, matrix->yy,
                         matrix->x0, matrix->y0);

      pango_layout_get_extents (layout, NULL, &rect);
      pango_matrix_transform_rectangle (matrix, &rect);
      pango_extents_to_pixels (&rect, NULL);

      cairo_matrix.x0 += x - rect.x;
      cairo_matrix.y0 += y - rect.y;

      cairo_set_matrix (cr, &cairo_matrix);
    }
  else
    cairo_translate (cr, x, y);

  cairo_new_path (cr);

  if (state_type == GTK_STATE_INSENSITIVE)
    {
      gdk_cairo_set_source_color (cr, &style->white);
      cairo_move_to (cr, 1, 1);
      gtk2_gtk_pango_fill_layout (cr, layout);
      cairo_new_path (cr);
    }

  gc = use_text ? &style->text[state_type] : &style->fg[state_type];

  gdk_cairo_set_source_color (cr, gc);

  pango_cairo_show_layout (cr, layout);
}

/**
 * gtk_cairo_paint_layout:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @use_text: whether to use the text or foreground
 *            graphics context of @style
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin
 * @y: y origin
 * @layout: the layout to draw
 *
 * Draws a layout on @cr using the given parameters.
 **/
void
gtk_cairo_paint_layout (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        gboolean            use_text,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        PangoLayout        *layout)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_layout (style, cr, state_type, use_text, widget, detail, x, y, layout);

  cairo_restore (cr);
}

void
gtk_render_layout (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   PangoLayout     *layout)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_layout (context, cr, state_type, FALSE, NULL, NULL, x, y, layout);
}

static void
gtk_default_draw_line (GtkStyle      *style,
                       cairo_t       *cr,
                       GtkStateType  state_type,
                       GtkWidget     *widget, /* not used */
                       const gchar   *detail,
                       gint          x1,
                       gint          y1,
                       gint          x2,
                       gint          y2)
{
  gint xthickness_light, ythickness_light;
  gint xthickness_dark, ythickness_dark;
  gint thickness_light;
  gint thickness_dark;
  gint i;
  gdouble inv_hyp;
  gdouble cos;
  gdouble sin;
  gint dx;
  gint dy;

  xthickness_light = style->xthickness / 2;
  ythickness_light = style->ythickness / 2;
  xthickness_dark = style->xthickness - xthickness_light;
  ythickness_dark = style->ythickness - ythickness_light;

  /* speed up regular cases */
  if (x1 == x2) { /* 0 degrees */
    cos = 1;
    sin = 0;
  }
  else if (y1 == y2) { /* 90 degrees */
    cos = 0;
    sin = 1;
  }
  else {
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    if (dx == dy) { /* 45 degrees */
      cos = 0.70710678118654752440084436210484903928483593768847403658833986899536623923;
      sin = 0.70710678118654752440084436210484903928483593768847403658833986899536623923;
    }
    else {
      /* can't avoid sqrt, the loop index can avoid it, but it is needed as an array index */
      inv_hyp = 1/sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
      cos = dx * inv_hyp;
      sin = dy * inv_hyp;
    }
  }

  thickness_light = xthickness_light * cos + ythickness_light * sin;
  thickness_dark = xthickness_dark * cos + ythickness_dark * sin;

  cairo_set_line_width (cr, 1.0);

  for (i = 0; i < thickness_dark; i++)
    { 
      _cairo_draw_line (cr, &style->dark[state_type],
                        x1 + i, y1, x2 + i, y2 - i - 1);
      _cairo_draw_line (cr, &style->light[state_type],
                        x2 + i, y2 - i, x2 + i, y2);
    }
  
  x1 += thickness_dark;
  x2 += thickness_dark;
  for (i = 0; i < thickness_light; i++)
    {
      _cairo_draw_line (cr, &style->dark[state_type],
                        x1 + i, y1, x2 + i, y1 + thickness_light - i - 1);
      _cairo_draw_line (cr, &style->light[state_type],
                        x1 + i, y1 + thickness_light - i, x2 + i, y2);
    }
}

/**
 * gtk_cairo_paint_line:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x1: the starting x coordinate
 * @y1: the starting y coordinate
 * @x2: the ending x coordinate
 * @y2: the ending y coordinate
 *
 * Draws a vertical line from (@x1, @y1) to (@x2, @y2) in @cr
 * using the given style and state.
 */
void
gtk_cairo_paint_line (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x1,
                       gint                y1,
                       gint                x2,
                       gint                y2)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_line (style, cr, state_type, widget, detail, x1, y1, x2, y2);

  cairo_restore (cr);
}

void
gtk_render_line (GtkStyleContext *context,
                 cairo_t         *cr,
                 gdouble          x0,
                 gdouble          y0,
                 gdouble          x1,
                 gdouble          y1)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_line (context, cr, state_type, NULL, NULL, x0, y0, x1, y1);
}

static void
gtk_default_draw_hline (GtkStyle      *style,
                        cairo_t       *cr,
                        GtkStateType  state_type,
                        GtkWidget     *widget,
                        const gchar   *detail,
                        gint          x1,
                        gint          x2,
                        gint          y)
{
  gint thickness_light;
  gint thickness_dark;
  gint i;
  
  thickness_light = style->ythickness / 2;
  thickness_dark = style->ythickness - thickness_light;
  
  cairo_set_line_width (cr, 1.0);

  if (detail && !strcmp (detail, "label"))
    {
      if (state_type == GTK_STATE_INSENSITIVE)
        _cairo_draw_line (cr, &style->white, x1 + 1, y + 1, x2 + 1, y + 1);
      _cairo_draw_line (cr, &style->fg[state_type], x1, y, x2, y);
    }
  else
    {
      for (i = 0; i < thickness_dark; i++)
        {
          _cairo_draw_line (cr, &style->dark[state_type], x1, y + i, x2 - i - 1, y + i);
          _cairo_draw_line (cr, &style->light[state_type], x2 - i, y + i, x2, y + i);
        }
      
      y += thickness_dark;
      for (i = 0; i < thickness_light; i++)
        {
          _cairo_draw_line (cr, &style->dark[state_type], x1, y + i, x1 + thickness_light - i - 1, y + i);
          _cairo_draw_line (cr, &style->light[state_type], x1 + thickness_light - i, y + i, x2, y + i);
        }
    }
}

/**
 * gtk_cairo_paint_hline:
 * @style: a #GtkStyle
 * @cr: a #caio_t
 * @state_type: a state
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x1: the starting x coordinate
 * @x2: the ending x coordinate
 * @y: the y coordinate
 *
 * Draws a horizontal line from (@x1, @y) to (@x2, @y) in @cr
 * using the given style and state.
 **/ 
void 
gtk_cairo_paint_hline (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x1,
                       gint                x2,
                       gint                y)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_hline (style, cr, state_type, widget, detail, x1, x2, y);

  cairo_restore (cr);
}

static void
gtk_default_draw_vline (GtkStyle      *style,
                        cairo_t       *cr,
                        GtkStateType  state_type,
                        GtkWidget     *widget,
                        const gchar   *detail,
                        gint          y1,
                        gint          y2,
                        gint          x)
{
  gint thickness_light;
  gint thickness_dark;
  gint i;
  
  thickness_light = style->xthickness / 2;
  thickness_dark = style->xthickness - thickness_light;

  cairo_set_line_width (cr, 1.0);

  for (i = 0; i < thickness_dark; i++)
    { 
      _cairo_draw_line (cr, &style->dark[state_type],
                        x + i, y1, x + i, y2 - i - 1);
      _cairo_draw_line (cr, &style->light[state_type],
                        x + i, y2 - i, x + i, y2);
    }
  
  x += thickness_dark;
  for (i = 0; i < thickness_light; i++)
    {
      _cairo_draw_line (cr, &style->dark[state_type],
                        x + i, y1, x + i, y1 + thickness_light - i - 1);
      _cairo_draw_line (cr, &style->light[state_type],
                        x + i, y1 + thickness_light - i, x + i, y2);
    }
}

/**
 * gtk_cairo_paint_vline:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @y1_: the starting y coordinate
 * @y2_: the ending y coordinate
 * @x: the x coordinate
 *
 * Draws a vertical line from (@x, @y1_) to (@x, @y2_) in @cr
 * using the given style and state.
 */
void
gtk_cairo_paint_vline (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                y1_,
                       gint                y2_,
                       gint                x)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_vline (style, cr, state_type, widget, detail, y1_, y2_, x);

  cairo_restore (cr);
}

static void
option_menu_get_props (GtkWidget      *widget,
		       GtkRequisition *indicator_size,
		       GtkBorder      *indicator_spacing)
{
  GtkRequisition *tmp_size = NULL;
  GtkBorder *tmp_spacing = NULL;

  if (tmp_size)
    {
      *indicator_size = *tmp_size;
      gtk_requisition_free (tmp_size);
    }
  else
    *indicator_size = default_option_indicator_size;

  if (tmp_spacing)
    {
      *indicator_spacing = *tmp_spacing;
      gtk_border_free (tmp_spacing);
    }
  else
    *indicator_spacing = default_option_indicator_spacing;
}

static void 
gtk_default_draw_box (GtkStyle      *style,
		      cairo_t       *cr,
		      GtkStateType   state_type,
		      GtkShadowType  shadow_type,
		      GtkWidget     *widget,
		      const gchar   *detail,
		      gint           x,
		      gint           y,
		      gint           width,
		      gint           height)
{
  gboolean is_spinbutton_box = FALSE;
  
  if (GTK_IS_SPIN_BUTTON (widget) && detail)
    {
      if (strcmp (detail, "spinbutton_up") == 0)
	{
	  y += 2;
	  width -= 3;
	  height -= 2;

	  if (get_direction (widget) == GTK_TEXT_DIR_RTL)
	    x += 2;
	  else
	    x += 1;

	  is_spinbutton_box = TRUE;
	}
      else if (strcmp (detail, "spinbutton_down") == 0)
	{
	  width -= 3;
	  height -= 2;

	  if (get_direction (widget) == GTK_TEXT_DIR_RTL)
	    x += 2;
	  else
	    x += 1;

	  is_spinbutton_box = TRUE;
	}
    }
  
  if (!style->bg_pixmap[state_type])
    {
      GdkColor *gc = &style->bg[state_type];

      if (state_type == GTK_STATE_SELECTED && detail && strcmp (detail, "paned") == 0)
	{
	  if (widget && !gtk_widget_has_focus (widget))
	    gc = &style->base[GTK_STATE_ACTIVE];
	}

      _cairo_draw_rectangle (cr, gc, TRUE,
                             x, y, width, height);
    }
  else
    _gtk_style_apply_default_background (style, cr, gtk_widget_get_window (widget),
                                        state_type, x, y, width, height);


  if (is_spinbutton_box)
    {
      GdkColor *upper;
      GdkColor *lower;

      lower = &style->dark[state_type];
      if (shadow_type == GTK_SHADOW_OUT)
	upper = &style->light[state_type];
      else
	upper = &style->dark[state_type];

      _cairo_draw_line (cr, upper, x, y, x + width - 1, y);
      _cairo_draw_line (cr, lower, x, y + height - 1, x + width - 1, y + height - 1);

      return;
    }

  gtk_cairo_paint_shadow (style, cr, state_type, shadow_type, widget, detail,
                          x, y, width, height);

  if (detail && strcmp (detail, "optionmenu") == 0)
    {
      GtkRequisition indicator_size;
      GtkBorder indicator_spacing;
      gint vline_x;

      option_menu_get_props (widget, &indicator_size, &indicator_spacing);

      if (get_direction (widget) == GTK_TEXT_DIR_RTL)
	vline_x = x + indicator_size.width + indicator_spacing.left + indicator_spacing.right;
      else 
	vline_x = x + width - (indicator_size.width + indicator_spacing.left + indicator_spacing.right) - style->xthickness;

      gtk_cairo_paint_vline (style, cr, state_type, widget,
                             detail,
                             y + style->ythickness + 1,
                             y + height - style->ythickness - 3,
                             vline_x);
    }
}

/**
 * gtk_cairo_paint_box:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: the type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the box
 * @y: y origin of the box
 * @width: the width of the box
 * @height: the height of the box
 * 
 * Draws a box on @cr with the given parameters.
 */
void
gtk_cairo_paint_box (GtkStyle           *style,
                     cairo_t            *cr,
                     GtkStateType        state_type,
                     GtkShadowType       shadow_type,
                     GtkWidget          *widget,
                     const gchar        *detail,
                     gint                x,
                     gint                y,
                     gint                width,
                     gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_box (style, cr, state_type, shadow_type, widget, detail, x, y, width, height);

  cairo_restore (cr);
}


static void 
gtk_default_draw_slider (GtkStyle      *style,
                         cairo_t       *cr,
                         GtkStateType   state_type,
                         GtkShadowType  shadow_type,
                         GtkWidget     *widget,
                         const gchar   *detail,
                         gint           x,
                         gint           y,
                         gint           width,
                         gint           height,
                         GtkOrientation orientation)
{
  gtk_cairo_paint_box (style, cr, state_type, shadow_type,
                       widget, detail, x, y, width, height);

  if (detail &&
      (strcmp ("hscale", detail) == 0 ||
       strcmp ("vscale", detail) == 0))
    {
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_cairo_paint_vline (style, cr, state_type, widget, detail, 
                               y + style->ythickness, 
                               y + height - style->ythickness - 1, x + width / 2);
      else
        gtk_cairo_paint_hline (style, cr, state_type, widget, detail, 
                               x + style->xthickness, 
                               x + width - style->xthickness - 1, y + height / 2);
    }
}

/**
 * gtk_cairo_paint_slider:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: a shadow
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: the x origin of the rectangle in which to draw a slider
 * @y: the y origin of the rectangle in which to draw a slider
 * @width: the width of the rectangle in which to draw a slider
 * @height: the height of the rectangle in which to draw a slider
 * @orientation: the orientation to be used
 *
 * Draws a slider in the given rectangle on @cr using the
 * given style and orientation.
 **/
void
gtk_cairo_paint_slider (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height,
                        GtkOrientation      orientation)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_slider (style, cr, state_type, shadow_type, widget, detail, x, y, width, height, orientation);

  cairo_restore (cr);
}

void
gtk_render_slider (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   gdouble          width,
                   gdouble          height,
                   GtkOrientation   orientation)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_slider (context, cr, state_type, GTK_SHADOW_NONE, (GtkWidget*)NULL, NULL, x, y, width, height, orientation);
}

static void 
gtk_default_draw_shadow_gap (GtkStyle       *style,
                             cairo_t        *cr,
                             GtkStateType    state_type,
                             GtkShadowType   shadow_type,
                             GtkWidget      *widget, /* not used */
                             const gchar    *detail,
                             gint            x,
                             gint            y,
                             gint            width,
                             gint            height,
                             GtkPositionType gap_side,
                             gint            gap_x,
                             gint            gap_width)
{
  GdkColor *color1 = NULL;
  GdkColor *color2 = NULL;
  GdkColor *color3 = NULL;
  GdkColor *color4 = NULL;
  
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    default:
      return;
    case GTK_SHADOW_IN:
      color1 = &style->dark[state_type];
      color2 = &style->black;
      color3 = &style->bg[state_type];
      color4 = &style->light[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      color1 = &style->dark[state_type];
      color2 = &style->light[state_type];
      color3 = &style->dark[state_type];
      color4 = &style->light[state_type];
      break;
    case GTK_SHADOW_OUT:
      color1 = &style->light[state_type];
      color2 = &style->bg[state_type];
      color3 = &style->dark[state_type];
      color4 = &style->black;
      break;
    case GTK_SHADOW_ETCHED_OUT:
      color1 = &style->light[state_type];
      color2 = &style->dark[state_type];
      color3 = &style->light[state_type];
      color4 = &style->dark[state_type];
      break;
    }

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      switch (gap_side)
        {
        case GTK_POS_TOP:
          _cairo_draw_line (cr, color1,
                            x, y, x, y + height - 1);
          _cairo_draw_line (cr, color2,
                            x + 1, y, x + 1, y + height - 2);
          
          _cairo_draw_line (cr, color3,
                            x + 1, y + height - 2, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, color3,
                            x + width - 2, y, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, color4,
                            x, y + height - 1, x + width - 1, y + height - 1);
          _cairo_draw_line (cr, color4,
                            x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              _cairo_draw_line (cr, color1,
                                x, y, x + gap_x - 1, y);
              _cairo_draw_line (cr, color2,
                                x + 1, y + 1, x + gap_x - 1, y + 1);
              _cairo_draw_line (cr, color2,
                                x + gap_x, y, x + gap_x, y);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              _cairo_draw_line (cr, color1,
                                x + gap_x + gap_width, y, x + width - 2, y);
              _cairo_draw_line (cr, color2,
                                x + gap_x + gap_width, y + 1, x + width - 3, y + 1);
              _cairo_draw_line (cr, color2,
                                x + gap_x + gap_width - 1, y, x + gap_x + gap_width - 1, y);
            }
          break;
        case GTK_POS_BOTTOM:
          _cairo_draw_line (cr, color1,
                            x, y, x + width - 1, y);
          _cairo_draw_line (cr, color1,
                            x, y, x, y + height - 1);
          _cairo_draw_line (cr, color2,
                            x + 1, y + 1, x + width - 2, y + 1);
          _cairo_draw_line (cr, color2,
                            x + 1, y + 1, x + 1, y + height - 1);
          
          _cairo_draw_line (cr, color3,
                            x + width - 2, y + 1, x + width - 2, y + height - 1);
          _cairo_draw_line (cr, color4,
                            x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              _cairo_draw_line (cr, color4,
                                x, y + height - 1, x + gap_x - 1, y + height - 1);
              _cairo_draw_line (cr, color3,
                                x + 1, y + height - 2, x + gap_x - 1, y + height - 2);
              _cairo_draw_line (cr, color3,
                                x + gap_x, y + height - 1, x + gap_x, y + height - 1);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              _cairo_draw_line (cr, color4,
                                x + gap_x + gap_width, y + height - 1, x + width - 2, y + height - 1);
              _cairo_draw_line (cr, color3,
                                x + gap_x + gap_width, y + height - 2, x + width - 2, y + height - 2);
              _cairo_draw_line (cr, color3,
                                x + gap_x + gap_width - 1, y + height - 1, x + gap_x + gap_width - 1, y + height - 1);
            }
          break;
        case GTK_POS_LEFT:
          _cairo_draw_line (cr, color1,
                            x, y, x + width - 1, y);
          _cairo_draw_line (cr, color2,
                            x, y + 1, x + width - 2, y + 1);
          
          _cairo_draw_line (cr, color3,
                            x, y + height - 2, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, color3,
                            x + width - 2, y + 1, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, color4,
                            x, y + height - 1, x + width - 1, y + height - 1);
          _cairo_draw_line (cr, color4,
                            x + width - 1, y, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              _cairo_draw_line (cr, color1,
                                x, y, x, y + gap_x - 1);
              _cairo_draw_line (cr, color2,
                                x + 1, y + 1, x + 1, y + gap_x - 1);
              _cairo_draw_line (cr, color2,
                                x, y + gap_x, x, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              _cairo_draw_line (cr, color1,
                                x, y + gap_x + gap_width, x, y + height - 2);
              _cairo_draw_line (cr, color2,
                                x + 1, y + gap_x + gap_width, x + 1, y + height - 2);
              _cairo_draw_line (cr, color2,
                                x, y + gap_x + gap_width - 1, x, y + gap_x + gap_width - 1);
            }
          break;
        case GTK_POS_RIGHT:
          _cairo_draw_line (cr, color1,
                            x, y, x + width - 1, y);
          _cairo_draw_line (cr, color1,
                            x, y, x, y + height - 1);
          _cairo_draw_line (cr, color2,
                            x + 1, y + 1, x + width - 1, y + 1);
          _cairo_draw_line (cr, color2,
                            x + 1, y + 1, x + 1, y + height - 2);
          
          _cairo_draw_line (cr, color3,
                            x + 1, y + height - 2, x + width - 1, y + height - 2);
          _cairo_draw_line (cr, color4,
                            x, y + height - 1, x + width - 1, y + height - 1);
          if (gap_x > 0)
            {
              _cairo_draw_line (cr, color4,
                                x + width - 1, y, x + width - 1, y + gap_x - 1);
              _cairo_draw_line (cr, color3,
                                x + width - 2, y + 1, x + width - 2, y + gap_x - 1);
              _cairo_draw_line (cr, color3,
                                x + width - 1, y + gap_x, x + width - 1, y + gap_x);
            }
          if ((width - (gap_x + gap_width)) > 0)
            {
              _cairo_draw_line (cr, color4,
                                x + width - 1, y + gap_x + gap_width, x + width - 1, y + height - 2);
              _cairo_draw_line (cr, color3,
                                x + width - 2, y + gap_x + gap_width, x + width - 2, y + height - 2);
              _cairo_draw_line (cr, color3,
                                x + width - 1, y + gap_x + gap_width - 1, x + width - 1, y + gap_x + gap_width - 1);
            }
          break;
        }
    }
}

/**
 * gtk_cairo_paint_shadow_gap:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the rectangle
 * @y: y origin of the rectangle
 * @width: width of the rectangle
 * @height: width of the rectangle
 * @gap_side: side in which to leave the gap
 * @gap_x: starting position of the gap
 * @gap_width: width of the gap
 *
 * Draws a shadow around the given rectangle in @cr
 * using the given style and state and shadow type, leaving a 
 * gap in one side.
*/
void
gtk_cairo_paint_shadow_gap (GtkStyle           *style,
                            cairo_t            *cr,
                            GtkStateType        state_type,
                            GtkShadowType       shadow_type,
                            GtkWidget          *widget,
                            const gchar        *detail,
                            gint                x,
                            gint                y,
                            gint                width,
                            gint                height,
                            GtkPositionType     gap_side,
                            gint                gap_x,
                            gint                gap_width)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_shadow_gap (style, cr, state_type, shadow_type, widget, detail, x, y, width, height, gap_side, gap_x, gap_width);

  cairo_restore (cr);
}

void
gtk_render_frame_gap (GtkStyleContext *context,
                      cairo_t         *cr,
                      gdouble          x,
                      gdouble          y,
                      gdouble          width,
                      gdouble          height,
                      GtkPositionType  gap_side,
                      gdouble          xy0_gap,
                      gdouble          xy1_gap)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_shadow_gap (context, cr, state_type, GTK_SHADOW_NONE, NULL, NULL, x, y, width, height, gap_side, xy0_gap, abs(xy1_gap - xy0_gap));
}

static void 
gtk_default_draw_extension (GtkStyle       *style,
                            cairo_t        *cr,
                            GtkStateType    state_type,
                            GtkShadowType   shadow_type,
                            GtkWidget      *widget,
                            const gchar    *detail,
                            gint            x,
                            gint            y,
                            gint            width,
                            gint            height,
                            GtkPositionType gap_side)
{
  GdkWindow *window = gtk_widget_get_window (widget);
  GdkColor color1;
  GdkColor color2;
  GdkColor color3;
  GdkColor color4;
  
  switch (gap_side)
    {
    case GTK_POS_TOP:
      _gtk_style_apply_default_background (style, cr, window,
                                           state_type,
                                           x + 1,
                                           y,
                                           width - 2,
                                           height - 1);
      break;
    case GTK_POS_BOTTOM:
      _gtk_style_apply_default_background (style, cr, window,
                                           state_type,
                                           x + 1,
                                           y + 1,
                                           width - 2,
                                           height - 1);
      break;
    case GTK_POS_LEFT:
      _gtk_style_apply_default_background (style, cr, window,
                                           state_type,
                                           x,
                                           y + 1,
                                           width - 1,
                                           height - 2);
      break;
    case GTK_POS_RIGHT:
      _gtk_style_apply_default_background (style, cr, window,
                                           state_type,
                                           x + 1,
                                           y + 1,
                                           width - 1,
                                           height - 2);
      break;
    }

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
      color1 = style->dark[state_type];
      color2 = style->black;
      color3 = style->bg[state_type];
      color4 = style->light[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      color1 = style->dark[state_type];
      color2 = style->light[state_type];
      color3 = style->dark[state_type];
      color4 = style->light[state_type];
      break;
    case GTK_SHADOW_OUT:
      color1 = style->light[state_type];
      color2 = style->bg[state_type];
      color3 = style->dark[state_type];
      color4 = style->black;
      break;
    case GTK_SHADOW_ETCHED_OUT:
      color1 = style->light[state_type];
      color2 = style->dark[state_type];
      color3 = style->light[state_type];
      color4 = style->dark[state_type];
      break;
    }

  cairo_set_line_width (cr, 1.0);

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
    case GTK_SHADOW_IN:
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      switch (gap_side)
        {
        case GTK_POS_TOP:
          _cairo_draw_line (cr, &color1,
                            x, y, x, y + height - 2);
          _cairo_draw_line (cr, &color2,
                            x + 1, y, x + 1, y + height - 2);
          
          _cairo_draw_line (cr, &color3,
                            x + 2, y + height - 2, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, &color3,
                            x + width - 2, y, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, &color4,
                            x + 1, y + height - 1, x + width - 2, y + height - 1);
          _cairo_draw_line (cr, &color4,
                            x + width - 1, y, x + width - 1, y + height - 2);
          break;
        case GTK_POS_BOTTOM:
          _cairo_draw_line (cr, &color1,
                            x + 1, y, x + width - 2, y);
          _cairo_draw_line (cr, &color1,
                            x, y + 1, x, y + height - 1);
          _cairo_draw_line (cr, &color2,
                            x + 1, y + 1, x + width - 2, y + 1);
          _cairo_draw_line (cr, &color2,
                            x + 1, y + 1, x + 1, y + height - 1);
          
          _cairo_draw_line (cr, &color3,
                            x + width - 2, y + 2, x + width - 2, y + height - 1);
          _cairo_draw_line (cr, &color4,
                            x + width - 1, y + 1, x + width - 1, y + height - 1);
          break;
        case GTK_POS_LEFT:
          _cairo_draw_line (cr, &color1,
                            x, y, x + width - 2, y);
          _cairo_draw_line (cr, &color2,
                            x + 1, y + 1, x + width - 2, y + 1);
          
          _cairo_draw_line (cr, &color3,
                            x, y + height - 2, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, &color3,
                            x + width - 2, y + 2, x + width - 2, y + height - 2);
          _cairo_draw_line (cr, &color4,
                            x, y + height - 1, x + width - 2, y + height - 1);
          _cairo_draw_line (cr, &color4,
                            x + width - 1, y + 1, x + width - 1, y + height - 2);
          break;
        case GTK_POS_RIGHT:
          _cairo_draw_line (cr, &color1,
                            x + 1, y, x + width - 1, y);
          _cairo_draw_line (cr, &color1,
                            x, y + 1, x, y + height - 2);
          _cairo_draw_line (cr, &color2,
                            x + 1, y + 1, x + width - 1, y + 1);
          _cairo_draw_line (cr, &color2,
                            x + 1, y + 1, x + 1, y + height - 2);
          
          _cairo_draw_line (cr, &color3,
                            x + 2, y + height - 2, x + width - 1, y + height - 2);
          _cairo_draw_line (cr, &color4,
                            x + 1, y + height - 1, x + width - 1, y + height - 1);
          break;
        }
    }
}

/**
 * gtk_cairo_paint_extension: 
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the extension
 * @y: y origin of the extension
 * @width: width of the extension
 * @height: width of the extension
 * @gap_side: the side on to which the extension is attached
 * 
 * Draws an extension, i.e. a notebook tab.
 **/
void
gtk_cairo_paint_extension (GtkStyle           *style,
                           cairo_t            *cr,
                           GtkStateType        state_type,
                           GtkShadowType       shadow_type,
                           GtkWidget          *widget,
                           const gchar        *detail,
                           gint                x,
                           gint                y,
                           gint                width,
                           gint                height,
                           GtkPositionType     gap_side)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_extension (style, cr, state_type, shadow_type, widget, detail, x, y, width, height, gap_side);

  cairo_restore (cr);
}

void
gtk_render_extension (GtkStyleContext *context,
                      cairo_t         *cr,
                      gdouble          x,
                      gdouble          y,
                      gdouble          width,
                      gdouble          height,
                      GtkPositionType  gap_side)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_extension (context, cr, state_type, GTK_SHADOW_NONE, (GtkWidget*)NULL, NULL, x, y, width, height, gap_side);
}

static void
draw_dot (cairo_t    *cr,
	  GdkColor   *light,
	  GdkColor   *dark,
	  gint        x,
	  gint        y,
	  gushort     size)
{
  size = CLAMP (size, 2, 3);

  if (size == 2)
    {
      _cairo_draw_point (cr, light, x, y);
      _cairo_draw_point (cr, light, x+1, y+1);
    }
  else if (size == 3)
    {
      _cairo_draw_point (cr, light, x, y);
      _cairo_draw_point (cr, light, x+1, y);
      _cairo_draw_point (cr, light, x, y+1);
      _cairo_draw_point (cr, dark, x+1, y+2);
      _cairo_draw_point (cr, dark, x+2, y+1);
      _cairo_draw_point (cr, dark, x+2, y+2);
    }
}

static void 
gtk_default_draw_handle (GtkStyle      *style,
			 cairo_t       *cr,
			 GtkStateType   state_type,
			 GtkShadowType  shadow_type,
			 GtkWidget     *widget,
			 const gchar   *detail,
			 gint           x,
			 gint           y,
			 gint           width,
			 gint           height,
			 GtkOrientation orientation)
{
  gint xx, yy;
  gint xthick, ythick;
  GdkColor light, dark;
  
  gtk_cairo_paint_box (style, cr, state_type, shadow_type, widget, 
                       detail, x, y, width, height);
  
  if (detail && !strcmp (detail, "paned"))
    {
      /* we want to ignore the shadow border in paned widgets */
      xthick = 0;
      ythick = 0;

      if (state_type == GTK_STATE_SELECTED && widget && !gtk_widget_has_focus (widget))
	  gtk2_gtk_style_shade (&style->base[GTK_STATE_ACTIVE], &light,
                            LIGHTNESS_MULT);
      else
	light = style->light[state_type];

      dark = style->black;
    }
  else
    {
      xthick = style->xthickness;
      ythick = style->ythickness;

      light = style->light[state_type];
      dark = style->dark[state_type];
    }
  
  cairo_rectangle(cr, x + xthick, y + ythick,
                  width - (xthick * 2), height - (ythick * 2));
  cairo_clip (cr);

  if (detail && !strcmp (detail, "paned"))
    {
      if (orientation == GTK_ORIENTATION_HORIZONTAL)
	for (xx = x + width/2 - 15; xx <= x + width/2 + 15; xx += 5)
	  draw_dot (cr, &light, &dark, xx, y + height/2 - 1, 3);
      else
	for (yy = y + height/2 - 15; yy <= y + height/2 + 15; yy += 5)
	  draw_dot (cr, &light, &dark, x + width/2 - 1, yy, 3);
    }
  else
    {
      for (yy = y + ythick; yy < (y + height - ythick); yy += 3)
	for (xx = x + xthick; xx < (x + width - xthick); xx += 6)
	  {
	    draw_dot (cr, &light, &dark, xx, yy, 2);
	    draw_dot (cr, &light, &dark, xx + 3, yy + 1, 2);
	  }
    }
}

/**
 * gtk_cairo_paint_handle:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @shadow_type: type of shadow to draw
 * @widget: (allow-none): the widget
 * @detail: (allow-none): a style detail
 * @x: x origin of the handle
 * @y: y origin of the handle
 * @width: with of the handle
 * @height: height of the handle
 * @orientation: the orientation of the handle
 * 
 * Draws a handle as used in #GtkHandleBox and #GtkPaned.
 **/
void
gtk_cairo_paint_handle (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height,
                        GtkOrientation      orientation)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (width >= 0);
  g_return_if_fail (height >= 0);

  cairo_save (cr);

  gtk_default_draw_handle (style, cr, state_type, shadow_type, widget, detail, x, y, width, height, orientation);

  cairo_restore (cr);
}

void
gtk_render_handle (GtkStyleContext *context,
                   cairo_t         *cr,
                   gdouble          x,
                   gdouble          y,
                   gdouble          width,
                   gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_handle (context, cr, state_type, GTK_SHADOW_NONE, (GtkWidget*)NULL, NULL, x, y, width, height, GTK_ORIENTATION_HORIZONTAL);
}

static void
gtk_default_draw_spinner (GtkStyle     *style,
                          cairo_t      *cr,
                          GtkStateType  state_type,
                          GtkWidget    *widget, /* not used */
                          const gchar  *detail,
                          guint         step,
                          gint          x,
                          gint          y,
                          gint          width,
                          gint          height)
{
  GdkColor *color;
  guint num_steps;
  gdouble dx, dy;
  gdouble radius;
  gdouble half;
  gint i;
  guint real_step;

  gtk_style_get (style, GTK_TYPE_SPINNER,
                 "num-steps", &num_steps,
                 NULL);
  real_step = step % num_steps;

  /* set a clip region for the expose event */
  cairo_rectangle (cr, x, y, width, height);
  cairo_clip (cr);

  cairo_translate (cr, x, y);

  /* draw clip region */
  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

  color = &style->fg[state_type];
  dx = width / 2;
  dy = height / 2;
  radius = MIN (width / 2, height / 2);
  half = num_steps / 2;

  for (i = 0; i < num_steps; i++)
    {
      gint inset = 0.7 * radius;

      /* transparency is a function of time and intial value */
      gdouble t = (gdouble) ((i + num_steps - real_step)
                             % num_steps) / num_steps;

      cairo_save (cr);

      cairo_set_source_rgba (cr,
                             color->red / 65535.,
                             color->green / 65535.,
                             color->blue / 65535.,
                             t);

      cairo_set_line_width (cr, 2.0);
      cairo_move_to (cr,
                     dx + (radius - inset) * cos (i * G_PI / half),
                     dy + (radius - inset) * sin (i * G_PI / half));
      cairo_line_to (cr,
                     dx + radius * cos (i * G_PI / half),
                     dy + radius * sin (i * G_PI / half));
      cairo_stroke (cr);

      cairo_restore (cr);
    }
}

/**
 * gtk_cairo_paint_spinner:
 * @style: a #GtkStyle
 * @cr: a #cairo_t
 * @state_type: a state
 * @widget: (allow-none): the widget (may be %NULL)
 * @detail: (allow-none): a style detail (may be %NULL)
 * @step: the nth step, a value between 0 and #GtkSpinner:num-steps
 * @x: the x origin of the rectangle in which to draw the spinner
 * @y: the y origin of the rectangle in which to draw the spinner
 * @width: the width of the rectangle in which to draw the spinner
 * @height: the height of the rectangle in which to draw the spinner
 *
 * Draws a spinner on @window using the given parameters.
 */
void
gtk_cairo_paint_spinner (GtkStyle           *style,
                         cairo_t            *cr,
                         GtkStateType        state_type,
                         GtkWidget          *widget,
                         const gchar        *detail,
                         guint               step,
                         gint                x,
                         gint                y,
                         gint                width,
                         gint                height)
{
  g_return_if_fail (GTK_IS_STYLE (style));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  gtk_default_draw_spinner (style, cr, state_type, widget, detail, step, x, y, width, height);

  cairo_restore (cr);
}

void
gtk_render_activity (GtkStyleContext *context,
                     cairo_t         *cr,
                     gdouble          x,
                     gdouble          y,
                     gdouble          width,
                     gdouble          height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_spinner (context, cr, state_type, NULL, NULL, 0, x, y, width, height);
}

static GdkPixbuf *
scale_or_ref (GdkPixbuf *src,
              gint       width,
              gint       height)
{
  if (width == gdk_pixbuf_get_width (src) &&
      height == gdk_pixbuf_get_height (src))
    {
      return g_object_ref (src);
    }
  else
    {
      return gdk_pixbuf_scale_simple (src,
                                      width, height,
                                      GDK_INTERP_BILINEAR);
    }
}

static gboolean
lookup_icon_size (GtkStyle    *style,
		  GtkWidget   *widget,
		  GtkIconSize  size,
		  gint        *width,
		  gint        *height)
{
  GdkScreen *screen;
  GtkSettings *settings;

  if (widget && gtk_widget_has_screen (widget))
    {
      screen = gtk_widget_get_screen (widget);
      settings = gtk_settings_get_for_screen (screen);
    }
  else if (style && style->colormap)
    {
      screen = gdk_colormap_get_screen (style->colormap);
      settings = gtk_settings_get_for_screen (screen);
    }
  else
    {
      settings = gtk_settings_get_default ();
      GTK_NOTE (MULTIHEAD,
		g_warning ("Using the default screen for gtk_default_render_icon()"));
    }

  return gtk_icon_size_lookup_for_settings (settings, size, width, height);
}

static GdkPixbuf *
gtk_default_render_icon_pixbuf (GtkStyle            *style,
                                const GtkIconSource *source,
                                GtkTextDirection     direction,
                                GtkStateType         state,
                                GtkIconSize          size,
                                GtkWidget           *widget,
                                const gchar         *detail)
{
  gint width = 1;
  gint height = 1;
  GdkPixbuf *scaled;
  GdkPixbuf *stated;
  GdkPixbuf *base_pixbuf;

  /* Oddly, style can be NULL in this function, because
   * GtkIconSet can be used without a style and if so
   * it uses this function.
   */

  base_pixbuf = gtk_icon_source_get_pixbuf (source);

  g_return_val_if_fail (base_pixbuf != NULL, NULL);

  if (size != (GtkIconSize) -1 && !lookup_icon_size(style, widget, size, &width, &height))
    {
      g_warning (G_STRLOC ": invalid icon size '%d'", size);
      return NULL;
    }

  /* If the size was wildcarded, and we're allowed to scale, then scale; otherwise,
   * leave it alone.
   */
  if (size != (GtkIconSize)-1 && gtk_icon_source_get_size_wildcarded (source))
    scaled = scale_or_ref (base_pixbuf, width, height);
  else
    scaled = g_object_ref (base_pixbuf);

  /* If the state was wildcarded, then generate a state. */
  if (gtk_icon_source_get_state_wildcarded (source))
    {
      switch (state) {
      case GTK_STATE_INSENSITIVE:
        stated = gdk_pixbuf_copy (scaled);

        gdk_pixbuf_saturate_and_pixelate (scaled, stated,
                                            0.8, TRUE);

        g_object_unref (scaled);
      break;
      case GTK_STATE_PRELIGHT:
        stated = gdk_pixbuf_copy (scaled);
        gdk_pixbuf_saturate_and_pixelate (scaled, stated,
                                            1.2, FALSE);
        g_object_unref (scaled);
      break;
      default:
        stated = scaled;
      break;
      }
    }
  else
    stated = scaled;

  return stated;
}

/**
 * gtk_render_icon_pixbuf:
 * @context: a #GtkStyleContext
 * @source: the #GtkIconSource specifying the icon to render
 * @size: (type int): the size (#GtkIconSize) to render the icon at.
 *        A size of `(GtkIconSize) -1` means render at the size of the source
 *        and don’t scale.
 *
 * Renders the icon specified by @source at the given @size, returning the result
 * in a pixbuf.
 *
 * Returns: (transfer full): a newly-created #GdkPixbuf containing the rendered icon
 *
 * Since: 3.0
 *
 * Deprecated: 3.10: Use gtk_icon_theme_load_icon() instead.
 **/
GdkPixbuf *
gtk_render_icon_pixbuf (GtkStyleContext     *context,
                        const GtkIconSource *source,
                        GtkIconSize          size)
{
  GtkStateType state_type;
  GtkTextDirection direction;

  state_type = gtk_style_context_get_state_type (context);
  direction = gtk_style_context_get_direction (context);
  return gtk_default_render_icon_pixbuf (context, source, direction, state_type, size, (GtkWidget*)NULL, NULL);
}

static void
gtk_default_draw_icon_surface (GtkStyleContext    *context,
                               cairo_t            *cr,
                               cairo_surface_t    *surface,
                               gdouble             x,
                               gdouble             y)
{
  /* g_return_if_fail (GTK_IS_STYLE_CONTEXT (context)); */
  g_return_if_fail (cr != NULL);
  g_return_if_fail (surface != NULL);

  cairo_set_source_surface (cr, surface, x, y);
  /* coould draw a shadow here */
  /* right now, this doesn't do anything */
  cairo_paint (cr);
}

/**
 * gtk_render_icon:
 * @context: a #GtkStyleContext
 * @cr: a #cairo_t
 * @pixbuf: a #GdkPixbuf containing the icon to draw
 * @x: X position for the @pixbuf
 * @y: Y position for the @pixbuf
 *
 * Renders the icon in @pixbuf at the specified @x and @y coordinates.
 *
 * This function will render the icon in @pixbuf at exactly its size,
 * regardless of scaling factors, which may not be appropriate when
 * drawing on displays with high pixel densities.
 *
 * You probably want to use gtk_render_icon_surface() instead, if you
 * already have a Cairo surface.
 *
 * Since: 3.2
 **/
void
gtk_render_icon (GtkStyleContext *context,
                 cairo_t         *cr,
                 GdkPixbuf       *pixbuf,
                 gdouble          x,
                 gdouble          y)
{
  cairo_surface_t *surface;

  /* g_return_if_fail (GTK_IS_STYLE_CONTEXT (context)); */
  g_return_if_fail (cr != NULL);

  surface = gdk_cairo_surface_create_from_pixbuf (pixbuf, 1, NULL);

  gtk_default_draw_icon_surface (context, cr, surface, x, y);

  cairo_surface_destroy (surface);
}

/**
 * gtk_render_icon_surface:
 * @context: a #GtkStyleContext
 * @cr: a #cairo_t
 * @surface: a #cairo_surface_t containing the icon to draw
 * @x: X position for the @icon
 * @y: Y position for the @incon
 *
 * Renders the icon in @surface at the specified @x and @y coordinates.
 *
 * Since: 3.10
 **/
void
gtk_render_icon_surface (GtkStyleContext *context,
                         cairo_t         *cr,
                         cairo_surface_t *surface,
                         gdouble          x,
                         gdouble          y)
{
  /* g_return_if_fail (GTK_IS_STYLE_CONTEXT (context)); */
  g_return_if_fail (cr != NULL);

  gtk_default_draw_icon_surface (context, cr, surface, x, y);
}

/*
 * gtk_render_content_path:
 * @context: style context to get style information from
 * @cr: cairo context to add path to
 * @x: x coordinate of CSS box
 * @y: y coordinate of CSS box
 * @width: width of CSS box
 * @height: height of CSS box
 *
 * Adds the path of the content box to @cr for a given border box.
 * This function respects rounded corners.
 *
 * This is useful if you are drawing content that is supposed to
 * fill the whole content area, like the color buttons in
 * #GtkColorChooserDialog.
 **/
void
gtk_render_content_path (GtkStyleContext *context,
                         cairo_t         *cr,
                         double           x,
                         double           y,
                         double           width,
                         double           height)
{
  GtkStateType state_type;

  state_type = gtk_style_context_get_state_type (context);
  gtk_cairo_paint_box (context, cr, state_type, GTK_SHADOW_NONE, (GtkWidget*)NULL, NULL, x, y, width, height);
}

static const GdkColor *
get_insertion_cursor_color (GtkWidget *widget,
			    gboolean   is_primary)
{
  CursorInfo *cursor_info;
  GtkStyle *style;
  GdkColor *cursor_color;

  style = gtk_widget_get_style (widget);

  cursor_info = g_object_get_data (G_OBJECT (style), "gtk-style-cursor-info");
  if (!cursor_info)
    {
      cursor_info = g_new0 (CursorInfo, 1);
      g_object_set_data (G_OBJECT (style), I_("gtk-style-cursor-info"), cursor_info);
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

      /* Cursors in text widgets are drawn only in NORMAL state,
       * so we can use text[GTK_STATE_NORMAL] as text color here */
      gtk_widget_style_get (widget, "cursor-color", &cursor_color, NULL);
      if (cursor_color)
        {
          cursor_info->primary = *cursor_color;
          gdk_color_free (cursor_color);
        }
      else
        {
          cursor_info->primary = style->text[GTK_STATE_NORMAL];
        }

      gtk_widget_style_get (widget, "secondary-cursor-color", &cursor_color, NULL);
      if (cursor_color)
        {
          cursor_info->secondary = *cursor_color;
          gdk_color_free (cursor_color);
        }
      else
        {
          /* text_aa is the average of text and base colors,
           * in usual black-on-white case it's grey. */
          cursor_info->secondary = style->text_aa[GTK_STATE_NORMAL];
        }
    }

  if (is_primary)
    return &cursor_info->primary;
  else
    return &cursor_info->secondary;
}

void
draw_insertion_cursor (GtkWidget          *widget, /* is NULL with gtk3 api */
                       cairo_t            *cr,
		       const GdkRectangle *location,
		       gfloat              cursor_aspect_ratio, /* is 0 with gtk2 api */
		       GtkTextDirection    direction,
		       gboolean            draw_arrow,
		       gboolean            new_api /* gtk2/gtk3 api */)
{
  gint stem_width;
  gint arrow_width;
  gint x, y;
  gint offset;
  
  /* When changing the shape or size of the cursor here,
   * propagate the changes to gtktextview.c:text_window_invalidate_cursors().
   */

  if (!new_api) {
    gtk_widget_style_get (widget, "cursor-aspect-ratio", &cursor_aspect_ratio, NULL);
  }
  else {
    cairo_save (cr);
    cairo_new_path (cr);
  }

  stem_width = location->height * cursor_aspect_ratio + 1;
  arrow_width = stem_width + 1;

  /* put (stem_width % 2) on the proper side of the cursor */
  if (direction == GTK_TEXT_DIR_LTR)
    offset = stem_width / 2;
  else
    offset = stem_width - stem_width / 2;
  
  cairo_rectangle (cr, 
                   location->x - offset, location->y,
                   stem_width, location->height);
  cairo_fill (cr);

  if (draw_arrow)
    {
      if (direction == GTK_TEXT_DIR_RTL)
        {
          x = location->x - offset - 1;
          y = location->y + location->height - arrow_width * 2 - arrow_width + 1;
  
          cairo_move_to (cr, x, y + 1);
          cairo_line_to (cr, x - arrow_width, y + arrow_width);
          cairo_line_to (cr, x, y + 2 * arrow_width);
          cairo_fill (cr);
        }
      else if (direction == GTK_TEXT_DIR_LTR)
        {
          x = location->x + stem_width - offset;
          y = location->y + location->height - arrow_width * 2 - arrow_width + 1;
  
          cairo_move_to (cr, x, y + 1);
          cairo_line_to (cr, x + arrow_width, y + arrow_width);
          cairo_line_to (cr, x, y + 2 * arrow_width);
          cairo_fill (cr);
        }
    }
  if (new_api) {
    cairo_restore (cr);
  }
}

/**
 * gtk_draw_insertion_cursor:
 * @widget:  a #GtkWidget
 * @drawable: a #GdkDrawable
 * @area: (allow-none): rectangle to which the output is clipped, or %NULL if the
 *        output should not be clipped
 * @location: location where to draw the cursor (@location->width is ignored)
 * @is_primary: if the cursor should be the primary cursor color.
 * @direction: whether the cursor is left-to-right or
 *             right-to-left. Should never be #GTK_TEXT_DIR_NONE
 * @draw_arrow: %TRUE to draw a directional arrow on the
 *        cursor. Should be %FALSE unless the cursor is split.
 * 
 * Draws a text caret on @drawable at @location. This is not a style function
 * but merely a convenience function for drawing the standard cursor shape.
 *
 * Since: 2.4
 **/

/**
 * gtk_draw_insertion_cursor:
 * @widget:  a #GtkWidget
 * @cr: cairo context to draw to
 * @location: location where to draw the cursor (@location->width is ignored)
 * @is_primary: if the cursor should be the primary cursor color.
 * @direction: whether the cursor is left-to-right or
 *             right-to-left. Should never be #GTK_TEXT_DIR_NONE
 * @draw_arrow: %TRUE to draw a directional arrow on the
 *        cursor. Should be %FALSE unless the cursor is split.
 *
 * Draws a text caret on @cr at @location. This is not a style function
 * but merely a convenience function for drawing the standard cursor shape.
 *
 * Since: 3.0
 * Deprecated: 3.4: Use gtk_render_insertion_cursor() instead.
 */

/* XXX conflicts with gtk2 XXX */
/* XXX abuses the dynamic linker to make this work XXX */
void
gtk_draw_insertion_cursor (GtkWidget          *widget,
			   cairo_t            *cr,
			   const GdkRectangle *location,
                           ...)
/*
			   gboolean            is_primary,
			   GtkTextDirection    direction,
			   gboolean            draw_arrow)
*/
{
  static void *gtk2;
  static void (*gtk2_gtk_draw_insertion_cursor)(GtkWidget          *widget,
                                                GdkDrawable        *drawable,
                                                const GdkRectangle *area,
                                                const GdkRectangle *location,
                                                gboolean            is_primary,
                                                GtkTextDirection    direction,
                                                gboolean            draw_arrow);

  char *dl_error;

  /* gtk3 args */
  gboolean            is_primary;
  GtkTextDirection    direction;
  gboolean            draw_arrow;

  /* gtk2 extra args */
  GdkDrawable        *drawable;
  const GdkRectangle *area;

  va_list list;
  va_start (list, location);

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (cr != NULL);

  if (!GDK_IS_DRAWABLE (cr)) { /* gtk3 call */

    is_primary = va_arg (list, gboolean);
    direction = va_arg (list, GtkTextDirection);
    draw_arrow = va_arg (list, gboolean);

    g_return_if_fail (location != NULL);
    g_return_if_fail (direction != GTK_TEXT_DIR_NONE);

    gdk_cairo_set_source_color (cr, get_insertion_cursor_color (widget, is_primary));
    draw_insertion_cursor (widget, cr, location, 0, direction, draw_arrow, FALSE);

    va_end (list);
    return;
  }

  /* gtk2 call */
  drawable = (GdkDrawable*)cr;
  area = location;
  location = va_arg (list, const GdkRectangle *);
  is_primary = va_arg (list, gboolean);
  direction = va_arg (list, GtkTextDirection);
  draw_arrow = va_arg (list, gboolean);

  if (!gtk2) {
    gtk2 = dlopen (
#ifdef X11
                   "libgtk-x11-2.0.so",
#else
                   "libgtk-directfb-2.0.so",
#endif
                   RTLD_NOW);

    g_return_if_fail (gtk2 != NULL);

    gtk2_gtk_draw_insertion_cursor = dlsym (gtk2, "gtk_draw_insertion_cursor");

    dl_error = dlerror();

    if (dl_error) {
      g_warning ("%s\n", dl_error);
      return;
    }

    /* dlclose never called */
  }

  gtk2_gtk_draw_insertion_cursor (widget, drawable, area, location, is_primary, direction, draw_arrow);
}
/**
 * gtk_render_insertion_cursor:
 * @context: a #GtkStyleContext
 * @cr: a #cairo_t
 * @x: X origin
 * @y: Y origin
 * @layout: the #PangoLayout of the text
 * @index: the index in the #PangoLayout
 * @direction: the #PangoDirection of the text
 *
 * Draws a text caret on @cr at the specified index of @layout.
 *
 * Since: 3.4
 **/
void
gtk_render_insertion_cursor (GtkStyleContext *context,
                             cairo_t         *cr,
                             gdouble          x,
                             gdouble          y,
                             PangoLayout     *layout,
                             int              index,
                             PangoDirection   direction)
{
  gboolean split_cursor;
  float aspect_ratio;
  PangoRectangle strong_pos, weak_pos;
  PangoRectangle *cursor1, *cursor2;
  PangoDirection keymap_direction;
  PangoDirection direction2;
  GdkScreen *screen;

  g_return_if_fail (GTK_IS_STYLE_CONTEXT (context));
  g_return_if_fail (cr != NULL);
  g_return_if_fail (PANGO_IS_LAYOUT (layout));
  g_return_if_fail (index >= 0);

  screen = gtk_style_context_get_screen (context);

  g_object_get (gtk_settings_get_for_screen (screen),
                "gtk-split-cursor", &split_cursor,
                "gtk-cursor-aspect-ratio", &aspect_ratio,
                NULL);

  /* Fall back to style property if the GtkSetting property is unchanged */
  if (aspect_ratio == 0.04f)
    {
      gtk_style_context_get_style (context,
                                   "cursor-aspect-ratio", &aspect_ratio,
                                   NULL);
    }

  keymap_direction = gdk_keymap_get_direction (gdk_keymap_get_for_display (gdk_screen_get_display (screen)));

  pango_layout_get_cursor_pos (layout, index, &strong_pos, &weak_pos);

  direction2 = PANGO_DIRECTION_NEUTRAL;

  if (split_cursor)
    {
      cursor1 = &strong_pos;

      if (strong_pos.x != weak_pos.x || strong_pos.y != weak_pos.y)
        {
          direction2 = (direction == PANGO_DIRECTION_LTR) ? PANGO_DIRECTION_RTL : PANGO_DIRECTION_LTR;
          cursor2 = &weak_pos;
        }
    }
  else
    {
      if (keymap_direction == direction)
        cursor1 = &strong_pos;
      else
        cursor1 = &weak_pos;
    }

  gdk_cairo_set_source_color (cr, &context->text[GTK_STATE_NORMAL]);

  GdkRectangle location = (GdkRectangle){.x = x + PANGO_PIXELS (cursor1->x),
                                         .y = y + PANGO_PIXELS (cursor1->y),
                                         .height = PANGO_PIXELS (cursor1->height),
                                         /* .width = 0 */ /* unused */
                                        };

  draw_insertion_cursor (NULL, /* context */
                         cr,
                         &location,
                         aspect_ratio,
                         /* TRUE, */ /* is_primary */
                         GtkTextDirection_from_PangoDirection(direction),
                         direction2 != PANGO_DIRECTION_NEUTRAL,
                         TRUE);

  if (direction2 != PANGO_DIRECTION_NEUTRAL)
    {
      location = (GdkRectangle){.x = x + PANGO_PIXELS (cursor2->x),
                                .y = y + PANGO_PIXELS (cursor2->y),
                                .height = PANGO_PIXELS (cursor2->height),
                                /* .width = 0 */ /* unused */
                               };
      draw_insertion_cursor (NULL, /* context */
                             cr,
                             &location,
                             aspect_ratio,
                             /* FALSE, */ /* is_primary */
                             GtkTextDirection_from_PangoDirection(direction2),
                             TRUE,
                             TRUE);
    }
}
