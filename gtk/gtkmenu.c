#include <gtk/gtkmenu.h>
#include <gtk/gtkmenuitem.h>
#include <gtk/gtkmain.h>

#include <gdk/gdkseat.h>

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

/**
 * gtk_menu_popup_for_device:
 * @menu: a #GtkMenu
 * @device: (allow-none): a #GdkDevice
 * @parent_menu_shell: (allow-none): the menu shell containing the triggering
 *     menu item, or %NULL
 * @parent_menu_item: (allow-none): the menu item whose activation triggered
 *     the popup, or %NULL
 * @func: (allow-none): a user supplied function used to position the menu,
 *     or %NULL
 * @data: (allow-none): user supplied data to be passed to @func
 * @destroy: (allow-none): destroy notify for @data
 * @button: the mouse button which was pressed to initiate the event
 * @activate_time: the time at which the activation event occurred
 *
 * Displays a menu and makes it available for selection.
 *
 * Applications can use this function to display context-sensitive menus,
 * and will typically supply %NULL for the @parent_menu_shell,
 * @parent_menu_item, @func, @data and @destroy parameters. The default
 * menu positioning function will position the menu at the current position
 * of @device (or its corresponding pointer).
 *
 * The @button parameter should be the mouse button pressed to initiate
 * the menu popup. If the menu popup was initiated by something other than
 * a mouse button press, such as a mouse button release or a keypress,
 * @button should be 0.
 *
 * The @activate_time parameter is used to conflict-resolve initiation of
 * concurrent requests for mouse/keyboard grab requests. To function
 * properly, this needs to be the time stamp of the user event (such as
 * a mouse click or key press) that caused the initiation of the popup.
 * Only if no such event is available, gtk_get_current_event_time() can
 * be used instead.
 *
 * Note that this function does not work very well on GDK backends that
 * do not have global coordinates, such as Wayland or Mir. You should
 * probably use one of the gtk_menu_popup_at_ variants, which do not
 * have this problem.
 *
 * Since: 3.0
 *
 * Deprecated: 3.22: Please use gtk_menu_popup_at_widget(),
 *     gtk_menu_popup_at_pointer(). or gtk_menu_popup_at_rect() instead
 */
void
gtk_menu_popup_for_device (GtkMenu             *menu,
                           GdkDevice           *device,
                           GtkWidget           *parent_menu_shell,
                           GtkWidget           *parent_menu_item,
                           GtkMenuPositionFunc  func,
                           gpointer             data,
                           GDestroyNotify       destroy,
                           guint                button,
                           guint32              activate_time)
{
  g_return_if_fail (GTK_IS_MENU (menu));

  menu->parent_menu_item = NULL;

  gtk_menu_popup (menu,
                  parent_menu_shell,
                  parent_menu_item,
                  func,
                  data,
                  button,
                  activate_time);
}


static inline GdkDevice *
get_device_for_event (const GdkEvent *event)
{
  return gdk_event_get_device (event);
}


/**
 * gtk_menu_popup_at_rect:
 * @menu: the #GtkMenu to pop up
 * @rect_window: (not nullable): the #GdkWindow @rect is relative to
 * @rect: (not nullable): the #GdkRectangle to align @menu with
 * @rect_anchor: the point on @rect to align with @menu's anchor point
 * @menu_anchor: the point on @menu to align with @rect's anchor point
 * @trigger_event: (nullable): the #GdkEvent that initiated this request or
 *                 %NULL if it's the current event
 *
 * Displays @menu and makes it available for selection.
 *
 * See gtk_menu_popup_at_widget () and gtk_menu_popup_at_pointer (), which
 * handle more common cases for popping up menus.
 *
 * @menu will be positioned at @rect, aligning their anchor points. @rect is
 * relative to the top-left corner of @rect_window. @rect_anchor and
 * @menu_anchor determine anchor points on @rect and @menu to pin together.
 * @menu can optionally be offset by #GtkMenu:rect-anchor-dx and
 * #GtkMenu:rect-anchor-dy.
 *
 * Anchors should be specified under the assumption that the text direction is
 * left-to-right; they will be flipped horizontally automatically if the text
 * direction is right-to-left.
 *
 * Other properties that influence the behaviour of this function are
 * #GtkMenu:anchor-hints and #GtkMenu:menu-type-hint. Connect to the
 * #GtkMenu::popped-up signal to find out how it was actually positioned.
 *
 * Since: 3.22
 */
void
gtk_menu_popup_at_rect (GtkMenu            *menu,
                        GdkWindow          *rect_window,
                        const GdkRectangle *rect,
                        GdkGravity          rect_anchor,
                        GdkGravity          menu_anchor,
                        const GdkEvent     *trigger_event)
{
  GdkEvent *current_event = NULL;
  guint button = 0;
  guint32 activate_time = GDK_CURRENT_TIME;

  g_return_if_fail (GTK_IS_MENU (menu));

  menu->parent_menu_item = NULL;

  if (!trigger_event)
    {
      current_event = gtk_get_current_event ();
      trigger_event = current_event;
    }

  if (trigger_event)
    {
      gdk_event_get_button (trigger_event, &button);
      activate_time = gdk_event_get_time (trigger_event);
    }
  else
    g_warning ("no trigger event for menu popup");

  gtk_menu_popup (menu,
                  NULL,
                  NULL,
                  NULL,
                  NULL,
                  button,
                  activate_time);

  g_clear_pointer (&current_event, gdk_event_free);
}

