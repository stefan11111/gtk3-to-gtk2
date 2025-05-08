typedef struct _GtkGrid              GtkGrid;
typedef struct _GtkGridPrivate       GtkGridPrivate;
typedef struct _GtkGridClass         GtkGridClass;

struct _GtkGrid
{
  /*< private >*/
  GtkContainer container;

  GtkGridPrivate *priv;
};

/**
 * GtkGridClass:
 * @parent_class: The parent class.
 */
struct _GtkGridClass
{
  GtkContainerClass parent_class;

  /*< private >*/

  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
  void (*_gtk_reserved5) (void);
  void (*_gtk_reserved6) (void);
  void (*_gtk_reserved7) (void);
  void (*_gtk_reserved8) (void);
};
