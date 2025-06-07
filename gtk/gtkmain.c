#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "util.h"

guint
gtk_get_major_version (void)
{
  return 3;
}

guint
gtk_get_minor_version (void)
{
  return 24;
}

guint
gtk_get_micro_version (void)
{
  return 39;
}

guint
gtk_get_binary_age (void)
{
  return 2439;
}

guint
gtk_get_interface_age (void)
{
  return 32;
}