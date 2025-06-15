#include <gtk/gtkwidget.h>
#include <gtk/gtkmenu.h>
#include <gtk/gtkactionmuxer.h>
#include <gtk/gtkwindow.h>
#include <gtk/gtkapplication.h>
#include <gtk/gtkpopover.h>
#include <gtk/gtkprivate.h>
#include <gtk/gtkmain.h>

#include <gdk/gdkcairo.h>

#include <cairo-gobject.h>

#include "gtkapplicationprivate.h"
#include "gtkwidgetprivate.h"
#include "gtkenumsprivate.h"
#include "gtkmarshalers.h"
#include "gtkintl.h"

#include "gdkregionprivate.h"
#include "gdkwindowinternal.h"
#include "gdkrgbaprivate.h"

#include "gtkglobals.h"

enum {
  DRAW,
  LAST_EXTRA_SIGNAL
};

static GQuark           quark_action_muxer = 0;

static guint            widget_extra_signals[LAST_EXTRA_SIGNAL] = { 0 };

static const cairo_user_data_key_t mark_for_draw_key;

static inline gboolean
gtk_cairo_is_marked_for_draw (cairo_t *cr)
{
  return cairo_get_user_data (cr, &mark_for_draw_key) != NULL;
}

static inline void
gtk_cairo_set_marked_for_draw (cairo_t  *cr,
                               gboolean  marked)
{
  cairo_set_user_data (cr, &mark_for_draw_key, GINT_TO_POINTER (marked), NULL);
}

/**
 * gtk_widget_set_opacity:
 * @widget: a #GtkWidget
 * @opacity: desired opacity, between 0 and 1
 *
 * Request the @widget to be rendered partially transparent,
 * with opacity 0 being fully transparent and 1 fully opaque. (Opacity values
 * are clamped to the [0,1] range.).
 * This works on both toplevel widget, and child widgets, although there
 * are some limitations:
 *
 * For toplevel widgets this depends on the capabilities of the windowing
 * system. On X11 this has any effect only on X screens with a compositing manager
 * running. See gtk_widget_is_composited(). On Windows it should work
 * always, although setting a window’s opacity after the window has been
 * shown causes it to flicker once on Windows.
 *
 * For child widgets it doesn’t work if any affected widget has a native window, or
 * disables double buffering.
 *
 * Since: 3.8
 **/
void
gtk_widget_set_opacity (GtkWidget *widget,
                        gdouble    opacity)
{
  GdkWindow *window = gtk_widget_get_window (widget);
  gdk_window_set_opacity (window, opacity);
}

/**
 * gtk_widget_get_opacity:
 * @widget: a #GtkWidget
 *
 * Fetches the requested opacity for this widget.
 * See gtk_widget_set_opacity().
 *
 * Returns: the requested opacity for this widget.
 *
 * Since: 3.8
 **/
gdouble
gtk_widget_get_opacity (GtkWidget *widget)
{
  /* could be better implemented,
     but that would require backend-speciffic code,
     which I don't want to write for now */
  GtkWindow *window = gtk_widget_get_tooltip_window (widget);
  return gtk_window_get_opacity (window);
}

/**
 * gtk_widget_shape_combine_region:
 * @widget: a #GtkWidget
 * @region: (allow-none): shape to be added, or %NULL to remove an existing shape
 *
 * Sets a shape for this widget’s GDK window. This allows for
 * transparent windows etc., see gdk_window_shape_combine_region()
 * for more information.
 *
 * Since: 3.0
 **/
void
gtk_widget_shape_combine_region (GtkWidget *widget,
                                 cairo_region_t *region)
{
  GdkWindow *window;
  GdkRegion *gdk_region;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (region != NULL);

  window = gtk_widget_get_window (widget);
  gdk_region = gdk_region_new ();
  GdkRegion_from_cairo_region (gdk_region, region);
  gdk_window_shape_combine_region (window, gdk_region, 0, 0);
  /* do not use gdk_region_destroy, and it holds the same data as region */
  /* use plain free */
  free (gdk_region);
}

/**
 * gtk_widget_input_shape_combine_region:
 * @widget: a #GtkWidget
 * @region: (allow-none): shape to be added, or %NULL to remove an existing shape
 *
 * Sets an input shape for this widget’s GDK window. This allows for
 * windows which react to mouse click in a nonrectangular region, see
 * gdk_window_input_shape_combine_region() for more information.
 *
 * Since: 3.0
 **/
