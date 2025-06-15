#include <gtk/gtkbutton.h>
#include <gtk/gtkimage.h>

/**
 * gtk_button_new_from_icon_name:
 * @icon_name: (nullable): an icon name or %NULL
 * @size: (type int): an icon size (#GtkIconSize)
 *
 * Creates a new button containing an icon from the current icon theme.
 *
 * If the icon name isn’t known, a “broken image” icon will be
 * displayed instead. If the current icon theme is changed, the icon
 * will be updated appropriately.
 *
 * This function is a convenience wrapper around gtk_button_new() and
 * gtk_button_set_image().
 *
 * Returns: a new #GtkButton displaying the themed icon
 *
 * Since: 3.10
 */
GtkWidget*
gtk_button_new_from_icon_name (const gchar *icon_name,
                               GtkIconSize  size)
{
  GtkWidget *button;
  GtkWidget *image;

  image = gtk_image_new_from_icon_name (icon_name, size);
  button =  g_object_new (GTK_TYPE_BUTTON,
                          "image", image,
                          NULL);

  return button;
}

