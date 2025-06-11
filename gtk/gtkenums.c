#include <gtk/gtkenums.h>

GtkStateType
GtkStateType_from_GtkStateFlags (GtkStateFlags flags)
{
  switch(flags) {
  case GTK_STATE_FLAG_NORMAL:
    return GTK_STATE_NORMAL;
  case GTK_STATE_FLAG_ACTIVE:
    return GTK_STATE_ACTIVE;
  case GTK_STATE_FLAG_PRELIGHT:
    return GTK_STATE_PRELIGHT;
  case GTK_STATE_FLAG_SELECTED:
    return GTK_STATE_SELECTED;
  case GTK_STATE_FLAG_INSENSITIVE:
    return GTK_STATE_INSENSITIVE;
  default:
    return GTK_STATE_NORMAL;
  }
  /* UNREACHABLE */
  return GTK_STATE_NORMAL;
}

GtkStateFlags
GtkStateFlags_from_GtkStateType (GtkStateType type)
{
  switch(type) {
  case GTK_STATE_NORMAL:
    return GTK_STATE_FLAG_NORMAL;
  case GTK_STATE_ACTIVE:
    return GTK_STATE_FLAG_ACTIVE;
  case GTK_STATE_PRELIGHT:
    return GTK_STATE_FLAG_PRELIGHT;
  case GTK_STATE_SELECTED:
    return GTK_STATE_FLAG_SELECTED;
  case GTK_STATE_INSENSITIVE:
    return GTK_STATE_FLAG_INSENSITIVE;
  }
  /* UNREACHABLE */
  return GTK_STATE_NORMAL;
}

