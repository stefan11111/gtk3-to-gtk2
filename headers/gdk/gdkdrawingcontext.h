/* GDK - The GIMP Drawing Kit
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GDK_DRAWING_CONTEXT_H__
#define __GDK_DRAWING_CONTEXT_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdk.h> can be included directly."
#endif

#include <gdk/gdktypes.h>
#include <cairo/cairo.h>

/* Not Implemented */
/* TODO: Maybe Implement Later */
/* This is part of the gl support for gtk3 */

G_BEGIN_DECLS

#define GDK_TYPE_DRAWING_CONTEXT (gdk_drawing_context_get_type ())

#define GDK_DRAWING_CONTEXT(obj)        (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_DRAWING_CONTEXT, GdkDrawingContext))
#define GDK_IS_DRAWING_CONTEXT(obj)     (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDK_TYPE_DRAWING_CONTEXT))

typedef struct _GdkDrawingContext       GdkDrawingContext;
typedef struct _GdkDrawingContextClass  GdkDrawingContextClass;


GType gdk_drawing_context_get_type (void) G_GNUC_CONST;


GdkWindow *     gdk_drawing_context_get_window          (GdkDrawingContext *context);

cairo_region_t *gdk_drawing_context_get_clip            (GdkDrawingContext *context);


gboolean        gdk_drawing_context_is_valid            (GdkDrawingContext *context);


cairo_t *       gdk_drawing_context_get_cairo_context   (GdkDrawingContext *context);

G_END_DECLS

#endif /* __GDK_DRAWING_CONTEXT_H__ */
