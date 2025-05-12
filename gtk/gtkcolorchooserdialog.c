#include <gtk/gtk.h>

GType
gtk_color_chooser_dialog_get_type (void)
{
#if 0
  return gtk_color_selection_dialog_get_type ();
#else
  return GTK_TYPE_COLOR_SELECTION_DIALOG;
#endif
}

GtkWidget *
gtk_color_chooser_dialog_new (const gchar *title,
                              GtkWindow   *parent)
{
  return gtk_color_selection_dialog_new (title);
}
