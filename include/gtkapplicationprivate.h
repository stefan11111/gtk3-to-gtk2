#ifndef __INTERNAL_GTK_APPLICATION_PRIVATE_H__
#define __INTERNAL_GTK_APPLICATION_PRIVATE_H__


#include "gtkapplicationimpl.h"
#include "gtkapplicationaccelsprivate.h"

G_BEGIN_DECLS

#if 0 /* TODO: remove after gtkapplicationwindow is implemented */
void                    gtk_application_window_set_id                   (GtkApplicationWindow     *window,
                                                                         guint                     id);
GActionGroup *          gtk_application_window_get_action_group         (GtkApplicationWindow     *window);
#endif

void                    gtk_application_handle_window_realize           (GtkApplication           *application,
                                                                         GtkWindow                *window);
void                    gtk_application_handle_window_map               (GtkApplication           *application,
                                                                         GtkWindow                *window);
GtkActionMuxer *        gtk_application_get_parent_muxer_for_window     (GtkWindow                *window);

GtkActionMuxer *        gtk_application_get_action_muxer                (GtkApplication           *application);
void                    gtk_application_insert_action_group             (GtkApplication           *application,
                                                                         const gchar              *name,
                                                                         GActionGroup             *action_group);

GtkApplicationAccels *  gtk_application_get_application_accels          (GtkApplication           *application);

void                    gtk_application_set_screensaver_active          (GtkApplication           *application,
                                                                         gboolean                  active);

G_END_DECLS


#endif /* __INTERNAL_GTK_APPLICATION_PRIVATE_H__ */
