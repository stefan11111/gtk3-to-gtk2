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

typedef struct _GdkGrabOwnership GdkGrabOwnership;

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
#ifdef X11
typedef struct _GdkX11Screen GdkX11Screen;

struct _GdkX11Screen
{
  GdkScreen parent_instance;

  GdkDisplay *display;
  Display *xdisplay;
  Screen *xscreen;
  Window xroot_window;
  GdkWindow *root_window;
  gint screen_num;

  gint width;
  gint height;

  gint window_scale;
  gboolean fixed_window_scale;

  /* Xft resources for the display, used for default values for
   * the Xft/ XSETTINGS
   */
  gint xft_hintstyle;
  gint xft_rgba;
  gint xft_dpi;

  /* Window manager */
  long last_wmspec_check_time;
  Window wmspec_check_window;
  char *window_manager_name;

  /* X Settings */
  GdkWindow *xsettings_manager_window;
  Atom xsettings_selection_atom;
  GHashTable *xsettings; /* string of GDK settings name => GValue */

  /* TRUE if wmspec_check_window has changed since last
   * fetch of _NET_SUPPORTED
   */
  guint need_refetch_net_supported : 1;
  /* TRUE if wmspec_check_window has changed since last
   * fetch of window manager name
   */
  guint need_refetch_wm_name : 1;
  guint is_composited : 1;
  guint xft_init : 1; /* Whether we've intialized these values yet */
  guint xft_antialias : 1;
  guint xft_hinting : 1;

  /* Visual Part */
  gint nvisuals;
  GdkVisual **visuals;
  GdkVisual *system_visual;
  gint available_depths[7];
  GdkVisualType available_types[6];
  gint16 navailable_depths;
  gint16 navailable_types;
  GHashTable *visual_hash;
  GdkVisual *rgba_visual;

  /* cache for window->translate vfunc */
  GC subwindow_gcs[32];
};

struct _GdkX11ScreenClass
{
  GdkScreenClass parent_class;

  void (* window_manager_changed) (GdkX11Screen *x11_screen);
};

typedef struct _GdkX11ScreenClass GdkX11ScreenClass;

static void
gdk_x11_screen_init(GdkX11Screen *self)
{
}

static void
gdk_x11_screen_class_init(GdkX11ScreenClass *klass)
{
}

G_DEFINE_TYPE (GdkX11Screen, gdk_x11_screen, GDK_TYPE_SCREEN)
#else
GType gdk_x11_screen_get_type (void)
{
  return 0;
}
#endif
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

gboolean
gdk_rgba_parse (GdkRGBA     *rgba,
                const gchar *spec)
{
  if (!rgba) {
    return TRUE; /* copied from upstream */
  }

  GdkColor color;

  if (!gdk_color_parse (spec, &color)) { /* argument order reversed */
    return FALSE;
  }

  rgba->red = (gdouble)color.red/65535;
  rgba->green = (gdouble)color.green/65535;
  rgba->blue = (gdouble)color.blue/65535;
#ifndef __FAST_MATH__ /* lossless but slow */
#include <math.h>
  if (strncmp (spec, "rgba", 4)) {
    rgba->alpha = 1;
    return TRUE;
  }
  if (*spec != ',') {
    return FALSE;
  }

  spec++;

  while (*spec == ' ') {
    spec++;
  }

  gchar *p;

  gdouble a = g_ascii_strtod (spec, &p);
  if (errno == ERANGE || p == spec || isinf (a) || isnan (a)) {
    return FALSE;
  }

  spec = p;

  while (*spec == ' ') {
    spec++;
  }

  if (*spec != ')') {
    return FALSE;
  }

  spec++;

  while (*spec == ' ') {
    spec++;
  }

  if (*spec != '\0') {
    return FALSE;
  }

  rgba->alpha = CLAMP (a, 0, 1);
#else /* lossy but fast */
  rgba->alpha = 1;
#endif
  return TRUE;
}

GType
gdk_x11_display_get_type (void)
{
  return 0;
}

GdkWindow *
gdk_window_get_device_position_double (GdkWindow       *window,
                                       GdkDevice       *device,
                                       double          *x,
                                       double          *y,
                                       GdkModifierType *mask)
{
  return NULL;
}

GdkWindow *
gdk_window_get_device_position (GdkWindow       *window,
                                GdkDevice       *device,
                                gint            *x,
                                gint            *y,
                                GdkModifierType *mask)
{
  return NULL;
}

void
gdk_device_get_position_double (GdkDevice        *device,
                                GdkScreen       **screen,
                                gdouble          *x,
                                gdouble          *y)
{
}

void
gdk_device_get_position (GdkDevice        *device,
                         GdkScreen       **screen,
                         gint             *x,
                         gint             *y)
{
}

