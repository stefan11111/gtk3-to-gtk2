#include <gtk/gtkcomboboxtext.h>

/**
 * gtk_combo_box_text_remove_all:
 * @combo_box: A #GtkComboBoxText
 *
 * Removes all the text entries from the combo box.
 *
 * Since: 3.0
 */
void
gtk_combo_box_text_remove_all (GtkComboBoxText *combo_box)
{
  GtkListStore *store;

  g_return_if_fail (GTK_IS_COMBO_BOX_TEXT (combo_box));

  store = GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box)));
  gtk_list_store_clear (store);
}

/**
 * gtk_combo_box_text_insert:
 * @combo_box: A #GtkComboBoxText
 * @position: An index to insert @text
 * @id: (allow-none): a string ID for this value, or %NULL
 * @text: A string to display
 *
 * Inserts @text at @position in the list of strings stored in @combo_box.
 * If @id is non-%NULL then it is used as the ID of the row.  See
 * #GtkComboBox:id-column.
 *
 * If @position is negative then @text is appended.
 *
 * Since: 3.0
 */
void
gtk_combo_box_text_insert (GtkComboBoxText *combo_box,
                           gint             position,
                           const gchar     *id,
                           const gchar     *text)
{
  GtkListStore *store;
  GtkTreeIter iter;
  gint text_column;
  gint column_type;

  g_return_if_fail (GTK_IS_COMBO_BOX_TEXT (combo_box));
  g_return_if_fail (text != NULL);

  store = GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box)));
  g_return_if_fail (GTK_IS_LIST_STORE (store));

  text_column = gtk_combo_box_get_entry_text_column (GTK_COMBO_BOX (combo_box));

  if (gtk_combo_box_get_has_entry (GTK_COMBO_BOX (combo_box)))
    g_return_if_fail (text_column >= 0);
  else if (text_column < 0)
    text_column = 0;

  column_type = gtk_tree_model_get_column_type (GTK_TREE_MODEL (store), text_column);
  g_return_if_fail (column_type == G_TYPE_STRING);

  if (position < 0)
    gtk_list_store_append (store, &iter);
  else
    gtk_list_store_insert (store, &iter, position);

  gtk_list_store_set (store, &iter, text_column, text, -1);

#if 0 /* gtk_combo_box_get_id_column not implemented */
  if (id != NULL)
    {
      gint id_column;

      id_column = gtk_combo_box_get_id_column (GTK_COMBO_BOX (combo_box));
      g_return_if_fail (id_column >= 0);
      column_type = gtk_tree_model_get_column_type (GTK_TREE_MODEL (store), id_column);
      g_return_if_fail (column_type == G_TYPE_STRING);

      gtk_list_store_set (store, &iter, id_column, id, -1);
    }
#endif
}

/**
 * gtk_combo_box_text_append:
 * @combo_box: A #GtkComboBoxText
 * @id: (allow-none): a string ID for this value, or %NULL
 * @text: A string
 *
 * Appends @text to the list of strings stored in @combo_box.
 * If @id is non-%NULL then it is used as the ID of the row.
 *
 * This is the same as calling gtk_combo_box_text_insert() with a
 * position of -1.
 *
 * Since: 2.24
 */
void
gtk_combo_box_text_append (GtkComboBoxText *combo_box,
                           const gchar     *id,
                           const gchar     *text)
{
  gtk_combo_box_text_insert (combo_box, -1, id, text);
}

/**
 * gtk_combo_box_text_prepend:
 * @combo_box: A #GtkComboBox
 * @id: (allow-none): a string ID for this value, or %NULL
 * @text: a string
 *
 * Prepends @text to the list of strings stored in @combo_box.
 * If @id is non-%NULL then it is used as the ID of the row.
 *
 * This is the same as calling gtk_combo_box_text_insert() with a
 * position of 0.
 *
 * Since: 2.24
 */
void
gtk_combo_box_text_prepend (GtkComboBoxText *combo_box,
                            const gchar     *id,
                            const gchar     *text)
{
  gtk_combo_box_text_insert (combo_box, 0, id, text);
}
