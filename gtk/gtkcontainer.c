#include <gtk/gtkcontainer.h>
#include <gobject/gobjectnotifyqueue.c>

#define PARAM_SPEC_PARAM_ID(pspec)              ((pspec)->param_id)
#define PARAM_SPEC_SET_PARAM_ID(pspec, id)      ((pspec)->param_id = (id))


#define _gtk_widget_child_property_pool (*gtk2_gtk_widget_child_property_pool())
#define _gtk_widget_child_property_notify_context ((GObjectNotifyContext*)*gtk2_gtk_widget_child_property_notify_context())


/**
 * gtk_container_class_install_child_properties:
 * @cclass: a #GtkContainerClass
 * @n_pspecs: the length of the #GParamSpec array
 * @pspecs: (array length=n_pspecs): the #GParamSpec array defining the new
 *     child properties
 *
 * Installs child properties on a container class.
 *
 * Since: 3.18
 */
void
gtk_container_class_install_child_properties (GtkContainerClass  *cclass,
                                              guint               n_pspecs,
                                              GParamSpec        **pspecs)
{
  gint i;

  g_return_if_fail (GTK_IS_CONTAINER_CLASS (cclass));
  g_return_if_fail (n_pspecs > 1);
  g_return_if_fail (pspecs[0] == NULL);

  /* we skip the first element of the array as it would have a 0 prop_id */
  for (i = 1; i < n_pspecs; i++)
    {
      gtk_container_class_install_child_property (cclass, i, pspecs[i]);
    }
}

/* --- GtkContainer child property mechanism --- */

/**
 * gtk_container_child_notify:
 * @container: the #GtkContainer
 * @child: the child widget
 * @child_property: the name of a child property installed on
 *     the class of @container
 *
 * Emits a #GtkWidget::child-notify signal for the
 * [child property][child-properties]
 * @child_property on the child.
 *
 * This is an analogue of g_object_notify() for child properties.
 *
 * Also see gtk_widget_child_notify().
 *
 * Since: 3.2
 */
void
gtk_container_child_notify (GtkContainer *container,
                            GtkWidget    *child,
                            const gchar  *child_property)
{
  GObject *obj;
  GParamSpec *pspec;

  g_return_if_fail (GTK_IS_CONTAINER (container));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (child_property != NULL);

  obj = G_OBJECT (child);

  if (obj->ref_count == 0)
    return;

  g_object_ref (obj);

  pspec = g_param_spec_pool_lookup (_gtk_widget_child_property_pool,
                                    child_property,
                                    G_OBJECT_TYPE (container),
                                    TRUE);

  if (pspec == NULL)
    {
      g_warning ("%s: container class '%s' has no child property named '%s'",
                 G_STRLOC,
                 G_OBJECT_TYPE_NAME (container),
                 child_property);
    }
  else
    {
      GObjectNotifyQueue *nqueue;

      nqueue = g_object_notify_queue_freeze (obj, _gtk_widget_child_property_notify_context);

      g_object_notify_queue_add (obj, nqueue, pspec);
      g_object_notify_queue_thaw (obj, nqueue);
    }

  g_object_unref (obj);
}

/**
 * gtk_container_child_notify_by_pspec:
 * @container: the #GtkContainer
 * @child: the child widget
 * @pspec: the #GParamSpec of a child property instealled on
 *     the class of @container
 *
 * Emits a #GtkWidget::child-notify signal for the
 * [child property][child-properties] specified by
 * @pspec on the child.
 *
 * This is an analogue of g_object_notify_by_pspec() for child properties.
 *
 * Since: 3.18
 */
void
gtk_container_child_notify_by_pspec (GtkContainer *container,
                                     GtkWidget    *child,
                                     GParamSpec   *pspec)
{
  GObject *obj = G_OBJECT (child);
  GObjectNotifyQueue *nqueue;

  g_return_if_fail (GTK_IS_CONTAINER (container));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (G_IS_PARAM_SPEC (pspec));

  if (obj->ref_count == 0)
    return;

  g_object_ref (obj);

  nqueue = g_object_notify_queue_freeze (obj, _gtk_widget_child_property_notify_context);

  g_object_notify_queue_add (obj, nqueue, pspec);
  g_object_notify_queue_thaw (obj, nqueue);

  g_object_unref (obj);
}

void
gtk_container_class_handle_border_width (GtkContainerClass *klass)
{
  /* Not Implemented */
}

/**
 * gtk_container_get_path_for_child:
 * @container: a #GtkContainer
 * @child: a child of @container
 *
 * Returns a newly created widget path representing all the widget hierarchy
 * from the toplevel down to and including @child.
 *
 * Returns: A newly created #GtkWidgetPath
 **/
GtkWidgetPath *
gtk_container_get_path_for_child (GtkContainer *container,
                                  GtkWidget    *child)
{
  /* Not Implemented */
  static GtkWidgetPath *path;
  if (!path) {
    path = gtk_widget_path_new();
  }
  return path;
}

/* ^^^ public ^^^ */

/* vvv private vvv */

void
gtk_container_queue_resize_handler (GtkContainer *container)
{
  GtkWidget *widget;

  G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
  g_return_if_fail (GTK_IS_RESIZE_CONTAINER (container));
  G_GNUC_END_IGNORE_DEPRECATIONS;

  widget = GTK_WIDGET (container);

  if (gtk_widget_get_visible (widget) &&
      (gtk_widget_is_toplevel (widget) ||
       gtk_widget_get_realized (widget)))
    {
      switch (container->resize_mode)
        {
        case GTK_RESIZE_QUEUE:
#if 0 /* XXX needs gdkframeclock XXX */
          if (gtk_widget_needs_allocate (widget))
            gtk_container_start_idle_sizer (container);
#endif
          break;

        case GTK_RESIZE_IMMEDIATE:
          gtk_container_check_resize (container);
          break;

        case GTK_RESIZE_PARENT:
        default:
          g_assert_not_reached ();
          break;
        }
    }
}

void
_gtk_container_queue_restyle (GtkContainer *container)
{
  g_return_if_fail (GTK_IS_CONTAINER (container));

#if 0 /* XXX needs gdkframeclock XXX */
  if (container->need_resize)
    return;

  gtk_container_start_idle_sizer (container);
#endif
  container->need_resize = TRUE;
}

