#include <gtk/gtkcolorsel.h>

GType
gtk_color_chooser_widget_get_type (void)
{
#if 0
  return gtk_color_selection_get_type ();
#else
  return GTK_TYPE_COLOR_SELECTION;
#endif
}

GtkWidget *
gtk_color_chooser_widget_new (void)
{
  return gtk_color_selection_new ();
}
