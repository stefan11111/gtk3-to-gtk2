#ifndef __GTK_COLOR_CHOOSER_DIALOG_H__
#define __GTK_COLOR_CHOOSER_DIALOG_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkdialog.h>
#include <gtk/gtkcolorseldialog.h>

G_BEGIN_DECLS

#define GTK_TYPE_COLOR_CHOOSER_DIALOG              (gtk_color_chooser_dialog_get_type ())
#define GTK_COLOR_CHOOSER_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COLOR_CHOOSER_DIALOG, GtkColorChooserDialog))
#define GTK_COLOR_CHOOSER_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_COLOR_CHOOSER_DIALOG, GtkColorChooserDialogClass))
#define GTK_IS_COLOR_CHOOSER_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COLOR_CHOOSER_DIALOG))
#define GTK_IS_COLOR_CHOOSER_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COLOR_CHOOSER_DIALOG))
#define GTK_COLOR_CHOOSER_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_COLOR_CHOOSER_DIALOG, GtkColorChooserDialogClass))

typedef GtkColorSelectionDialog        GtkColorChooserDialog;
typedef GtkColorSelectionDialogClass   GtkColorChooserDialogClass;

GType       gtk_color_chooser_dialog_get_type (void) G_GNUC_CONST;


GtkWidget * gtk_color_chooser_dialog_new      (const gchar *title,
                                               GtkWindow   *parent);

G_END_DECLS

#endif /* __GTK_COLOR_CHOOSER_DIALOG_H__ */

