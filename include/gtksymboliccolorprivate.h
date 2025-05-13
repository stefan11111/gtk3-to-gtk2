#ifndef __INTERNAL_GTK_SYMBOLIC_COLOR_PRIVATE_H__
#define __INTERNAL_GTK_SYMBOLIC_COLOR_PRIVATE_H__

#include <gtk/gtksymboliccolor.h>

G_BEGIN_DECLS

typedef struct _GtkCssParser GtkCssParser;
typedef struct _GtkCssValue GtkCssValue;

GtkSymbolicColor * _gtk_css_symbolic_value_new            (GtkCssParser               *parser);

GtkCssValue *      _gtk_symbolic_color_get_css_value      (GtkSymbolicColor           *symbolic);

G_END_DECLS

#endif /* __INTERNAL_GTK_SYMBOLIC_COLOR_PRIVATE_H__ */
