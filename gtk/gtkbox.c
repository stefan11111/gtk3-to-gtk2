#include <gtk/gtk.h>

GtkWidget*
gtk_box_new (GtkOrientation orientation,
             gint           spacing)
{
#if 1
  return (orientation == GTK_ORIENTATION_VERTICAL)? gtk_vbox_new(FALSE, spacing) : gtk_hbox_new(FALSE, spacing);
#else
  return _gtk_box_new(orientation, FALSE, spacing);
#endif
}

GtkBaselinePosition
gtk_box_get_baseline_position (GtkBox *box)
{
  /* Not Implemented */
  return GTK_BASELINE_POSITION_CENTER;
}

void
gtk_box_set_baseline_position (GtkBox             *box,
                               GtkBaselinePosition position)
{
  /* Not Implemented */
}

GtkWidget *
gtk_box_get_center_widget (GtkBox *box)
{
  /* Not Implemented */
  return NULL;
}

void
gtk_box_set_center_widget (GtkBox    *box,
                           GtkWidget *widget)
{
  /* Not Implemented */
}
