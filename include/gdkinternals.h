#include <pango/pango.h>

#include <gdk/gdkwindow.h>
#include <gdk/gdkdrawingcontext.h>

extern gboolean   _gdk_disable_multidevice;

PangoDirection gdk_unichar_direction (gunichar ch);

void            gdk_cairo_set_drawing_context    (cairo_t           *cr,
                                                  GdkDrawingContext *context);


cairo_region_t *gdk_window_get_current_paint_region (GdkWindow *window);

GdkRegion *gdk_window_get_current_paint_gdk_region (GdkWindow *window);

GdkDrawingContext *gdk_window_get_drawing_context (GdkWindow *window);
