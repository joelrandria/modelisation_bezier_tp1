#ifndef __O_RBCURVE_H__
#define __O_RBCURVE_H__

#include "o_objet.h"
#include "t_geometrie.h"

/**
 * Courbe de Bézier rationnelle.
 */
struct rbcurve
{
  // --- Champs accessibles par l'utilisateur ---

  Table_quadruplet polycontrol;	// Points 4D homogènes définissant le polygone de contrôle
  Booleen display_polycontrol;	// Bascule d'affichage du polygone de contrôle
  int display_point_count;	// Nombre de points de la courbe à afficher

  // --- Interne ---

  Table_triplet curve_points;	// Points 3D de la courbe à afficher
};

#endif
