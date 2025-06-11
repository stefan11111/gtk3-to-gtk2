#include <gdk/gdkwindow.h>

/* Not Implemented */
/* TODO: Maybe Implement Later */
/* This is part of the gl support for gtk3 */

/* stub typedef */
typedef struct _GdkGLContext GdkGLContext;

typedef struct {
  float x1, y1, x2, y2;
  float u1, v1, u2, v2;
} GdkTexturedQuad;

void
gdk_cairo_surface_mark_as_direct (cairo_surface_t *surface,
                                  GdkWindow *window)
{
}

void
gdk_gl_texture_quads (GdkGLContext *paint_context,
                      guint texture_target,
                      int n_quads,
                      GdkTexturedQuad *quads,
                      gboolean flip_colors)
{
}

/* x,y,width,height describes a rectangle in the gl render buffer
   coordinate space, and its top left corner is drawn at the current
   position according to the cairo translation. */

/**
 * gdk_cairo_draw_from_gl:
 * @cr: a cairo context
 * @window: The window we're rendering for (not necessarily into)
 * @source: The GL ID of the source buffer
 * @source_type: The type of the @source
 * @buffer_scale: The scale-factor that the @source buffer is allocated for
 * @x: The source x position in @source to start copying from in GL coordinates
 * @y: The source y position in @source to start copying from in GL coordinates
 * @width: The width of the region to draw
 * @height: The height of the region to draw
 *
 * This is the main way to draw GL content in GTK+. It takes a render buffer ID
 * (@source_type == #GL_RENDERBUFFER) or a texture id (@source_type == #GL_TEXTURE)
 * and draws it onto @cr with an OVER operation, respecting the current clip.
 * The top left corner of the rectangle specified by @x, @y, @width and @height
 * will be drawn at the current (0,0) position of the cairo_t.
 *
 * This will work for *all* cairo_t, as long as @window is realized, but the
 * fallback implementation that reads back the pixels from the buffer may be
 * used in the general case. In the case of direct drawing to a window with
 * no special effects applied to @cr it will however use a more efficient
 * approach.
 *
 * For #GL_RENDERBUFFER the code will always fall back to software for buffers
 * with alpha components, so make sure you use #GL_TEXTURE if using alpha.
 *
 * Calling this may change the current GL context.
 *
 * Since: 3.16
 */
void
gdk_cairo_draw_from_gl (cairo_t              *cr,
                        GdkWindow            *window,
                        int                   source,
                        int                   source_type,
                        int                   buffer_scale,
                        int                   x,
                        int                   y,
                        int                   width,
                        int                   height)
{
}

/* This is always called with the paint context current */
void
gdk_gl_texture_from_surface (cairo_surface_t *surface,
                             cairo_region_t  *region)
{
}
