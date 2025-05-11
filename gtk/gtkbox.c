#include <gtk/gtk.h>

GtkWidget*
gtk_box_new (GtkOrientation orientation,
             gint           spacing)
{
#if 1
  return orientation ? gtk_vbox_new(FALSE, spacing) : gtk_hbox_new(FALSE, spacing);
#else
  return _gtk_box_new(orientation, FALSE, spacing);
#endif
}
