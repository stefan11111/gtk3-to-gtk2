#include <gtk/gtk.h>

#include "util.h"

static GQuark           quark_action_muxer = 0;

int
gtk_widget_get_allocated_width (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.width;
}

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

GtkActionMuxer *
_gtk_widget_get_action_muxer (GtkWidget *widget,
                              gboolean   create)
{
  GtkActionMuxer *muxer;

  muxer = (GtkActionMuxer*)g_object_get_qdata (G_OBJECT (widget), quark_action_muxer);
  if (muxer)
    return muxer;

  if (create)
    {
      muxer = gtk_action_muxer_new ();
      g_object_set_qdata_full (G_OBJECT (widget),
                               quark_action_muxer,
                               muxer,
                               g_object_unref);
      _gtk_widget_update_parent_muxer (widget);

      return muxer;
    }
  else
    return _gtk_widget_get_parent_muxer (widget, FALSE);
}

GtkActionMuxer *
_gtk_widget_get_parent_muxer (GtkWidget *widget,
                              gboolean   create)
{
  GtkWidget *parent;

  if (GTK_IS_WINDOW (widget))
    return gtk_application_get_parent_muxer_for_window (GTK_WINDOW (widget));

  if (GTK_IS_MENU (widget))
    parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
/* TODO: remove when gtkpopover is implemented */
#if 0
  else if (GTK_IS_POPOVER (widget))
    parent = gtk_popover_get_relative_to (GTK_POPOVER (widget));
#endif
  else
    parent = gtk_widget_get_parent (widget);

  if (parent)
    return _gtk_widget_get_action_muxer (parent, create);

  return NULL;
}

void
_gtk_widget_update_parent_muxer (GtkWidget *widget)
{
  GtkActionMuxer *muxer;

  if (!quark_action_muxer) {
    quark_action_muxer = g_quark_from_static_string ("gtk-widget-action-muxer");
  }

  muxer = (GtkActionMuxer*)g_object_get_qdata (G_OBJECT (widget), quark_action_muxer);
  if (muxer == NULL)
    return;

  gtk_action_muxer_set_parent (muxer,
                               _gtk_widget_get_parent_muxer (widget, TRUE));
}

gboolean
gtk_widget_needs_allocate (GtkWidget *widget)
{
  /* don't touch if the widget isn't visible */
  if (!gtk_widget_get_visible (widget) || !gtk_widget_get_child_visible (widget))
    return FALSE;

  return GTK_WIDGET_ALLOC_NEEDED (widget);
}
