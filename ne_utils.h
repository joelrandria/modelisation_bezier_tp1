/**
 * Fournit des fonctions communes de manipulation des entités NanoEdit.
 */
#ifndef __NE_UTILS_H__
#define __NE_UTILS_H__

#include "o_objet.h"
#include "t_geometrie.h"

/**
 * Alloue une table de quadruplets.
 *
 * @size: Taille de la table.
 * @return: Un pointeur vers la table de quadruplets allouée.
 */
Table_quadruplet* malloc_table_quadruplet(int size);
/**
 * Copie une table de quadruplets dans une autre table de quadruplets.
 *
 * @table_src: La table source.
 * @table_dst: La table de destination.
 */
void copy_table_quadruplet(Table_quadruplet* table_src, Table_quadruplet* table_dst);
/**
 * Détruit la table de quadruplets spécifiée.
 *
 * @table: La table à détruire.
 */
void free_table_quadruplet(Table_quadruplet* table);

/**
 * Interpole linéairement deux quadruplets.
 *
 * @hpoint1: Pointeur vers le quadruplet de départ.
 * @hpoint2: Pointeur vers le quadruplet d'arrivée.
 * @hresult: Pointeur vers le quadruplet recevant le résultat de l'interpolation.
 * @position: Flottant e [0,1] définissant le taux d'interpolation entre le quadruplet de départ et le quadruplet d'arrivée.
 */
void quadruplet_linear_interpolation(Quadruplet* hpoint1, Quadruplet* hpoint2,
				     Quadruplet* hresult, float position);
/**
 * Projète un quadruplet dans l'espace 3D.
 *
 * @hpoint: Pointeur vers le quadruplet à projeter.
 * @result: Pointeur vers le triplet recevant le résultat de la projection.
 */
void quadruplet_project(Quadruplet* hpoint, Triplet* result);

#endif
