#ifndef __GTK_FONT_CHOOSER_H__
#define __GTK_FONT_CHOOSER_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkfontsel.h>

G_BEGIN_DECLS

typedef GtkFontSelection GtkFontChooser;

/**
 * GtkFontFilterFunc:
 * @family: a #PangoFontFamily
 * @face: a #PangoFontFace belonging to @family
 * @data: (closure): user data passed to gtk_font_chooser_set_filter_func()
 *
 * The type of function that is used for deciding what fonts get
 * shown in a #GtkFontChooser. See gtk_font_chooser_set_filter_func().
 *
 * Returns: %TRUE if the font should be displayed
 */
typedef gboolean (*GtkFontFilterFunc) (const PangoFontFamily *family,
                                       const PangoFontFace   *face,
                                       gpointer               data);

/**
 * GtkFontChooserLevel:
 * @GTK_FONT_CHOOSER_LEVEL_FAMILY: Allow selecting a font family
 * @GTK_FONT_CHOOSER_LEVEL_STYLE: Allow selecting a specific font face
 * @GTK_FONT_CHOOSER_LEVEL_SIZE: Allow selecting a specific font size
 * @GTK_FONT_CHOOSER_LEVEL_VARIATION: Allow changing OpenType font variation axes
 * @GTK_FONT_CHOOSER_LEVEL_FEATURES: Allow selecting specific OpenType font features
 *
 * This enumeration specifies the granularity of font selection
 * that is desired in a font chooser.
 *
 * This enumeration may be extended in the future; applications should
 * ignore unknown values.
 */
typedef enum {
  GTK_FONT_CHOOSER_LEVEL_FAMILY     = 0,
  GTK_FONT_CHOOSER_LEVEL_STYLE      = 1 << 0,
  GTK_FONT_CHOOSER_LEVEL_SIZE       = 1 << 1,
  GTK_FONT_CHOOSER_LEVEL_VARIATIONS = 1 << 2,
  GTK_FONT_CHOOSER_LEVEL_FEATURES   = 1 << 3
} GtkFontChooserLevel;


#define GTK_TYPE_FONT_CHOOSER                   (gtk_font_chooser_get_type ())
#define GTK_FONT_CHOOSER(obj)                   (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FONT_CHOOSER, GtkFontChooser))
#define GTK_IS_FONT_CHOOSER(obj)                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FONT_CHOOSER))
#define GTK_FONT_CHOOSER_GET_IFACE(inst)         GTK_FONT_SELECTION_GET_CLASS(inst)

GType            gtk_font_chooser_get_type                 (void) G_GNUC_CONST;


PangoFontFamily *gtk_font_chooser_get_font_family          (GtkFontChooser   *fontchooser);

PangoFontFace   *gtk_font_chooser_get_font_face            (GtkFontChooser   *fontchooser);

gint             gtk_font_chooser_get_font_size            (GtkFontChooser   *fontchooser);


PangoFontDescription *
                 gtk_font_chooser_get_font_desc            (GtkFontChooser             *fontchooser);

void             gtk_font_chooser_set_font_desc            (GtkFontChooser             *fontchooser,
                                                            const PangoFontDescription *font_desc);


gchar*           gtk_font_chooser_get_font                 (GtkFontChooser   *fontchooser);


void             gtk_font_chooser_set_font                 (GtkFontChooser   *fontchooser,
                                                            const gchar      *fontname);

gchar*           gtk_font_chooser_get_preview_text         (GtkFontChooser   *fontchooser);

void             gtk_font_chooser_set_preview_text         (GtkFontChooser   *fontchooser,
                                                            const gchar      *text);

gboolean         gtk_font_chooser_get_show_preview_entry   (GtkFontChooser   *fontchooser);

void             gtk_font_chooser_set_show_preview_entry   (GtkFontChooser   *fontchooser,
                                                            gboolean          show_preview_entry);

void             gtk_font_chooser_set_filter_func          (GtkFontChooser   *fontchooser,
                                                            GtkFontFilterFunc filter,
                                                            gpointer          user_data,
                                                            GDestroyNotify    destroy);

void             gtk_font_chooser_set_font_map             (GtkFontChooser   *fontchooser,
                                                            PangoFontMap     *fontmap);

PangoFontMap *   gtk_font_chooser_get_font_map             (GtkFontChooser   *fontchooser);


void             gtk_font_chooser_set_level                (GtkFontChooser   *fontchooser,
                                                            GtkFontChooserLevel level);

GtkFontChooserLevel
                 gtk_font_chooser_get_level                (GtkFontChooser   *fontchooser);

char *           gtk_font_chooser_get_font_features        (GtkFontChooser   *fontchooser);

char *           gtk_font_chooser_get_language             (GtkFontChooser   *fontchooser);

void             gtk_font_chooser_set_language             (GtkFontChooser   *fontchooser,
                                                            const char       *language);


G_END_DECLS

#endif /* __GTK_FONT_CHOOSER_H__ */
