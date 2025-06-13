#include <gtk/gtkmenubar.h>
#include <gtk/gtkmenushell.h>

/**
 * gtk_menu_bar_new_from_model:
 * @model: a #GMenuModel
 *
 * Creates a new #GtkMenuBar and populates it with menu items
 * and submenus according to @model.
 *
 * The created menu items are connected to actions found in the
 * #GtkApplicationWindow to which the menu bar belongs - typically
 * by means of being contained within the #GtkApplicationWindows
 * widget hierarchy.
 *
 * Returns: a new #GtkMenuBar
 *
 * Since: 3.4
 */
GtkWidget *
gtk_menu_bar_new_from_model (GMenuModel *model)
{
  GtkWidget *menubar;

  g_return_val_if_fail (G_IS_MENU_MODEL (model), NULL);

  menubar = gtk_menu_bar_new ();
  gtk_menu_shell_bind_model (GTK_MENU_SHELL (menubar), model, NULL, FALSE);

  return menubar;
}