GdkWindow *
gdk_device_get_window_at_position_double (GdkDevice  *device,
                                          gdouble    *win_x,
                                          gdouble    *win_y)
{
  return NULL;
}

GdkWindow *
gdk_device_get_window_at_position (GdkDevice  *device,
                                   gint       *win_x,
                                   gint       *win_y)
{
  return NULL;
}

GdkDisplay *
gdk_device_get_display (GdkDevice *device)
{
  return NULL;
}

GdkDevice *
gdk_device_get_associated_device (GdkDevice *device)
{
  return NULL;
}

GList *
gdk_device_list_slave_devices (GdkDevice *device)
{
  return NULL;
}

typedef enum {
  GDK_DEVICE_TYPE_MASTER,
  GDK_DEVICE_TYPE_SLAVE,
  GDK_DEVICE_TYPE_FLOATING
} GdkDeviceType;

GdkDeviceType
gdk_device_get_device_type (GdkDevice *device)
{
  return GDK_DEVICE_TYPE_MASTER;
}

GList *
gdk_device_list_axes (GdkDevice *device)
{
  return NULL;
}

gboolean
gdk_device_get_axis_value (GdkDevice *device,
                           gdouble   *axes,
                           GdkAtom    axis_label,
                           gdouble   *value)
{
#if 0 /* closest translation to gtk2 calls */
  return gdk_device_get_axis(device, axes, (GdkAxisUse)(*(char*)axis_label), value);
#endif /* might return bogus data otherwise */
  return FALSE;
}

GdkGrabStatus
gdk_device_grab ()
{
#if 0 /* not in gtk2 */
  return GDK_GRAB_FAILED;
#endif
  return GDK_GRAB_SUCCESS;
}

void
gdk_device_ungrab (GdkDevice  *device,
                   guint32     time_)
{
}

void
gdk_device_warp (GdkDevice  *device,
                 GdkScreen  *screen,
                 gint        x,
                 gint        y)
{
}

GdkWindow *
gdk_device_get_last_event_window (GdkDevice *device)
{
  return NULL;
}

const gchar *
gdk_device_get_vendor_id (GdkDevice *device)
{
  return NULL;
}

const gchar *
gdk_device_get_product_id (GdkDevice *device)
{
  return NULL;
}

typedef struct _GdkSeat GdkSeat;

void
gdk_device_set_seat (GdkDevice *device,
                     GdkSeat   *seat)
{
}

GdkSeat *
gdk_device_get_seat (GdkDevice *device)
{
  return NULL;
}

typedef enum
{
  GDK_AXIS_FLAG_X        = 1 << GDK_AXIS_X,
  GDK_AXIS_FLAG_Y        = 1 << GDK_AXIS_Y,
} GdkAxisFlags;

GdkAxisFlags
gdk_device_get_axes (GdkDevice *device)
{
  return 0;
}

void
gdk_device_update_tool ()
{
}

GdkInputMode
gdk_device_get_input_mode (GdkDevice *device)
{
  return device ? device->mode : GDK_MODE_DISABLED;
}

GdkDisplay *
gdk_device_manager_get_display ()
{
  return NULL;
}

GList *
gdk_device_manager_list_devices ()
{
  return NULL;
}

GdkDevice *
gdk_device_manager_get_client_pointer ()
{
  return NULL;
}

void
gdk_screen_get_monitor_workarea (GdkScreen    *screen,
                                 gint          monitor_num,
                                 GdkRectangle *dest)
{
  gdk_screen_get_monitor_geometry (screen, monitor_num, dest);
}

gint
gdk_screen_get_monitor_scale_factor (GdkScreen *screen,
                                     gint       monitor_num)
{
  return 1;
}

typedef struct _GdkMonitor GdkMonitor;

GdkMonitor *
gdk_display_get_monitor (GdkDisplay *display,
                         gint        monitor_num)
{
  return NULL;
}

typedef enum {
  GDK_SUBPIXEL_LAYOUT_UNKNOWN,
  GDK_SUBPIXEL_LAYOUT_NONE,
  GDK_SUBPIXEL_LAYOUT_HORIZONTAL_RGB,
  GDK_SUBPIXEL_LAYOUT_HORIZONTAL_BGR,
  GDK_SUBPIXEL_LAYOUT_VERTICAL_RGB,
  GDK_SUBPIXEL_LAYOUT_VERTICAL_BGR
} GdkSubpixelLayout;

struct _GdkMonitor {
  GObject parent;

  GdkDisplay *display;
  char *manufacturer;
  char *model;
  char *connector;
  GdkRectangle geometry;
  int width_mm;
  int height_mm;
  int scale_factor;
  int refresh_rate;
  GdkSubpixelLayout subpixel_layout;
};

GdkDisplay *
gdk_monitor_get_display (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, NULL);

  return monitor->display;
}

