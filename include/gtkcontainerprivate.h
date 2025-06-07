#ifndef __INTERNAL_GTK_CONTAINER_PRIVATE_H__
#define __INTERNAL_GTK_CONTAINER_PRIVATE_H__

G_BEGIN_DECLS

#include <gtk/gtkcontainer.h>

void     gtk_container_queue_resize_handler    (GtkContainer *container);
void     _gtk_container_queue_restyle          (GtkContainer *container);

G_END_DECLS

#endif /* __INTERNAL_GTK_CONTAINER_PRIVATE_H__ */
