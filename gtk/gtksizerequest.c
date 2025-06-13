#include <gtk/gtkwidget.h>
#include <gtk/gtksizerequest.h>

/**
 * gtk_widget_get_request_mode:
 * @widget: a #GtkWidget instance
 *
 * Gets whether the widget prefers a height-for-width layout
 * or a width-for-height layout.
 *
 * #GtkBin widgets generally propagate the preference of
 * their child, container widgets need to request something either in
 * context of their children or in context of their allocation
 * capabilities.
 *
 * Returns: The #GtkSizeRequestMode preferred by @widget.
 *
 * Since: 3.0
 */
GtkSizeRequestMode
gtk_widget_get_request_mode (GtkWidget *widget)
{
  return GTK_SIZE_REQUEST_CONSTANT_SIZE;
}

static gint
compare_gap (gconstpointer p1,
             gconstpointer p2,
             gpointer      data)
{
  GtkRequestedSize *sizes = data;
  const guint *c1 = p1;
  const guint *c2 = p2;

  const gint d1 = MAX (sizes[*c1].natural_size -
                       sizes[*c1].minimum_size,
                       0);
  const gint d2 = MAX (sizes[*c2].natural_size -
                       sizes[*c2].minimum_size,
                       0);

  gint delta = (d2 - d1);

  if (0 == delta)
    delta = (*c2 - *c1);

  return delta;
}

/**
 * gtk_distribute_natural_allocation:
 * @extra_space: Extra space to redistribute among children after subtracting
 *               minimum sizes and any child padding from the overall allocation
 * @n_requested_sizes: Number of requests to fit into the allocation
 * @sizes: An array of structs with a client pointer and a minimum/natural size
 *         in the orientation of the allocation.
 *
 * Distributes @extra_space to child @sizes by bringing smaller
 * children up to natural size first.
 *
 * The remaining space will be added to the @minimum_size member of the
 * GtkRequestedSize struct. If all sizes reach their natural size then
 * the remaining space is returned.
 *
 * Returns: The remainder of @extra_space after redistributing space
 * to @sizes.
 */
gint
gtk_distribute_natural_allocation (gint              extra_space,
                                   guint             n_requested_sizes,
                                   GtkRequestedSize *sizes)
{
  guint *spreading;
  gint   i;

  g_return_val_if_fail (extra_space >= 0, 0);

  spreading = g_newa (guint, n_requested_sizes);

  for (i = 0; i < n_requested_sizes; i++)
    spreading[i] = i;

  /* Distribute the container's extra space c_gap. We want to assign
   * this space such that the sum of extra space assigned to children
   * (c^i_gap) is equal to c_cap. The case that there's not enough
   * space for all children to take their natural size needs some
   * attention. The goals we want to achieve are:
   *
   *   a) Maximize number of children taking their natural size.
   *   b) The allocated size of children should be a continuous
   *   function of c_gap.  That is, increasing the container size by
   *   one pixel should never make drastic changes in the distribution.
   *   c) If child i takes its natural size and child j doesn't,
   *   child j should have received at least as much gap as child i.
   *
   * The following code distributes the additional space by following
   * these rules.
   */

  /* Sort descending by gap and position. */
  g_sort_array (spreading,
                n_requested_sizes, sizeof (guint),
                compare_gap, sizes);

  /* Distribute available space.
   * This master piece of a loop was conceived by Behdad Esfahbod.
   */
  for (i = n_requested_sizes - 1; extra_space > 0 && i >= 0; --i)
    {
      /* Divide remaining space by number of remaining children.
       * Sort order and reducing remaining space by assigned space
       * ensures that space is distributed equally.
       */
      gint glue = (extra_space + i) / (i + 1);
      gint gap = sizes[(spreading[i])].natural_size
        - sizes[(spreading[i])].minimum_size;

      gint extra = MIN (glue, gap);

      sizes[spreading[i]].minimum_size += extra;

      extra_space -= extra;
    }

  return extra_space;
}

/**
 * gtk_widget_get_preferred_width:
 * @widget: a #GtkWidget instance
 * @minimum_width: (out) (allow-none): location to store the minimum width, or %NULL
 * @natural_width: (out) (allow-none): location to store the natural width, or %NULL
 *
 * Retrieves a widget’s initial minimum and natural width.
 *
 * This call is specific to height-for-width requests.
 *
 * The returned request will be modified by the
 * GtkWidgetClass::adjust_size_request virtual method and by any
 * #GtkSizeGroups that have been applied. That is, the returned request
 * is the one that should be used for layout, not necessarily the one
 * returned by the widget itself.
 *
 * Since: 3.0
 */
void
gtk_widget_get_preferred_width (GtkWidget *widget,
                                gint      *minimum_width,
                                gint      *natural_width)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (minimum_width != NULL || natural_width != NULL);

  *minimum_width = widget->requisition.width;
  *natural_width = widget->allocation.width;
}

