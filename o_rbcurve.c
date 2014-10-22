#include "o_objet.h"
#include "t_geometrie.h"

#include "ne_utils.h"
#include "rbcurve.h"
#include "rbcurve_casteljau.h"
#include "rbcurve_rendering.h"

#include <stdio.h>

static Triplet* rbcurve_compute_display_points(Table_quadruplet* polygon, int point_count)
{
  int i;
  float step;
  Triplet* curve_points;

  step = 1.0f / (point_count - 1);
  ALLOUER(curve_points, point_count);

  for (i = 1; i < point_count; ++i)
    curve_points[i] = rbcurve_casteljau(polygon, i * step, 0, 0);

  return curve_points;
}
static void rbcurve_compute_curve_points(Table_quadruplet* polygon,
					 int point_count,
					 Table_triplet* result_points)
{
  if (result_points->nb > 0)
    free(result_points->table);

  result_points->nb = point_count;
  result_points->table = rbcurve_compute_display_points(polygon, point_count);
}

static void rbcurve_update_base_curve_points(struct rbcurve* curve)
{
  rbcurve_compute_curve_points(&curve->base_curve_polygon,
			       curve->base_curve_point_count,
			       &curve->base_curve_points);
}

static void rbcurve_update_param_curve_points(struct rbcurve* curve)
{
  rbcurve_compute_curve_points(&curve->param_curve_polygon,
			       curve->param_curve_point_count,
			       &curve->param_curve_points);
}
static void rbcurve_update_param_polygon(struct rbcurve* curve)
{
  Flottant new_range_end;

  if (curve->param_curve_polygon.nb > 0)
    free(curve->param_curve_polygon.table);

  curve->param_curve_polygon.nb = curve->base_curve_polygon.nb;
  ALLOUER(curve->param_curve_polygon.table, curve->param_curve_polygon.nb);

  new_range_end = (curve->param_range_end - curve->param_range_start) / (1 - curve->param_range_start);

  rbcurve_casteljau(&curve->base_curve_polygon,
		    curve->param_range_start,
		    0,
		    &curve->param_curve_polygon);
  rbcurve_casteljau(&curve->param_curve_polygon,
		    new_range_end,
		    &curve->param_curve_polygon,
		    0);

  rbcurve_update_param_curve_points(curve);
}

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
    rbcurve_update(curve);
  }
  if (CHAMP_CHANGE(curve, base_curve_point_count))
  {
    if (curve->base_curve_point_count < 2)
      curve->base_curve_point_count = 10;

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

    rbcurve_update_param_polygon(curve);
  }
  if (CHAMP_CHANGE(curve, param_curve_point_count))
  {
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
