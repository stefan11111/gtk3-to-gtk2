#include <gtk/gtk.h>
#include "util.h"
#include "gtkstyleproviderprivate.h"
#include "gtkstylecascadeprivate.h"
#include "gtksettingsprivate.h"

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

GtkStyleContext *
gtk_style_context_new (void)
{
  return gtk_style_new ();
}

/**
 * gtk_style_context_add_provider_for_screen:
 * @screen: a #GdkScreen
 * @provider: a #GtkStyleProvider
 * @priority: the priority of the style provider. The lower
 *            it is, the earlier it will be used in the style
 *            construction. Typically this will be in the range
 *            between %GTK_STYLE_PROVIDER_PRIORITY_FALLBACK and
 *            %GTK_STYLE_PROVIDER_PRIORITY_USER
 *
 * Adds a global style provider to @screen, which will be used
 * in style construction for all #GtkStyleContexts under @screen.
 *
 * GTK+ uses this to make styling information from #GtkSettings
 * available.
 *
 * Note: If both priorities are the same, A #GtkStyleProvider
 * added through gtk_style_context_add_provider() takes precedence
 * over another added through this function.
 *
 * Since: 3.0
 **/
void
gtk_style_context_add_provider_for_screen (GdkScreen        *screen,
                                           GtkStyleProvider *provider,
                                           guint             priority)
{
  GtkStyleCascade *cascade;

  g_return_if_fail (GDK_IS_SCREEN (screen));
  g_return_if_fail (GTK_IS_STYLE_PROVIDER (provider));
  g_return_if_fail (!GTK_IS_SETTINGS (provider) || _gtk_settings_get_screen (GTK_SETTINGS (provider)) == screen);

  cascade = _gtk_settings_get_style_cascade (gtk_settings_get_for_screen (screen), 1);
  _gtk_style_cascade_add_provider (cascade, provider, priority);
}

/* later */

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
