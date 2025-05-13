#ifndef __INTERNAL_GTK_GRADIENT_PRIVATE_H__
#define __INTERNAL_GTK_GRADIENT_PRIVATE_H__

#include "gtk/gtkgradient.h"

G_BEGIN_DECLS

typedef struct _GtkGradient GtkGradient;
typedef struct _GtkCssStyle GtkCssStyle;
typedef struct _GtkStyleProviderPrivate GtkStyleProviderPrivate;

cairo_pattern_t *       _gtk_gradient_resolve_full            (GtkGradient             *gradient,
                                                               GtkStyleProviderPrivate *provider,
                                                               GtkCssStyle             *style,
                                                               GtkCssStyle             *parent_style);

GtkGradient *           _gtk_gradient_transition              (GtkGradient             *start,
                                                               GtkGradient             *end,
                                                               guint                    property_id,
                                                               double                   progress);

G_END_DECLS

#endif /* __INTERNAL_GTK_STYLE_PROPERTIES_PRIVATE_H__ */
