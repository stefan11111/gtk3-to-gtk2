#include <gtk/gtk.h>

const PangoFontDescription *
gtk_style_context_get_font ()
{
  /* TODO: maybe fix this */
  /* Pango expects this to not be NULL, but handles this case gracefully */
  /* Maybe fix this by returning a new empty font */
  /* Doint that uses more memory, but silences a warning */
  return NULL;
}

void
gtk_style_context_get_color ()
{
}
