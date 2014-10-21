#include "rbcurve_rendering.h"

#include <GL/gl.h>

void rbcurve_render_polygon(Table_quadruplet* polygon)
{
  int i;

  glBegin(GL_LINE_STRIP);
  for (i = 0; i < polygon->nb; ++i)
  {
    glPointSize(2);
    glVertex3f(polygon->table[i].x,
	       polygon->table[i].y,
	       polygon->table[i].z);
  }
  glEnd();
}
void rbcurve_render_points(Table_triplet* points, int point_size)
{
  int i;

  glBegin(GL_POINTS);
  for (i = 0; i < points->nb; ++i)
  {
    glPointSize(point_size);
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
    rbcurve_render_polygon(&curve->base_curve_polygon);
  }

  // Affichage des points de la courbe initiale en blanc
  glColor3f(1, 1, 1);
  rbcurve_render_points(&curve->base_curve_points, 1);

  // Affichage du polygone de contrôle de la courbe paramétrée en bleu
  if (curve->display_param_curve_polygon)
  {
    glColor3f(0, 0, 1);
    rbcurve_render_polygon(&curve->param_curve_polygon);
  }

  // Affichage des points de la courbe paramétrée en bleu
  glColor3f(0, 0, 1);
  rbcurve_render_points(&curve->param_curve_points, 2);
}
