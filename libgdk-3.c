#include <gdk/gdk.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#if 0
#define X11
#endif
#endif

#ifdef GDK_WINDOWING_DIRECTFB
#if 0
#define DIRECTFB
#endif
#endif

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
/* even memcpy is too much, result gets copyed again on the next line */
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
/* even memcpy is too much, result gets copyed again on the next line */
  memcpy(((int*)&tmp_region + 2), (((struct _GdkWindowObjectReal*)window)->clip_region), sizeof(GdkRegion));

  return cairo_region_copy ((const cairo_region_t*)&tmp_region);
}

Window
gdk_x11_window_get_xid (GdkWindow *window)
{
#ifdef X11
  return gdk_x11_drawable_get_xid (window);
#endif
  return 0;
}

static cairo_format_t
gdk_cairo_format_for_content (cairo_content_t content)
{
  switch (content)
    {
    case CAIRO_CONTENT_COLOR:
      return CAIRO_FORMAT_RGB24;
    case CAIRO_CONTENT_ALPHA:
      return CAIRO_FORMAT_A8;
    case CAIRO_CONTENT_COLOR_ALPHA:
    default:
      return CAIRO_FORMAT_ARGB32;
    }
}

static cairo_surface_t *
gdk_cairo_surface_coerce_to_image (cairo_surface_t *surface,
                                   cairo_content_t  content,
                                   int              src_x,
                                   int              src_y,
                                   int              width,
                                   int              height)
{
  cairo_surface_t *copy;
  cairo_t *cr;

  copy = cairo_image_surface_create (gdk_cairo_format_for_content (content),
                                     width,
                                     height);

  cr = cairo_create (copy);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface (cr, surface, -src_x, -src_y);
  cairo_paint (cr);
  cairo_destroy (cr);

  return copy;
}

static void
convert_alpha (guchar *dest_data,
               int     dest_stride,
               guchar *src_data,
               int     src_stride,
               int     src_x,
               int     src_y,
               int     width,
               int     height)
{
  int y;

  src_data += src_stride * src_y + src_x * 4;

  for (y = 0; y < height; y++) {
    guint32 *src = (guint32 *) src_data;

    int x;

    for (x = 0; x < width; x++) {
      guint alpha = src[x] >> 24;

      if (alpha == 0)
        {
          dest_data[x * 4 + 0] = 0;
          dest_data[x * 4 + 1] = 0;
          dest_data[x * 4 + 2] = 0;
        }
      else
        {
          dest_data[x * 4 + 0] = (((src[x] & 0xff0000) >> 16) * 255 + alpha / 2) / alpha;
          dest_data[x * 4 + 1] = (((src[x] & 0x00ff00) >>  8) * 255 + alpha / 2) / alpha;
          dest_data[x * 4 + 2] = (((src[x] & 0x0000ff) >>  0) * 255 + alpha / 2) / alpha;
        }
      dest_data[x * 4 + 3] = alpha;
    }

    src_data += src_stride;
    dest_data += dest_stride;
  }
}

static void
convert_no_alpha (guchar *dest_data,
                  int     dest_stride,
                  guchar *src_data,
                  int     src_stride,
                  int     src_x,
                  int     src_y,
                  int     width,
                  int     height)
{
  int y;

  src_data += src_stride * src_y + src_x * 4;

  for (y = 0; y < height; y++) {
    guint32 *src = (guint32 *) src_data;
    int x;
    for (x = 0; x < width; x++) {
      dest_data[x * 3 + 0] = src[x] >> 16;
      dest_data[x * 3 + 1] = src[x] >>  8;
      dest_data[x * 3 + 2] = src[x];
    }

    src_data += src_stride;
    dest_data += dest_stride;
  }
}

GdkPixbuf *
gdk_pixbuf_get_from_surface  (cairo_surface_t *surface,
                              gint             src_x,
                              gint             src_y,
                              gint             width,
                              gint             height)
{
  cairo_content_t content;
  GdkPixbuf *dest;

  /* General sanity checks */
  g_return_val_if_fail (surface != NULL, NULL);
  g_return_val_if_fail (width > 0 && height > 0, NULL);

  content = cairo_surface_get_content (surface) | CAIRO_CONTENT_COLOR;
  dest = gdk_pixbuf_new (GDK_COLORSPACE_RGB,
                         !!(content & CAIRO_CONTENT_ALPHA),
                         8,
                         width, height);

  if (cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE &&
      cairo_image_surface_get_format (surface) == gdk_cairo_format_for_content (content))
    surface = cairo_surface_reference (surface);
  else
    {
      surface = gdk_cairo_surface_coerce_to_image (surface, content,
                                                   src_x, src_y,
                                                   width, height);
      src_x = 0;
      src_y = 0;
    }
  cairo_surface_flush (surface);
  if (cairo_surface_status (surface) || dest == NULL)
    {
      cairo_surface_destroy (surface);
      g_clear_object (&dest);
      return NULL;
    }

  if (gdk_pixbuf_get_has_alpha (dest))
    convert_alpha (gdk_pixbuf_get_pixels (dest),
                   gdk_pixbuf_get_rowstride (dest),
                   cairo_image_surface_get_data (surface),
                   cairo_image_surface_get_stride (surface),
                   src_x, src_y,
                   width, height);
  else
    convert_no_alpha (gdk_pixbuf_get_pixels (dest),
                      gdk_pixbuf_get_rowstride (dest),
                      cairo_image_surface_get_data (surface),
                      cairo_image_surface_get_stride (surface),
                      src_x, src_y,
                      width, height);

  cairo_surface_destroy (surface);
  return dest;
}

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