void
gtk_widget_input_shape_combine_region (GtkWidget      *widget,
                                       cairo_region_t *region)
{
  gtk_widget_shape_combine_region (widget, region);
}

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
 * gtk_widget_get_allocated_baseline:
 * @widget: the widget to query
 *
 * Returns the baseline that has currently been allocated to @widget.
 * This function is intended to be used when implementing handlers
 * for the #GtkWidget::draw function, and when allocating child
 * widgets in #GtkWidget::size_allocate.
 *
 * Returns: the baseline of the @widget, or -1 if none
 *
 * Since: 3.10
 **/
int
gtk_widget_get_allocated_baseline (GtkWidget *widget)
{
  return -1;
}

/**
 * gtk_widget_get_allocated_size:
 * @widget: a #GtkWidget
 * @allocation: (out): a pointer to a #GtkAllocation to copy to
 * @baseline: (out) (allow-none): a pointer to an integer to copy to
 *
 * Retrieves the widget’s allocated size.
 *
 * This function returns the last values passed to
 * gtk_widget_size_allocate_with_baseline(). The value differs from
 * the size returned in gtk_widget_get_allocation() in that functions
 * like gtk_widget_set_halign() can adjust the allocation, but not
 * the value returned by this function.
 *
 * If a widget is not visible, its allocated size is 0.
 *
 * Since: 3.20
 */
void
gtk_widget_get_allocated_size (GtkWidget     *widget,
                               GtkAllocation *allocation,
                               int           *baseline)
{
  gtk_widget_get_allocation (widget, allocation);

  if (baseline)
    *baseline = -1;
}

/**
 * gtk_widget_size_allocate_with_baseline:
 * @widget: a #GtkWidget
 * @allocation: position and size to be allocated to @widget
 * @baseline: The baseline of the child, or -1
 *
 * This function is only used by #GtkContainer subclasses, to assign a size,
 * position and (optionally) baseline to their child widgets.
 *
 * In this function, the allocation and baseline may be adjusted. It
 * will be forced to a 1x1 minimum size, and the
 * adjust_size_allocation virtual and adjust_baseline_allocation
 * methods on the child will be used to adjust the allocation and
 * baseline. Standard adjustments include removing the widget's
 * margins, and applying the widget’s #GtkWidget:halign and
 * #GtkWidget:valign properties.
 *
 * If the child widget does not have a valign of %GTK_ALIGN_BASELINE the
 * baseline argument is ignored and -1 is used instead.
 *
 * Since: 3.10
 **/
void
gtk_widget_size_allocate_with_baseline (GtkWidget     *widget,
                                        GtkAllocation *allocation,
                                        gint           baseline)
{
  gtk_widget_size_allocate (widget, allocation);
}

/**
 * gtk_widget_in_destruction:
 * @widget: a #GtkWidget
 *
 * Returns whether the widget is currently being destroyed.
 * This information can sometimes be used to avoid doing
 * unnecessary work.
 *
 * Returns: %TRUE if @widget is being destroyed
 */
