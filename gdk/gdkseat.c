#include <gdk/gdkseat.h>

/* XXX Not Implemented XXX */

/**
 * SECTION:gdkseat
 * @Short_description: Object representing an user seat
 * @Title: GdkSeat
 * @See_also: #GdkDisplay, #GdkDevice
 *
 * The #GdkSeat object represents a collection of input devices
 * that belong to a user.
 */

enum {
  DEVICE_ADDED,
  DEVICE_REMOVED,
  TOOL_ADDED,
  TOOL_REMOVED,
  N_SIGNALS
};

enum {
  PROP_0,
  PROP_DISPLAY,
  N_PROPS
};

typedef struct _GdkSeatClass GdkSeatClass;

struct _GdkSeatClass
{
  GObjectClass parent_class;
};

static void
gdk_seat_init (GdkSeat *seat)
{
}

static void
gdk_seat_class_init (GdkSeatClass *seat)
{
}

G_DEFINE_ABSTRACT_TYPE (GdkSeat, gdk_seat, G_TYPE_OBJECT)

GdkGrabStatus
gdk_seat_grab (GdkSeat                *seat,
               GdkWindow              *window,
               GdkSeatCapabilities     capabilities,
               gboolean                owner_events,
               GdkCursor              *cursor,
               const GdkEvent         *event,
               GdkSeatGrabPrepareFunc  prepare_func,
               gpointer                prepare_func_data)
{
  return GDK_GRAB_FAILED;
}

void
gdk_seat_ungrab (GdkSeat *seat)
{
}

GdkDisplay *
gdk_seat_get_display (GdkSeat *seat)
{
  return NULL;
}

GdkSeatCapabilities
gdk_seat_get_capabilities (GdkSeat *seat)
{
  return GDK_SEAT_CAPABILITY_NONE;
}

GList *
gdk_seat_get_slaves (GdkSeat             *seat,
                     GdkSeatCapabilities  capabilities)
{
  return NULL;
}

GdkDevice *
gdk_seat_get_pointer (GdkSeat *seat)
{
  return NULL;
}

GdkDevice *
gdk_seat_get_keyboard (GdkSeat *seat)
{
  return NULL;
}
