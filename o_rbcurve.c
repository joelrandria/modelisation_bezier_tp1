/**
 * Implémentation d'un plug-in NanoÉdit d'une courbe de Bézier rationnelle paramétrable.
 */
#include "o_objet.h"
#include "t_geometrie.h"

#include "ne_utils.h"
#include "rbcurve.h"
#include "rbcurve_casteljau.h"
#include "rbcurve_rendering.h"

#include <stdio.h>

/**
 * Construit les points de la courbe de Bézier définie par le polygone de contrôle spécifié.
 *
 * @polygon: Pointeur vers la table de quadruplets définissant le polygone de contrôle de la courbe.
 * @point_count: Le nombre de points de la courbe à calculer.
 * @result_points: Pointeur vers la table de triplets recevant les points calculés.
 */
static void rbcurve_compute_curve_points(Table_quadruplet* polygon,
					 int point_count,
					 Table_triplet* result_points)
{
  int i;
  float step;
  Triplet* curve_points;

  // Libération de la table de points courante
  if (result_points->nb > 0)
    free(result_points->table);

  // Allocation de la nouvelle table de points
  ALLOUER(curve_points, point_count);

  // Calcul du pas uniforme
  step = 1.0f / (point_count - 1);

  // Affectation directe des 2 points extrémums
  quadruplet_project(&polygon->table[0], &curve_points[0]);
  quadruplet_project(&polygon->table[polygon->nb - 1], &curve_points[point_count - 1]);

  // Calcul des points intermédiaires
  for (i = 1; i < point_count - 1; ++i)
    curve_points[i] = rbcurve_casteljau(polygon, i * step, 0, 0);

  // Affectation de la nouvelle table de points
  result_points->nb = point_count;
  result_points->table = curve_points;
}
/**
 * Recalcule les points de la courbe de Bézier initiale.
 *
 * @curve: Pointeur vers une courbe de Bézier.
 */
static void rbcurve_update_base_curve_points(struct rbcurve* curve)
{
  rbcurve_compute_curve_points(&curve->base_curve_polygon,
			       curve->base_curve_point_count,
			       &curve->base_curve_points);
}
/**
 * Recalcule les points de la courbe de Bézier paramétrique.
 *
 * @curve: Pointeur vers une courbe de Bézier.
 */
static void rbcurve_update_param_curve_points(struct rbcurve* curve)
{
  rbcurve_compute_curve_points(&curve->param_curve_polygon,
			       curve->param_curve_point_count,
			       &curve->param_curve_points);
}
/**
 * Recalcule le polygone de contrôle de la courbe de Bézier paramétrée et met à jour les points de la courbe paramétrique.
 *
 * @curve: Pointeur vers une courbe de Bézier.
 */
static void rbcurve_update_param_polygon(struct rbcurve* curve)
{
  Flottant new_range_end;

  // Libération de la table définissant le polygone de contrôle courant
  if (curve->param_curve_polygon.nb > 0)
    free(curve->param_curve_polygon.table);

  // Allocation de la nouvelle table de définition du polygone
  curve->param_curve_polygon.nb = curve->base_curve_polygon.nb;
  ALLOUER(curve->param_curve_polygon.table, curve->param_curve_polygon.nb);

  // Changement d'espace paramétrique de la borne supérieure de restriction
  new_range_end = (curve->param_range_end - curve->param_range_start) / (1 - curve->param_range_start);

  // Obtention de la sous-courbe gauche [0,param_range_start] via Casteljau
  rbcurve_casteljau(&curve->base_curve_polygon,
		    curve->param_range_start,
		    0,
		    &curve->param_curve_polygon);
  // Restriction de la sous-courbe gauche à l'espace [0,new_range_end] via Casteljau
  rbcurve_casteljau(&curve->param_curve_polygon,
		    new_range_end,
		    &curve->param_curve_polygon,
		    0);

  // MàJ des points de la courbe paramétrée
  rbcurve_update_param_curve_points(curve);
}
/**
 * Fonction de mise à jour intégrale d'une courbe de Bézier.
 *
 * @curve: Pointeur vers la courbe de Bézier à mettre à jour.
 */
static void rbcurve_update(struct rbcurve* curve)
{
  rbcurve_update_base_curve_points(curve);
  rbcurve_update_param_polygon(curve);
}

static void update(struct rbcurve* curve)
{
  if (!(UN_CHAMP_CHANGE(curve)||CREATION(curve)))
    return;

  if (CHAMP_CHANGE(curve, base_curve_polygon))
  {
    // Met intégralement à jour la courbe
    rbcurve_update(curve);
  }
  if (CHAMP_CHANGE(curve, base_curve_point_count))
  {
    if (curve->base_curve_point_count < 2)
      curve->base_curve_point_count = 10;

    // Met uniquement à jour les points de la courbe de Bézier initiale
    rbcurve_update_base_curve_points(curve);
  }

  if (CHAMP_CHANGE(curve, param_range_start) || CHAMP_CHANGE(curve, param_range_end))
  {
    if ((curve->param_range_start < 0) || (curve->param_range_end > 1)
	|| (curve->param_range_start >= curve->param_range_end))
    {
      curve->param_range_start = 0.0f;
      curve->param_range_end = 1.0f;
    }

    // Met à jour la courbe paramétrée
    rbcurve_update_param_polygon(curve);
  }
  if (CHAMP_CHANGE(curve, param_curve_point_count))
  {
    if (curve->param_curve_point_count < 2)
      curve->param_curve_point_count = 10;

    // Met uniquement à jour les points de la courbe paramétrée
    rbcurve_update_param_curve_points(curve);
  }
}

CLASSE(rbcurve, struct rbcurve,

       CHAMP(base_curve_polygon,
	     LABEL("Polygone de contrôle")
	     L_table_point P_table_quadruplet
	     Extrait Obligatoire Affiche Edite Sauve)
       CHAMP(display_base_curve_polygon,
	     LABEL("Affichage du polygone de contrôle")
	     L_booleen Edite DEFAUT("1"))
       CHAMP(base_curve_point_count,
	     LABEL("Nombre de points à afficher")
             L_entier  Edite Sauve DEFAUT("10"))

       CHAMP(param_range_start,
	     LABEL("Début de l'intervalle de paramétrisation:")
	     L_flottant P_flottant
	     Extrait Affiche Edite Sauve DEFAUT("0.0"))
       CHAMP(param_range_end,
	     LABEL("Fin de l'intervalle de paramétrisation:")
	     L_flottant P_flottant
	     Extrait Affiche Edite Sauve DEFAUT("1.0"))
       CHAMP(display_param_curve_polygon,
	     LABEL("Affichage du polygone de contrôle (courbe paramétrique)")
	     L_booleen Edite DEFAUT("1"))
       CHAMP(param_curve_point_count,
	     LABEL("Nombre de points à afficher (courbe paramétrique)")
             L_entier  Edite Sauve DEFAUT("10"))

       CHANGEMENT(update)

       CHAMP_VIRTUEL(L_affiche_gl(rbcurve_rendering))

       MENU("Jo/Rationnal B-Curve")

       EVENEMENT("Ctrl+RB")
       )