gboolean
gtk_widget_in_destruction (GtkWidget *widget)
{
  /* Not Implemented */
  return FALSE;
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
gtk_widget_override_color (GtkWidget     *widget,
                                      GtkStateFlags  state,
                                      const GdkRGBA *color)
{
  GdkColor c = GdkColor_from_GdkRGBA (color);
  gtk_widget_modify_base (widget, GTK_STATE_NORMAL, &c);
}

void
gtk_widget_override_background_color (GtkWidget     *widget,
                                      GtkStateFlags  state,
                                      const GdkRGBA *color)
{
  GdkColor c = GdkColor_from_GdkRGBA (color);
  gtk_widget_modify_bg (widget, GTK_STATE_NORMAL, &c);
}

void
gtk_widget_override_font (GtkWidget                  *widget,
                          const PangoFontDescription *font_desc)
{
  gtk_widget_modify_font (widget, (PangoFontDescription *)font_desc);
}

void
gtk_widget_override_symbolic_color (GtkWidget     *widget,
                                    const gchar   *name,
                                    const GdkRGBA *color)
{
  /* Not Implemented */
}

void
gtk_widget_override_cursor (GtkWidget     *widget,
                            const GdkRGBA *cursor,
                            const GdkRGBA *secondary_cursor)
{
  GdkColor c1 = GdkColor_from_GdkRGBA (cursor);
  GdkColor c2 = GdkColor_from_GdkRGBA (secondary_cursor);

  gtk_widget_modify_cursor (widget, &c1, &c2);
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
  /* Not Implemented */
  return 0;
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
  /* Not Implemented */
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
  /* Not Implemented */
  return 0;
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
  /* Not Implemented */
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
  /* Not Implemented */
  return 0;
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
  /* Not Implemented */
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
  /* Not Implemented */
  return 0;
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
  /* Not Implemented */
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
  else if (GTK_IS_POPOVER (widget))
    parent = gtk_popover_get_relative_to (GTK_POPOVER (widget));
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

/**
 * gtk_widget_set_state_flags:
 * @widget: a #GtkWidget
 * @flags: State flags to turn on
 * @clear: Whether to clear state before turning on @flags
 *
 * This function is for use in widget implementations. Turns on flag
 * values in the current widget state (insensitive, prelighted, etc.).
 *
 * This function accepts the values %GTK_STATE_FLAG_DIR_LTR and
 * %GTK_STATE_FLAG_DIR_RTL but ignores them. If you want to set the widget's
 * direction, use gtk_widget_set_direction().
 *
 * It is worth mentioning that any other state than %GTK_STATE_FLAG_INSENSITIVE,
 * will be propagated down to all non-internal children if @widget is a
 * #GtkContainer, while %GTK_STATE_FLAG_INSENSITIVE itself will be propagated
 * down to all #GtkContainer children by different means than turning on the
 * state flag down the hierarchy, both gtk_widget_get_state_flags() and
 * gtk_widget_is_sensitive() will make use of these.
 *
 * Since: 3.0
 **/
void
gtk_widget_set_state_flags (GtkWidget     *widget,
                            GtkStateFlags  flags,
                            gboolean       clear)
{
#define ALLOWED_FLAGS (~(GTK_STATE_FLAG_DIR_LTR | GTK_STATE_FLAG_DIR_RTL))

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (flags < (1 << GTK_STATE_FLAGS_BITS));

  if (clear) {
    gtk_widget_set_state (widget, GtkStateType_from_GtkStateFlags (flags));
  }
  else {
    GtkStateFlags prev_state_flags;
    prev_state_flags = GtkStateFlags_from_GtkStateType (gtk_widget_get_state (widget));
    gtk_widget_set_state (widget, GtkStateType_from_GtkStateFlags (prev_state_flags & flags & ALLOWED_FLAGS));
  }

#undef ALLOWED_FLAGS
}

/**
 * gtk_widget_unset_state_flags:
 * @widget: a #GtkWidget
 * @flags: State flags to turn off
 *
 * This function is for use in widget implementations. Turns off flag
 * values for the current widget state (insensitive, prelighted, etc.).
 * See gtk_widget_set_state_flags().
 *
 * Since: 3.0
 **/
void
gtk_widget_unset_state_flags (GtkWidget     *widget,
                              GtkStateFlags  flags)
{
  gtk_widget_set_state_flags (widget, (~flags) & ((1 << GTK_STATE_FLAGS_BITS) - 1), FALSE);
}

/**
 * gtk_widget_get_state_flags:
 * @widget: a #GtkWidget
 *
 * Returns the widget state as a flag set. It is worth mentioning
 * that the effective %GTK_STATE_FLAG_INSENSITIVE state will be
 * returned, that is, also based on parent insensitivity, even if
 * @widget itself is sensitive.
 *
 * Also note that if you are looking for a way to obtain the
 * #GtkStateFlags to pass to a #GtkStyleContext method, you
 * should look at gtk_style_context_get_state().
 *
 * Returns: The state flags for widget
 *
 * Since: 3.0
 **/
GtkStateFlags
gtk_widget_get_state_flags (GtkWidget *widget)
{
  return GtkStateFlags_from_GtkStateType (gtk_widget_get_state (widget));
}

/**
 * gtk_widget_set_focus_on_click:
 * @widget: a #GtkWidget
 * @focus_on_click: whether the widget should grab focus when clicked with the mouse
 *
 * Sets whether the widget should grab focus when it is clicked with the mouse.
 * Making mouse clicks not grab focus is useful in places like toolbars where
 * you don’t want the keyboard focus removed from the main area of the
 * application.
 *
 * Since: 3.20
 **/
void
gtk_widget_set_focus_on_click (GtkWidget *widget,
                               gboolean   focus_on_click)
{
  /* Not Implemented */
}

/**
 * gtk_widget_get_focus_on_click:
 * @widget: a #GtkWidget
 *
 * Returns whether the widget should grab focus when it is clicked with the mouse.
 * See gtk_widget_set_focus_on_click().
 *
 * Returns: %TRUE if the widget should grab focus when it is clicked with
 *               the mouse.
 *
 * Since: 3.20
 **/
gboolean
gtk_widget_get_focus_on_click (GtkWidget *widget)
{
  /* Not Implemented */
  return FALSE;
}

/**
 * gtk_widget_is_visible:
 * @widget: a #GtkWidget
 *
 * Determines whether the widget and all its parents are marked as
 * visible.
 *
 * This function does not check if the widget is obscured in any way.
 *
 * See also gtk_widget_get_visible() and gtk_widget_set_visible()
 *
 * Returns: %TRUE if the widget and all its parents are visible
 *
 * Since: 3.8
 **/
gboolean
gtk_widget_is_visible (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), FALSE);

  while (widget)
    {
      if (!gtk_widget_get_visible (widget))
        return FALSE;

      widget = gtk_widget_get_parent (widget);
    }

  return TRUE;
}