/**
 * gtk_widget_get_preferred_height:
 * @widget: a #GtkWidget instance
 * @minimum_height: (out) (allow-none): location to store the minimum height, or %NULL
 * @natural_height: (out) (allow-none): location to store the natural height, or %NULL
 *
 * Retrieves a widget’s initial minimum and natural height.
 *
 * This call is specific to width-for-height requests.
 *
 * The returned request will be modified by the
 * GtkWidgetClass::adjust_size_request virtual method and by any
 * #GtkSizeGroups that have been applied. That is, the returned request
 * is the one that should be used for layout, not necessarily the one
 * returned by the widget itself.
 *
 * Since: 3.0
 */
void
gtk_widget_get_preferred_height (GtkWidget *widget,
                                 gint      *minimum_height,
                                 gint      *natural_height)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (minimum_height != NULL || natural_height != NULL);

  *minimum_height = widget->requisition.height;
  *natural_height = widget->allocation.height;
}

/**
 * gtk_widget_get_preferred_width_for_height:
 * @widget: a #GtkWidget instance
 * @height: the height which is available for allocation
 * @minimum_width: (out) (allow-none): location for storing the minimum width, or %NULL
 * @natural_width: (out) (allow-none): location for storing the natural width, or %NULL
 *
 * Retrieves a widget’s minimum and natural width if it would be given
 * the specified @height.
 *
 * The returned request will be modified by the
 * GtkWidgetClass::adjust_size_request virtual method and by any
 * #GtkSizeGroups that have been applied. That is, the returned request
 * is the one that should be used for layout, not necessarily the one
 * returned by the widget itself.
 *
 * Since: 3.0
 */
void
gtk_widget_get_preferred_width_for_height (GtkWidget *widget,
                                           gint       height,
                                           gint      *minimum_width,
                                           gint      *natural_width)
{
  gtk_widget_get_preferred_width (widget, minimum_width, natural_width);
}

/**
 * gtk_widget_get_preferred_height_for_width:
 * @widget: a #GtkWidget instance
 * @width: the width which is available for allocation
 * @minimum_height: (out) (allow-none): location for storing the minimum height, or %NULL
 * @natural_height: (out) (allow-none): location for storing the natural height, or %NULL
 *
 * Retrieves a widget’s minimum and natural height if it would be given
 * the specified @width.
 *
 * The returned request will be modified by the
 * GtkWidgetClass::adjust_size_request virtual method and by any
 * #GtkSizeGroups that have been applied. That is, the returned request
 * is the one that should be used for layout, not necessarily the one
 * returned by the widget itself.
 *
 * Since: 3.0
 */
void
gtk_widget_get_preferred_height_for_width (GtkWidget *widget,
                                           gint       width,
                                           gint      *minimum_height,
                                           gint      *natural_height)
{
  gtk_widget_get_preferred_height (widget, minimum_height, natural_height);
}

/**
 * gtk_widget_get_preferred_height_and_baseline_for_width:
 * @widget: a #GtkWidget instance
 * @width: the width which is available for allocation, or -1 if none
 * @minimum_height: (out) (allow-none): location for storing the minimum height, or %NULL
 * @natural_height: (out) (allow-none): location for storing the natural height, or %NULL
 * @minimum_baseline: (out) (allow-none): location for storing the baseline for the minimum height, or %NULL
 * @natural_baseline: (out) (allow-none): location for storing the baseline for the natural height, or %NULL
 *
 * Retrieves a widget’s minimum and natural height and the corresponding baselines if it would be given
 * the specified @width, or the default height if @width is -1. The baselines may be -1 which means
 * that no baseline is requested for this widget.
 *
 * The returned request will be modified by the
 * GtkWidgetClass::adjust_size_request and GtkWidgetClass::adjust_baseline_request virtual methods
 * and by any #GtkSizeGroups that have been applied. That is, the returned request
 * is the one that should be used for layout, not necessarily the one
 * returned by the widget itself.
 *
 * Since: 3.10
 */
void
gtk_widget_get_preferred_height_and_baseline_for_width (GtkWidget *widget,
                                                        gint       width,
                                                        gint      *minimum_height,
                                                        gint      *natural_height,
                                                        gint      *minimum_baseline,
                                                        gint      *natural_baseline)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (minimum_height != NULL || natural_height != NULL);

  *minimum_height = widget->requisition.height;
  *natural_height = widget->allocation.height;

  if (minimum_baseline) {
    *minimum_baseline = -1;
  }

  if (natural_baseline) {
    *natural_baseline = -1;
  }
}

