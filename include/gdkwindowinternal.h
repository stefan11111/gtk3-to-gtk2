#ifndef __INTERNAL_GDK_WINDOW__INTERNAL_H__
#define __INTERNAL_GDK_WINDOW__INTERNAL_H__

#include <gdk/gdkwindow.h>

G_BEGIN_DECLS

typedef struct _GdkInputWindow GdkInputWindow;
typedef struct _GdkWindowPaint GdkWindowPaint;

struct _GdkWindowObjectReal
{
  /* vvvvvvv THIS PART IS PUBLIC. DON'T CHANGE vvvvvvvvvvvvvv */
  GdkDrawable parent_instance;

  GdkDrawable *impl; /* window-system-specific delegate object */  
  
  GdkWindowObject *parent;

  gpointer user_data;

  gint x;
  gint y;
  
  gint extension_events;

  GList *filters;
  GList *children;

  GdkColor bg_color;
  GdkPixmap *bg_pixmap;
  
  GSList *paint_stack;
  
  GdkRegion *update_area;
  guint update_freeze_count;
  
  guint8 window_type;
  guint8 depth;
  guint8 resize_count;

  GdkWindowState state;
  
  guint guffaw_gravity : 1;
  guint input_only : 1;
  guint modal_hint : 1;
  guint composited : 1;
  
  guint destroyed : 2;

  guint accept_focus : 1;
  guint focus_on_map : 1;
  guint shaped : 1;
  
  GdkEventMask event_mask;

  guint update_and_descendants_freeze_count;

  GdkWindowRedirect *redirect;

  /* ^^^^^^^^^^ THIS PART IS PUBLIC. DON'T CHANGE ^^^^^^^^^^ */
  
  /* The GdkWindowObject that has the impl, ref:ed if another window.
   * This ref is required to keep the wrapper of the impl window alive
   * for as long as any GdkWindow references the impl. */
  GdkWindowObject *impl_window; 
  int abs_x, abs_y; /* Absolute offset in impl */
  gint width, height;
  guint32 clip_tag;
  GdkRegion *clip_region; /* Clip region (wrt toplevel) in window coords */
  GdkRegion *clip_region_with_children; /* Clip region in window coords */
  GdkCursor *cursor;
  gint8 toplevel_window_type;
  guint synthesize_crossing_event_queued : 1;
  guint effective_visibility : 2;
  guint visibility : 2; /* The visibility wrt the toplevel (i.e. based on clip_region) */
  guint native_visibility : 2; /* the native visibility of a impl windows */
  guint viewable : 1; /* mapped and all parents mapped */
  guint applied_shape : 1;

  guint num_offscreen_children;
  GdkWindowPaint *implicit_paint;
  GdkInputWindow *input_window; /* only set for impl windows */

  GList *outstanding_moves;

  GdkRegion *shape;
  GdkRegion *input_shape;
  
  cairo_surface_t *cairo_surface;
  guint outstanding_surfaces; /* only set on impl window */

  cairo_pattern_t *background;
};

struct _GdkWindowPaint
{
  GdkRegion *region;
  GdkPixmap *pixmap;
  gint x_offset;
  gint y_offset;
  cairo_surface_t *surface;
  guint uses_implicit : 1;
  guint flushed : 1;
  guint32 region_tag;
};

G_END_DECLS

#endif /* __INTERNAL_GDK_WINDOW_INTERNAL_H__ */
