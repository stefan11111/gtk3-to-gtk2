#ifndef __GTK_COLOR_CHOOSER_H__
#define __GTK_COLOR_CHOOSER_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkwidget.h>
#include <gtk/gtkcolorsel.h>

G_BEGIN_DECLS

#define GTK_TYPE_COLOR_CHOOSER                  (gtk_color_chooser_get_type ())
#define GTK_COLOR_CHOOSER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COLOR_CHOOSER, GtkColorChooser))
#define GTK_IS_COLOR_CHOOSER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COLOR_CHOOSER))

#if 0
#define GTK_COLOR_CHOOSER_GET_IFACE(inst)       (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GTK_TYPE_COLOR_CHOOSER, GtkColorChooserInterface))
#else
#define GTK_COLOR_CHOOSER_GET_IFACE(inst)        GTK_COLOR_SELECTION_GET_CLASS(inst)
#endif

typedef GtkColorSelection      GtkColorChooser;
typedef GtkColorSelectionClass GtkColorChooserInterface;

GType    gtk_color_chooser_get_type        (void) G_GNUC_CONST;


void     gtk_color_chooser_get_rgba       (GtkColorChooser *chooser,
                                           GdkRGBA         *color);

void     gtk_color_chooser_set_rgba       (GtkColorChooser *chooser,
                                           const GdkRGBA   *color);

gboolean gtk_color_chooser_get_use_alpha  (GtkColorChooser *chooser);


void     gtk_color_chooser_set_use_alpha  (GtkColorChooser *chooser,
                                           gboolean         use_alpha);


void     gtk_color_chooser_add_palette    (GtkColorChooser *chooser,
                                           GtkOrientation   orientation,
                                           gint             colors_per_line,
                                           gint             n_colors,
                                           GdkRGBA         *colors);

G_END_DECLS

#endif /* __GTK_COLOR_CHOOSER_H__ */
