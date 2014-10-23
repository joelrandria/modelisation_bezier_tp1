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

  table_dst->nb = table_src->nb;

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

void quadruplet_linear_interpolation(Quadruplet* hpoint1, Quadruplet* hpoint2,
				     Quadruplet* hresult, float position)
{
  float origin_offset;

  origin_offset = 1.0f - position;

  hresult->x = hpoint1->x * origin_offset + hpoint2->x * position;
  hresult->y = hpoint1->y * origin_offset + hpoint2->y * position;
  hresult->z = hpoint1->z * origin_offset + hpoint2->z * position;
  hresult->h = hpoint1->h * origin_offset + hpoint2->h * position;
}
void quadruplet_project(Quadruplet* hpoint, Triplet* result)
{
  result->x = hpoint->x / hpoint->h;
  result->y = hpoint->y / hpoint->h;
  result->z = hpoint->z / hpoint->h;
}
