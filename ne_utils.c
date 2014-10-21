#include "ne_utils.h"

Table_quadruplet* malloc_table_quadruplet(int size)
{
  Table_quadruplet* table;

  ALLOUER(table, 1);

  table->nb = size;
  ALLOUER(table->table, table->nb);

  return table;
}
void copy_table_quadruplet(Table_quadruplet* table_src, Table_quadruplet* table_dst)
{
  int i;

  for (i = 0; i < table_src->nb; ++i)
    table_dst->table[i] = table_src->table[i];
}
void free_table_quadruplet(Table_quadruplet* table)
{
  if (table == 0)
    return;

  if (table->nb > 0)
    free(table->table);

  free(table);
}