void
gdk_monitor_get_geometry (GdkMonitor   *monitor,
                          GdkRectangle *geometry)
{
  g_return_if_fail (monitor);
  g_return_if_fail (geometry != NULL);

  *geometry = monitor->geometry;
}

void
gdk_monitor_get_workarea (GdkMonitor   *monitor,
                          GdkRectangle *workarea)
{
  g_return_if_fail (monitor);
  g_return_if_fail (workarea);

  *workarea = monitor->geometry;
}

int
gdk_monitor_get_width_mm (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, 0);

  return monitor->width_mm;
}

int
gdk_monitor_get_height_mm (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, 0);

  return monitor->height_mm;
}

const char *
gdk_monitor_get_connector (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, NULL);

  return monitor->connector;
}

const char *
gdk_monitor_get_manufacturer (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, NULL);

  return monitor->manufacturer;
}

const char *
gdk_monitor_get_model (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, NULL);

  return monitor->model;
}

int
gdk_monitor_get_scale_factor (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, 1);

  return monitor->scale_factor;
}

int
gdk_monitor_get_refresh_rate (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, 0);

  return monitor->refresh_rate;
}

GdkSubpixelLayout
gdk_monitor_get_subpixel_layout (GdkMonitor *monitor)
{
  g_return_val_if_fail (monitor, GDK_SUBPIXEL_LAYOUT_UNKNOWN);

  return monitor->subpixel_layout;
}

gboolean
gdk_monitor_is_primary (GdkMonitor *monitor)
{
  return TRUE;
}

struct _GdkMonitorClass {
  GObjectClass parent_class;

  void (* get_workarea) (GdkMonitor   *monitor,
                         GdkRectangle *geometry);
};

typedef struct _GdkMonitorClass GdkMonitorClass;

static void
gdk_monitor_init(GdkMonitor *self)
{
}

static void
gdk_monitor_class_init(GdkMonitorClass *klass)
{
}

G_DEFINE_TYPE (GdkMonitor, gdk_monitor, G_TYPE_OBJECT)

GdkMonitor *
gdk_monitor_new (GdkDisplay *display)
{
  return g_object_new (gdk_monitor_get_type (), "display", display, NULL);
}

void
gdk_monitor_set_manufacturer (GdkMonitor *monitor,
                              const char *manufacturer)
{
  g_free (monitor->manufacturer);
  monitor->manufacturer = g_strdup (manufacturer);

  g_object_notify (G_OBJECT (monitor), "manufacturer");
}

void
gdk_monitor_set_model (GdkMonitor *monitor,
                       const char *model)
{
  g_free (monitor->model);
  monitor->model = g_strdup (model);

  g_object_notify (G_OBJECT (monitor), "model");
}

void
gdk_monitor_set_connector (GdkMonitor *monitor,
                           const char *connector)
{
  g_free (monitor->connector);
  monitor->connector = g_strdup (connector);

  /* g_object_notify (G_OBJECT (monitor), "connector"); */
}

void
gdk_monitor_set_position (GdkMonitor *monitor,
                          int         x,
                          int         y)
{
  g_object_freeze_notify (G_OBJECT (monitor));

  if (monitor->geometry.x != x)
    {
      monitor->geometry.x = x;
      g_object_notify (G_OBJECT (monitor), "geometry");
    }

  if (monitor->geometry.y != y)
    {
      monitor->geometry.y = y;
      g_object_notify (G_OBJECT (monitor), "geometry");
    }

  g_object_thaw_notify (G_OBJECT (monitor));
}

void
gdk_monitor_set_size (GdkMonitor *monitor,
                      int         width,
                      int         height)
{
  g_object_freeze_notify (G_OBJECT (monitor));

  if (monitor->geometry.width != width)
    {
      monitor->geometry.width = width;
      g_object_notify (G_OBJECT (monitor), "geometry");
    }

  if (monitor->geometry.height != height)
    {
      monitor->geometry.height = height;
      g_object_notify (G_OBJECT (monitor), "geometry");
    }

  g_object_thaw_notify (G_OBJECT (monitor));
}

void
gdk_monitor_set_physical_size (GdkMonitor *monitor,
                               int         width_mm,
                               int         height_mm)
{
  g_object_freeze_notify (G_OBJECT (monitor));

  if (monitor->width_mm != width_mm)
    {
      monitor->width_mm = width_mm;
      g_object_notify (G_OBJECT (monitor), "width-mm");
    }

  if (monitor->height_mm != height_mm)
    {
      monitor->height_mm = height_mm;
      g_object_notify (G_OBJECT (monitor), "height-mm");
    }

  g_object_thaw_notify (G_OBJECT (monitor));
}

void
gdk_monitor_set_scale_factor (GdkMonitor *monitor,
                              int         scale_factor)
{
  if (monitor->scale_factor == scale_factor)
    return;

  monitor->scale_factor = scale_factor;

  g_object_notify (G_OBJECT (monitor), "scale-factor");
}

