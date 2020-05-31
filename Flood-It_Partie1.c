#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"
#include "S_Zsg.h"
#include "Graphe_Zone.h"
 
 
int main(int argc,char**argv){

  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=7){
    printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-1-2> <aff 0/1>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);


  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Allocation puis generation de l'instance */

  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);

  

  if (aff==1){ /* Affichage de la grille */
    Grille_init(dim,nbcl, 500,&G);

    Grille_ouvre_fenetre(G);

    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }

    Grille_redessine_Grille(G);
    
  }

  temps_initial = clock ();
  /*
  if (exo==0){
     printf("%d essais", sequence_aleatoire_rec(M, G, dim, nbcl, aff)); 
  }
*/
ListeCase *l = malloc (sizeof(ListeCase));
init_liste(l);
int t=0;
int choix;
printf("Veuillez choisir la methode de parcours:\nPour la version recursive tapez 1, pour l'imperative tapez 2, pour l'acyclique tapez 3, pour maxBordure tapez 4, pour Strategie_Largeur tapez 5: ");
	
  scanf("%d", &choix);
  if(choix == 1){
	trouve_zone_rec(M,dim,0,0,&t,l);
	printf ("%d changements de couleurs sont necessaires pour gagner le jeu\n",sequence_aleatoire_rec(M, G, dim, nbcl, 1));
  }
    
  if(choix == 2){
	trouve_zone_imp(M,dim,0,0,&t,l);
	printf ("%d changements de couleurs sont necessaires pour gagner le jeu\n",sequence_aleatoire_rec_imp(M, G, dim, nbcl, 1));
  }
  if(choix == 3){
	S_Zsg * Z=init_Zsg(dim,nbcl);
	printf ("%d changements de couleurs sont necessaires pour gagner le jeu\n",strequence_aleatoire_rapide(M, G, dim, nbcl, 1));
  }
  if(choix == 4){
     	printf ("%d changements de couleurs sont necessaires pour gagner le jeu\n",maxBordure(G, M, dim));
  }
  /*
  if(choix == 5){
	printf ("%d changements de couleurs sont necessaires pour gagner le jeu\n",StrategieLargeur(G, M, nbcase));
  }
  */
  

  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
  printf("%f\n",temps_cpu);

  
 /* Desallocation de la matrice */
  for(i = 0; i< dim; i++) {
    if (M[i])
        free(M[i]);
  }
  if (M) free(M);

  
  
  if (aff==1){ /* Fermeture et désallocation de la grille */
    Grille_attente_touche();
    Grille_ferme_fenetre();

    Grille_free(&G);
  }

  return 0;
}
