typedef struct _GtkApplication        GtkApplication;
typedef struct _GtkApplicationClass   GtkApplicationClass;
typedef struct _GtkApplicationPrivate GtkApplicationPrivate;

struct _GtkApplication
{
  GApplication parent;

  /*< private >*/
  GtkApplicationPrivate *priv;
};

/**
 * GtkApplicationClass:
 * @parent_class: The parent class.
 * @window_added: Signal emitted when a #GtkWindow is added to
 *    application through gtk_application_add_window().
 * @window_removed: Signal emitted when a #GtkWindow is removed from
 *    application, either as a side-effect of being destroyed or
 *    explicitly through gtk_application_remove_window().
 */
struct _GtkApplicationClass
{
  GApplicationClass parent_class;

  /*< public >*/

  void (*window_added)   (GtkApplication *application,
                          GtkWindow      *window);
  void (*window_removed) (GtkApplication *application,
                          GtkWindow      *window);

  /*< private >*/
  gpointer padding[12];
};
