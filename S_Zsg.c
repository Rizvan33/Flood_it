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
void detruit_Zsg (S_Zsg * Z){
  detruit_liste(&(Z->Lzsg));
  free(Z->Lzsg);
  int i;
  for (i=0; i<Z->nbcl ; i++){
    detruit_liste(&(Z->B[i]));
    free(Z->B[i]);
  }
  free(Z->B);

  for (i=0; i<Z->dim ; i++){
    free(Z->App[i]);
  }
  free(Z->App);
  free(Z);
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

int strequence_aleatoire_rapide (int **M, Grille *G, int dim, int nbcl, int aff){
  if(G->nbcl<2){
	return 1;
  }
  int t = 0;
  int cpt=0;
  int cl = 0;
  int i;
  int j;

  S_Zsg * Z = init_Zsg(dim, nbcl);
  t = t + agrandit_Zsg( M, Z, M[0][0], 0, 0);
  while(t<(dim*dim)){
    cl=rand()%(nbcl);
    if (cl!=M[0][0]){
      cpt++;
      ListeCase tmp = Z->Lzsg;

      while(Z->Lzsg){
        M[Z->Lzsg->i][Z->Lzsg->j]=cl;
        Z->Lzsg=Z->Lzsg->suiv;
      }

      Z->Lzsg = Z->B[cl];
      while(Z->Lzsg){
        t = t + agrandit_Zsg(M , Z, cl, Z->Lzsg->i, Z->Lzsg->j);
        Z->Lzsg=Z->Lzsg->suiv;
      }
      detruit_liste(&(Z->B[cl]));
      Z->B[cl]=NULL;
      detruit_liste(&Z->Lzsg);

      if (aff==1){  
        for (i=0;i<dim;i++){
          for (j=0;j<dim;j++){
            Grille_attribue_couleur_case(G,i,j,M[i][j]);
          }
        }
        Grille_redessine_Grille(G);
      }
    }
  }
  detruit_Zsg (Z);
  return cpt;
}

