#include "o_rbcurve.h"
#include "o_objet.h"
#include "t_geometrie.h"

#include <GL/gl.h>

#include <stdio.h>

static Triplet rbcurve_point_at(struct rbcurve* curve, float position)
{
  int i;
  int j;
  Triplet curve_point;
  Quadruplet current_hpoint;
  Quadruplet parent_hpoint1;
  Quadruplet parent_hpoint2;
  Table_quadruplet working_vect;
  Table_quadruplet current_matrix_column;

  // Allocation des tables de travail
  current_matrix_column.nb = curve->polycontrol.nb;
  ALLOUER(current_matrix_column.table, current_matrix_column.nb);

  working_vect.nb = curve->polycontrol.nb - 1;
  ALLOUER(working_vect.table, working_vect.nb);

  // Initialisation de la 1ère colonne: Points du polygone de contrôle
  for (i = 0; i < curve->polycontrol.nb; ++i)
    current_matrix_column.table[i] = curve->polycontrol.table[i];

  // Itération jusqu'à obtention de la colonne finale (i.e. du point final)
  for (i = 1; i < curve->polycontrol.nb; ++i)
  {
    for (j = 0; j < curve->polycontrol.nb - i; ++j)
    {
      parent_hpoint1 = current_matrix_column.table[j];
      parent_hpoint2 = current_matrix_column.table[j + 1];

      current_hpoint.x = parent_hpoint1.x * (1 - position) + parent_hpoint2.x * position;
      current_hpoint.y = parent_hpoint1.y * (1 - position) + parent_hpoint2.y * position;
      current_hpoint.z = parent_hpoint1.z * (1 - position) + parent_hpoint2.z * position;
      current_hpoint.h = parent_hpoint1.h * (1 - position) + parent_hpoint2.h * position;

      working_vect.table[j] = current_hpoint;
    }

    for (j = 0; j < curve->polycontrol.nb - i; ++j)
      current_matrix_column.table[j] = working_vect.table[j];
  }

  // Projection finale du point obtenu
  curve_point.x = current_matrix_column.table[0].x / current_matrix_column.table[0].h;
  curve_point.y = current_matrix_column.table[0].y / current_matrix_column.table[0].h;
  curve_point.z = current_matrix_column.table[0].z / current_matrix_column.table[0].h;

  // Libération des tables de travail
  free(current_matrix_column.table);
  free(working_vect.table);

  return curve_point;
}
static Triplet* rbcurve_points(struct rbcurve* curve)
{
  int i;
  float step;
  Triplet* curve_points;

  if (curve->display_point_count <= 2)
    return 0;

  step = 1.0f / (curve->display_point_count - 1);
  ALLOUER(curve_points, curve->display_point_count);

  for (i = 1; i < curve->display_point_count; ++i)
    curve_points[i] = rbcurve_point_at(curve, i * step);

  return curve_points;
}

static void rbcurve_update(struct rbcurve* curve)
{
  if (curve->curve_points.nb > 0)
    free(curve->curve_points.table);

  curve->curve_points.nb = curve->display_point_count;
  curve->curve_points.table = rbcurve_points(curve);
}

static void update(struct rbcurve* curve)
{
  if (!(UN_CHAMP_CHANGE(curve)||CREATION(curve)))
    return ;

  if (CHAMP_CHANGE(curve, polycontrol) || CHAMP_CHANGE(curve, display_point_count))
  {
    if (curve->display_point_count < 2)
      curve->display_point_count = 10;

    rbcurve_update(curve);
  }
}

static void draw(struct rbcurve* curve)
{
  int i;

  glBegin(GL_POINTS);

  // Affichage des points de contrôle
  if (curve->display_polycontrol)
  {
    glColor3f(0, 1, 0);
    for (i = 0; i < curve->polycontrol.nb; ++i)
    {
      glPointSize(2);
      glVertex3f(curve->polycontrol.table[i].x,
		 curve->polycontrol.table[i].y,
		 curve->polycontrol.table[i].z);
    }
  }

  // Affichage des points de la courbe
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
