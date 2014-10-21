#include "rbcurve_rendering.h"

#include <GL/gl.h>

void rbcurve_rendering(struct rbcurve* curve)
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
