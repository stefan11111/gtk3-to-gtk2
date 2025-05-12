#include <gtk/gtk.h>

#define DEFAULT_FONT_NAME "Sans 10"

GType
gtk_font_chooser_get_type (void)
{
#if 0
  return gtk_font_selection_get_type();
#else
  return GTK_TYPE_FONT_SELECTION;
#endif
}

PangoFontFamily *
gtk_font_chooser_get_font_family (GtkFontChooser *fontchooser)
{
  return gtk_font_selection_get_family (fontchooser);
}

PangoFontFace *
gtk_font_chooser_get_font_face (GtkFontChooser *fontchooser)
{
  return gtk_font_selection_get_face (fontchooser);
}

gint
gtk_font_chooser_get_font_size (GtkFontChooser *fontchooser)
{
  return gtk_font_selection_get_size (fontchooser);
}

PangoFontDescription *
gtk_font_chooser_get_font_desc (GtkFontChooser *fontchooser)
{
  PangoFontDescription *font_desc;
  PangoFontFace *face;
  gint size;

  face = gtk_font_selection_get_face (fontchooser);

  if (face)
    {
      font_desc = pango_font_face_describe (face);
      size = gtk_font_selection_get_size (fontchooser);
      pango_font_description_set_size (font_desc, size);
    }
  else
    font_desc = pango_font_description_from_string (DEFAULT_FONT_NAME);

  return font_desc;
}

void
gtk_font_chooser_set_font_desc (GtkFontChooser             *fontchooser,
                                const PangoFontDescription *font_desc)
{
  /* Not Implemented */
  /* Doesn't translate nicely to gtk2 calls, like gtk_font_chooser_get_font_desc does */
}

gchar *
gtk_font_chooser_get_font (GtkFontChooser *fontchooser)
{
  return gtk_font_selection_get_font_name (fontchooser);
}

void
gtk_font_chooser_set_font (GtkFontChooser *fontchooser,
                           const gchar    *fontname)
{
  gtk_font_selection_set_font_name (fontchooser, fontname);
}

gchar *
gtk_font_chooser_get_preview_text (GtkFontChooser *fontchooser)
{
  return (gchar*)gtk_font_selection_get_preview_text (fontchooser);
}

void
gtk_font_chooser_set_preview_text (GtkFontChooser *fontchooser,
                                   const gchar    *text)
{
  gtk_font_selection_set_preview_text (fontchooser, text);
}

gboolean
gtk_font_chooser_get_show_preview_entry (GtkFontChooser *fontchooser)
{
  /* Not Implemented */
  return FALSE;
}

void
gtk_font_chooser_set_show_preview_entry (GtkFontChooser *fontchooser,
                                         gboolean        show_preview_entry)
{
  /* Not Implemented */
}

void
gtk_font_chooser_set_filter_func (GtkFontChooser   *fontchooser,
                                  GtkFontFilterFunc filter,
                                  gpointer          user_data,
                                  GDestroyNotify    destroy)
{
  /* Not Implemented */
}

void
gtk_font_chooser_set_font_map (GtkFontChooser *fontchooser,
                               PangoFontMap   *fontmap)
{
  /* Not Implemented */
}

PangoFontMap *
gtk_font_chooser_get_font_map (GtkFontChooser *fontchooser)
{
  /* Not Implemented */
  return NULL;
}

void
gtk_font_chooser_set_level (GtkFontChooser      *fontchooser,
                            GtkFontChooserLevel  level)
{
  /* Not Implemented */
}

GtkFontChooserLevel
gtk_font_chooser_get_level (GtkFontChooser *fontchooser)
{
  /* Not Implemented */
  return 0;
}

char *
gtk_font_chooser_get_font_features (GtkFontChooser *fontchooser)
{
#if 0
  /* TODO: fix later */
  /* pango_font_description_get_features was added in pango 1.56 */
  /* which is too new, and isn't packaged in gentoo */
  /* stub for now */
  PangoFontDescription *font_desc;
  PangoFontFace *face;
  gint size;

  face = gtk_font_selection_get_face (fontchooser);
  font_desc = pango_font_face_describe (face);
  size = gtk_font_selection_get_size (fontchooser);
  pango_font_description_set_size (font_desc, size);

  return pango_font_description_get_features (font_desc);
#else
  return NULL;
#endif
}

char *
gtk_font_chooser_get_language (GtkFontChooser *fontchooser)
{
  /* Not Implemented */
  return "english";
}

void
gtk_font_chooser_set_language (GtkFontChooser *fontchooser,
                               const char     *language)
{
  /* Not Implemented */
}
