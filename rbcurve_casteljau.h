/**
 * Fournit une implémentation de l'algorithme de Casteljau.
 */
#ifndef __RBCURVE_CASTELJAU_H__
#define __RBCURVE_CASTELJAU_H__

#include "o_objet.h"
#include "t_geometrie.h"

/**
 * Implémentation de l'algorithme de Casteljau permettant de calculer un point spécifique d'une courbe de Bézier.
 *
 * @polycontrol: Pointeur vers la table de quadruplets définissant le polygone de contrôle de la courbe de Bézier.
 * @position: Flottant définissant la position paramétrique e [0,1] du point de la courbe à calculer.
 * @subcurve1_polycontrol: Pointeur vers une table de quadruplets. Si spécifié, reçoit la sous-courbe de la courbe de Bézier spécifiée, définie sur l'intervalle paramétrique [0,@position].
 * @subcurve2_polycontrol: Pointeur vers une table de quadruplets. Si spécifié, reçoit la sous-courbe de la courbe de Bézier spécifiée, définie sur l'intervalle paramétrique [@position, 1].
 * @return: Le triplet correspondant au point calculé.
 */
Triplet rbcurve_casteljau(Table_quadruplet* polycontrol,
			  float position,
			  Table_quadruplet* subcurve1_polycontrol,
			  Table_quadruplet* subcurve2_polycontrol);

#endif
