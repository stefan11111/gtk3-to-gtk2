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

/**
 * gtk_widget_get_clip:
 * @widget: a #GtkWidget
 * @clip: (out): a pointer to a #GtkAllocation to copy to
 *
 * Retrieves the widget’s clip area.
 *
 * The clip area is the area in which all of @widget's drawing will
 * happen. Other toolkits call it the bounding box.
 *
 * Historically, in GTK+ the clip area has been equal to the allocation
 * retrieved via gtk_widget_get_allocation().
 *
 * Since: 3.14
 */
void
gtk_widget_get_clip (GtkWidget     *widget,
                     GtkAllocation *clip)
{
  gtk_widget_get_allocation (widget, clip);
}

/**
 * gtk_widget_set_clip:
 * @widget: a #GtkWidget
 * @clip: a pointer to a #GtkAllocation to copy from
 *
 * Sets the widget’s clip.  This must not be used directly,
 * but from within a widget’s size_allocate method.
 * It must be called after gtk_widget_set_allocation() (or after chaining up
 * to the parent class), because that function resets the clip.
 *
 * The clip set should be the area that @widget draws on. If @widget is a
 * #GtkContainer, the area must contain all children's clips.
 *
 * If this function is not called by @widget during a ::size-allocate handler,
 * the clip will be set to @widget's allocation.
 *
 * Since: 3.14
 */
