#include "gtk.h"

GtkWidget *
gtk_grid_new (void)
{
  return gtk_table_new (0, 0, FALSE);
}

void
gtk_grid_attach (GtkGrid   *grid,
                 GtkWidget *child,
                 gint       left,
                 gint       top,
                 gint       width,
                 gint       height)
{
  gtk_table_attach (grid, child, left, 0, top, 0, GTK_FILL, GTK_FILL, width, height);
}

GType
gtk_grid_get_type (void)
{
#if 0
  return gtk_table_get_type ();
#else
  return GTK_TYPE_TABLE;
#endif
}

void
gtk_grid_set_column_spacing (GtkGrid *grid,
                             guint    spacing)
{
  int i;

  for(i = 0; i < grid->ncols; i++) {
    gtk_table_set_col_spacing (grid, i, spacing);
  }
}

void
gtk_grid_set_column_homogeneous (GtkGrid  *grid,
                                 gboolean  homogeneous)
{
  gtk_table_set_homogeneous  (grid, homogeneous);
}
