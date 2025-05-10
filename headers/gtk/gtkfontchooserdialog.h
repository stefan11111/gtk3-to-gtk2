#ifndef __GTK_FONT_CHOOSER_DIALOG_H__
#define __GTK_FONT_CHOOSER_DIALOG_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkfontsel.h>

G_BEGIN_DECLS

#define GTK_TYPE_FONT_CHOOSER_DIALOG              (gtk_font_chooser_dialog_get_type ())
#define GTK_FONT_CHOOSER_DIALOG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FONT_CHOOSER_DIALOG, GtkFontChooserDialog))
#define GTK_FONT_CHOOSER_DIALOG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_FONT_CHOOSER_DIALOG, GtkFontChooserDialogClass))
#define GTK_IS_FONT_CHOOSER_DIALOG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FONT_CHOOSER_DIALOG))
#define GTK_IS_FONT_CHOOSER_DIALOG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FONT_CHOOSER_DIALOG))
#define GTK_FONT_CHOOSER_DIALOG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_FONT_CHOOSER_DIALOG, GtkFontChooserDialogClass))

typedef GtkFontSelectionDialog              GtkFontChooserDialog;
typedef GtkFontSelectionDialogClass         GtkFontChooserDialogClass;

GType      gtk_font_chooser_dialog_get_type         (void) G_GNUC_CONST;

GtkWidget* gtk_font_chooser_dialog_new              (const gchar          *title,
                                                     GtkWindow            *parent);

G_END_DECLS

#endif /* __GTK_FONT_CHOOSER_DIALOG_H__ */
