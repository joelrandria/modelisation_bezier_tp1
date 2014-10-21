#ifndef __RBCURVE_CASTELJAU_H__
#define __RBCURVE_CASTELJAU_H__

#include "o_objet.h"
#include "t_geometrie.h"

Triplet rbcurve_casteljau(Table_quadruplet* polycontrol,
			  float position,
			  Table_quadruplet* subcurve1_polycontrol,
			  Table_quadruplet* subcurve2_polycontrol);

#endif
