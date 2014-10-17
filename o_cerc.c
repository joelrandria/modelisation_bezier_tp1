
#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "figure.h"

struct rond
{
  Table_triplet rond;
  Triplet centre;
  double rayon;
  int nb_points;
} ; 



static void changement(struct rond *o)
{ 
  double centre[3];
  double *points_sur_cercle;


  if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;

  if (CHAMP_CHANGE(o, centre) ||  CHAMP_CHANGE(o,nb_points) ||
      CHAMP_CHANGE(o, rayon))
    {
      if (o->rond.nb > 0 )
         free(o->rond.table);

      if (o->nb_points < 2)
          o->nb_points = 10;

      centre[0] = o->centre.x; 
      centre[1] = o->centre.y;
      centre[2] = o->centre.z;

      points_sur_cercle = calcul_cercle(centre,o->rayon,o->nb_points);  
      o->rond.nb = o->nb_points;
      /*
      o->rond.table = malloc(o->nb_points*sizeof(Triplet));
      ALLOUER(o->rond.table,o->nb_points);  
      for (i=0 ; i<o->nb_points ; i++)
	{
        o->rond.table[i].x = points_sur_cercle[3*i];
	}
      free(points_sur_cercle);   
      */
      o->rond.table = (Triplet *)points_sur_cercle;
     
      printf("dans changement\n");
    }
  else
   printf(" sans changement\n");
}


static void affiche_rond(struct rond *o)
{
  int j;

  glBegin(GL_LINE_LOOP) ;

  for(j=0  ; j<o->rond.nb ; j++)
    glVertex3f(o->rond.table[j].x,o->rond.table[j].y,o->rond.table[j].z); 
    
  glEnd();
}




CLASSE(rond, struct rond,      
       CHAMP(rond, L_table_point P_table_triplet 
	     Sauve)
       CHAMP(nb_points, LABEL("Nombre de points") 
             L_entier  Edite Sauve DEFAUT("10") )
       
       CHAMP(centre,L_triplet Extrait Obligatoire  Edite)
       CHAMP(rayon, L_flottant Extrait  Edite DEFAUT("1"))       
       
       CHANGEMENT(changement)
       CHAMP_VIRTUEL(L_affiche_gl(affiche_rond))
        
       MENU("Exemples_MG/rond")
       EVENEMENT("Ctrl+RD")
       )