void
gdk_monitor_set_refresh_rate (GdkMonitor *monitor,
                              int         refresh_rate)
{
  if (monitor->refresh_rate == refresh_rate)
    return;

  monitor->refresh_rate = refresh_rate;

  g_object_notify (G_OBJECT (monitor), "refresh-rate");
}

void
gdk_monitor_set_subpixel_layout (GdkMonitor        *monitor,
                                 GdkSubpixelLayout  subpixel_layout)
{
  if (monitor->subpixel_layout == subpixel_layout)
    return;

  monitor->subpixel_layout = subpixel_layout;

  g_object_notify (G_OBJECT (monitor), "subpixel-layout");
}

void
gdk_monitor_invalidate (GdkMonitor *monitor)
{
}

gboolean
gdk_keymap_get_num_lock_state (GdkKeymap *keymap)
{
  return TRUE;
}

gboolean
gdk_keymap_get_scroll_lock_state (GdkKeymap *keymap)
{
  return FALSE;
}

guint
gdk_keymap_get_modifier_state (GdkKeymap *keymap)
{
  return 0;
}

static gboolean
gdk_cairo_surface_extents (cairo_surface_t *surface,
                            GdkRectangle    *extents)
{
  double x1, x2, y1, y2;
  cairo_t *cr;

  g_return_val_if_fail (surface != NULL, FALSE);
  g_return_val_if_fail (extents != NULL, FALSE);

  cr = cairo_create (surface);
  cairo_clip_extents (cr, &x1, &y1, &x2, &y2);
  cairo_destroy (cr);
#if 0
  x1 = (long)x1;
  y1 = (long)y1;
  x2 = x2 == (long)(x2) ? (long)(x2) : (long)(x2) + 1;
  y2 = y2 == (long)(y2) ? (long)(y2) : (long)(y2) + 1;
#endif

  extern double floor(double x);
  extern double ceil(double x);

  x1 = floor(x1);
  y1 = floor(y1);
  x2 = ceil(x2);
  y2 = ceil(y2);

  x2 -= x1;
  y2 -= y1;

  if (x1 < G_MININT || x1 > G_MAXINT ||
      y1 < G_MININT || y1 > G_MAXINT ||
      x2 > G_MAXINT || y2 > G_MAXINT)
    {
      extents->x = extents->y = extents->width = extents->height = 0;
      return FALSE;
    }

  extents->x = x1;
  extents->y = y1;
  extents->width = x2;
  extents->height = y2;

  return TRUE;
}

cairo_region_t *
gdk_cairo_region_create_from_surface (cairo_surface_t *surface)
{
  cairo_region_t *region;
  GdkRectangle extents, rect;
  cairo_surface_t *image;
  cairo_t *cr;
  gint x, y, stride;
  guchar *data;

  gdk_cairo_surface_extents (surface, &extents);

  if (cairo_surface_get_content (surface) == CAIRO_CONTENT_COLOR) {
    return cairo_region_create_rectangle ((const cairo_rectangle_int_t*)&extents);
  }

  if (cairo_surface_get_type (surface) != CAIRO_SURFACE_TYPE_IMAGE ||
      cairo_image_surface_get_format (surface) != CAIRO_FORMAT_A1)
    {
      /* coerce to an A1 image */
      image = cairo_image_surface_create (CAIRO_FORMAT_A1,
                                          extents.width, extents.height);
      cr = cairo_create (image);
      cairo_set_source_surface (cr, surface, -extents.x, -extents.y);
      cairo_paint (cr);
      cairo_destroy (cr);
    }
  else
    image = cairo_surface_reference (surface);

  /* Flush the surface to make sure that the rendering is up to date. */
  cairo_surface_flush (image);

  data = cairo_image_surface_get_data (image);
  stride = cairo_image_surface_get_stride (image);

  region = cairo_region_create ();

  for (y = 0; y < extents.height; y++)
    {
      for (x = 0; x < extents.width; x++)
        {
          /* Search for a continuous range of "non transparent pixels"*/
          gint x0 = x;
          while (x < extents.width)
            {
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
              if (((data[x / 8] >> (x%8)) & 1) == 0)
#else
              if (((data[x / 8] >> (7-(x%8))) & 1) == 0)
#endif
                /* This pixel is "transparent"*/
                break;
              x++;
            }

          if (x > x0)
            {
              /* Add the pixels (x0, y) to (x, y+1) as a new rectangle
               * in the region
               */
              rect.x = x0;
              rect.width = x - x0;
              rect.y = y;
              rect.height = 1;

              cairo_region_union_rectangle (region, (const cairo_rectangle_int_t*)&rect);
            }
        }
      data += stride;
    }

  cairo_surface_destroy (image);

  cairo_region_translate (region, extents.x, extents.y);

  return region;
}
