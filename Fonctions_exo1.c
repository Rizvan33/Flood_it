#include<stdio.h>
#include "Entete_Fonctions.h"

int appartient_liste(ListeCase L, int i, int j){
	while (L){
		if ((L->i == i) && (L->j == j)){
			return 1;
		}
		L = L->suiv;
	}
	return 0;
}

void trouve_zone_rec(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){  
	if (!appartient_liste(*L, i, j)){
		ajoute_en_tete(L, i, j);
		(*taille)++;
	}
//Test de la case pointé si elle n'appartient pas déja à la zone 
	
	if((i>0) && (M[i-1][j]== M[i][j]) && (!appartient_liste(*L, i-1, j))){
		trouve_zone_rec(M, nbcase ,i-1 ,j ,taille, L);
	}
	//Test pour la case au dessus	
	
	if((i+1<nbcase) && (M[i+1][j]== M[i][j]) && (!appartient_liste(*L, i+1, j))){
		trouve_zone_rec(M, nbcase, i+1, j, taille, L);
	}
	//Test pour la case en dessous
	
	if((j>0) && (M[i][j-1]== M[i][j]) && (!appartient_liste(*L, i, j-1))){
		trouve_zone_rec(M, nbcase, i, j-1, taille, L);	
	}
	//Test pour la case a gauche
	
	if((j+1<nbcase) && (M[i][j+1]== M[i][j]) && (!appartient_liste(*L, i, j+1))){
		trouve_zone_rec(M, nbcase, i, j+1, taille, L);	
	}
	//Test pour la case a droite
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff){   
	int cpt=0;
	if(nbcl<2){
		return 1;
	}
	int choix_couleur = rand()%(G->nbcl);
	while (choix_couleur == M[0][0]){
		choix_couleur = rand()%(G->nbcl);
	}
	ListeCase L = malloc(sizeof(ListeCase));
	init_liste(&L);
	int taille=0;
	trouve_zone_rec(M, dim, 0, 0, &taille, &L);	
	while(L){
		M[L->i][L->j] = choix_couleur;
		Grille_attribue_couleur_case(G, L->i, L->j, M[L->i][L->j]);
		L = L->suiv;
	}
	if (taille != dim*dim){
		 aff = 1;
		Grille_redessine_Grille(G);
		cpt = 1 + sequence_aleatoire_rec(M,G,dim,nbcl,aff);
	} else {
		return cpt;
	}
}

void trouve_zone_imp(int **M, int dim, int i, int j, int *taille, ListeCase *L){
	if (!appartient_liste(*L, i, j)){
		ajoute_en_tete(L,i,j);
		(*taille)++;
	}
	int k, l;
	for (k = i; k<dim; k++){
		for (l = j; l<dim; l++){
			if (M[k][l] == M[i][j]){
				if (((k>0) && (appartient_liste(*L, k-1,l)))  ||
					 ((l>0) && (appartient_liste(*L, k,l-1))) ||
					 ((k+1<dim) && (appartient_liste(*L, k+1,l))) ||
					  ((l+1<dim) && (appartient_liste(*L, k, l+1)))){
					if (!appartient_liste(*L, k, l)){
						ajoute_en_tete(L, k, l);
						(*taille)++;
					}
				}
			}
		}
	}
}

int sequence_aleatoire_rec_imp(int **M, Grille *G, int dim, int nbcl, int aff){   
	int cpt=0;
	if(nbcl<2){
		return 1;
	}
	int choix_couleur = rand()%(G->nbcl);
	while (choix_couleur == M[0][0]){
		choix_couleur = rand()%(G->nbcl);
	}
	ListeCase L = malloc(sizeof(ListeCase));
	init_liste(&L);
	int taille=0;
	trouve_zone_imp(M, dim, 0, 0, &taille, &L);	
	while(L){
		M[L->i][L->j] = choix_couleur;
		Grille_attribue_couleur_case(G, L->i, L->j, M[L->i][L->j]);
		L = L->suiv;
	}
	if (taille != dim*dim){
		 aff = 1;
		Grille_redessine_Grille(G);
		cpt = 1 + sequence_aleatoire_rec(M,G,dim,nbcl,aff);
	} else {
		return cpt;
	}
}

