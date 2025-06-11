#include <gtk/gtkgrid.h>
#include <string.h>

GType
gtk_grid_get_type (void)
{
#if 0
  return gtk_table_get_type ();
#else
  return GTK_TYPE_TABLE;
#endif
}

GtkWidget *
gtk_grid_new (void)
{
  return gtk_table_new (0, 0, FALSE);
}

#define CHILD_LEFT(child)    ((child)->left_attach)
#define CHILD_RIGHT(child)   ((child)->right_attach)
#define CHILD_TOP(child)     ((child)->top_attach)
#define CHILD_BOTTOM(child)  ((child)->bottom_attach)

typedef GtkTableChild         GtkGridChild;


#if 0 /* if forward memmove is slow */

typedef void* WORD;

static void fwd_memmove(void *_dst, const void *_src, size_t count)
{
  /* dst <= src */
  char *dst = _dst;
  const char *src = _src;

  if (count < 8) { /* too few bytes to copy */
    goto unrolled_copy;
  }

  /* align dst to word size */
  switch ((int)dst & 0x7) {
  case 7:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 6:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 5:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 4:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 3:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 2:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 1:
    *dst++ = *src++;
    count--;
  }

  while(count >= 8) { /* copy word by word */
    *(WORD*)dst++ = *(const WORD*)src++;
    count -= 8;
  }

unrolled_copy:
  /* unrolled copy of the remaining bytes */
  switch (count & 0x7) {
  case 7:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 6:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 5:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 4:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 3:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 2:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 1:
    *dst++ = *src++;
    count--;
  }
}
#endif

static GtkGridChild *
find_grid_child (GtkGrid   *grid,
                 GtkWidget *widget)
{
  GtkGridChild *child;
  GList *list;

  for (list = grid->children; list; list = list->next)
    {
      child = list->data;

      if (child->widget == widget)
        return child;
    }

  return NULL;
}

static gint
find_attach_position (GtkGrid         *grid,
                      GtkOrientation   orientation,
                      gint             op_pos,
                      gint             op_span,
                      gboolean         max)
{
  GtkGridChild *grid_child;
  GList *list;
  gint pos;
  gboolean hit;

  guint16 attach_1;
  guint16 attach_2;

  guint16 opposite_1;
  guint16 opposite_2;

  if (max)
    pos = -G_MAXINT;
  else
    pos = G_MAXINT;

  hit = FALSE;

  for (list = grid->children; list; list = list->next)
    {
      grid_child = list->data;

      if (orientation == GTK_ORIENTATION_HORIZONTAL) {
          attach_1 = grid_child->left_attach;
          attach_2 = grid_child->right_attach;

          opposite_1 = grid_child->top_attach;
          opposite_2 = grid_child->bottom_attach;
      }
      else {
          attach_1 = grid_child->top_attach;
          attach_2 = grid_child->bottom_attach;

          opposite_1 = grid_child->left_attach;
          opposite_2 = grid_child->right_attach;
      }

      /* check if the ranges overlap */
      if (opposite_1 <= op_pos + op_span && op_pos <= opposite_2)
        {
          hit = TRUE;

          if (max)
            pos = MAX (pos, attach_2);
          else
            pos = MIN (pos, attach_1);
        }
     }

  if (!hit)
    pos = 0;

  return pos;
}

void
gtk_grid_attach (GtkGrid   *grid,
                 GtkWidget *child,
                 gint       left,
                 gint       top,
                 gint       width,
                 gint       height)
{
  gtk_table_attach (grid, child,
                    left, left + width,
                    top, top + height,
                    GTK_EXPAND | GTK_FILL, /* from gtk_table_attach_defaults */
                    GTK_EXPAND | GTK_FILL,
                    width, height);
}

/**
 * gtk_grid_attach_next_to:
 * @grid: a #GtkGrid
 * @child: the widget to add
 * @sibling: (allow-none): the child of @grid that @child will be placed
 *     next to, or %NULL to place @child at the beginning or end
 * @side: the side of @sibling that @child is positioned next to
 * @width: the number of columns that @child will span
 * @height: the number of rows that @child will span
 *
 * Adds a widget to the grid.
 *
 * The widget is placed next to @sibling, on the side determined by
 * @side. When @sibling is %NULL, the widget is placed in row (for
 * left or right placement) or column 0 (for top or bottom placement),
 * at the end indicated by @side.
 *
 * Attaching widgets labeled [1], [2], [3] with @sibling == %NULL and
 * @side == %GTK_POS_LEFT yields a layout of [3][2][1].
 */
