#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "util.h"

typedef GtkStyle GtkStyleContext;
typedef GtkStyleClass GtkStyleContextClass;

GType
gtk_style_context_get_type (void)
{
#if 0
  return gtk_style_get_type ();
#else
  return GTK_TYPE_STYLE;
#endif
}

const PangoFontDescription *
gtk_style_context_get_font (GtkStyleContext *context,
                            GtkStateFlags    state)
{
  return context->font_desc;
}

void
gtk_style_context_get_color (GtkStyleContext *context,
                             GtkStateFlags    state,
                             GdkRGBA         *color)
{
  *color = GdkRGBA_from_GdkColor(&context->base[GtkStateType_from_GtkStateFlags(state)]);
}
