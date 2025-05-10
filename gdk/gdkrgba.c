#include "gdk.h"

GdkRGBA GdkRGBA_from_GdkColor (const GdkColor * const restrict color)
{
  GdkRGBA rgba;
  rgba.red = (gdouble)color->red/65535;
  rgba.green = (gdouble)color->green/65535;
  rgba.blue = (gdouble)color->blue/65535;
  rgba.alpha = 1;

  return rgba;
}

GdkColor GdkColor_from_GdkRGBA (const GdkRGBA * const restrict rgba)
{
  GdkColor color;

  color.red = (int)(rgba->red * 65535);
  color.green = (int)(rgba->green * 65535);
  color.blue = (int)(rgba->blue * 65535);

  return color;
}