void
gtk_grid_attach_next_to (GtkGrid         *grid,
                         GtkWidget       *child,
                         GtkWidget       *sibling,
                         GtkPositionType  side,
                         gint             width,
                         gint             height)
{
  GtkGridChild *grid_sibling;
  gint left, top;

  g_return_if_fail (GTK_IS_GRID (grid));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (gtk_widget_get_parent (child) == NULL);
  g_return_if_fail (sibling == NULL || gtk_widget_get_parent (sibling) == (GtkWidget*)grid);
  g_return_if_fail (width > 0);
  g_return_if_fail (height > 0);

  if (sibling)
    {
      grid_sibling = find_grid_child (grid, sibling);

      switch (side)
        {
        case GTK_POS_LEFT:
          left = CHILD_LEFT (grid_sibling) - width;
          top = CHILD_TOP (grid_sibling);
          break;
        case GTK_POS_RIGHT:
          left = CHILD_RIGHT (grid_sibling);
          top = CHILD_TOP (grid_sibling);
          break;
        case GTK_POS_TOP:
          left = CHILD_LEFT (grid_sibling);
          top = CHILD_TOP (grid_sibling) - height;
          break;
        case GTK_POS_BOTTOM:
          left = CHILD_LEFT (grid_sibling);
          top = CHILD_BOTTOM (grid_sibling);
          break;
        default:
          g_assert_not_reached ();
        }
    }
  else
    {
      switch (side)
        {
        case GTK_POS_LEFT:
          left = find_attach_position (grid, GTK_ORIENTATION_HORIZONTAL, 0, height, FALSE);
          left -= width;
          top = 0;
          break;
        case GTK_POS_RIGHT:
          left = find_attach_position (grid, GTK_ORIENTATION_HORIZONTAL, 0, height, TRUE);
          top = 0;
          break;
        case GTK_POS_TOP:
          left = 0;
          top = find_attach_position (grid, GTK_ORIENTATION_VERTICAL, 0, width, FALSE);
          top -= height;
          break;
        case GTK_POS_BOTTOM:
          left = 0;
          top = find_attach_position (grid, GTK_ORIENTATION_VERTICAL, 0, width, TRUE);
          break;
        default:
          g_assert_not_reached ();
        }
    }

  gtk_grid_attach (grid, child, left, top, width, height);
}

/**
 * gtk_grid_get_child_at:
 * @grid: a #GtkGrid
 * @left: the left edge of the cell
 * @top: the top edge of the cell
 *
 * Gets the child of @grid whose area covers the grid
 * cell whose upper left corner is at @left, @top.
 *
 * Returns: (transfer none) (nullable): the child at the given position, or %NULL
 *
 * Since: 3.2
 */
GtkWidget *
gtk_grid_get_child_at (GtkGrid *grid,
                       gint     left,
                       gint     top)
{
  GtkGridChild *child;
  GList *list;

  g_return_val_if_fail (GTK_IS_GRID (grid), NULL);

  for (list = grid->children; list; list = list->next)
    {
      child = list->data;

      if (CHILD_LEFT (child) <= left &&
          CHILD_RIGHT (child) > left &&
          CHILD_TOP (child) <= top &&
          CHILD_BOTTOM (child) > top)
        return child->widget;
    }

  return NULL;
}

/**
 * gtk_grid_insert_row:
 * @grid: a #GtkGrid
 * @position: the position to insert the row at
 *
 * Inserts a row at the specified position.
 *
 * Children which are attached at or below this position
 * are moved one row down. Children which span across this
 * position are grown to span the new row.
 *
 * Since: 3.2
 */
