#include "gtk.h"

GType
gtk_font_chooser_get_type (void)
{
#if 0
  return gtk_font_selection_get_type();
#else
  return GTK_TYPE_FONT_SELECTION;
#endif
}

void
gtk_font_chooser_set_font (GtkFontChooser *fontchooser,
                           const gchar    *fontname)
{
  gtk_font_selection_set_font_name(fontchooser, fontname);
}

gchar *
gtk_font_chooser_get_font (GtkFontChooser *fontchooser)
{
  return gtk_font_selection_get_font_name(fontchooser);
}
