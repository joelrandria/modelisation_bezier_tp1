#include "o_objet.h"
#include "t_geometrie.h"

#include <GL/gl.h>

#include <stdio.h>

struct rbcurve
{
  Table_quadruplet polycontrol;
  int display_point_count;

  Table_triplet curve_points;
};

static Triplet rbcurve_point_at(struct rbcurve* curve, float position)
{
  int i;
  int j;
  Triplet curve_point;
  Grille_quadruplet hpoint_grid;
  Quadruplet parent_hpoint1;
  Quadruplet parent_hpoint2;
  Quadruplet current_hpoint;
  Quadruplet curve_hpoint;

  // Allocation de la matrice de travail
  hpoint_grid.nb_lignes = curve->polycontrol.nb;
  hpoint_grid.nb_colonnes = curve->polycontrol.nb;

  hpoint_grid.grille = malloc(sizeof(Quadruplet*) * hpoint_grid.nb_lignes);
  for (i = 0; i < hpoint_grid.nb_lignes; ++i)
    hpoint_grid.grille[i] = malloc(sizeof(Quadruplet) * hpoint_grid.nb_colonnes);

  // Initialisation de la 1ère colonne: Points du polygone de contrôle
  for (i = 0; i < curve->polycontrol.nb; ++i)
    hpoint_grid.grille[i][0] = curve->polycontrol.table[i];

  for (i = 1; i < hpoint_grid.nb_colonnes; ++i)
  {
    for (j = 0; j < hpoint_grid.nb_lignes - i; ++j)
    {
      parent_hpoint1 = hpoint_grid.grille[j][i - 1];
      parent_hpoint2 = hpoint_grid.grille[j + 1][i - 1];

      current_hpoint.x = parent_hpoint1.x * (1 - position) + parent_hpoint2.x * position;
      current_hpoint.y = parent_hpoint1.y * (1 - position) + parent_hpoint2.y * position;
      current_hpoint.z = parent_hpoint1.z * (1 - position) + parent_hpoint2.z * position;
      current_hpoint.h = parent_hpoint1.h * (1 - position) + parent_hpoint2.h * position;

      hpoint_grid.grille[j][i] = current_hpoint;
    }
  }

  // ToDo: Libération de la matrice de travail

  curve_hpoint = hpoint_grid.grille[0][hpoint_grid.nb_colonnes - 1];

  curve_point.x = curve_hpoint.x / curve_hpoint.h;
  curve_point.y = curve_hpoint.y / curve_hpoint.h;
  curve_point.z = curve_hpoint.z / curve_hpoint.h;

  printf("c(u=%f)=<%f,%f,%f>\r\n", position, curve_point.x, curve_point.y, curve_point.z);

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

  printf("----- Polygone de contrôle-----\r\n");
  for (i = 0; i < curve->polycontrol.nb; ++i)
    printf("<%f,%f,%f>\r\n",
	   curve->polycontrol.table[i].x,
	   curve->polycontrol.table[i].y,
	   curve->polycontrol.table[i].z);
  printf("-------------------------------\r\n");

  for (i = 1; i < curve->display_point_count; ++i)
    curve_points[i] = rbcurve_point_at(curve, i * step);

  return curve_points;
}

static void rbcurve_update(struct rbcurve* curve)
{
  printf("\r\n\r\n======= rbcurve_update() =======\r\n\r\n");

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
  glColor3f(0, 1, 0);

  for (i = 0; i < curve->polycontrol.nb; ++i)
  {
    glPointSize(2);
    glVertex3f(curve->polycontrol.table[i].x,
  	       curve->polycontrol.table[i].y,
  	       curve->polycontrol.table[i].z);
  }

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
       CHAMP(display_point_count,
	     LABEL("Nombre de points à afficher")
             L_entier  Edite Sauve DEFAUT("10"))

       CHANGEMENT(update)

       CHAMP_VIRTUEL(L_affiche_gl(draw))

       MENU("Jo/Rationnal B-Curve")
       EVENEMENT("Ctrl+RB")
       )
