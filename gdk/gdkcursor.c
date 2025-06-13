#include <gdk/gdkcursor.h>

#include "gtkglobals.h"

/* XXX horrible hacks XXX */
/* gtk3 changed the abi for GdkCursor to inherit from GObject instead of being a boxed type */
/* Normally, this wouldn't be a problem, but they also deprecated the api for reffing/unreffing */
/* to use g_object_{ref,unref} instead */
/* This means that the programs that use the new api segfault */

/* To fix this, use the dynamic linker to get the glib symbols */
/* then override them with ones that call gdk_cursor_{ref/unref} as needed */

#define CURSOR_REF_COUNT_TRESH 0xff /* a reasonable guess for the maximum refcount of a cursor */

#undef g_object_ref /* glib has this as both a symbol and a macro at the same type */
gpointer
g_object_ref (gpointer object)
{
  GdkCursor *cursor = object;

  if (!glib_g_object_loaded) {
    glib_g_object_init ();
  }

  /* check for something that doesn't 'look' like a pointer to a pointer and instead looks like a cursor */
  if (cursor->type >= GDK_BLANK_CURSOR && cursor->type <= GDK_LAST_CURSOR
                                       && ((sizeof(void*) < 2 * sizeof(int)) /* only do this if pointers are at least as big as 2 ints */
                                           || cursor->ref_count < CURSOR_REF_COUNT_TRESH)) {
    return gdk_cursor_ref (object);
  }

  return glib_g_object_ref (object);
}

void
g_object_unref (gpointer object)
{
  GdkCursor *cursor = object;

  if (!glib_g_object_loaded) {
    glib_g_object_init ();
  }

  /* check for something that doesn't 'look' like a pointer to a pointer and instead looks like a cursor */
  if (cursor->type >= GDK_BLANK_CURSOR && cursor->type <= GDK_LAST_CURSOR
                                       && ((sizeof(void*) < 2 * sizeof(int)) /* only do this if pointers are at least as big as 2 ints */
                                           || cursor->ref_count < CURSOR_REF_COUNT_TRESH)) {
    return gdk_cursor_unref (object);
  }

  return glib_g_object_unref (object);
}
