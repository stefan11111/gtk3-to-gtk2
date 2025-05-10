#ifndef __INTERNAL_GTK_FONT_CHOOSER_H__
#define __INTERNAL_GTK_FONT_CHOOSER_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef GtkFontSelection GtkFontChooser;

#define GTK_TYPE_FONT_CHOOSER                   (gtk_font_chooser_get_type ())
#define GTK_FONT_CHOOSER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FONT_CHOOSER, GtkFontChooser))
#define GTK_IS_FONT_CHOOSER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FONT_CHOOSER))
#define GTK_FONT_CHOOSER_GET_IFACE(inst)        (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GTK_TYPE_FONT_CHOOSER, GtkFontChooserIface))

GType            gtk_font_chooser_get_type                 (void) G_GNUC_CONST;

gchar*           gtk_font_chooser_get_font                 (GtkFontChooser   *fontchooser);

void             gtk_font_chooser_set_font                 (GtkFontChooser   *fontchooser,
                                                            const gchar      *fontname);
G_END_DECLS

#endif /* __INTERNAL_GTK_FONT_CHOOSER_H__ */