/**
 * gtk_widget_queue_allocate:
 * @widget: a #GtkWidget
 *
 * This function is only for use in widget implementations.
 *
 * Flags the widget for a rerun of the GtkWidgetClass::size_allocate
 * function. Use this function instead of gtk_widget_queue_resize()
 * when the @widget's size request didn't change but it wants to
 * reposition its contents.
 *
 * An example user of this function is gtk_widget_set_halign().
 *
 * Since: 3.20
 */
void
gtk_widget_queue_allocate (GtkWidget *widget)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  GTK_PRIVATE_SET_FLAG (widget, GTK_ALLOC_NEEDED);
}

/**
 * gtk_widget_register_window:
 * @widget: a #GtkWidget
 * @window: a #GdkWindow
 *
 * Registers a #GdkWindow with the widget and sets it up so that
 * the widget receives events for it. Call gtk_widget_unregister_window()
 * when destroying the window.
 *
 * Before 3.8 you needed to call gdk_window_set_user_data() directly to set
 * this up. This is now deprecated and you should use gtk_widget_register_window()
 * instead. Old code will keep working as is, although some new features like
 * transparency might not work perfectly.
 *
 * Since: 3.8
 */
void
gtk_widget_register_window (GtkWidget    *widget,
                            GdkWindow    *window)
{
  gpointer user_data;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (GDK_IS_WINDOW (window));

  gdk_window_get_user_data (window, &user_data);
  g_assert (user_data == NULL);

  gdk_window_set_user_data (window, widget);
}

/**
 * gtk_widget_unregister_window:
 * @widget: a #GtkWidget
 * @window: a #GdkWindow
 *
 * Unregisters a #GdkWindow from the widget that was previously set up with
 * gtk_widget_register_window(). You need to call this when the window is
 * no longer used by the widget, such as when you destroy it.
 *
 * Since: 3.8
 */
void
gtk_widget_unregister_window (GtkWidget    *widget,
                              GdkWindow    *window)
{
  gpointer user_data;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (GDK_IS_WINDOW (window));

  gdk_window_get_user_data (window, &user_data);
  g_assert (user_data == widget);

  gdk_window_set_user_data (window, NULL);
}

