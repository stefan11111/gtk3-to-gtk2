#include <gio/gio.h>

/* No better place for these... */
gchar *                 gtk_print_action_and_target                     (const gchar    *action_namespace,
                                                                         const gchar    *action_name,
                                                                         GVariant       *target);

gchar *                 gtk_normalise_detailed_action_name              (const gchar *detailed_action_name);
