#ifndef __INTERNAL_GTK_CAIRO_H__
#define __INTERNAL_GTK_CAIRO_H__

#include <gtk/gtkstyle.h>

void
gtk_cairo_paint_check (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkShadowType       shadow_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height);

void
gtk_cairo_paint_option (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height);

void
gtk_cairo_paint_arrow (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkShadowType       shadow_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       GtkArrowType        arrow_type,
                       gboolean            fill,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height);

void
gtk_cairo_paint_shadow (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height);

void
gtk_cairo_paint_expander (GtkStyle           *style,
                          cairo_t            *cr,
                          GtkStateType        state_type,
                          GtkWidget          *widget,
                          const gchar        *detail,
                          gint                x,
                          gint                y,
                          GtkExpanderStyle    expander_style);

void
gtk_cairo_paint_focus (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x,
                       gint                y,
                       gint                width,
                       gint                height);

void
gtk_cairo_paint_layout (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        gboolean            use_text,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        PangoLayout        *layout);

void
gtk_cairo_paint_line (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x1,
                       gint                y1,
                       gint                x2,
                       gint                y2);

void
gtk_cairo_paint_hline (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                x1,
                       gint                x2,
                       gint                y);

void
gtk_cairo_paint_vline (GtkStyle           *style,
                       cairo_t            *cr,
                       GtkStateType        state_type,
                       GtkWidget          *widget,
                       const gchar        *detail,
                       gint                y1_,
                       gint                y2_,
                       gint                x);

void
gtk_cairo_paint_box (GtkStyle           *style,
                     cairo_t            *cr,
                     GtkStateType        state_type,
                     GtkShadowType       shadow_type,
                     GtkWidget          *widget,
                     const gchar        *detail,
                     gint                x,
                     gint                y,
                     gint                width,
                     gint                height);

void
gtk_cairo_paint_slider (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height,
                        GtkOrientation      orientation);

void
gtk_cairo_paint_shadow_gap (GtkStyle           *style,
                            cairo_t            *cr,
                            GtkStateType        state_type,
                            GtkShadowType       shadow_type,
                            GtkWidget          *widget,
                            const gchar        *detail,
                            gint                x,
                            gint                y,
                            gint                width,
                            gint                height,
                            GtkPositionType     gap_side,
                            gint                gap_x,
                            gint                gap_width);

void
gtk_cairo_paint_extension (GtkStyle           *style,
                           cairo_t            *cr,
                           GtkStateType        state_type,
                           GtkShadowType       shadow_type,
                           GtkWidget          *widget,
                           const gchar        *detail,
                           gint                x,
                           gint                y,
                           gint                width,
                           gint                height,
                           GtkPositionType     gap_side);

void
gtk_cairo_paint_handle (GtkStyle           *style,
                        cairo_t            *cr,
                        GtkStateType        state_type,
                        GtkShadowType       shadow_type,
                        GtkWidget          *widget,
                        const gchar        *detail,
                        gint                x,
                        gint                y,
                        gint                width,
                        gint                height,
                        GtkOrientation      orientation);

void
gtk_cairo_paint_spinner (GtkStyle           *style,
                         cairo_t            *cr,
                         GtkStateType        state_type,
                         GtkWidget          *widget,
                         const gchar        *detail,
                         guint               step,
                         gint                x,
                         gint                y,
                         gint                width,
                         gint                height);


#endif /* __INTERNAL_GTK_CAIRO_H__ */
