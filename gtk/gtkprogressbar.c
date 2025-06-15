#include <gtk/gtkprogressbar.h>

/**
 * gtk_progress_bar_get_inverted:
 * @pbar: a #GtkProgressBar
 *
 * Gets the value set by gtk_progress_bar_set_inverted().
 *
 * Returns: %TRUE if the progress bar is inverted
 */
gboolean
gtk_progress_bar_get_inverted (GtkProgressBar *pbar)
{
  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), FALSE);

  return (pbar->orientation == GTK_PROGRESS_RIGHT_TO_LEFT) ||
         (pbar->orientation == GTK_PROGRESS_TOP_TO_BOTTOM);
}

/**
 * gtk_progress_bar_set_inverted:
 * @pbar: a #GtkProgressBar
 * @inverted: %TRUE to invert the progress bar
 *
 * Progress bars normally grow from top to bottom or left to right.
 * Inverted progress bars grow in the opposite direction.
 */
void
gtk_progress_bar_set_inverted (GtkProgressBar *pbar,
                               gboolean        inverted)
{
  g_return_if_fail (GTK_IS_PROGRESS_BAR (pbar));
  if (inverted) {
    switch (pbar->orientation) {
    case GTK_PROGRESS_LEFT_TO_RIGHT:
        gtk_progress_bar_set_orientation (pbar, GTK_PROGRESS_RIGHT_TO_LEFT);
        return;
    case GTK_PROGRESS_BOTTOM_TO_TOP:
        gtk_progress_bar_set_orientation (pbar, GTK_PROGRESS_TOP_TO_BOTTOM);
        return;
    default: /* silence gcc */
        return;
    }
  } else {
    switch (pbar->orientation) {
    case GTK_PROGRESS_RIGHT_TO_LEFT:
        gtk_progress_bar_set_orientation (pbar, GTK_PROGRESS_LEFT_TO_RIGHT);
        return;
    case GTK_PROGRESS_TOP_TO_BOTTOM:
        gtk_progress_bar_set_orientation (pbar, GTK_PROGRESS_BOTTOM_TO_TOP);
        return;
    default: /* silence gcc */
        return;
    }
  }
}

/**
 * gtk_progress_bar_set_show_text:
 * @pbar: a #GtkProgressBar
 * @show_text: whether to show text
 *
 * Sets whether the progress bar will show text next to the bar.
 * The shown text is either the value of the #GtkProgressBar:text
 * property or, if that is %NULL, the #GtkProgressBar:fraction value,
 * as a percentage.
 *
 * To make a progress bar that is styled and sized suitably for containing
 * text (even if the actual text is blank), set #GtkProgressBar:show-text to
 * %TRUE and #GtkProgressBar:text to the empty string (not %NULL).
 *
 * Since: 3.0
 */
void
gtk_progress_bar_set_show_text (GtkProgressBar *pbar,
                                gboolean        show_text)
{
  gtk_progress_set_show_text (GTK_PROGRESS (pbar), show_text);
}

/**
 * gtk_progress_bar_get_show_text:
 * @pbar: a #GtkProgressBar
 *
 * Gets the value of the #GtkProgressBar:show-text property.
 * See gtk_progress_bar_set_show_text().
 *
 * Returns: %TRUE if text is shown in the progress bar
 *
 * Since: 3.0
 */
gboolean
gtk_progress_bar_get_show_text (GtkProgressBar *pbar)
{
  GtkProgress *progress = GTK_PROGRESS (pbar);

  g_return_val_if_fail (GTK_IS_PROGRESS_BAR (pbar), FALSE);

  return progress->show_text;
}
