all: Flood-It_Partie1

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1.c

S_Zsg.o: S_Zsg.c S_Zsg.h Liste_case.h
	gcc -c S_Zsg.c

Flood-It_Partie1.o: Flood-It_Partie1.c
	gcc -c Flood-It_Partie1.c 

Flood-It_Partie1: Flood-It_Partie1.o Liste_case.o  API_Grille.o API_Gene_instance.o Fonctions_exo1.o S_Zsg.o
	gcc -o bin/Flood-It_Partie1 Flood-It_Partie1.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o S_Zsg.o  -I include -L lib -lmingw32 -lSDL2main -lSDL2

clean:
	rm -f *.o Flood-It_Partie1