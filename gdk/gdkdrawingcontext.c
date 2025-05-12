#include <gdk/gdk.h>
#include "util.h"

/* Not Implemented */
/* TODO: Maybe Implement Later */
/* This is part of the gl support for gtk3 */

G_DEFINE_TYPE (GdkDrawingContext, gdk_drawing_context, G_TYPE_OBJECT)

static void
gdk_drawing_context_class_init (GdkDrawingContextClass *klass)
{
}

static void
gdk_drawing_context_init (GdkDrawingContext *self)
{
}

void
gdk_cairo_set_drawing_context (cairo_t           *cr,
                               GdkDrawingContext *context)
{
}

GdkDrawingContext *
gdk_cairo_get_drawing_context (cairo_t *cr)
{
  return NULL;
}

cairo_t *
gdk_drawing_context_get_cairo_context (GdkDrawingContext *context)
{
  return NULL;
}

GdkWindow *
gdk_drawing_context_get_window (GdkDrawingContext *context)
{
  return NULL;
}

cairo_region_t *
gdk_drawing_context_get_clip (GdkDrawingContext *context)
{
  return NULL;
}

gboolean
gdk_drawing_context_is_valid (GdkDrawingContext *context)
{
  return FALSE;
}
