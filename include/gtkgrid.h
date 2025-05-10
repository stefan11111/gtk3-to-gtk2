#ifndef __INTERNAL_GTK_GRID_H__
#define __INTERNAL_GTK_GRID_H__


#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtktable.h>

#define GTK_TYPE_GRID                   (gtk_grid_get_type ())
#define GTK_GRID(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_GRID, GtkGrid))
#define GTK_GRID_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_GRID, GtkGridClass))
#define GTK_IS_GRID(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_GRID))
#define GTK_IS_GRID_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_GRID))
#define GTK_GRID_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_GRID, GtkGridClass))


typedef GtkTable              GtkGrid;
typedef GtkTableClass         GtkGridClass;

G_END_DECLS

#endif /* __INTERNAL_GTK_GRID_H__ */
