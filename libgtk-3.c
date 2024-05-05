#include <gtk/gtk.h>
#include "headers/gtk/gtk.h"

GtkWidget *
gtk_grid_new (void)
{
  return NULL;
}

gboolean
gtk_widget_get_hexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_hexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

gboolean
gtk_widget_get_vexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_vexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

/**
 * gtk_box_new:
 * @orientation: the box’s orientation.
 * @spacing: the number of pixels to place by default between children.
 *
 * Creates a new #GtkBox.
 *
 * Returns: a new #GtkBox.
 *
 * Since: 3.0
 **/
GtkWidget*
gtk_box_new (GtkOrientation orientation,
             gint           spacing)
{
  return g_object_new (GTK_TYPE_BOX,
                       "orientation", orientation,
                       "spacing",     spacing,
                       NULL);
}

/**
 * gtk_widget_get_allocated_width:
 * @widget: the widget to query
 *
 * Returns the width that has currently been allocated to @widget.
 * This function is intended to be used when implementing handlers
 * for the #GtkWidget::draw function.
 *
 * Returns: the width of the @widget
 **/
int
gtk_widget_get_allocated_width (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.width;
}

/**
 * gtk_widget_get_allocated_height:
 * @widget: the widget to query
 *
 * Returns the height that has currently been allocated to @widget.
 * This function is intended to be used when implementing handlers
 * for the #GtkWidget::draw function.
 *
 * Returns: the height of the @widget
 **/
int
gtk_widget_get_allocated_height (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.height;
}

/**
 * gtk_widget_get_style_context:
 * @widget: a #GtkWidget
 *
 * Returns the style context associated to @widget. The returned object is
 * guaranteed to be the same for the lifetime of @widget.
 *
 * Returns: (transfer none): a #GtkStyleContext. This memory is owned by @widget and
 *          must not be freed.
 **/
GtkStyleContext *
gtk_widget_get_style_context (GtkWidget *widget)
{
  return NULL;
}