static inline void
gdk_window_cairo_surface_destroy (void *data)
{
#if 0 /* wastes memory and cpu cycles */
  struct _GdkWindowObjectReal *private = (struct _GdkWindowObjectReal*) data;

  private->cairo_surface = NULL;
  ((struct _GdkWindowObjectReal*)private->impl_window)->outstanding_surfaces--;
#endif /* lean, fast, unreadable */
  ((struct _GdkWindowObjectReal*)data)->cairo_surface = NULL;
  ((struct _GdkWindowObjectReal*)((struct _GdkWindowObjectReal*)data)->impl_window)->outstanding_surfaces--;
}

static inline GdkDrawable *
_gdk_drawable_get_source_drawable (GdkDrawable *drawable)
{
  g_return_val_if_fail (GDK_IS_DRAWABLE (drawable), NULL);

  if (GDK_DRAWABLE_GET_CLASS (drawable)->get_source_drawable)
    return GDK_DRAWABLE_GET_CLASS (drawable)->get_source_drawable (drawable);

  return drawable;
}

static cairo_surface_t *
gdk_window_ref_cairo_surface (GdkDrawable *drawable)
{
  struct _GdkWindowObjectReal *private = (struct _GdkWindowObjectReal*) drawable;
  cairo_surface_t *surface;

  if (private->paint_stack)
    {
      GdkWindowPaint *paint = private->paint_stack->data;

      surface = paint->surface;
      cairo_surface_reference (surface);
    }
  else
    {

      /* This will be drawing directly to the window, so flush implicit paint */
      gdk_window_flush ((GdkWindow *)drawable);

      if (!private->cairo_surface)
        {
          int width, height;

          gdk_drawable_get_size ((GdkWindow *) private->impl_window,
                                 &width, &height);

          GdkDrawable *source = _gdk_drawable_get_source_drawable(drawable);

          private->cairo_surface = GDK_DRAWABLE_GET_CLASS (source)->create_cairo_surface (source, width, height);
          if (private->cairo_surface)
            {
              ((struct _GdkWindowObjectReal*)private->impl_window)->outstanding_surfaces++;

              cairo_surface_set_device_offset (private->cairo_surface,
                                               private->abs_x,
                                               private->abs_y);

              static const cairo_user_data_key_t gdk_window_cairo_key;

              cairo_surface_set_user_data (private->cairo_surface, &gdk_window_cairo_key,
                                           drawable, gdk_window_cairo_surface_destroy);
            }
        }
      else
        cairo_surface_reference (private->cairo_surface);

      surface = private->cairo_surface;
    }

  return surface;
}

GdkPixbuf *
gdk_pixbuf_get_from_window (GdkWindow *src,
                            gint       src_x,
                            gint       src_y,
                            gint       width,
                            gint       height)
{
  cairo_surface_t *copy;
  cairo_t *cr;
  GdkPixbuf *dest;

  g_return_val_if_fail (GDK_IS_WINDOW (src), NULL);
  g_return_val_if_fail (gdk_window_is_viewable (src), NULL);

  cairo_surface_t *surface = gdk_window_ref_cairo_surface (src);
  int scale = gdk_window_get_scale_factor (src);

  /* We do not know what happened to this surface outside of GDK.
   * Especially for foreign windows, they will have been modified
   * by external applications.
   *
   * So be on the safe side and:
   * - flush the Cairo state
   * - mark the surface as dirty, in case the GdkWindow was
   *   created from a foreign X11 surface
   *
   * THE ORDER IS IMPORTANT. DO NOT CHANGE IT.
   *
   * For reference, see:
   * - https://bugzilla.gnome.org/show_bug.cgi?id=754952
   * - https://gitlab.gnome.org/GNOME/gtk/-/issues/4456
   * - https://gitlab.gnome.org/GNOME/gtk/-/issues/5691
   */
  cairo_surface_flush (surface);
  cairo_surface_mark_dirty (surface);

  if (cairo_surface_get_content (surface) & CAIRO_CONTENT_ALPHA)
    copy = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width * scale, height * scale);
  else
    copy = cairo_image_surface_create (CAIRO_FORMAT_RGB24, width * scale, height * scale);

  cairo_surface_set_device_scale (copy, scale, scale);

  cr = cairo_create (copy);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_set_source_surface (cr, surface, -src_x, -src_y);
  cairo_paint (cr);
  cairo_destroy (cr);

  dest = gdk_pixbuf_get_from_surface (copy, 0, 0, width * scale, height * scale);

  cairo_surface_destroy (copy);
  cairo_surface_destroy (surface);

  return dest;
}

GType gdk_x11_screen_get_type (void)
{
  return 0;
}

typedef struct _GdkFrameClock GdkFrameClock;

GdkFrameClock*
gdk_window_get_frame_clock (GdkWindow *window)
{
  return NULL;
}

void
gdk_rgba_free (GdkRGBA *rgba)
{
  g_slice_free (GdkRGBA, rgba);
}

void
gdk_cairo_set_source_rgba (cairo_t       *cr,
                           const GdkRGBA *rgba)
{
#if 0 /* slow, wastes memory, needlessly lossy */
  GdkColor color;

  color.pixel = 0;
  color.red = (guint16)(rgba->red * 65535);
  color.green = (guint16)(rgba->green * 65535);
  color.blue = (guint16)(rgba->blue * 65535);

  gdk_cairo_set_source_color (cr, &color);
#endif /* call the actual function directly */
  g_return_if_fail (cr != NULL);
  g_return_if_fail (rgba != NULL);

  cairo_set_source_rgba (cr,
                         rgba->red,
                         rgba->green,
                         rgba->blue,
                         rgba->alpha);
}
