#include <pango/pangocairo.h>
#include <gtk/gtkenums.h>

GtkTextDirection
GtkTextDirection_from_PangoDirection (PangoDirection direction)
{
  switch (direction) {
  case PANGO_DIRECTION_LTR:
  case PANGO_DIRECTION_TTB_LTR:
  case PANGO_DIRECTION_WEAK_LTR:
    return GTK_TEXT_DIR_LTR;
  case PANGO_DIRECTION_RTL:
  case PANGO_DIRECTION_TTB_RTL:
  case PANGO_DIRECTION_WEAK_RTL:
    return GTK_TEXT_DIR_RTL;
  case PANGO_DIRECTION_NEUTRAL:
    return GTK_TEXT_DIR_NONE;
  }

  /* unreachable */
  return GTK_TEXT_DIR_LTR;
}

PangoDirection
PangoDirection_from_GtkTextDirection (GtkTextDirection direction)
{
  switch (direction) {
  case GTK_TEXT_DIR_LTR:
    return PANGO_DIRECTION_LTR;
  case GTK_TEXT_DIR_RTL:
    return PANGO_DIRECTION_RTL;
  case GTK_TEXT_DIR_NONE:
    return PANGO_DIRECTION_NEUTRAL;
  }

  /* unreachable */
  return PANGO_DIRECTION_LTR;
}

