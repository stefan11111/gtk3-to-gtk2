#ifndef __INTERNAL_GTK_ENUMS_H__
#define __INTERNAL_GTK_ENUMS_H__

#include <gtk/gtkenums.h>

G_BEGIN_DECLS

GtkStateType GtkStateType_from_GtkStateFlags (GtkStateFlags flags);
GtkStateFlags GtkStateFlags_from_GtkStateType (GtkStateType flags);

G_END_DECLS

#endif /* __INTERNAL_GTK_ENUMS_H__ */
