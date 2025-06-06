#ifndef __GTK_ACTION_MUXER_H__
#define __GTK_ACTION_MUXER_H__


#include <gio/gio.h>

G_BEGIN_DECLS

/* No better place for these... */
gchar *                 gtk_print_action_and_target                     (const gchar    *action_namespace,
                                                                         const gchar    *action_name,
                                                                         GVariant       *target);

gchar *                 gtk_normalise_detailed_action_name              (const gchar *detailed_action_name);


G_END_DECLS

#endif /* __GTK_ACTION_MUXER_H__ */
