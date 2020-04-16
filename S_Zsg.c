#include<stdlib.h>
#include <stdio.h>
#include "S_Zsg.h"
S_Zsg * init_Zsg(int dim,int nbcl){
	S_Zsg * Z=(S_Zsg*)malloc(sizeof(S_Zsg));
	Z->dim=dim;
	Z->nbcl=nbcl;
	init_liste(&(Z->Lzsg));
	Z->B=malloc(sizeof(ListeCase *)*nbcl);
	int i,j;
	for(i=0;i<nbcl;i++){
		Z->B[i]=malloc(sizeof(ListeCase));
	}
	Z->App=(int**)malloc(sizeof(int*)*dim);
	for(i=0;i<dim;i++){
		Z->App[i]=(int *)(malloc(sizeof(int)*dim));
		for(j=0;j<dim;j++){
			Z->App[i][j]=-2;
		}
	}
	return Z;
}
void ajoute_Zsg(S_Zsg * Z,int i,int j){
	if(Z==NULL){
		return ;
	}
	ajoute_en_tete(&(Z->Lzsg),i,j);
	Z->App[i][j]=-1;
}

void ajoute_Bordure(S_Zsg * Z,int i,int j,int cl){
	if (Z==NULL){
		return ;
	}
	ajoute_en_tete(&(Z->B[cl]),i,j);
	Z->App[i][j]=cl;
}

int appartient_Zsg(S_Zsg * Z,int i, int j){
	return Z->App[i][j]==-1;
}

int appartient_Bordure(S_Zsg * Z,int i , int j,int cl){
	return Z->App[i][j]==cl;
}

int agrandit_Zsg(int **M, S_Zsg * Z, int cl, int k, int l){
	int dim=Z->dim;
	int cpt=0;
	if(!appartient_Zsg(Z,k,l)){
		ajoute_en_tete(&(Z->Lzsg),k,l);
		Z->App[k][l]=-1;
		cpt++;
	}
	int i,j;
	for (i=k;i<dim;i++){
		for(j=l;j<dim;j++){
			if ((i-1>=0 && appartient_Zsg(Z,i-1,j))  || (j-1>=0 && appartient_Zsg(Z,i,j-1))
			||(i+1<dim && appartient_Zsg(Z,i+1,j)) || (j+1<dim && appartient_Zsg(Z,i,j+1))){
				if (M[i][j] == cl){
					if (Z->App[i][j]!=-1){
						ajoute_en_tete(&(Z->Lzsg),i,j);
						Z->App[i][j]=-1;
						cpt++;
					}
				} else if(M[i][j] != cl && Z->App[i][j]!=cl){
					ajoute_Bordure(Z,i,j,M[i][j]);
				}
			}
		}
	}
	return cpt;	
}

int fini(int dim,int **App){
	int i,j;
	for (i=dim-1;i>=0;i--){
		for (j=dim-1;j>0;j--){
			if(App[i][j]!=-1){
				return 0;
			}
		}
	}
	return 1;
}

int strequence_aleatoire_rapide(int ** M, Grille *G,S_Zsg * Z){
	if(G->nbcl<2){
		return 1;
	}
	if (fini(Z->dim,Z->App)){ 
		return 0;
	}
	int i,cpt;
	for(i=0;i<Z->nbcl;i++)init_liste(&(Z->B[i]));
	agrandit_Zsg(M,Z,M[0][0],0,0);
	int choix_couleur = rand()%(Z->nbcl);
	while (Z->B[choix_couleur]==NULL){ 
		choix_couleur= rand()%(Z->nbcl);
	}
	while(Z->Lzsg){
		M[Z->Lzsg->i][Z->Lzsg->j] = choix_couleur;
		Grille_attribue_couleur_case(G, Z->Lzsg->i, Z->Lzsg->j, M[Z->Lzsg->i][Z->Lzsg->j]);
		Z->Lzsg = Z->Lzsg->suiv;
	}
	Grille_redessine_Grille(G);
	while(Z->B[M[0][0]])	{	
		int k,l;
		enleve_en_tete(&(Z->B[M[0][0]]), &k, &l);
		agrandit_Zsg(M,Z,choix_couleur,k,l);
	}
	return 1 + strequence_aleatoire_rapide(M,G,Z);
}