void
gtk_widget_set_clip (GtkWidget           *widget,
                     const GtkAllocation *clip)
{
  gtk_widget_set_allocation (widget, clip);
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
 * gtk_widget_get_margin_left:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-left property.
 *
 * Returns: The left margin of @widget
 *
 * Deprecated: 3.12: Use gtk_widget_get_margin_start() instead.
 *
 * Since: 3.0
 */
gint
gtk_widget_get_margin_left (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  GtkAllocation allocation;
  gtk_widget_get_allocation (widget, &allocation);

  return allocation.x;
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_left:
 * @widget: a #GtkWidget
 * @margin: the left margin
 *
 * Sets the left margin of @widget.
 * See the #GtkWidget:margin-left property.
 *
 * Deprecated: 3.12: Use gtk_widget_set_margin_start() instead.
 *
 * Since: 3.0
 */
void
gtk_widget_set_margin_left (GtkWidget *widget,
                            gint       margin)
{
#if 0 /* Not Implemented */
  GtkAllocation allocation;
#if 0
  GtkRequsition requisition;
  GtkWindow *window;
#endif

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  gtk_widget_get_allocation (widget, &allocation);
#if 0
  gtk_widget_size_request (widget, &requisition);
#endif

  if (allocation.x != -1) {
    allocation.width += allocation.x - margin;
#if 0
    requisition.width += allocation.x - margin;
#endif
  }
  allocation.x = margin;

  gtk_widget_size_allocate (widget, &allocation);

#if 0
  window = gtk_widget_get_window (widget);
  if (window) {
    gtk_window_set_default_size(window, requisition.width, requisition.height);
  }
#endif
#endif /* Not Implemented */
}

/**
 * gtk_widget_get_margin_right:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-right property.
 *
 * Returns: The right margin of @widget
 *
 * Deprecated: 3.12: Use gtk_widget_get_margin_end() instead.
 *
 * Since: 3.0
 */
gint
gtk_widget_get_margin_right (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  GtkAllocation allocation;
  gtk_widget_get_allocation (widget, &allocation);

  return allocation.x + allocation.width;
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_right:
 * @widget: a #GtkWidget
 * @margin: the right margin
 *
 * Sets the right margin of @widget.
 * See the #GtkWidget:margin-right property.
 *
 * Deprecated: 3.12: Use gtk_widget_set_margin_end() instead.
 *
 * Since: 3.0
 */
void
gtk_widget_set_margin_right (GtkWidget *widget,
                             gint       margin)
{
#if 0 /* Not Implemented */
  GtkAllocation allocation;
#if 0
  GtkRequsition requisition;
  GtkWindow *window;
#endif

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  gtk_widget_get_allocation (widget, &allocation);
#if 0
  gtk_widget_size_request (widget, &requisition);
#endif

#if 0
  requisition.width += (margin - (allocation.x == -1 ? 0 : allocation.x)) - allocation.width;
#endif
  allocation.width = margin - (allocation.x == -1 ? 0 : allocation.x);

  gtk_widget_size_allocate (widget, &allocation);

#if 0
  window = gtk_widget_get_window (widget);
  if (window) {
    gtk_window_set_default_size(window, requisition.width, requisition.height);
  }
#endif
#endif /* Not Implemented */
}

/**
 * gtk_widget_get_margin_start:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-start property.
 *
 * Returns: The start margin of @widget
 *
 * Since: 3.12
 */
gint
gtk_widget_get_margin_start (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  return (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_get_margin_right (widget) :
                                                                   gtk_widget_get_margin_left (widget);
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_start:
 * @widget: a #GtkWidget
 * @margin: the start margin
 *
 * Sets the start margin of @widget.
 * See the #GtkWidget:margin-start property.
 *
 * Since: 3.12
 */
void
gtk_widget_set_margin_start (GtkWidget *widget,
                             gint       margin)
{
#if 0 /* Not Implemented */
  (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_set_margin_right (widget, margin) :
                                                            gtk_widget_set_margin_left (widget, margin);
#endif /* Not Implemented */
}

/**
 * gtk_widget_get_margin_end:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-end property.
 *
 * Returns: The end margin of @widget
 *
 * Since: 3.12
 */
gint
gtk_widget_get_margin_end (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  return (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_get_margin_left (widget) :
                                                                   gtk_widget_get_margin_right (widget);
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_end:
 * @widget: a #GtkWidget
 * @margin: the end margin
 *
 * Sets the end margin of @widget.
 * See the #GtkWidget:margin-end property.
 *
 * Since: 3.12
 */
void
gtk_widget_set_margin_end (GtkWidget *widget,
                           gint       margin)
{
#if 0 /* Not Implemented */
  (gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ? gtk_widget_set_margin_left (widget, margin) :
                                                            gtk_widget_set_margin_right (widget, margin);
#endif /* Not Implemented */
}

/**
 * gtk_widget_get_margin_top:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-top property.
 *
 * Returns: The top margin of @widget
 *
 * Since: 3.0
 */
gint
gtk_widget_get_margin_top (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  GtkAllocation allocation;
  gtk_widget_get_allocation (widget, &allocation);

  return allocation.y;
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_top:
 * @widget: a #GtkWidget
 * @margin: the top margin
 *
 * Sets the top margin of @widget.
 * See the #GtkWidget:margin-top property.
 *
 * Since: 3.0
 */
void
gtk_widget_set_margin_top (GtkWidget *widget,
                           gint       margin)
{
#if 0 /* Not Implemented */
  GtkAllocation allocation;
#if 0
  GtkRequsition requisition;
  GtkWindow *window;
#endif

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  gtk_widget_get_allocation (widget, &allocation);
#if 0
  gtk_widget_size_request (widget, &requisition);
#endif

  if (allocation.y != -1) {
    allocation.height += allocation.y - margin;
#if 0
    requisition.height += allocation.y - margin;
#endif
  }
  allocation.y = margin;

  gtk_widget_size_allocate (widget, &allocation);

#if 0
  window = gtk_widget_get_window (widget);
  if (window) {
    gtk_window_set_default_size(window, requisition.width, requisition.height);
  }
#endif
#endif /* Not Implemented */
}

/**
 * gtk_widget_get_margin_bottom:
 * @widget: a #GtkWidget
 *
 * Gets the value of the #GtkWidget:margin-bottom property.
 *
 * Returns: The bottom margin of @widget
 *
 * Since: 3.0
 */
gint
gtk_widget_get_margin_bottom (GtkWidget *widget)
{
#if 0 /* Not Implemented */
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  GtkAllocation allocation;
  gtk_widget_get_allocation (widget, &allocation);

  return allocation.y + allocation.height;
#else
  /* Not Implemented */
  return 0;
#endif
}

/**
 * gtk_widget_set_margin_bottom:
 * @widget: a #GtkWidget
 * @margin: the bottom margin
 *
 * Sets the bottom margin of @widget.
 * See the #GtkWidget:margin-bottom property.
 *
 * Since: 3.0
 */
void
gtk_widget_set_margin_bottom (GtkWidget *widget,
                              gint       margin)
{
#if 0 /* Not Implemented */
  GtkAllocation allocation;
#if 0
  GtkRequsition requisition;
  GtkWindow *window;
#endif

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (margin <= G_MAXINT16);

  gtk_widget_get_allocation (widget, &allocation);
#if 0
  gtk_widget_size_request (widget, &requisition);
#endif

#if 0
  requisition.height += (margin - (allocation.y == -1 ? 0 : allocation.y)) - allocation.height;
#endif
  allocation.height = margin - (allocation.y == -1 ? 0 : allocation.y);

  gtk_widget_size_allocate (widget, &allocation);

#if 0
  window = gtk_widget_get_window (widget);
  if (window) {
    gtk_window_set_default_size(window, requisition.width, requisition.height);
  }
#endif
#endif /* Not Implemented */
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

/**
 * gtk_widget_get_scale_factor:
 * @widget: a #GtkWidget
 *
 * Retrieves the internal scale factor that maps from window coordinates
 * to the actual device pixels. On traditional systems this is 1, on
 * high density outputs, it can be a higher value (typically 2).
 *
 * See gdk_window_get_scale_factor().
 *
 * Returns: the scale factor for @widget
 *
 * Since: 3.10
 */
gint
gtk_widget_get_scale_factor (GtkWidget *widget)
{
  return 1;
}
