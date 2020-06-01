#include <stdio.h>
#include <stdlib.h>
#include "Liste_case.h"
#include "S_Zsg.h"
#include "Graphe_Zone.h"
#include "Entete_Fonctions.h"


Cellule_som* ajoute_liste_sommet(Sommet* sommet, Cellule_som* cell_som){
	Cellule_som* cell;
	cell = (Cellule_som*) malloc(sizeof(Cellule_som));
	cell->sommet = sommet;
	cell->suiv = cell_som;
	return cell;
}

void detruit_liste_sommet(Cellule_som* cell_som){
	Cellule_som* tmp1=cell_som;
	Cellule_som* tmp2=NULL;
	while (tmp1){
		tmp2=tmp1;
		tmp1=tmp1->suiv;
		free(tmp2);
	}
}

void ajoute_voisin(Sommet* s1, Sommet* s2){
	s1->sommet_adj = ajoute_liste_sommet(s2, s1->sommet_adj);
	s2->sommet_adj = ajoute_liste_sommet(s1, s2->sommet_adj);
}

/* fonction pour determiner l'adjacence de deux sommets dans un graphe
	renvoie 1 si s1 et s2 sont adjacent
 	renvoie 0 si ils ne sont pas adjacent
*/
int adjacent(Sommet* s1, Sommet* s2){
	int bool=0;
	Cellule_som* tmp = s1->sommet_adj;
	while (tmp){
		if (tmp->sommet == s2) {
			bool=1;
     			break; // on saute le while car s1 est adjacent a s2
    		}
    		else{
			tmp = tmp->suiv;
		}
	}
  	tmp = s2->sommet_adj;
  	while (tmp){
    		if (tmp->sommet == s1) {
     			bool=1;
      		break; // on saute le while car s2 est adjacent a s1
    		}
    		else{
			tmp = tmp->suiv;
		}
	}

	return bool;
}

Graphe_zone* cree_graphe_zone(int **M, int dim, int nbcl){
	int i,j;	
	Graphe_zone *G = (Graphe_zone*) malloc(sizeof(Graphe_zone));
	G->nbsom=0;
	G->som=NULL;	
	G->mat = (Sommet ***)malloc(sizeof(Sommet **)*dim);
	assert(G->mat != NULL);

	for(i=0;i<dim;i++){
		G->mat[i] = (Sommet **) malloc(sizeof(Sommet *)*dim);
		assert(G->mat[i]);
		for(j=0;j<dim;j++){
			G->mat[i][j] = NULL;
		}
	}
	
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){	
			Sommet* s = (Sommet *)malloc(sizeof(Sommet));
			s->num=G->nbsom;
			s->cl = M[i][j];
			s->sommet_adj=NULL;
			
			init_liste(&(s->cases));
			int taille=0;
			trouve_zone_rec(M,dim,i,j,&taille,&(s->cases));

			while(s->cases!=NULL){
				G->mat[s->cases->i][s->cases->j]=s;
				M[s->cases->i][s->cases->j]=s->cl;
				s->cases=s->cases->suiv;
			}
			
			s->nbcase_som=taille;
			ajoute_liste_sommet(s, G->som);
			(G->nbsom)++;
		}
	}
	Sommet *s1 = NULL, *s2 = NULL;
	for (i=0; i < dim; i++){
    		for(j=0; j < (dim - 1); j++){
      		s1 = G->mat[i][j];
      		s2 = G->mat[i][j+1];
      		if (s1 == s2 || adjacent(s1, s2) != 0){
				continue;
			}
      		ajoute_voisin(s1, s2);
    		}
	}
	return G;	
}

void affichage_graphe(Graphe_zone* G, int dim){
	int i=0, j=0;
	Cellule_som* parcours; /*sert a parcourir la liste des sommets */

	for(i=0; i<dim; i++){
    		for(j=0; j<dim; j++) {
      		parcours = G->mat[i][j]->sommet_adj;
      	 	printf("Dans le sommet [%d][%d], on a les noeuds ", i, j);
      		while (parcours) {
				printf("%d ", parcours->sommet->num);
				parcours = parcours->suiv;
      		}
      		printf("\n");
    		}
	}
}

