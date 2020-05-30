#ifndef GRAPHE_ZONE_H
#define GRAPHE_ZONE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Liste_case.h"
#include "API_Grille.h"
#include "Entete_Fonctions.h"

typedef struct sommet Sommet ;
/* Element d’une liste cha\^in\’ee de pointeurs sur Sommets */
typedef struct cellule_som {
Sommet * sommet ;
struct cellule_som * suiv ;
} Cellule_som ;

struct sommet {
int num ; /* Numero du sommet (sert uniquement a l’affichage) */
int cl ; /* Couleur d’origine du sommet-zone */
ListeCase cases ; /* Listes des cases du sommet-zone */
int nbcase_som ; /* Nombre de cases de cette liste */

Cellule_som *sommet_adj ; /* Liste des ar\eteses pointeurs sur les sommets
adjacents */

int marque; 

int distance ; /* Nombre d’aretes reliant ce sommet a la racine
du parcours en largeur */
Sommet *pere ; /* Pere du sommet dans l’arborescence du parcours en largeur */
};

typedef struct graphe_zone {
int nbsom ; /* Nombre de sommets dans le graphe */
Cellule_som *som ; /* Liste chainee des sommets du graphe */
Sommet ***mat ; /* Matrice de pointeurs sur les sommets indiquant a quel sommet appartient une case (i,j) de la grille */
} Graphe_zone;



/* ajoute un pointeur sur Sommet a une liste cha?nee de Cellule som passee en parametre */
Cellule_som* ajoute_liste_sommet(Sommet* sommet, Cellule_som* cell_som);

/* detruit une liste cha?nee de Cellule som SANS detruire les sommets pointees par cette liste */
void detruit_liste_sommet(Cellule_som* cell_som);

/* met a jour deux sommet s1 et s2 en indiquant qu’ils sont adjacents l’un de l’autre */
void ajoute_voisin(Sommet* s1, Sommet* s2);

/* renvoie vrai si et seulement si deux sommets sont adjacents */
int adjacent(Sommet* s1, Sommet* s2);

/* va creer le graphe tout entier */
void cree_graphe_zone(int** M, int nbCases, Graphe_zone* G);

/* fonction d’affichage du graphe */ 
void affichage_graphe(Graphe_zone* G, int nbCases);

void nouvelle_couleur(Graphe_zone* G, int** M,  int cl);

void mise_a_jour_bg(Graphe_zone* G, int** M, int nbCl);

int maxBordure(Grille* Grille, int** M, int nbCases);


#endif  /* GRAPHE_ZONE_H */