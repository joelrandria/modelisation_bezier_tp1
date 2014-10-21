#ifndef __RBCURVE_H__
#define __RBCURVE_H__

#include "o_objet.h"
#include "t_geometrie.h"

/**
 * Définit une courbe de Bézier rationnelle paramétrisable.
 */
struct rbcurve
{
  // --- Courbe initiale ---

  Table_quadruplet base_curve_polygon;	// Polygone de contrôle initial
  Table_triplet base_curve_points;	// Points de la courbe initiale à afficher
  Booleen display_base_curve_polygon;	// Bascule d'affichage du polygone de contrôle initial
  int base_curve_point_count;		// Nombre de points de la courbe initiale à afficher

  // --- Courbe paramétrée ---

  Flottant param_range_start;		// Début de l'intervalle de restriction paramétrique
  Flottant param_range_end;		// Fin de l'intervalle de restriction paramétrique
  Table_quadruplet param_curve_polygon;	// Polygone de contrôle de la courbe paramétrée

  Table_triplet param_curve_points;
  Booleen display_param_curve_polygon;
  int param_curve_point_count;

};

#endif
