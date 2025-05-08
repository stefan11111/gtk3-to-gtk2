#include <gtk/gtk.h>

GtkWidget*
gtk_font_chooser_dialog_new (const gchar *title,
                             GtkWindow   *parent)
{
  return gtk_font_selection_dialog_new(title);
}
