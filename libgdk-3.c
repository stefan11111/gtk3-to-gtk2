#include <gdk/gdk.h>

void
gdk_disable_multidevice (void)
{
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

typedef struct _GdkDeviceManager GdkDeviceManager;

GdkDeviceManager *
gdk_display_get_device_manager (GdkDisplay *display)
{
  return NULL;
}

void
gdk_device_ungrab (GdkDevice  *device,
                   guint32     time_)
{
}

typedef struct _GdkGrabOwnership GdkGrabOwnership;

GdkGrabStatus
gdk_device_grab ()
{
#if 0
  return 5;
#endif
  return 0;
}

void
gdk_window_set_background_pattern (GdkWindow       *window,
                                   cairo_pattern_t *pattern)
{
}

gint
gdk_window_get_scale_factor (GdkWindow *window)
{
  return 1;
}

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

typedef GdkSegment GdkRegionBox;

struct _GdkRegion
{
  long size;
  long numRects;
  GdkRegionBox *rects;
  GdkRegionBox extents;
};

#if 0
#define CAIRO_REGION_FROM_GDKREGION (region) (cairo_region_t*)((int*)(region) - 2)
#define GDKREGION_FROM_CAIRO_REGION (region) (GdkRegion*)((int*)(region) + 2)
#endif

#if 0
static void
miRegionCopy (GdkRegion       *dstrgn,
              const GdkRegion *rgn)
{
  if (dstrgn != rgn) /*  don't want to copy to itself */
    {
      if (dstrgn->size < rgn->numRects)
        {
          if (dstrgn->rects != &dstrgn->extents)
            g_free (dstrgn->rects);

          dstrgn->rects = g_new (GdkRegionBox, rgn->numRects);
          dstrgn->size = rgn->numRects;
        }

      dstrgn->numRects = rgn->numRects;
      dstrgn->extents = rgn->extents;

      memcpy (dstrgn->rects, rgn->rects, rgn->numRects * sizeof (GdkRegionBox));
    }
}
#endif

cairo_region_t*
gdk_window_get_clip_region (GdkWindow *window)
{
  g_return_val_if_fail (GDK_WINDOW (window), NULL);

  struct {
    int foo;
    int bar;
    GdkRegion baz;
  } tmp_region;

  memset(&tmp_region, 0, 2 * sizeof(int));
#if 0 /* no need for miRegionCopy, simple memcpy works */
  miRegionCopy((GdkRegion*)((int*)&tmp_region + 2), (const GdkRegion*)(((struct _GdkWindowObjectReal*)window)->clip_region));
#endif
/* even memcpy is too much */
  memcpy(((int*)&tmp_region + 2), (((struct _GdkWindowObjectReal*)window)->clip_region), sizeof(GdkRegion));

  cairo_region_t *result = cairo_region_copy ((const cairo_region_t*)&tmp_region);

  if (((struct _GdkWindowObjectReal*)window)->paint_stack)
    {
      GdkRegion *paint_region = gdk_region_new ();
      GSList *tmp_list = ((struct _GdkWindowObjectReal*)window)->paint_stack;

      while (tmp_list)
        {
          GdkWindowPaint *paint = tmp_list->data;

          gdk_region_union (paint_region, *(GdkRegion**)paint);

          tmp_list = tmp_list->next;
        }

      gdk_region_intersect ((GdkRegion*)((int*)result + 2), paint_region);
      gdk_region_destroy (paint_region);
    }

  return result;
}

cairo_region_t *
gdk_window_get_visible_region (GdkWindow *window)
{
  g_return_val_if_fail (GDK_IS_WINDOW (window), NULL);

  struct {
    int foo;
    int bar;
    GdkRegion baz;
  } tmp_region;

  memset(&tmp_region, 0, 2 * sizeof(int));
#if 0 /* no need for miRegionCopy, simple memcpy works */
  miRegionCopy((GdkRegion*)((int*)&tmp_region + 2), (const GdkRegion*)(((struct _GdkWindowObjectReal*)window)->clip_region));
#endif
/* even memcpy is too much */
  memcpy(((int*)&tmp_region + 2), (((struct _GdkWindowObjectReal*)window)->clip_region), sizeof(GdkRegion));

  return cairo_region_copy ((const cairo_region_t*)&tmp_region);
}

