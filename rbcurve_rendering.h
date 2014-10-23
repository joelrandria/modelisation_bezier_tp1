/**
 * Fonctions relatives au rendu graphique d'une courbe de Bézier.
 */
#ifndef __RBCURVE_RENDERING_H__
#define __RBCURVE_RENDERING_H__

#include "rbcurve.h"

/**
 * Effectue le rendu OpenGL de la courbe de Bézier spécifiée.
 *
 * Le polygone de contrôle de la courbe initiale est arbitrairement dessiné en vert. Les points de la courbe initiale sont quant à eux dessinés en blanc.
 *
 * Le polygone de contrôle et les points de la courbe paramétrée sont arbitrairement dessinés en bleu.
 *
 * @curve: Pointeur vers la courbe de Bézier à rendre.
 */
void rbcurve_rendering(struct rbcurve* curve);

#endif
