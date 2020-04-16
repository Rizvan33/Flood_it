#ifndef __ENTETE_FONCTIONS__
#define __ENTETE_FONCTIONS__

#include "API_Grille.h"
#include "Liste_case.h"


int appartient_liste(ListeCase L, int i, int j);

/* Retourne dans L la liste des cases de meme couleur que la case i,j
   et met -1 dans ces cases */
void trouve_zone_rec(int **M, int nbcase,int i, int j, int *taille, ListeCase *L);

/* Algorithme tirant au sort une couleur: il utilise la fonction recursive pour determiner la Zsg */
int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);

/* Même fonction que trouve_zone_rec mais sans récursivité */
void trouve_zone_imp(int **M, int dim, int i, int j, int *taille, ListeCase *L);

/* Même fonction que sequence_aleatoire_rec mais pour trouve_zone_imp  */
int sequence_aleatoire_rec_imp(int **M, Grille *G, int dim, int nbcl, int aff);

#endif  /* __ENTETE_FONCTIONS__ */
