#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// La structure joueur
struct joueur {
	int fiche1Joueur[8]; // Tableau des scores des combinaisons basiques
	int fiche2Joueur[7]; // Tableau des scores des autres combinaisons
	int combValide[12]; // Tableau qui stocke la validation des combinaisons
	int cptRelance; // Le compteur de relance du joueur
};

typedef joueur;

void init(void);
void partie(int nombreJoueur);

void lancerDes(int *L);
void relancerDe(int de, int *L);
void listerDes(int *L, int *L2);

int total(struct joueur j, int num);
int multiple(int valeur, int *L);
int brelan(int *L);
int petiteSuite(int *L);
int grandeSuite(int *L);
int full(int *L);
int carre(int *L);
int yams(int *L);
int sommeDes(int *L);
void afficher(struct joueur j, int num, int *des);