#include <GL/gl.h>
#include "o_objet.h"
#include "t_geometrie.h"

#include "figure.h"


 

struct sphere
{
  Grille_triplet  affichage ;
   
  Triplet centre;
  double rayon; 
  int nb_point_affiche;     
} ;


static void affiche_sphere(struct sphere *o)
{
 int i,j;

  for (i=0 ; i<o->affichage.nb_lignes ; i++)
 {
  glBegin(GL_LINE_STRIP) ; 
  for (j=0 ; j<o->affichage.nb_colonnes ; j++)
    glVertex3f(o->affichage.grille[i][j].x, o->affichage.grille[i][j].y,
               o->affichage.grille[i][j].z) ;
  glEnd();
 }
  
  for (j=0 ; j<o->affichage.nb_colonnes ; j++)
 {
  glBegin(GL_LINE_STRIP) ; 
  for (i=0 ; i<o->affichage.nb_lignes ; i++)
    glVertex3f(o->affichage.grille[i][j].x, o->affichage.grille[i][j].y,
               o->affichage.grille[i][j].z) ;
  glEnd();
 }
  



}



static void changement(struct sphere *o)
{
  int i,j ;
  double theta,phi,pas;
  
 if ( ! (UN_CHAMP_CHANGE(o)||CREATION(o)) )
    return ;  

    
 if ( CREATION(o) || CHAMP_CHANGE(o,centre) || CHAMP_CHANGE(o,rayon) ||
     CHAMP_CHANGE(o,nb_point_affiche)  )
   {
     if (o->nb_point_affiche != o->affichage.nb_lignes)
       {
        for (i=0 ; i<o->affichage.nb_lignes ; i++)
          free(o->affichage.grille[i]);
        free(o->affichage.grille);
       }
        
        o->affichage = calcul_sphere(o->centre, o->rayon , o->nb_point_affiche);
        
   }
       
}



CLASSE(Sphere, struct sphere,
      
       //CHAMP(affichage, L_grille_point P_grille_triplet Affiche) 
       // On peut utiliser la fenêtre d'édition pour afficher rapidement 
       // le contenu de variables lors du debbugage 
      
       CHAMP_VIRTUEL(L_affiche_gl(affiche_sphere))

       CHAMP(centre, L_triplet  Extrait Edite Sauve DEFAUT("0 0 0"))
       CHAMP(rayon, L_flottant  Edite Sauve DEFAUT("1"))    
       CHAMP(nb_point_affiche, L_entier Edite Sauve DEFAUT("10"))


       CHANGEMENT(changement)
       MENU("Exemples_MG/Sphere")
       EVENEMENT("Shft+SP")
       )
