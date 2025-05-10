#include "gtk.h"

GType
gtk_font_chooser_dialog_get_type (void)
{
#if 0
  return gtk_font_selection_dialog_get_type();
#else
  return GTK_TYPE_FONT_SELECTION_DIALOG;
#endif
}

GtkWidget*
gtk_font_chooser_dialog_new (const gchar *title,
                             GtkWindow   *parent)
{
  return gtk_font_selection_dialog_new(title);
}