/**
 * gtk_widget_add_tick_callback:
 * @widget: a #GtkWidget
 * @callback: function to call for updating animations
 * @user_data: data to pass to @callback
 * @notify: function to call to free @user_data when the callback is removed.
 *
 * Queues an animation frame update and adds a callback to be called
 * before each frame. Until the tick callback is removed, it will be
 * called frequently (usually at the frame rate of the output device
 * or as quickly as the application can be repainted, whichever is
 * slower). For this reason, is most suitable for handling graphics
 * that change every frame or every few frames. The tick callback does
 * not automatically imply a relayout or repaint. If you want a
 * repaint or relayout, and aren’t changing widget properties that
 * would trigger that (for example, changing the text of a #GtkLabel),
 * then you will have to call gtk_widget_queue_resize() or
 * gtk_widget_queue_draw_area() yourself.
 *
 * gdk_frame_clock_get_frame_time() should generally be used for timing
 * continuous animations and
 * gdk_frame_timings_get_predicted_presentation_time() if you are
 * trying to display isolated frames at particular times.
 *
 * This is a more convenient alternative to connecting directly to the
 * #GdkFrameClock::update signal of #GdkFrameClock, since you don't
 * have to worry about when a #GdkFrameClock is assigned to a widget.
 *
 * Returns: an id for the connection of this callback. Remove the callback
 *     by passing it to gtk_widget_remove_tick_callback()
 *
 * Since: 3.8
 */
guint
gtk_widget_add_tick_callback (GtkWidget       *widget,
                              GtkTickCallback  callback,
                              gpointer         user_data,
                              GDestroyNotify   notify)
{
  /* Not Implemented */
  return 0;
}

/**
 * gtk_widget_remove_tick_callback:
 * @widget: a #GtkWidget
 * @id: an id returned by gtk_widget_add_tick_callback()
 *
 * Removes a tick callback previously registered with
 * gtk_widget_add_tick_callback().
 *
 * Since: 3.8
 */
void
gtk_widget_remove_tick_callback (GtkWidget *widget,
                                 guint      id)
{
  /* Not Implemented */
}

