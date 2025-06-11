#include <gtk/gtkmenushell.h>

/**
 * gtk_menu_shell_get_selected_item:
 * @menu_shell: a #GtkMenuShell
 *
 * Gets the currently selected item.
 *
 * Returns: (transfer none): the currently selected item
 *
 * Since: 3.0
 */
GtkWidget *
gtk_menu_shell_get_selected_item (GtkMenuShell *menu_shell)
{
  g_return_val_if_fail (GTK_IS_MENU_SHELL (menu_shell), NULL);

  return menu_shell->active_menu_item;
}

/**
 * gtk_menu_shell_get_parent_shell:
 * @menu_shell: a #GtkMenuShell
 *
 * Gets the parent menu shell.
 *
 * The parent menu shell of a submenu is the #GtkMenu or #GtkMenuBar
 * from which it was opened up.
 *
 * Returns: (transfer none): the parent #GtkMenuShell
 *
 * Since: 3.0
 */
GtkWidget *
gtk_menu_shell_get_parent_shell (GtkMenuShell *menu_shell)
{
  g_return_val_if_fail (GTK_IS_MENU_SHELL (menu_shell), NULL);

  return menu_shell->parent_menu_shell;
}

/**
 * gtk_menu_shell_bind_model:
 * @menu_shell: a #GtkMenuShell
 * @model: (allow-none): the #GMenuModel to bind to or %NULL to remove
 *   binding
 * @action_namespace: (allow-none): the namespace for actions in @model
 * @with_separators: %TRUE if toplevel items in @shell should have
 *   separators between them
 *
 * Establishes a binding between a #GtkMenuShell and a #GMenuModel.
 *
 * The contents of @shell are removed and then refilled with menu items
 * according to @model.  When @model changes, @shell is updated.
 * Calling this function twice on @shell with different @model will
 * cause the first binding to be replaced with a binding to the new
 * model. If @model is %NULL then any previous binding is undone and
 * all children are removed.
 *
 * @with_separators determines if toplevel items (eg: sections) have
 * separators inserted between them.  This is typically desired for
 * menus but doesn’t make sense for menubars.
 *
 * If @action_namespace is non-%NULL then the effect is as if all
 * actions mentioned in the @model have their names prefixed with the
 * namespace, plus a dot.  For example, if the action “quit” is
 * mentioned and @action_namespace is “app” then the effective action
 * name is “app.quit”.
 *
 * This function uses #GtkActionable to define the action name and
 * target values on the created menu items.  If you want to use an
 * action group other than “app” and “win”, or if you want to use a
 * #GtkMenuShell outside of a #GtkApplicationWindow, then you will need
 * to attach your own action group to the widget hierarchy using
 * gtk_widget_insert_action_group().  As an example, if you created a
 * group with a “quit” action and inserted it with the name “mygroup”
 * then you would use the action name “mygroup.quit” in your
 * #GMenuModel.
 *
 * For most cases you are probably better off using
 * gtk_menu_new_from_model() or gtk_menu_bar_new_from_model() or just
 * directly passing the #GMenuModel to gtk_application_set_app_menu() or
 * gtk_application_set_menubar().
 *
 * Since: 3.6
 */
void
gtk_menu_shell_bind_model (GtkMenuShell *menu_shell,
                           GMenuModel   *model,
                           const gchar  *action_namespace,
                           gboolean      with_separators)
{
  /* Not Implemented */
}
