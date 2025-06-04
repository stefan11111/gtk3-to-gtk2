#include <gtk/gtk.h>

#define GTK_TYPE_APPLICATION_IMPL                           (gtk_application_impl_get_type ())
#define GTK_APPLICATION_IMPL_CLASS(class)                   (G_TYPE_CHECK_CLASS_CAST ((class),                     \
                                                             GTK_TYPE_APPLICATION_IMPL,                            \
                                                             GtkApplicationImplClass))
#define GTK_APPLICATION_IMPL_GET_CLASS(obj)                 (G_TYPE_INSTANCE_GET_CLASS ((obj),                     \
                                                             GTK_TYPE_APPLICATION_IMPL,                            \
                                                             GtkApplicationImplClass))

typedef struct
{
  GObject parent_instance;
  GtkApplication *application;
  GdkDisplay *display;
} GtkApplicationImpl;

typedef struct
{
  GObjectClass parent_class;

  void        (* startup)                   (GtkApplicationImpl          *impl,
                                             gboolean                     register_session);
  void        (* shutdown)                  (GtkApplicationImpl          *impl);

  void        (* before_emit)               (GtkApplicationImpl          *impl,
                                             GVariant                    *platform_data);

  void        (* window_added)              (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window);
  void        (* window_removed)            (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window);
  void        (* active_window_changed)     (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window);
  void        (* handle_window_realize)     (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window);
  void        (* handle_window_map)         (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window);

  void        (* set_app_menu)              (GtkApplicationImpl          *impl,
                                             GMenuModel                  *app_menu);
  void        (* set_menubar)               (GtkApplicationImpl          *impl,
                                             GMenuModel                  *menubar);

  guint       (* inhibit)                   (GtkApplicationImpl          *impl,
                                             GtkWindow                   *window,
                                             GtkApplicationInhibitFlags   flags,
                                             const gchar                 *reason);
  void        (* uninhibit)                 (GtkApplicationImpl          *impl,
                                             guint                        cookie);
  gboolean    (* is_inhibited)              (GtkApplicationImpl          *impl,
                                             GtkApplicationInhibitFlags   flags);

  gboolean    (* prefers_app_menu)          (GtkApplicationImpl          *impl);


} GtkApplicationImplClass;

GType                   gtk_application_impl_get_type                   (void);

GtkApplicationImpl *    gtk_application_impl_new                        (GtkApplication              *application,
                                                                         GdkDisplay                  *display);
void                    gtk_application_impl_startup                    (GtkApplicationImpl          *impl,
                                                                         gboolean                     register_sesion);
void                    gtk_application_impl_shutdown                   (GtkApplicationImpl          *impl);
void                    gtk_application_impl_before_emit                (GtkApplicationImpl          *impl,
                                                                         GVariant                    *platform_data);
void                    gtk_application_impl_window_added               (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window);
void                    gtk_application_impl_window_removed             (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window);
void                    gtk_application_impl_active_window_changed      (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window);
void                    gtk_application_impl_handle_window_realize      (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window);
void                    gtk_application_impl_handle_window_map          (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window);
void                    gtk_application_impl_set_app_menu               (GtkApplicationImpl          *impl,
                                                                         GMenuModel                  *app_menu);
void                    gtk_application_impl_set_menubar                (GtkApplicationImpl          *impl,
                                                                         GMenuModel                  *menubar);
guint                   gtk_application_impl_inhibit                    (GtkApplicationImpl          *impl,
                                                                         GtkWindow                   *window,
                                                                         GtkApplicationInhibitFlags   flags,
                                                                         const gchar                 *reason);
void                    gtk_application_impl_uninhibit                  (GtkApplicationImpl          *impl,
                                                                         guint                        cookie);
gboolean                gtk_application_impl_is_inhibited               (GtkApplicationImpl          *impl,
                                                                         GtkApplicationInhibitFlags   flags);
gboolean                gtk_application_impl_prefers_app_menu           (GtkApplicationImpl          *impl);
