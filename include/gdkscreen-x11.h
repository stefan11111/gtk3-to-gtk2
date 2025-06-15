/*
 * gdkscreen-x11.h
 *
 * Copyright 2001 Sun Microsystems Inc.
 *
 * Erwann Chenede <erwann.chenede@sun.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __INTERNAL_GDK_SCREEN_X11_H__
#define __INTERNAL_GDK_SCREEN_X11_H__

#if 0
#include "gdkprivate-x11.h"
#include "xsettings-client.h"
#endif

#include <gdk/gdkscreen.h>
#include <gdk/gdkvisual.h>
#include <X11/X.h>
#include <X11/Xlib.h>

G_BEGIN_DECLS

typedef struct _XSettingsClient XSettingsClient;
typedef struct _GdkVisualPrivate       GdkVisualPrivate;

typedef struct _GdkScreenX11 GdkScreenX11;
typedef struct _GdkScreenX11Class GdkScreenX11Class;

#if 0
#define GDK_TYPE_SCREEN_X11              (_gdk_screen_x11_get_type ())
#define GDK_SCREEN_X11(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_SCREEN_X11, GdkScreenX11))
#define GDK_SCREEN_X11_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_SCREEN_X11, GdkScreenX11Class))
#define GDK_IS_SCREEN_X11(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_SCREEN_X11))
#define GDK_IS_SCREEN_X11_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_SCREEN_X11))
#define GDK_SCREEN_X11_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_SCREEN_X11, GdkScreenX11Class))
#endif

/* expose gtk2 internals */
GType gtk2_gdk_screen_x11_get_type (void);
GdkScreenX11 *gtk2_gdk_screen_x11 (void *ptr);
GdkScreenX11Class *gtk2_gdk_screen_x11_class (void *ptr);
gboolean gtk2_gdk_is_screen_x11 (void *ptr);
gboolean gtk2_gdk_is_screen_x11_class (void *ptr);
GdkScreenX11Class *gtk2_gdk_screen_x11_get_class (void *ptr);

#define GDK_TYPE_X11_SCREEN              (gtk2_gdk_screen_x11_get_type ())
#define GDK_X11_SCREEN(object)           (gtk2_gdk_screen_x11 (object))
#define GDK_X11_SCREEN_CLASS(klass)      (gtk2_gdk_screen_x11_class (klass))
#define GDK_IS_X11_SCREEN(object)        (gtk2_gdk_is_screen_x11 (object))
#define GDK_IS_X11_SCREEN_CLASS(klass)   (gtk2_gdk_is_screen_x11_class (klass))
#define GDK_X11_SCREEN_GET_CLASS(obj)    (gtk2_gdk_screen_x11_get_class (obj))

#define GDK_TYPE_SCREEN_X11               GDK_TYPE_X11_SCREEN
#define GDK_SCREEN_X11(object)            GDK_X11_SCREEN(object)
#define GDK_SCREEN_X11_CLASS(klass)       GDK_X11_SCREEN_CLASS(klass)
#define GDK_IS_SCREEN_X11(object)         GDK_IS_X11_SCREEN(object)
#define GDK_IS_SCREEN_X11_CLASS(klass)    GDK_IS_X11_SCREEN_CLASS(klass)
#define GDK_SCREEN_X11_GET_CLASS(obj)     GDK_X11_SCREEN_GET_CLASS(obj)


typedef struct _GdkX11Monitor GdkX11Monitor;

struct _GdkX11Monitor
{
  GdkRectangle  geometry;
  XID           output;
  int           width_mm;
  int           height_mm;
  char *        output_name;
  char *        manufacturer;
};

struct _GdkScreenX11
{
  GdkScreen parent_instance;

  GdkDisplay *display;
  Display *xdisplay;
  Screen *xscreen;
  gint screen_num;
  Window xroot_window;
  GdkWindow *root_window;

  /* Window manager */
  long last_wmspec_check_time;
  Window wmspec_check_window;
  char *window_manager_name;
  /* TRUE if wmspec_check_window has changed since last
   * fetch of _NET_SUPPORTED
   */
  guint need_refetch_net_supported : 1;
  /* TRUE if wmspec_check_window has changed since last
   * fetch of window manager name
   */
  guint need_refetch_wm_name : 1;

  /* Visual Part */
  GdkVisualPrivate *system_visual;
  GdkVisualPrivate **visuals;
  gint nvisuals;
  gint available_depths[7];
  gint navailable_depths;
  GdkVisualType available_types[6];
  gint navailable_types;
  GHashTable *visual_hash;
  GHashTable *colormap_hash;
  GdkVisual *rgba_visual;

  /* Colormap Part */
  GdkColormap *default_colormap;
  GdkColormap *system_colormap;
  GdkColormap *rgba_colormap;

  /* X settings */
  XSettingsClient *xsettings_client;
  guint xsettings_in_init : 1;

  /* Xinerama/RandR 1.2 */
  gint           n_monitors;
  GdkX11Monitor *monitors;
  gint           primary_monitor;

  /* Pango renderer object singleton */
  PangoRenderer *renderer;

  /* Xft resources for the display, used for default values for
   * the Xft/ XSETTINGS
   */
  gboolean xft_init;            /* Whether we've intialized these values yet */
  gboolean xft_antialias;
  gboolean xft_hinting;
  gint xft_hintstyle;
  gint xft_rgba;
  gint xft_dpi;

  GdkAtom cm_selection_atom;
  gboolean is_composited;
};

struct _GdkScreenX11Class
{
  GdkScreenClass parent_class;

  void (* window_manager_changed) (GdkScreenX11 *screen_x11);
};

#endif /* __INTERNAL_GDK_SCREEN_X11_H__ */
