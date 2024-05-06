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
