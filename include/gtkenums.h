#ifndef __INTERNAL_GTK_ENUMS_H__
#define __INTERNAL_GTK_ENUMS_H__

#if !defined (__GDK_H_INSIDE__) && !defined (GDK_COMPILATION)
#error "Only <gdk/gdk.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

GtkStateType GtkStateType_from_GtkStateFlags (GtkStateFlags flags);

G_END_DECLS

#endif /* __INTERNAL_GTK_ENUMS_H__ */
