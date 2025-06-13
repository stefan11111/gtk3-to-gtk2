#include <gtk/gtkentry.h>

/**
 * gtk_entry_grab_focus_without_selecting:
 * @entry: a #GtkEntry
 *
 * Causes @entry to have keyboard focus.
 *
 * It behaves like gtk_widget_grab_focus(),
 * except that it doesn't select the contents of the entry.
 * You only want to call this on some special entries
 * which the user usually doesn't want to replace all text in,
 * such as search-as-you-type entries.
 *
 * Since: 3.16
 */
void
gtk_entry_grab_focus_without_selecting (GtkEntry *entry)
{
  g_return_if_fail (GTK_IS_ENTRY (entry));
  gtk2_gtk_entry_grab_focus (GTK_WIDGET (entry));
}