void
gtk_grid_insert_row (GtkGrid *grid,
                     gint     position)
{
  GtkGridChild *child;
  GList *list;

  g_return_if_fail (GTK_IS_GRID (grid));

  if (position > grid->nrows) {
    g_warning ("gtk3-to-gtk2 warning: gtk_grid_insert_row called with position: %d, but grid has only %d rows",
               position, grid->nrows);
    position = grid->nrows;
  }

  gtk_table_resize (grid, grid->nrows + 1, grid->ncols);

  for (list = grid->children; list; list = list->next)
    {
      child = list->data;

      if (CHILD_TOP (child) >= position)
        {
          gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                   "top-attach", CHILD_TOP (child) + 1,
                                   NULL);
        }
      else if (CHILD_BOTTOM (child) > position)
        {
          gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                   "bottom-attach", CHILD_BOTTOM (child) + 1,
                                   NULL);
        }
    }

  memmove(grid->rows + position + 1, grid->rows + position, (grid->nrows - position - 1) * sizeof (GtkTableRowCol));
  grid->rows[position] = (GtkTableRowCol){.spacing = grid->row_spacing};
}

/**
 * gtk_grid_remove_row:
 * @grid: a #GtkGrid
 * @position: the position of the row to remove
 *
 * Removes a row from the grid.
 *
 * Children that are placed in this row are removed,
 * spanning children that overlap this row have their
 * height reduced by one, and children below the row
 * are moved up.
 *
 * Since: 3.10
 */
void
gtk_grid_remove_row (GtkGrid *grid,
                     gint     position)
{
  GtkGridChild *child;
  GList *list;
  gint top, bottom;

  g_return_if_fail (GTK_IS_GRID (grid));

  if (position > grid->nrows - 1) {
    g_warning ("gtk3-to-gtk2 warning: gtk_grid_remove_row called with position: %d, but grid has only %d rows",
               position, grid->nrows);
    position = grid->nrows - 1;
  }

  list = grid->children;
  while (list)
    {
      child = list->data;
      list = list->next;

      top = CHILD_TOP (child);
      bottom = CHILD_BOTTOM (child);

      if (top <= position && bottom > position)
        bottom--;
      if (top > position)
        top--;

      if (bottom <= top)
        gtk_container_remove (GTK_CONTAINER (grid), child->widget);
      else
        gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                 "bottom-attach", bottom,
                                 "top-attach", top,
                                 NULL);
    }

  memmove (grid->rows + position, grid->rows + position + 1, (grid->nrows - position - 1) * sizeof (GtkTableRowCol));
  gtk_table_resize (grid, grid->nrows - 1, grid->ncols);
}

/**
 * gtk_grid_insert_column:
 * @grid: a #GtkGrid
 * @position: the position to insert the column at
 *
 * Inserts a column at the specified position.
 *
 * Children which are attached at or to the right of this position
 * are moved one column to the right. Children which span across this
 * position are grown to span the new column.
 *
 * Since: 3.2
 */
void
gtk_grid_insert_column (GtkGrid *grid,
                        gint     position)
{
  GtkGridChild *child;
  GList *list;

  g_return_if_fail (GTK_IS_GRID (grid));

  if (position > grid->ncols) {
    g_warning ("gtk3-to-gtk2 warning: gtk_grid_insert_column called with position: %d, but grid has only %d columns",
               position, grid->ncols);
    position = grid->ncols;
  }

  gtk_table_resize (grid, grid->nrows, grid->ncols + 1);

  for (list = grid->children; list; list = list->next)
    {
      child = list->data;

      if (CHILD_LEFT (child) >= position)
        {
          gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                   "left-attach", CHILD_LEFT (child) + 1,
                                   NULL);
        }
      else if (CHILD_RIGHT (child) > position)
        {
          gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                   "right-attach", CHILD_RIGHT (child) + 1,
                                   NULL);
        }
    }

  memmove(grid->cols + position + 1, grid->cols + position, (grid->ncols - position - 1) * sizeof (GtkTableRowCol));
  grid->cols[position] = (GtkTableRowCol){.spacing = grid->column_spacing};
}

/**
 * gtk_grid_remove_column:
 * @grid: a #GtkGrid
 * @position: the position of the column to remove
 *
 * Removes a column from the grid.
 *
 * Children that are placed in this column are removed,
 * spanning children that overlap this column have their
 * width reduced by one, and children after the column
 * are moved to the left.
 *
 * Since: 3.10
 */
