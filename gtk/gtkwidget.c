#include <gtk/gtk.h>

int
gtk_widget_get_allocated_height (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.height;
}

void
gtk_widget_override_font (GtkWidget                  *widget,
                          const PangoFontDescription *font_desc)
{
  gtk_widget_modify_font (widget, (PangoFontDescription *)font_desc);
}

GtkStyleContext *
gtk_widget_get_style_context (GtkWidget *widget)
{
  return gtk_widget_get_style (widget);
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

void
gtk_widget_queue_compute_expand (GtkWidget      *widget)
{
}

gboolean
gtk_widget_compute_expand (GtkWidget      *widget,
                           GtkOrientation  orientation)
{
  return FALSE;
}

void
gtk_widget_set_halign (GtkWidget *widget,
                       GtkAlign   align)
{
}

GtkAlign
gtk_widget_get_halign (GtkWidget *widget)
{
  return GTK_ALIGN_FILL;
}

void
gtk_widget_set_valign (GtkWidget *widget,
                       GtkAlign   align)
{
}

GtkAlign
gtk_widget_get_valign (GtkWidget *widget)
{
  return GTK_ALIGN_FILL;
}

GtkAlign
gtk_widget_get_valign_with_baseline (GtkWidget *widget)
{
  /* doesn't actually return GTK_ALIGN_BASELINE */
  return GTK_ALIGN_FILL;
}

/**
 * gtk_widget_reset_style:
 * @widget: a #GtkWidget
 *
 * Updates the style context of @widget and all descendants
 * by updating its widget path. #GtkContainers may want
 * to use this on a child when reordering it in a way that a different
 * style might apply to it. See also gtk_container_get_path_for_child().
 *
 * Since: 3.0
 */
void
gtk_widget_reset_style (GtkWidget *widget)
{
  gtk_widget_reset_rc_styles (widget);
}