void nouvelle_couleur(Graphe_zone* G, int** M,  int cl){
  	ListeCase tmp = (G->mat[0][0])->cases;
  	int i, j;
  	(G->mat[0][0])->cl = cl;
  	while(tmp != NULL) {
      	i = tmp->i;
      	j = tmp->j;
      	M[i][j] = cl;
      	tmp = tmp->suiv;
    	}
}

/* met a jour la bordure-graphe en basculant une couleur de la bordure dans la Zsg */
void mise_a_jour_bg(Graphe_zone* G, int** M, int nbCl){
  	int i, j, clp = 0; //clp:indice de la couleur la plus presente 
  	int* tab = NULL;
  	Cellule_som* bordure = NULL;
  	Cellule_som* tmp1 = G->mat[0][0]->sommet_adj;
  	Cellule_som* tmp2 = tmp1;
  	Sommet* Zsg = G->mat[0][0];
  	ListeCase Case = NULL;  
	ListeCase elem = NULL;

  	tab = (int *) malloc(sizeof(int) * nbCl);
  	assert(tab != NULL);

  	for(i = 0; i < nbCl; i++)
      	tab[i] = 0;

  	while(tmp1 != NULL) {
      	tab[(tmp1->sommet)->cl]++;
      	tmp1 = tmp1->suiv;
    	}
  
  	for(i = 1; i < nbCl; i++)
      	clp = tab[i] > tab[clp] ? i : clp;

  	nouvelle_couleur(G, M, clp);

  	tmp1 = (G->mat[0][0])->sommet_adj;
  	elem = Zsg->cases;
  	while(elem->suiv != NULL)
  		elem = elem->suiv;

  	while(tmp1 != NULL) {
  		if((tmp1->sommet)->cl == clp) {
     			bordure = tmp1->sommet->sommet_adj;
      		while (bordure) { // ajout des adjacences
				ajoute_voisin(tmp1->sommet, bordure->sommet);
				bordure = bordure->suiv;
      		}
      
           	Case = tmp1->sommet->cases;
      		elem->suiv = Case;
      
      		while (elem->suiv) {
				i = elem->i; // ajout de Zsg dans la matrice de Sommets
				j = elem->j; // pour les nouvelles cases
				G->mat[i][j] = Zsg;
				elem = elem->suiv;
      		}

      		// destruction du sommet
      		tmp2->suiv = tmp1->suiv;
      		free(tmp1); //on supprime un sommet
      		tmp1 = tmp2->suiv;
      		G->nbsom--; // on le retire du compteur
    		}
    		else {
      		// destruction du sommet
      		tmp2 = tmp1;
      		tmp1 = tmp1->suiv;
    		}
  	}
}

int maxBordure(Grille* Grille, int** M, int dim, int nbcl){
	int cpt = 0;
		
	Graphe_zone* G=cree_graphe_zone(M, dim, nbcl);

	while(G->nbsom > 0) {
		mise_a_jour_bg(G, M, Grille->nbcl);
		cpt++;
	}

	return cpt;
}

/*
Cellule_som *Chemin_court(Graphe_zone *G, int dim){
  	Sommet* zsg = G->mat[0][0];
  	Sommet* zid = G->mat[dim][dim];//zone inferieure droite 	zsg->distance = 9999;
	zsg->pere=NULL;
  	Cellule_som *chemin = ajoute_liste_sommet(zsg, NULL);
	
	if(zsg == zid){
		zsg->marque=0;
		zid->marque=0;
		zsg->distance=0;
		return ajoute_liste_sommet(zid, chemin);
	}

  	if(adjacent(zsg, zid)== 1) {
		zsg->marque=0;
		zid->marque=1;
		zid->pere=zsg;
		zsg->distance=1;
      	return ajoute_liste_sommet(zid, chemin);
    	}
	while(zsg->sommet_adj){
		if (zsg->sommet_adj->sommet->marque == 0) {

	}
	
		return chemin;
}
  	

int StrategieLargeur(Grille* Grille, int **M, int dim, int nbcl){
	int cpt=0;
	Cellule_som* chemin = NULL;

	Graphe_zone* G=cree_graphe_zone(M, dim, nbcl);
	
	chemin=Chemin_court(G,dim);

	for(i=0;chemin!=NULL;i++){
		mise_a_jour_bg(G, M, (chemin->sommet)->cl));
	} 

	cpt += maxBordure(Grille, M, dim, nbcl);
	
	return cpt;
}


*/