void
gtk_grid_remove_column (GtkGrid *grid,
                        gint     position)
{
  GtkGridChild *child;
  GList *list;
  gint left, right;

  g_return_if_fail (GTK_IS_GRID (grid));

  if (position > grid->ncols - 1) {
    g_warning ("gtk3-to-gtk2 warning: gtk_grid_remove_column called with position: %d, but grid has only %d columns",
               position, grid->ncols);
    position = grid->ncols - 1;
  }

  list = grid->children;
  while (list)
    {
      child = list->data;
      list = list->next;

      left = CHILD_LEFT (child);
      right = CHILD_RIGHT (child);

      if (left <= position && right > position)
        right--;
      if (left > position)
        left--;

      if (right <= left)
        gtk_container_remove (GTK_CONTAINER (grid), child->widget);
      else
        gtk_container_child_set (GTK_CONTAINER (grid), child->widget,
                                 "right-attach", right,
                                 "left-attach", left,
                                 NULL);
    }

  memmove(grid->cols + position, grid->cols + position + 1, (grid->ncols - position - 1) * sizeof (GtkTableRowCol));
  gtk_table_resize (grid, grid->nrows, grid->ncols - 1);
}

/**
 * gtk_grid_insert_next_to:
 * @grid: a #GtkGrid
 * @sibling: the child of @grid that the new row or column will be
 *     placed next to
 * @side: the side of @sibling that @child is positioned next to
 *
 * Inserts a row or column at the specified position.
 *
 * The new row or column is placed next to @sibling, on the side
 * determined by @side. If @side is %GTK_POS_TOP or %GTK_POS_BOTTOM,
 * a row is inserted. If @side is %GTK_POS_LEFT of %GTK_POS_RIGHT,
 * a column is inserted.
 *
 * Since: 3.2
 */
void
gtk_grid_insert_next_to (GtkGrid         *grid,
                         GtkWidget       *sibling,
                         GtkPositionType  side)
{
  GtkGridChild *child;

  g_return_if_fail (GTK_IS_GRID (grid));
  g_return_if_fail (GTK_IS_WIDGET (sibling));
  g_return_if_fail (gtk_widget_get_parent (sibling) == (GtkWidget*)grid);

  child = find_grid_child (grid, sibling);

  switch (side)
    {
    case GTK_POS_LEFT:
      gtk_grid_insert_column (grid, CHILD_LEFT (child));
      break;
    case GTK_POS_RIGHT:
      gtk_grid_insert_column (grid, CHILD_RIGHT (child));
      break;
    case GTK_POS_TOP:
      gtk_grid_insert_row (grid, CHILD_TOP (child));
      break;
    case GTK_POS_BOTTOM:
      gtk_grid_insert_row (grid, CHILD_BOTTOM (child));
      break;
    default:
      g_assert_not_reached ();
    }
}

void
gtk_grid_set_row_homogeneous (GtkGrid  *grid,
                              gboolean  homogeneous)
{
  gtk_table_set_homogeneous (grid, homogeneous);
}

gboolean
gtk_grid_get_row_homogeneous (GtkGrid *grid)
{
  return gtk_table_get_homogeneous (grid);
}

void
gtk_grid_set_column_homogeneous (GtkGrid  *grid,
                                 gboolean  homogeneous)
{
  gtk_table_set_homogeneous (grid, homogeneous);
}

gboolean
gtk_grid_get_column_homogeneous (GtkGrid *grid)
{
  return gtk_table_get_homogeneous (grid);
}

void
gtk_grid_set_row_spacing (GtkGrid *grid,
                          guint    spacing)
{
  gtk_table_set_row_spacings (grid, spacing);
}

void
gtk_grid_set_column_spacing (GtkGrid *grid,
                             guint    spacing)
{
  gtk_table_set_col_spacings (grid, spacing);
}

guint
gtk_grid_get_row_spacing (GtkGrid *grid)
{
  return gtk_table_get_default_row_spacing (grid);
}

guint
gtk_grid_get_column_spacing (GtkGrid *grid)
{
  return gtk_table_get_default_col_spacing (grid);
}

void
gtk_grid_set_row_baseline_position (GtkGrid            *grid,
                                    gint                row,
                                    GtkBaselinePosition pos)
{
  /* Not Implemented */
}

GtkBaselinePosition
gtk_grid_get_row_baseline_position (GtkGrid      *grid,
                                    gint          row)
{
  /* Not Implemented */
  return GTK_BASELINE_POSITION_CENTER;
}

void
gtk_grid_set_baseline_row (GtkGrid *grid,
                           gint     row)
{
  /* Not Implemented */
}

gint
gtk_grid_get_baseline_row (GtkGrid *grid)
{
  /* Not Implemented */
  return 0;
}
