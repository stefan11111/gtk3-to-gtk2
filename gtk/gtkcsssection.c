#include <gtk/gtkcsssection.h>

/* XXX Not Implemented XXX*/

/* Allocation is implemented, but is disabled by default*/
/* To enable, define ENABLE_GTK_CSS_SECTION_ALLOCATION */
/* The allocation is useless, it just increments/decrements a refcount */

struct _GtkCssSection
{
#ifdef ENABLE_GTK_CSS_SECTION_ALLOCATION
  gint                ref_count;
#endif
};

G_DEFINE_BOXED_TYPE (GtkCssSection, gtk_css_section, gtk_css_section_ref, gtk_css_section_unref)

typedef struct _GtkCssParser GtkCssParser;

GtkCssSection *
_gtk_css_section_new (GtkCssSection     *parent,
                      GtkCssSectionType  type,
                      GtkCssParser      *parser)
{
#ifdef ENABLE_GTK_CSS_SECTION_ALLOCATION
  GtkCssSection *section;

  /* despite what the docs say, g_slice_new{,0} can return NULL */
  /* https://gitlab.gnome.org/GNOME/glib/-/issues/3688 */
  section = g_slice_new0 (GtkCssSection);

  section->ref_count = 1;

  return section;
#else
  return NULL;
#endif
}

GtkCssSection *
_gtk_css_section_new_for_file (GtkCssSectionType  type,
                               GFile             *file)
{
  return _gtk_css_section_new (NULL, 0, NULL);
}

void
_gtk_css_section_end (GtkCssSection *section)
{
}

GtkCssSection *
gtk_css_section_ref (GtkCssSection *section)
{
#ifdef ENABLE_GTK_CSS_SECTION_ALLOCATION
  g_return_val_if_fail (section != NULL, NULL);

  section->ref_count += 1;

  return section;
#else
  return NULL;
#endif
}

void
gtk_css_section_unref (GtkCssSection *section)
{
#ifdef ENABLE_GTK_CSS_SECTION_ALLOCATION
  g_return_if_fail (section != NULL);

  if (--section->ref_count > 0)
    return;

  g_slice_free (GtkCssSection, section);
#endif
}

GtkCssSectionType
gtk_css_section_get_section_type (const GtkCssSection *section)
{
  return GTK_CSS_SECTION_DOCUMENT;
}

GtkCssSection *
gtk_css_section_get_parent (const GtkCssSection *section)
{
  return NULL;
}

GFile *
gtk_css_section_get_file (const GtkCssSection *section)
{
  return NULL;
}

guint
gtk_css_section_get_start_line (const GtkCssSection *section)
{
  return 0;
}

guint
gtk_css_section_get_start_position (const GtkCssSection *section)
{
  return 0;
}

guint
gtk_css_section_get_end_line (const GtkCssSection *section)
{
  return 0;
}

guint
gtk_css_section_get_end_position (const GtkCssSection *section)
{
  return 0;
}

void
_gtk_css_section_print (const GtkCssSection  *section,
                        GString              *string)
{
}

char *
_gtk_css_section_to_string (const GtkCssSection *section)
{
  return NULL;
}
