#include <gtk/gtktypes.h>
#include <gtk/gtkenums.h>
#include <gdk/gdktypes.h>

#include <dlfcn.h>

/* We need to hijack various symbols by abusing the dynamic linker */
/* Do it here so that the ugliness is contained and I don't call dlopen more that I need */
/* Doing things like this also helps with keeping things tread safe */


/* variables that mark if we initialized a lib or not */
gsize gtk2_loaded = 0;
gsize glib_g_object_loaded = 0;

/* gtk2 symbols */
void (*gtk2_gtk_draw_insertion_cursor)(GtkWidget          *widget,
                                       GdkDrawable        *drawable,
                                       const GdkRectangle *area,
                                       const GdkRectangle *location,
                                       gboolean            is_primary,
                                       GtkTextDirection    direction,
                                       gboolean            draw_arrow) = NULL;

void (*gtk2_gtk_widget_draw) (GtkWidget          *widget,
                              const GdkRectangle *area) = NULL;


/* glib-gobject symbols */
gpointer (*glib_g_object_ref) (gpointer object) = NULL;
void (*glib_g_object_unref) (gpointer object) = NULL;



typedef struct symbol {
    void **symbol;
    char *symbol_name;
} symbol_t;

#define SYMBOL(sym, name) (symbol_t){.symbol = (void**)&sym, .symbol_name = name}

static void init_lib (const char *libname, symbol_t *symbols, gsize* init_checker)
{
  if (g_once_init_enter (init_checker)) {
    const char *dl_error;
    void *lib;

    lib = dlopen (libname, RTLD_NOW);

    g_return_if_fail (lib != NULL);

    while (symbols->symbol) {
      *symbols->symbol = dlsym (lib, symbols->symbol_name);

      dl_error = dlerror();

      if (dl_error) {
        g_warning ("%s\n", dl_error);
      }
      symbols++;
    }

    /* dlclose() never called */
    g_once_init_leave (init_checker, 1);
  }
}



void glib_g_object_init (void)
{
  symbol_t symbols[] = {SYMBOL (glib_g_object_ref, "g_object_ref"),
                        SYMBOL (glib_g_object_unref, "g_object_unref"),
                        (symbol_t){0}};
  init_lib ("libgobject-2.0.so", symbols, &glib_g_object_loaded);
}

void gtk2_init(void)
{
  symbol_t symbols[] = {SYMBOL (gtk2_gtk_draw_insertion_cursor, "gtk_draw_insertion_cursor"),
                        SYMBOL (gtk2_gtk_widget_draw, "gtk_widget_draw"),
                        (symbol_t){0}};
  init_lib (
#ifdef X11
            "libgtk-x11-2.0.so",
#else
            "libgtk-directfb-2.0.so",
#endif
            symbols, &gtk2_loaded);
}
