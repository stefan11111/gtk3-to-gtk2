#ifndef __INTERNAL_GDK_INTERNALS_H__
#define __INTERNAL_GDK_INTERNALS_H__

#include <pango/pango.h>

#include <gdk/gdkwindow.h>
#include <gdk/gdkdrawingcontext.h>

G_BEGIN_DECLS

/* Event handling */

typedef struct _GdkEventPrivate GdkEventPrivate;

typedef enum
{
  /* Following flag is set for events on the event queue during
   * translation and cleared afterwards.
   */
  GDK_EVENT_PENDING = 1 << 0
} GdkEventFlags;

struct _GdkEventPrivate
{
  GdkEvent   event;
  guint      flags;
  GdkScreen *screen;
  gpointer   windowing_data;
};

struct _GdkWindow {
  struct _GdkDrawable drawable;
};

extern gboolean   _gdk_disable_multidevice;

PangoDirection gdk_unichar_direction (gunichar ch);

void            gdk_cairo_set_drawing_context    (cairo_t           *cr,
                                                  GdkDrawingContext *context);


cairo_region_t *gdk_window_get_current_paint_region (GdkWindow *window);

GdkRegion *gdk_window_get_current_paint_gdk_region (GdkWindow *window);

GdkDrawingContext *gdk_window_get_drawing_context (GdkWindow *window);

G_END_DECLS

#endif /* __INTERNAL_GDK_INTERNALS_H__ */
