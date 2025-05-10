#ifndef __INTERNAL_GTK_STYLE_CONTEXT_H__
#define __INTERNAL_GTK_STYLE_CONTEXT_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include "gtkenums.h"
#include "gdkrgba.h"

G_BEGIN_DECLS

#define GTK_TYPE_STYLE_CONTEXT         (gtk_style_context_get_type ())
#define GTK_STYLE_CONTEXT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_STYLE_CONTEXT, GtkStyleContext))
#define GTK_STYLE_CONTEXT_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST    ((c), GTK_TYPE_STYLE_CONTEXT, GtkStyleContextClass))
#define GTK_IS_STYLE_CONTEXT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_STYLE_CONTEXT))
#define GTK_IS_STYLE_CONTEXT_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE    ((c), GTK_TYPE_STYLE_CONTEXT))
#define GTK_STYLE_CONTEXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS  ((o), GTK_TYPE_STYLE_CONTEXT, GtkStyleContextClass))

typedef GtkStyle GtkStyleContext;
typedef GtkStyleClass GtkStyleContextClass;

GType gtk_style_context_get_type (void) G_GNUC_CONST;

void gtk_style_context_get_color            (GtkStyleContext *context,
                                             GtkStateFlags    state,
                                             GdkRGBA         *color);

const PangoFontDescription *
gtk_style_context_get_font (GtkStyleContext *context,
                            GtkStateFlags    state);

G_END_DECLS

#endif /* __INTERNAL_GTK_STYLE_CONTEXT_H__ */