gboolean
gtk_widget_has_tick_callback (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_draw_internal (GtkWidget *widget,
                          cairo_t   *cr,
                          gboolean   clip_to_size)
{
  if (!gtk_widget_is_drawable (widget))
    return;

#if 0 /* Nothing to clip */
  if (clip_to_size)
    {
      cairo_rectangle (cr,
                       widget->priv->clip.x - widget->priv->allocation.x,
                       widget->priv->clip.y - widget->priv->allocation.y,
                       widget->priv->clip.width,
                       widget->priv->clip.height);
      cairo_clip (cr);
    }
#endif

  if (gdk_cairo_get_clip_rectangle (cr, NULL))
    {
      GdkWindow *event_window = NULL;
      gboolean result;

      /* If this was a cairo_t passed via gtk_widget_draw() then we don't
       * require a window; otherwise we check for the window associated
       * to the drawing context and mark it using the clip region of the
       * Cairo context.
       */
#if 0 /* TODO: remove when gdk_window_mark_paint_from_clip is implemented */
      if (!gtk_cairo_is_marked_for_draw (cr))
        {
          GdkDrawingContext *context = gdk_cairo_get_drawing_context (cr);

          if (context != NULL)
            {
              event_window = gdk_drawing_context_get_window (context);
              if (event_window != NULL)
                gdk_window_mark_paint_from_clip (event_window, cr);
            }
        }
#endif

#ifdef G_ENABLE_CONSISTENCY_CHECKS
      if (_gtk_widget_get_alloc_needed (widget))
        g_warning ("%s %p is drawn without a current allocation. This should not happen.", G_OBJECT_TYPE_NAME (widget), widget);
#endif

      if (g_signal_has_handler_pending (widget, widget_extra_signals[DRAW], 0, FALSE))
        {
          g_signal_emit (widget, widget_extra_signals[DRAW],
                         0, cr,
                         &result);
        }
      else
        if (GTK_WIDGET_GET_CLASS (widget)->draw)
        {
          cairo_save (cr);
          GTK_WIDGET_GET_CLASS (widget)->draw (widget, cr);
          cairo_restore (cr);
        }

#ifdef G_ENABLE_DEBUG
      if (GTK_DISPLAY_DEBUG_CHECK (gtk_widget_get_display (widget), BASELINES))
        {
          gint baseline = gtk_widget_get_allocated_baseline (widget);
          gint width = gtk_widget_get_allocated_width (widget);

          if (baseline != -1)
            {
              cairo_save (cr);
              cairo_new_path (cr);
              cairo_move_to (cr, 0, baseline+0.5);
              cairo_line_to (cr, width, baseline+0.5);
              cairo_set_line_width (cr, 1.0);
              cairo_set_source_rgba (cr, 1.0, 0, 0, 0.25);
              cairo_stroke (cr);
              cairo_restore (cr);
            }
        }
      if (widget->priv->highlight_resize)
        {
          GtkAllocation alloc;
          gtk_widget_get_allocation (widget, &alloc);

          cairo_rectangle (cr, 0, 0, alloc.width, alloc.height);
          cairo_set_source_rgba (cr, 1, 0, 0, 0.2);
          cairo_fill (cr);

          gtk_widget_queue_draw (widget);

          widget->priv->highlight_resize = FALSE;
        }
#endif

      if (cairo_status (cr) &&
          event_window != NULL)
        {
          /* We check the event so we only warn about internal GTK+ calls.
           * Errors might come from PDF streams having write failures and
           * we don't want to spam stderr in that case.
           * We do want to catch errors from
           */
          g_warning ("drawing failure for widget '%s': %s",
                     G_OBJECT_TYPE_NAME (widget),
                     cairo_status_to_string (cairo_status (cr)));
        }
    }
}

/**
 * gtk_cairo_should_draw_window:
 * @cr: a cairo context
 * @window: the window to check. @window may not be an input-only
 *          window.
 *
 * This function is supposed to be called in #GtkWidget::draw
 * implementations for widgets that support multiple windows.
 * @cr must be untransformed from invoking of the draw function.
 * This function will return %TRUE if the contents of the given
 * @window are supposed to be drawn and %FALSE otherwise. Note
 * that when the drawing was not initiated by the windowing
 * system this function will return %TRUE for all windows, so
 * you need to draw the bottommost window first. Also, do not
 * use “else if” statements to check which window should be drawn.
 *
 * Returns: %TRUE if @window should be drawn
 *
 * Since: 3.0
 */
gboolean
gtk_cairo_should_draw_window (cairo_t   *cr,
                              GdkWindow *window)
{
  GdkDrawingContext *context;
  GdkWindow *tmp;

  g_return_val_if_fail (cr != NULL, FALSE);
  g_return_val_if_fail (GDK_IS_WINDOW (window), FALSE);

  if (gtk_cairo_is_marked_for_draw (cr))
    return TRUE;

  context = gdk_cairo_get_drawing_context (cr);
  if (context == NULL)
    return TRUE;

  tmp = gdk_drawing_context_get_window (context);
  if (tmp == NULL)
    return TRUE;

  while (!gdk_window_has_native (window))
    window = gdk_window_get_parent (window);

  return tmp == window;
}

/* Returns TRUE if a translation should be done */
static gboolean
_gtk_widget_get_translation_to_window (GtkWidget      *widget,
                                       GdkWindow      *window,
                                       int            *x,
                                       int            *y)
{
  GdkWindow *w, *widget_window;

  if (!gtk_widget_get_has_window (widget))
    {
      GtkAllocation allocation;
      gtk_widget_get_allocation (widget, &allocation);
      *x = -allocation.x;
      *y = -allocation.y;
    }
  else
    {
      *x = 0;
      *y = 0;
    }

  widget_window = gtk_widget_get_window (widget);

  for (w = window; w && w != widget_window; w = gdk_window_get_parent (w))
    {
      int wx, wy;
      gdk_window_get_position (w, &wx, &wy);
      *x += wx;
      *y += wy;
    }

  if (w == NULL)
    {
      *x = 0;
      *y = 0;
      return FALSE;
    }

  return TRUE;
}

/**
 * gtk_cairo_transform_to_window:
 * @cr: the cairo context to transform
 * @widget: the widget the context is currently centered for
 * @window: the window to transform the context to
 *
 * Transforms the given cairo context @cr that from @widget-relative
 * coordinates to @window-relative coordinates.
 * If the @widget’s window is not an ancestor of @window, no
 * modification will be applied.
 *
 * This is the inverse to the transformation GTK applies when
 * preparing an expose event to be emitted with the #GtkWidget::draw
 * signal. It is intended to help porting multiwindow widgets from
 * GTK+ 2 to the rendering architecture of GTK+ 3.
 *
 * Since: 3.0
 **/
void
gtk_cairo_transform_to_window (cairo_t   *cr,
                               GtkWidget *widget,
                               GdkWindow *window)
{
  int x, y;

  g_return_if_fail (cr != NULL);
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (GDK_IS_WINDOW (window));

  if (_gtk_widget_get_translation_to_window (widget, window, &x, &y))
    cairo_translate (cr, x, y);
}

void
gtk_widget_render (GtkWidget            *widget,
                   GdkWindow            *window,
                   const cairo_region_t *region)
{
  GdkDrawingContext *context;
  gboolean do_clip;
  cairo_t *cr;
  int x, y;
  gboolean is_double_buffered;

  /* We take the value here, in case somebody manages to changes
   * the double_buffered value inside a ::draw call, and ends up
   * breaking everything.
   */
  is_double_buffered = gtk_widget_get_double_buffered (widget);

  if (is_double_buffered)
    {
      /* We only render double buffered on native windows */
      if (!gdk_window_has_native (window))
        return;

      context = gdk_window_begin_draw_frame (window, region);
      cr = gdk_drawing_context_get_cairo_context (context);
    }
  else
    {
      /* This is annoying, but it has to stay because Firefox
       * disables double buffering on a top-level GdkWindow,
       * which breaks the drawing context.
       *
       * Candidate for deletion in the next major API bump.
       */
      cr = gdk_cairo_create (window);
    }

  do_clip = _gtk_widget_get_translation_to_window (widget, window, &x, &y);
  cairo_translate (cr, -x, -y);

  gtk_widget_draw_internal (widget, cr, do_clip);

  if (is_double_buffered)
    gdk_window_end_draw_frame (window, context);
  else
    cairo_destroy (cr);
}

/**
 * gtk_widget_insert_action_group:
 * @widget: a #GtkWidget
 * @name: the prefix for actions in @group
 * @group: (allow-none): a #GActionGroup, or %NULL
 *
 * Inserts @group into @widget. Children of @widget that implement
 * #GtkActionable can then be associated with actions in @group by
 * setting their “action-name” to
 * @prefix.`action-name`.
 *
 * If @group is %NULL, a previously inserted group for @name is removed
 * from @widget.
 *
 * Since: 3.6
 */
void
gtk_widget_insert_action_group (GtkWidget    *widget,
                                const gchar  *name,
                                GActionGroup *group)
{
  GtkActionMuxer *muxer;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (name != NULL);

  muxer = _gtk_widget_get_action_muxer (widget, TRUE);

  if (group)
    gtk_action_muxer_insert (muxer, name, group);
  else
    gtk_action_muxer_remove (muxer, name);
}

/**
 * gtk_widget_draw:
 * @widget: the widget to draw. It must be drawable (see
 *   gtk_widget_is_drawable()) and a size must have been allocated.
 * @cr: a cairo context to draw to
 *
 * Draws @widget to @cr. The top left corner of the widget will be
 * drawn to the currently set origin point of @cr.
 *
 * You should pass a cairo context as @cr argument that is in an
 * original state. Otherwise the resulting drawing is undefined. For
 * example changing the operator using cairo_set_operator() or the
 * line width using cairo_set_line_width() might have unwanted side
 * effects.
 * You may however change the context’s transform matrix - like with
 * cairo_scale(), cairo_translate() or cairo_set_matrix() and clip
 * region with cairo_clip() prior to calling this function. Also, it
 * is fine to modify the context with cairo_save() and
 * cairo_push_group() prior to calling this function.
 *
 * Note that special-purpose widgets may contain special code for
 * rendering to the screen and might appear differently on screen
 * and when rendered using gtk_widget_draw().
 *
 * Since: 3.0
 **/

/* XXX conflicts with gtk2 XXX */
/* XXX abuses the dynamic linker to make this work XXX */
void
gtk_widget_draw (GtkWidget *widget,
                 cairo_t   *cr)
{
  if (GTK_IS_WIDGET (widget)) { /* gtk2 call */
    if (!gtk2_loaded) {
      gtk2_init ();
    }

    gtk2_gtk_widget_draw (widget, (GdkRectangle*)cr);
    return;
  }

  /* gtk3 call */
  gboolean was_marked;

  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (GTK_WIDGET_ALLOC_NEEDED (widget));
  g_return_if_fail (cr != NULL);

  cairo_save (cr);

  was_marked = gtk_cairo_is_marked_for_draw (cr);

  /* We mark the window so that gtk_cairo_should_draw_window()
   * will always return TRUE, and all GdkWindows get drawn
   */
  gtk_cairo_set_marked_for_draw (cr, TRUE);

  gtk_widget_draw_internal (widget, cr, TRUE);

  gtk_cairo_set_marked_for_draw (cr, was_marked);

  cairo_restore (cr);
}

/* We guard against the draw signal callbacks modifying the state of the
 * cairo context by surrounding it with save/restore.
 * Maybe we should also cairo_new_path() just to be sure?
 */
static void
gtk_widget_draw_marshaller (GClosure     *closure,
                            GValue       *return_value,
                            guint         n_param_values,
                            const GValue *param_values,
                            gpointer      invocation_hint,
                            gpointer      marshal_data)
{
  cairo_t *cr = g_value_get_boxed (&param_values[1]);

  cairo_save (cr);

  _gtk_marshal_BOOLEAN__BOXED (closure,
                               return_value,
                               n_param_values,
                               param_values,
                               invocation_hint,
                               marshal_data);


  cairo_restore (cr);
}


static void
gtk_widget_draw_marshallerv (GClosure     *closure,
                             GValue       *return_value,
                             gpointer      instance,
                             va_list       args,
                             gpointer      marshal_data,
                             int           n_params,
                             GType        *param_types)
{
  cairo_t *cr;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  cr = va_arg (args_copy, gpointer);

  cairo_save (cr);

  _gtk_marshal_BOOLEAN__BOXEDv (closure,
                                return_value,
                                instance,
                                args,
                                marshal_data,
                                n_params,
                                param_types);


  cairo_restore (cr);

  va_end (args_copy);
}

static gboolean
gtk_widget_real_expose_event (GtkWidget      *widget,
                              GdkEventExpose *expose)
{
  gboolean result = FALSE;
  cairo_t *cr;

  if (!gtk_widget_is_drawable (widget))
    return FALSE;

  cr = gdk_cairo_create (expose->window);
  gdk_cairo_region (cr, expose->region);
  cairo_clip (cr);

  if (!gtk_widget_get_has_window (widget))
    {
      cairo_translate (cr,
                       widget->allocation.x,
                       widget->allocation.y);
    }

  g_signal_emit (widget, widget_extra_signals[DRAW],
                 0, cr,
                 &result);

  cairo_destroy (cr);

  return result;
}

/* abuse the dynamic linker to hook into gtk_widget_class_init */
/* we do this for both gtk2 calls and gtk3 calls */
void
gtk2_gtk_widget_class_init_hook (GtkWidgetClass *klass)
{
  klass->draw = NULL;
  klass->expose_event = gtk_widget_real_expose_event;

  /**
   * GtkWidget::draw:
   * @widget: the object which received the signal
   * @cr: the cairo context to draw to
   *
   * This signal is emitted when a widget is supposed to render itself.
   * The @widget's top left corner must be painted at the origin of
   * the passed in context and be sized to the values returned by
   * gtk_widget_get_allocated_width() and
   * gtk_widget_get_allocated_height().
   *
   * Signal handlers connected to this signal can modify the cairo
   * context passed as @cr in any way they like and don't need to
   * restore it. The signal emission takes care of calling cairo_save()
   * before and cairo_restore() after invoking the handler.
   *
   * The signal handler will get a @cr with a clip region already set to the
   * widget's dirty region, i.e. to the area that needs repainting.  Complicated
   * widgets that want to avoid redrawing themselves completely can get the full
   * extents of the clip region with gdk_cairo_get_clip_rectangle(), or they can
   * get a finer-grained representation of the dirty region with
   * cairo_copy_clip_rectangle_list().
   *
   * Returns: %TRUE to stop other handlers from being invoked for the event.
   * %FALSE to propagate the event further.
   *
   * Since: 3.0
   */
  widget_extra_signals[DRAW] =
    g_signal_new (I_("draw"),
                   G_TYPE_FROM_CLASS (klass),
                   G_SIGNAL_RUN_LAST,
                   G_STRUCT_OFFSET (GtkWidgetClass, draw),
                   gtk2_gtk_boolean_handled_accumulator, NULL,
                   gtk_widget_draw_marshaller,
                   G_TYPE_BOOLEAN, 1,
                   CAIRO_GOBJECT_TYPE_CONTEXT);
  g_signal_set_va_marshaller (widget_extra_signals[DRAW], G_TYPE_FROM_CLASS (klass),
                              gtk_widget_draw_marshallerv);
}
