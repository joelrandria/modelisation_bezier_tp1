#include "o_rbcurve.h"
#include "o_objet.h"
#include "t_geometrie.h"

#include <GL/gl.h>

#include <stdio.h>

static Table_quadruplet* malloc_table_quadruplet(int size)
{
  Table_quadruplet* table;

  ALLOUER(table, 1);

  table->nb = size;
  ALLOUER(table->table, table->nb);

  return table;
}
static void copy_table_quadruplet(Table_quadruplet* table_src, Table_quadruplet* table_dst)
{
  int i;

  for (i = 0; i < table_src->nb; ++i)
    table_dst->table[i] = table_src->table[i];
}
static void free_table_quadruplet(Table_quadruplet* table)
{
  if (table == 0)
    return;

  if (table->nb > 0)
    free(table->table);

  free(table);
}

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

static Triplet rbcurve_casteljau(Table_quadruplet* polycontrol,
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
static Triplet* rbcurve_compute_display_points(struct rbcurve* curve)
{
  int i;
  float step;
  Triplet* curve_points;

  if (curve->display_point_count <= 2)
    return 0;

  step = 1.0f / (curve->display_point_count - 1);
  ALLOUER(curve_points, curve->display_point_count);

  for (i = 1; i < curve->display_point_count; ++i)
    curve_points[i] = rbcurve_casteljau(&curve->param_polycontrol, i * step, 0, 0);

  return curve_points;
}
static void rbcurve_update_display_points(struct rbcurve* curve)
{
  if (curve->curve_points.nb > 0)
    free(curve->curve_points.table);

  curve->curve_points.nb = curve->display_point_count;
  curve->curve_points.table = rbcurve_compute_display_points(curve);
}
static void rbcurve_update_param_polycontrol(struct rbcurve* curve)
{
  Flottant new_range_end;

  if (curve->param_polycontrol.nb > 0)
    free(curve->param_polycontrol.table);

  curve->param_polycontrol.nb = curve->polycontrol.nb;
  ALLOUER(curve->param_polycontrol.table, curve->param_polycontrol.nb);

  new_range_end = (curve->param_range_end - curve->param_range_start) / (1 - curve->param_range_start);

  rbcurve_casteljau(&curve->polycontrol, curve->param_range_start, 0, &curve->param_polycontrol);
  rbcurve_casteljau(&curve->param_polycontrol, new_range_end, &curve->param_polycontrol, 0);

  rbcurve_update_display_points(curve);
}

static void update(struct rbcurve* curve)
{
  if (!(UN_CHAMP_CHANGE(curve)||CREATION(curve)))
    return ;

  if (CHAMP_CHANGE(curve, polycontrol))
  {
    if (curve->display_point_count < 2)
      curve->display_point_count = 10;

    rbcurve_update_param_polycontrol(curve);
  }
  if (CHAMP_CHANGE(curve, param_range_start) || CHAMP_CHANGE(curve, param_range_end))
  {
    if ((curve->param_range_start < 0) || (curve->param_range_end > 1)
	|| (curve->param_range_start >= curve->param_range_end))
    {
      curve->param_range_start = 0.0f;
      curve->param_range_end = 1.0f;
    }

    rbcurve_update_param_polycontrol(curve);
  }
  if (CHAMP_CHANGE(curve, display_point_count))
  {
    rbcurve_update_display_points(curve);
  }
}

static void draw(struct rbcurve* curve)
{
  int i;

  // Affichage des points de contrôle
  if (curve->display_polycontrol)
  {
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1, 0);
    for (i = 0; i < curve->polycontrol.nb; ++i)
    {
      glPointSize(2);
      glVertex3f(curve->polycontrol.table[i].x,
  		 curve->polycontrol.table[i].y,
  		 curve->polycontrol.table[i].z);
    }
    glEnd();
  }

  // Affichage des points de la courbe
  glBegin(GL_POINTS);
  glColor3f(1, 1, 1);
  for (i = 0; i < curve->curve_points.nb; ++i)
  {
    glPointSize(2);
    glVertex3f(curve->curve_points.table[i].x,
  	       curve->curve_points.table[i].y,
  	       curve->curve_points.table[i].z);
  }
  glEnd();
}

CLASSE(rbcurve, struct rbcurve,

       CHAMP(polycontrol,
	     LABEL("Polygone de contrôle")
	     L_table_point P_table_quadruplet
	     Extrait Obligatoire Affiche Edite Sauve)

       //
       CHAMP(param_polycontrol,
	     LABEL("Polygone de contrôle re-paramétré")
	     L_table_point P_table_quadruplet
	     Affiche)
       //

       CHAMP(param_range_start,
	     LABEL("Début de l'intervalle de paramétrisation:")
	     L_flottant P_flottant
	     Extrait Affiche Edite Sauve DEFAUT("0.0"))
       CHAMP(param_range_end,
	     LABEL("Fin de l'intervalle de paramétrisation:")
	     L_flottant P_flottant
	     Extrait Affiche Edite Sauve DEFAUT("1.0"))

       CHAMP(display_polycontrol,
	     LABEL("Affichage du polygone de contrôle")
	     L_booleen Edite DEFAUT("1"))
       CHAMP(display_point_count,
	     LABEL("Nombre de points à afficher")
             L_entier  Edite Sauve DEFAUT("10"))

       CHANGEMENT(update)

       CHAMP_VIRTUEL(L_affiche_gl(draw))

       MENU("Jo/Rationnal B-Curve")

       EVENEMENT("Ctrl+RB")
       )
