#include <gtk/gtk.h>
#include "gtkprivate.h"

typedef struct _GtkStyleContext GtkStyleContext;

GtkStyleContext *
gtk_widget_get_style_context (GtkWidget *widget)
{
  return NULL;
}

int
gtk_widget_get_allocated_height (GtkWidget *widget)
{
  g_return_val_if_fail (GTK_IS_WIDGET (widget), 0);

  return widget->allocation.height;
}

void
gtk_widget_override_font (GtkWidget                  *widget,
                          const PangoFontDescription *font_desc)
{
  gtk_widget_modify_font (widget, (PangoFontDescription *)font_desc);
}

gboolean
gtk_widget_get_hexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_hexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_hexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

gboolean
gtk_widget_get_vexpand (GtkWidget *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand (GtkWidget      *widget,
                        gboolean        expand)
{
}

gboolean
gtk_widget_get_vexpand_set (GtkWidget      *widget)
{
  return FALSE;
}

void
gtk_widget_set_vexpand_set (GtkWidget      *widget,
                            gboolean        set)
{
}

void
gtk_widget_set_halign (GtkWidget *widget,
                       GtkAlign   align)
{
}

GtkAlign
gtk_widget_get_halign (GtkWidget *widget)
{
  return GTK_ALIGN_FILL;
}

void
gtk_widget_set_valign (GtkWidget *widget,
                       GtkAlign   align)
{
}

GtkAlign
gtk_widget_get_valign (GtkWidget *widget)
{
  return GTK_ALIGN_FILL;
}

GtkAlign
gtk_widget_get_valign_with_baseline (GtkWidget *widget)
{
  /* doesn't actually return GTK_ALIGN_BASELINE */
  return GTK_ALIGN_FILL;
}
