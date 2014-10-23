#include "rbcurve_rendering.h"

#include <GL/gl.h>

/**
 * Dessine la polyligne spécifiée.
 *
 * @polyline: Pointeur vers une table de quadruplets définissant la polyligne à dessiner.
 */
void rbcurve_render_polyline(Table_quadruplet* polyline)
{
  int i;

  glBegin(GL_LINE_STRIP);
  for (i = 0; i < polyline->nb; ++i)
  {
    glVertex3f(polyline->table[i].x,
	       polyline->table[i].y,
	       polyline->table[i].z);
  }
  glEnd();
}
/**
 * Dessine les points spécifiés.
 *
 * @points: Pointeur vers la table de triplets contenant les points à dessiner.
 */
void rbcurve_render_points(Table_triplet* points)
{
  int i;

  glBegin(GL_POINTS);
  for (i = 0; i < points->nb; ++i)
  {
    glVertex3f(points->table[i].x,
  	       points->table[i].y,
  	       points->table[i].z);
  }
  glEnd();
}

void rbcurve_rendering(struct rbcurve* curve)
{
  // Affichage du polygone de contrôle de la courbe initiale en vert
  if (curve->display_base_curve_polygon)
  {
    glColor3f(0, 1, 0);
    rbcurve_render_polyline(&curve->base_curve_polygon);
  }

  // Affichage des points de la courbe initiale en blanc
  glColor3f(1, 1, 1);
  rbcurve_render_points(&curve->base_curve_points);

  // Affichage du polygone de contrôle de la courbe paramétrée en bleu
  if (curve->display_param_curve_polygon)
  {
    glColor3f(0, 0, 1);
    rbcurve_render_polyline(&curve->param_curve_polygon);
  }

  // Affichage des points de la courbe paramétrée en bleu
  glColor3f(0, 0, 1);
  rbcurve_render_points(&curve->param_curve_points);
}
