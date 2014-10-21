#ifndef __NE_UTILS_H__
#define __NE_UTILS_H__

#include "o_objet.h"
#include "t_geometrie.h"

Table_quadruplet* malloc_table_quadruplet(int size);
void copy_table_quadruplet(Table_quadruplet* table_src, Table_quadruplet* table_dst);
void free_table_quadruplet(Table_quadruplet* table);

#endif
