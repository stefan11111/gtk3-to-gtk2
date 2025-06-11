#include <gtk/gtkhbbox.h>
#include <gtk/gtkvbbox.h>

GtkWidget *
gtk_button_box_new (GtkOrientation orientation)
{
  return (orientation == GTK_ORIENTATION_VERTICAL) ? gtk_vbutton_box_new () : gtk_hbutton_box_new ();
}

gboolean
gtk_button_box_get_child_non_homogeneous (GtkButtonBox *widget,
                                          GtkWidget    *child)
{
  return FALSE;
}

void
gtk_button_box_set_child_non_homogeneous (GtkButtonBox *widget,
                                          GtkWidget    *child,
                                          gboolean      non_homogeneous)
{
}
