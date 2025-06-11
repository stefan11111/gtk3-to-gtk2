#include <gtk/gtkmenu.h>

/**
 * gtk_menu_new_from_model:
 * @model: a #GMenuModel
 *
 * Creates a #GtkMenu and populates it with menu items and
 * submenus according to @model.
 *
 * The created menu items are connected to actions found in the
 * #GtkApplicationWindow to which the menu belongs - typically
 * by means of being attached to a widget (see gtk_menu_attach_to_widget())
 * that is contained within the #GtkApplicationWindows widget hierarchy.
 *
 * Actions can also be added using gtk_widget_insert_action_group() on the menu's
 * attach widget or on any of its parent widgets.
 *
 * Returns: a new #GtkMenu
 *
 * Since: 3.4
 */
GtkWidget *
gtk_menu_new_from_model (GMenuModel *model)
{
  /* Not Implemented */
  return gtk_menu_new();
}
