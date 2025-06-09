#include <glib.h>
#include <gdk/gdkcursor.h>
#include <dlfcn.h>

/* XXX horrible hacks XXX */
/* gtk3 changed the abi for GdkCursor to inherit from GObject instead of being a boxed type */
/* Normally, this wouldn't be a problem, but they also deprecated the api for reffing/unreffing */
/* to use g_object_{ref,unref} instead */
/* This means that the programs that use the new api segfault */

/* To fix this, use the dynamic linker to get the glib symbols */
/* then override them with ones that call gdk_cursor_{ref/unref} as needed */

static void* glib_gobject = NULL;
static gpointer (*glib_g_object_ref) (gpointer object) = NULL;
static void (*glib_g_object_unref) (gpointer object) = NULL;

static void glib_g_object_init (void)
{
  char *dl_error;
  glib_gobject = dlopen ("libgobject-2.0.so", RTLD_NOW);

  g_return_if_fail (glib_gobject != NULL);

  glib_g_object_ref = dlsym (glib_gobject, "g_object_ref");

  dl_error = dlerror();

  if (dl_error) {
    g_warning ("%s\n", dl_error);
  }

  glib_g_object_unref = dlsym (glib_gobject, "g_object_unref");

  dl_error = dlerror();

  if (dl_error) {
    g_warning ("%s\n", dl_error);
  }
}

#define CURSOR_REF_COUNT_TRESH 0xff /* a reasonable guess for the maximum refcount of a cursor */

#undef g_object_ref /* glib has this as both a symbol and a macro at the same type */
gpointer
g_object_ref (gpointer object)
{
  GdkCursor *cursor = object;

  if (!glib_gobject) {
    glib_g_object_init ();
  }

  /* check for something that doesn't 'look' like a pointer to a pointer and instead looks like a cursor */
  if (cursor->type >= GDK_BLANK_CURSOR && cursor->type <= GDK_LAST_CURSOR
                                       && ((sizeof(void*) < 2 * sizeof(int)) /* only do this if pointers are at least a big as 2 ints */
                                           || cursor->ref_count < CURSOR_REF_COUNT_TRESH)) {
    return gdk_cursor_ref ((GdkCursor*)object);
  }

  return glib_g_object_ref (object);
}

void
g_object_unref (gpointer object)
{
  GdkCursor *cursor = object;

  if (!glib_gobject) {
    glib_g_object_init ();
  }

  /* check for something that doesn't 'look' like a pointer to a pointer and instead looks like a cursor */
  if (cursor->type >= GDK_BLANK_CURSOR && cursor->type <= GDK_LAST_CURSOR
                                       && ((sizeof(void*) < 2 * sizeof(int)) /* only do this if pointers are at least a big as 2 ints */
                                           || cursor->ref_count < CURSOR_REF_COUNT_TRESH)) {
    return gdk_cursor_unref (object);
  }

  return glib_g_object_unref (object);
}
