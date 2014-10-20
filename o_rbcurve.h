#ifndef __O_RBCURVE_H__
#define __O_RBCURVE_H__

#include "o_objet.h"
#include "t_geometrie.h"

/**
 * Courbe de Bézier rationnelle.
 */
struct rbcurve
{
  // --- Polygone de contrôle définissant la courbe initiale ---

  Table_quadruplet polycontrol;

  // --- Paramétrisation ---

  Flottant param_range_start;		// Début de l'intervalle de restriction paramétrique
  Flottant param_range_end;		// Fin de l'intervalle de restriction paramétrique

  Table_quadruplet param_polycontrol;	// Polygone de contrôle de la courbe paramétrée

  // --- Options d'affichage

  Booleen display_polycontrol;	// Bascule d'affichage du polygone de contrôle
  int display_point_count;	// Nombre de points de la courbe à afficher

  // --- Interne ---

  Table_triplet curve_points;	// Points 3D de la courbe à afficher
};

#endif
