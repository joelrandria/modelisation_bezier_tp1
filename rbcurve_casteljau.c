#include "rbcurve_casteljau.h"

static void quadruplet_linear_interpolation(Quadruplet* hpoint1, Quadruplet* hpoint2,
					    Quadruplet* hresult, float position)
{
  float origin_offset;

  origin_offset = 1.0f - position;

  hresult->x = hpoint1->x * origin_offset + hpoint2->x * position;
  hresult->y = hpoint1->y * origin_offset + hpoint2->y * position;
  hresult->z = hpoint1->z * origin_offset + hpoint2->z * position;
  hresult->h = hpoint1->h * origin_offset + hpoint2->h * position;
}
static void quadruplet_project(Quadruplet* hpoint, Triplet* result)
{
  result->x = hpoint->x / hpoint->h;
  result->y = hpoint->y / hpoint->h;
  result->z = hpoint->z / hpoint->h;
}

Triplet rbcurve_casteljau(Table_quadruplet* polycontrol,
			  float position,
			  Table_quadruplet* subcurve1_polycontrol,
			  Table_quadruplet* subcurve2_polycontrol)
{
  int i;
  int j;
  int reverse_index;
  Triplet curve_point;
  Table_quadruplet* temp_points;
  Table_quadruplet* current_points;

  // Allocation des tables de travail
  temp_points = malloc_table_quadruplet(polycontrol->nb);
  current_points = malloc_table_quadruplet(polycontrol->nb);

  // Initialisation de la 1ère colonne: Points du polygone de contrôle
  copy_table_quadruplet(polycontrol, current_points);

  if (subcurve1_polycontrol != 0)
    subcurve1_polycontrol->table[0] = current_points->table[0];

  if (subcurve2_polycontrol != 0)
  {
    reverse_index = polycontrol->nb - 1;
    subcurve2_polycontrol->table[reverse_index] = current_points->table[reverse_index];
  }

  // Itération jusqu'à obtention du point final
  for (i = 1; i < polycontrol->nb; ++i)
  {
    for (j = 0; j < polycontrol->nb - i; ++j)
    {
      quadruplet_linear_interpolation(&current_points->table[j],
				      &current_points->table[j + 1],
				      &temp_points->table[j], position);
    }

    copy_table_quadruplet(temp_points, current_points);

    if (subcurve1_polycontrol != 0)
      subcurve1_polycontrol->table[i] = temp_points->table[0];

    if (subcurve2_polycontrol != 0)
    {
      reverse_index = polycontrol->nb - i - 1;
      subcurve2_polycontrol->table[reverse_index] = current_points->table[reverse_index];
    }
  }

  // Projection finale du point obtenu
  quadruplet_project(&current_points->table[0], &curve_point);

  // Libération des tables de travail
  free_table_quadruplet(temp_points);
  free_table_quadruplet(current_points);

  return curve_point;
}
