#include <gtk/gtkimage.h>
#include <cairo-gobject.h>

#include "util.h"

#ifndef P_
#define P_(x) x
#endif

static GParamSpec *prop_surface = NULL;

enum
{
  PROP_0,
  PROP_PIXBUF,
  PROP_PIXMAP,
  PROP_IMAGE,
  PROP_MASK,
  PROP_FILE,
  PROP_STOCK,
  PROP_ICON_SET,
  PROP_ICON_SIZE,
  PROP_PIXEL_SIZE,
  PROP_PIXBUF_ANIMATION,
  PROP_ICON_NAME,
  PROP_STORAGE_TYPE,
  PROP_GICON,
  PROP_SURFACE /* from gtk3 */
};


/**
 * gtk_image_set_from_surface:
 * @image: a #GtkImage
 * @surface: (nullable): a cairo_surface_t or %NULL
 *
 * See gtk_image_new_from_surface() for details.
 *
 * Since: 3.10
 **/
void
gtk_image_set_from_surface (GtkImage       *image,
                            cairo_surface_t *surface)
{
  g_return_if_fail (GTK_IS_IMAGE (image));

  g_object_freeze_notify (G_OBJECT (image));

  if (surface)
    cairo_surface_reference (surface);

  gtk_image_clear (image);

  if (surface)
    {
      image->storage_type = GTK_IMAGE_SURFACE;

      image->data.surface.surface = surface;
    }

  if (!prop_surface) {
      prop_surface = g_param_spec_boxed ("surface",
                                         P_("Surface"),
                                         P_("A cairo_surface_t to display"),
                                         CAIRO_GOBJECT_TYPE_SURFACE,
                                         GTK_PARAM_READWRITE);
  }

  /* install this even if we already installed it */
  g_object_class_install_property (G_OBJECT_CLASS (GTK_IMAGE_GET_CLASS (image)), PROP_SURFACE, prop_surface);

  g_object_notify_by_pspec (G_OBJECT (image), prop_surface);

  g_object_thaw_notify (G_OBJECT (image));
}

/**
 * gtk_image_new_from_surface:
 * @surface: (allow-none): a #cairo_surface_t, or %NULL
 *
 * Creates a new #GtkImage displaying @surface.
 * The #GtkImage does not assume a reference to the
 * surface; you still need to unref it if you own references.
 * #GtkImage will add its own reference rather than adopting yours.
 *
 * Returns: a new #GtkImage
 *
 * Since: 3.10
 **/
GtkWidget*
gtk_image_new_from_surface (cairo_surface_t *surface)
{
  GtkImage *image;

  image = g_object_new (GTK_TYPE_IMAGE, NULL);

  gtk_image_set_from_surface (image, surface);

  return GTK_WIDGET (image);
}
