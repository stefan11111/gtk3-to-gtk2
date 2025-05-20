/* GTK - The GIMP Toolkit
 * Copyright (C) 2010 Carlos Garnacho <carlosg@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GTK_STYLE_CONTEXT_H__
#define __GTK_STYLE_CONTEXT_H__

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkstyle.h>
#include <gdk/gdktypes.h>
#include <gtk/gtkenums.h>
#include <gtk/gtkstyleprovider.h>
#include <gtk/gtkcsssection.h>

#define GTK_TYPE_STYLE_CONTEXT         (gtk_style_context_get_type ())
#define GTK_STYLE_CONTEXT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_STYLE_CONTEXT, GtkStyleContext))
#define GTK_STYLE_CONTEXT_CLASS(c)     (G_TYPE_CHECK_CLASS_CAST    ((c), GTK_TYPE_STYLE_CONTEXT, GtkStyleContextClass))
#define GTK_IS_STYLE_CONTEXT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_STYLE_CONTEXT))
#define GTK_IS_STYLE_CONTEXT_CLASS(c)  (G_TYPE_CHECK_CLASS_TYPE    ((c), GTK_TYPE_STYLE_CONTEXT))
#define GTK_STYLE_CONTEXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS  ((o), GTK_TYPE_STYLE_CONTEXT, GtkStyleContextClass))

typedef GtkStyle GtkStyleContext;
typedef GtkStyleClass GtkStyleContextClass;

GType gtk_style_context_get_type (void) G_GNUC_CONST;

GtkStyleContext * gtk_style_context_new (void);

void gtk_style_context_add_provider_for_screen    (GdkScreen        *screen,
                                                   GtkStyleProvider *provider,
                                                   guint             priority);

void gtk_style_context_remove_provider_for_screen (GdkScreen        *screen,
                                                   GtkStyleProvider *provider);

void gtk_style_context_add_provider    (GtkStyleContext  *context,
                                        GtkStyleProvider *provider,
                                        guint             priority);

void gtk_style_context_remove_provider (GtkStyleContext  *context,
                                        GtkStyleProvider *provider);

void gtk_style_context_save    (GtkStyleContext *context);

void gtk_style_context_restore (GtkStyleContext *context);

/* GtkCssSection * gtk_style_context_get_section (GtkStyleContext *context,
                                               const gchar     *property);
*/
void gtk_style_context_get_property (GtkStyleContext *context,
                                     const gchar     *property,
                                     GtkStateFlags    state,
                                     GValue          *value);

void gtk_style_context_get_valist   (GtkStyleContext *context,
                                     GtkStateFlags    state,
                                     va_list          args);

void gtk_style_context_get          (GtkStyleContext *context,
                                     GtkStateFlags    state,
                                     ...) G_GNUC_NULL_TERMINATED;

void          gtk_style_context_set_state    (GtkStyleContext *context,
                                              GtkStateFlags    flags);

GtkStateFlags gtk_style_context_get_state    (GtkStyleContext *context);

void gtk_style_context_get_color            (GtkStyleContext *context,
                                             GtkStateFlags    state,
                                             GdkRGBA         *color);

const PangoFontDescription *
     gtk_style_context_get_font             (GtkStyleContext *context,
                                             GtkStateFlags    state);

G_END_DECLS

#endif /* __GTK_STYLE_CONTEXT_H__ */