/*
 * _gtk_widget_get_preferred_size_and_baseline:
 * @widget: a #GtkWidget instance
 * @minimum_size: (out) (allow-none): location for storing the minimum size, or %NULL
 * @natural_size: (out) (allow-none): location for storing the natural size, or %NULL
 *
 * Retrieves the minimum and natural size  and the corresponding baselines of a widget, taking
 * into account the widget’s preference for height-for-width management. The baselines may
 * be -1 which means that no baseline is requested for this widget.
 *
 * This is used to retrieve a suitable size by container widgets which do
 * not impose any restrictions on the child placement. It can be used
 * to deduce toplevel window and menu sizes as well as child widgets in
 * free-form containers such as GtkLayout.
 *
 * Handle with care. Note that the natural height of a height-for-width
 * widget will generally be a smaller size than the minimum height, since the required
 * height for the natural width is generally smaller than the required height for
 * the minimum width.
 */
void
_gtk_widget_get_preferred_size_and_baseline (GtkWidget      *widget,
                                             GtkRequisition *minimum_size,
                                             GtkRequisition *natural_size,
                                             gint           *minimum_baseline,
                                             gint           *natural_baseline)
{
  gint min_width, nat_width;
  gint min_height, nat_height;

  g_return_if_fail (GTK_IS_WIDGET (widget));

  if (gtk_widget_get_request_mode (widget) == GTK_SIZE_REQUEST_HEIGHT_FOR_WIDTH)
    {
      gtk_widget_get_preferred_width (widget, &min_width, &nat_width);

      if (minimum_size)
        {
          minimum_size->width = min_width;
          gtk_widget_get_preferred_height_and_baseline_for_width (widget, min_width,
                                                                  &minimum_size->height, NULL, minimum_baseline, NULL);
        }

      if (natural_size)
        {
          natural_size->width = nat_width;
          gtk_widget_get_preferred_height_and_baseline_for_width (widget, nat_width,
                                                                  NULL, &natural_size->height, NULL, natural_baseline);
        }
    }
  else /* GTK_SIZE_REQUEST_WIDTH_FOR_HEIGHT or CONSTANT_SIZE */
    {
      gtk_widget_get_preferred_height_and_baseline_for_width (widget, -1, &min_height, &nat_height, minimum_baseline, natural_baseline);

      if (minimum_size)
        {
          minimum_size->height = min_height;
          gtk_widget_get_preferred_width_for_height (widget, min_height,
                                                     &minimum_size->width, NULL);
        }

      if (natural_size)
        {
          natural_size->height = nat_height;
          gtk_widget_get_preferred_width_for_height (widget, nat_height,
                                                     NULL, &natural_size->width);
        }
    }
}

/**
 * gtk_widget_get_preferred_size:
 * @widget: a #GtkWidget instance
 * @minimum_size: (out) (allow-none): location for storing the minimum size, or %NULL
 * @natural_size: (out) (allow-none): location for storing the natural size, or %NULL
 *
 * Retrieves the minimum and natural size of a widget, taking
 * into account the widget’s preference for height-for-width management.
 *
 * This is used to retrieve a suitable size by container widgets which do
 * not impose any restrictions on the child placement. It can be used
 * to deduce toplevel window and menu sizes as well as child widgets in
 * free-form containers such as GtkLayout.
 *
 * Handle with care. Note that the natural height of a height-for-width
 * widget will generally be a smaller size than the minimum height, since the required
 * height for the natural width is generally smaller than the required height for
 * the minimum width.
 *
 * Use gtk_widget_get_preferred_height_and_baseline_for_width() if you want to support
 * baseline alignment.
 *
 * Since: 3.0
 */
void
gtk_widget_get_preferred_size (GtkWidget      *widget,
                               GtkRequisition *minimum_size,
                               GtkRequisition *natural_size)
{
  _gtk_widget_get_preferred_size_and_baseline (widget, minimum_size, natural_size,
                                               NULL, NULL);
}

void
_gtk_widget_get_preferred_size_for_size (GtkWidget      *widget,
                                         GtkOrientation  orientation,
                                         gint            size,
                                         gint           *minimum,
                                         gint           *natural,
                                         gint           *minimum_baseline,
                                         gint           *natural_baseline)
{
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (size >= -1);

  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      if (size < 0)
        gtk_widget_get_preferred_width (widget, minimum, natural);
      else
        gtk_widget_get_preferred_width_for_height (widget, size, minimum, natural);

      if (minimum_baseline)
        *minimum_baseline = -1;
      if (natural_baseline)
        *natural_baseline = -1;
    }
  else
    {
      gtk_widget_get_preferred_height_and_baseline_for_width (widget,
                                                              size,
                                                              minimum,
                                                              natural,
                                                              minimum_baseline,
                                                              natural_baseline);
    }
}

gboolean
_gtk_widget_has_baseline_support (GtkWidget *widget)
{
  /* Not Implemented */
  return FALSE;
}
