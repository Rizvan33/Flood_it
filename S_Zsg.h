#ifndef __S_ZSG__
#define __S_ZSG__
#include <stdio.h>
#include <stdlib.h>

#include "Liste_case.h"
#include "API_Grille.h"
typedef struct {
	int dim ; /* dimension de la grille */
	int nbcl ; /* nombre de couleurs */
	ListeCase Lzsg ; /* Liste des cases de la zone Zsg */
	ListeCase * B ; /* Tableau de listes de cases de la bordure*/
	int ** App ; /* Tableau a double entree des appartenances */
} S_Zsg ;

S_Zsg * init_Zsg(int dim,int nbcl);
void detruit_Zsg (S_Zsg * Z);
void ajoute_Zsg(S_Zsg * Z,int i,int j);
void ajoute_Bordure(S_Zsg * Z,int i,int j,int cl);
int appartient_Zsg(S_Zsg * Z,int i, int j);
int appartient_Bordure(S_Zsg * Z,int i , int j,int cl);
int agrandit_Zsg(int **M, S_Zsg * Z, int cl, int k, int l);
int strequence_aleatoire_rapide (int **M, Grille *G, int dim, int nbcl, int aff);

#endif  /* __S_ZSG__ */