/**
 * gtk_menu_popup_at_widget:
 * @menu: the #GtkMenu to pop up
 * @widget: (not nullable): the #GtkWidget to align @menu with
 * @widget_anchor: the point on @widget to align with @menu's anchor point
 * @menu_anchor: the point on @menu to align with @widget's anchor point
 * @trigger_event: (nullable): the #GdkEvent that initiated this request or
 *                 %NULL if it's the current event
 *
 * Displays @menu and makes it available for selection.
 *
 * See gtk_menu_popup_at_pointer () to pop up a menu at the master pointer.
 * gtk_menu_popup_at_rect () also allows you to position a menu at an arbitrary
 * rectangle.
 *
 * ![](popup-anchors.png)
 *
 * @menu will be positioned at @widget, aligning their anchor points.
 * @widget_anchor and @menu_anchor determine anchor points on @widget and @menu
 * to pin together. @menu can optionally be offset by #GtkMenu:rect-anchor-dx
 * and #GtkMenu:rect-anchor-dy.
 *
 * Anchors should be specified under the assumption that the text direction is
 * left-to-right; they will be flipped horizontally automatically if the text
 * direction is right-to-left.
 *
 * Other properties that influence the behaviour of this function are
 * #GtkMenu:anchor-hints and #GtkMenu:menu-type-hint. Connect to the
 * #GtkMenu::popped-up signal to find out how it was actually positioned.
 *
 * Since: 3.22
 */
void
gtk_menu_popup_at_widget (GtkMenu        *menu,
                          GtkWidget      *widget,
                          GdkGravity      widget_anchor,
                          GdkGravity      menu_anchor,
                          const GdkEvent *trigger_event)
{
  GdkEvent *current_event = NULL;
  guint button = 0;
  guint32 activate_time = GDK_CURRENT_TIME;
  GtkWidget *parent_menu_shell = NULL;
  GtkWidget *parent_menu_item = NULL;

  g_return_if_fail (GTK_IS_MENU (menu));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  menu->parent_menu_item = widget;

  if (!trigger_event)
    {
      current_event = gtk_get_current_event ();
      trigger_event = current_event;
    }

  if (trigger_event)
    {
      gdk_event_get_button (trigger_event, &button);
      activate_time = gdk_event_get_time (trigger_event);
    }
  else
    g_warning ("no trigger event for menu popup");

  if (GTK_IS_MENU_ITEM (menu->parent_menu_item))
    {
      parent_menu_item = menu->parent_menu_item;

      if (GTK_IS_MENU_SHELL (gtk_widget_get_parent (parent_menu_item)))
        parent_menu_shell = gtk_widget_get_parent (parent_menu_item);
    }

  gtk_menu_popup (menu,
                  parent_menu_shell,
                  parent_menu_item,
                  NULL,
                  NULL,
                  button,
                  activate_time);

  g_clear_pointer (&current_event, gdk_event_free);
}

/**
 * gtk_menu_popup_at_pointer:
 * @menu: the #GtkMenu to pop up
 * @trigger_event: (nullable): the #GdkEvent that initiated this request or
 *                 %NULL if it's the current event
 *
 * Displays @menu and makes it available for selection.
 *
 * See gtk_menu_popup_at_widget () to pop up a menu at a widget.
 * gtk_menu_popup_at_rect () also allows you to position a menu at an arbitrary
 * rectangle.
 *
 * @menu will be positioned at the pointer associated with @trigger_event.
 *
 * Properties that influence the behaviour of this function are
 * #GtkMenu:anchor-hints, #GtkMenu:rect-anchor-dx, #GtkMenu:rect-anchor-dy, and
 * #GtkMenu:menu-type-hint. Connect to the #GtkMenu::popped-up signal to find
 * out how it was actually positioned.
 *
 * Since: 3.22
 */
void
gtk_menu_popup_at_pointer (GtkMenu        *menu,
                           const GdkEvent *trigger_event)
{
  GdkEvent *current_event = NULL;
  GdkWindow *rect_window = NULL;
  GdkDevice *device = NULL;
  GdkRectangle rect = { 0, 0, 1, 1 };

  g_return_if_fail (GTK_IS_MENU (menu));

  if (!trigger_event)
    {
      current_event = gtk_get_current_event ();
      trigger_event = current_event;
    }

  if (trigger_event)
    {
      rect_window = gdk_event_get_window (trigger_event);

      if (rect_window)
        {
          device = get_device_for_event (trigger_event);

          if (device)
            gdk_window_get_device_position (rect_window, device, &rect.x, &rect.y, NULL);
        }
    }
  else
    g_warning ("no trigger event for menu popup");

  gtk_menu_popup_at_rect (menu,
                          rect_window,
                          &rect,
                          GDK_GRAVITY_SOUTH_EAST,
                          GDK_GRAVITY_NORTH_WEST,
                          trigger_event);

  g_clear_pointer (&current_event, gdk_event_free);
}
