#include <gdk/gdkwindow.h>
#include <gdk/gdkdrawingcontext.h>

#include "gdkwindowinternal.h"
#include "gdkregionprivate.h"

/*< private >
 * gdk_window_get_current_paint_region:
 * @window: a #GdkWindow
 *
 * Retrieves a copy of the current paint region.
 *
 * Returns: (transfer full): a Cairo region
 */
cairo_region_t *
gdk_window_get_current_paint_region (GdkWindow *_window)
{
  cairo_region_t *region;
  struct _GdkWindowObjectReal *window = (struct _GdkWindowObjectReal*)_window;
  struct _GdkWindowObjectReal *impl_window = (struct _GdkWindowObjectReal*)window->impl_window;

  if (impl_window->clip_region != NULL)
    {
      region = cairo_region_from_GdkRegion (impl_window->clip_region);
      cairo_region_translate (region, -window->abs_x, -window->abs_y);
    }
  else
    {
      region = cairo_region_from_GdkRegion (window->clip_region);
    }

  return region;
}

/*< private >
 * gdk_window_get_current_paint_gdk_region:
 * @window: a #GdkWindow
 *
 * Retrieves a copy of the current paint GdkRegion.
 *
 * Returns: (transfer full): a GdkRegion
 */
GdkRegion *
gdk_window_get_current_paint_gdk_region (GdkWindow *_window)
{
  GdkRegion *region;
  struct _GdkWindowObjectReal *window = (struct _GdkWindowObjectReal*)_window;
  struct _GdkWindowObjectReal *impl_window = (struct _GdkWindowObjectReal*)window->impl_window;

  if (impl_window->clip_region != NULL)
    {
      region = gdk_region_copy (impl_window->clip_region);
      gdk_region_offset (region, -window->abs_x, -window->abs_y);
    }
  else
    {
      region = gdk_region_copy (window->clip_region);
    }

  return region;
}

/*< private >
 * gdk_window_get_drawing_context:
 * @window: a #GdkWindow
 *
 * Retrieves the #GdkDrawingContext associated to @window by
 * gdk_window_begin_draw_frame().
 *
 * Returns: (transfer none) (nullable): a #GdkDrawingContext, if any is set
 */
GdkDrawingContext *
gdk_window_get_drawing_context (GdkWindow *window)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gdk_window_begin_draw_frame:
 * @window: a #GdkWindow
 * @region: a Cairo region
 *
 * Indicates that you are beginning the process of redrawing @region
 * on @window, and provides you with a #GdkDrawingContext.
 *
 * If @window is a top level #GdkWindow, backed by a native window
 * implementation, a backing store (offscreen buffer) large enough to
 * contain @region will be created. The backing store will be initialized
 * with the background color or background surface for @window. Then, all
 * drawing operations performed on @window will be diverted to the
 * backing store. When you call gdk_window_end_frame(), the contents of
 * the backing store will be copied to @window, making it visible
 * on screen. Only the part of @window contained in @region will be
 * modified; that is, drawing operations are clipped to @region.
 *
 * The net result of all this is to remove flicker, because the user
 * sees the finished product appear all at once when you call
 * gdk_window_end_draw_frame(). If you draw to @window directly without
 * calling gdk_window_begin_draw_frame(), the user may see flicker
 * as individual drawing operations are performed in sequence.
 *
 * When using GTK+, the widget system automatically places calls to
 * gdk_window_begin_draw_frame() and gdk_window_end_draw_frame() around
 * emissions of the `GtkWidget::draw` signal. That is, if you’re
 * drawing the contents of the widget yourself, you can assume that the
 * widget has a cleared background, is already set as the clip region,
 * and already has a backing store. Therefore in most cases, application
 * code in GTK does not need to call gdk_window_begin_draw_frame()
 * explicitly.
 *
 * Returns: (transfer none): a #GdkDrawingContext context that should be
 *   used to draw the contents of the window; the returned context is owned
 *   by GDK.
 *
 * Since: 3.22
 */
GdkDrawingContext *
gdk_window_begin_draw_frame (GdkWindow            *window,
                             const cairo_region_t *region)
{
  /* Not Implemented */
  return NULL;
}

/**
 * gdk_window_end_draw_frame:
 * @window: a #GdkWindow
 * @context: the #GdkDrawingContext created by gdk_window_begin_draw_frame()
 *
 * Indicates that the drawing of the contents of @window started with
 * gdk_window_begin_frame() has been completed.
 *
 * This function will take care of destroying the #GdkDrawingContext.
 *
 * It is an error to call this function without a matching
 * gdk_window_begin_frame() first.
 *
 * Since: 3.22
 */
void
gdk_window_end_draw_frame (GdkWindow         *window,
                           GdkDrawingContext *context)
{
  /* Not Implemented */
}

/**
 * gdk_window_get_device_position:
 * @window: a #GdkWindow.
 * @device: pointer #GdkDevice to query to.
 * @x: (out) (allow-none): return location for the X coordinate of @device, or %NULL.
 * @y: (out) (allow-none): return location for the Y coordinate of @device, or %NULL.
 * @mask: (out) (allow-none): return location for the modifier mask, or %NULL.
 *
 * Obtains the current device position and modifier state.
 * The position is given in coordinates relative to the upper left
 * corner of @window.
 *
 * Use gdk_window_get_device_position_double() if you need subpixel precision.
 *
 * Returns: (nullable) (transfer none): The window underneath @device
 * (as with gdk_device_get_window_at_position()), or %NULL if the
 * window is not known to GDK.
 *
 * Since: 3.0
 **/
GdkWindow *
gdk_window_get_device_position (GdkWindow       *window,
                                GdkDevice       *device,
                                gint            *x,
                                gint            *y,
                                GdkModifierType *mask)
{
  return gdk_window_get_pointer (window, x, y, mask);
}

/**
 * gdk_window_get_device_position_double:
 * @window: a #GdkWindow.
 * @device: pointer #GdkDevice to query to.
 * @x: (out) (allow-none): return location for the X coordinate of @device, or %NULL.
 * @y: (out) (allow-none): return location for the Y coordinate of @device, or %NULL.
 * @mask: (out) (allow-none): return location for the modifier mask, or %NULL.
 *
 * Obtains the current device position in doubles and modifier state.
 * The position is given in coordinates relative to the upper left
 * corner of @window.
 *
 * Returns: (nullable) (transfer none): The window underneath @device
 * (as with gdk_device_get_window_at_position()), or %NULL if the
 * window is not known to GDK.
 *
 * Since: 3.10
 **/
GdkWindow *
gdk_window_get_device_position_double (GdkWindow       *window,
                                       GdkDevice       *device,
                                       double          *x,
                                       double          *y,
                                       GdkModifierType *mask)
{
  gint _x, _y;
  GdkWindow *ret;

  ret = gdk_window_get_pointer (window, &_x, &_y, mask);
  if (x) {
    *x = _x;
  }

  if (y) {
    *y = _y;
  }

  return ret;
}
