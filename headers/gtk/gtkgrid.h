#ifndef __GTK_GRID_H__
#define __GTK_GRID_H__


#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtktable.h>
#include <gtk/gtkenums.h>


#define GTK_TYPE_GRID                   (gtk_grid_get_type ())
#define GTK_GRID(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_GRID, GtkGrid))
#define GTK_GRID_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_GRID, GtkGridClass))
#define GTK_IS_GRID(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_GRID))
#define GTK_IS_GRID_CLASS(klass)        (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_GRID))
#define GTK_GRID_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_GRID, GtkGridClass))

typedef GtkTable              GtkGrid;
typedef GtkTableClass         GtkGridClass;

GType      gtk_grid_get_type               (void) G_GNUC_CONST;

GtkWidget* gtk_grid_new                    (void);

void       gtk_grid_attach                 (GtkGrid         *grid,
                                            GtkWidget       *child,
                                            gint             left,
                                            gint             top,
                                            gint             width,
                                            gint             height);

void       gtk_grid_attach_next_to         (GtkGrid         *grid,
                                            GtkWidget       *child,
                                            GtkWidget       *sibling,
                                            GtkPositionType  side,
                                            gint             width,
                                            gint             height);

GtkWidget *gtk_grid_get_child_at           (GtkGrid         *grid,
                                            gint             left,
                                            gint             top);

void       gtk_grid_insert_row             (GtkGrid         *grid,
                                            gint             position);

void       gtk_grid_insert_column          (GtkGrid         *grid,
                                            gint             position);

void       gtk_grid_remove_row             (GtkGrid         *grid,
                                            gint             position);

void       gtk_grid_remove_column          (GtkGrid         *grid,
                                            gint             position);

void       gtk_grid_insert_next_to         (GtkGrid         *grid,
                                            GtkWidget       *sibling,
                                            GtkPositionType  side);

void       gtk_grid_set_row_homogeneous    (GtkGrid         *grid,
                                            gboolean         homogeneous);

gboolean   gtk_grid_get_row_homogeneous    (GtkGrid         *grid);

void       gtk_grid_set_row_spacing        (GtkGrid         *grid,
                                            guint            spacing);

guint      gtk_grid_get_row_spacing        (GtkGrid         *grid);

void       gtk_grid_set_column_homogeneous (GtkGrid         *grid,
                                            gboolean         homogeneous);

gboolean   gtk_grid_get_column_homogeneous (GtkGrid         *grid);

void       gtk_grid_set_column_spacing     (GtkGrid         *grid,
                                            guint            spacing);

guint      gtk_grid_get_column_spacing     (GtkGrid         *grid);

void       gtk_grid_set_row_baseline_position (GtkGrid      *grid,
                                               gint          row,
                                               GtkBaselinePosition pos);

GtkBaselinePosition gtk_grid_get_row_baseline_position (GtkGrid      *grid,
                                                        gint          row);

void       gtk_grid_set_baseline_row       (GtkGrid         *grid,
                                            gint             row);

gint       gtk_grid_get_baseline_row       (GtkGrid         *grid);


G_END_DECLS

#endif /* __GTK_GRID_H__ */
